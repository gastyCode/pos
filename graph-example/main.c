#include <pthread.h>
#include <time.h>
#include <stdatomic.h>
#include <stdbool.h>
#include "../libgraph/graph.h"
#include "../libsll/sll.h"
#include "../libbuffer/synchronized_buffer.h"

typedef struct shared_data {
  atomic_bool is_user_active_; 
  synchronized_buffer* input_;
} shared_data;

typedef struct consumer_data {
  graph* graph_;
  sll* paths_;
  shared_data* shared_;
} consumer_data;

typedef struct producer_data {
  shared_data* shared_;
} producer_data;

void compute_path(void* data) {
  consumer_data* consumer_data = data;
  shortest_path shortest_path;
  coordinates coords;
  while (consumer_data->shared_->is_user_active_ || !syn_buff_is_empty(consumer_data->shared_->input_)) {
    dijkstra_init(&shortest_path);
    pthread_mutex_lock(&consumer_data->shared_->input_->mutPC_);
    while (consumer_data->shared_->input_->buff_.size_ == 0 && consumer_data->shared_->is_user_active_) {
      pthread_cond_wait(&consumer_data->shared_->input_->consume_, &consumer_data->shared_->input_->mutPC_);
    }
    if (!consumer_data->shared_->is_user_active_ && consumer_data->shared_->input_->buff_.size_ == 0) {
      dijkstra_destroy(&shortest_path);
      pthread_mutex_unlock(&consumer_data->shared_->input_->mutPC_);
      break;
    }
    buffer_pop(&consumer_data->shared_->input_->buff_, &coords);
    pthread_cond_signal(&consumer_data->shared_->input_->produce_);
    pthread_mutex_unlock(&consumer_data->shared_->input_->mutPC_);
    dijkstra_find(consumer_data->graph_, coords.from_, coords.to_, &shortest_path);
    sll_add(consumer_data->paths_, &shortest_path);
  }
}

void* consume(void* data) {
  compute_path(data);
  return NULL;
}

bool try_get_user_input(coordinates* coords) {
  printf("Insert from and to: ");
  return scanf("%d %d", &coords->from_, &coords->to_) != EOF;
}

void get_user_input(producer_data* data) {
  coordinates coords;
  while (try_get_user_input(&coords)) {
    syn_buff_push(data->shared_->input_, &coords);
  }
  data->shared_->is_user_active_ = false;
  pthread_cond_signal(&data->shared_->input_->consume_);
}

void* produce(void* data) {
  get_user_input(data);
  return NULL;
}

void print_path(void* data, void* in, void* out, void* err) {
  int actualVertex = *((int*)data);
  int lastVertex = *((int*)in);
  printf("%d %s", actualVertex, actualVertex != lastVertex ? "-> " : "\n");
}

void clear_path(void* data, void* in, void* out, void* err) {
  dijkstra_destroy(data);
}

void print_whole_path(void* data, void* in, void* out, void* err) {
  shortest_path* shortest_path = data;

  if (shortest_path->length_ != DIJKSTRA_INF) {
    printf("Path from %d to %d is %1f long.\n", shortest_path->from_, shortest_path->to_, shortest_path->length_);
    sll_for_each(shortest_path->path_, print_path, &shortest_path->to_, NULL, NULL);
  } else {
    printf("Path from %d to %d doesn't exist!\n", shortest_path->from_, shortest_path->to_);
  }
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    printf("You need to use file path as a parameter.");
    return 1;
  }

  graph graph;
  sll paths;
  synchronized_buffer buffer;
  graph_init_read(&graph, argv[1]);
  sll_init(&paths, sizeof(shortest_path));
  syn_buff_init(&buffer, 10);

  pthread_t thread;
  shared_data shared = { true, &buffer };
  producer_data producer_data = { &shared };
  consumer_data consumer_data = { &graph, &paths, &shared };

  pthread_create(&thread, NULL, &consume, &consumer_data);
  produce(&producer_data);
  pthread_join(thread, NULL);

  sll_for_each(&paths, &print_whole_path, NULL, NULL, NULL);
  sll_for_each(&paths, &clear_path, NULL, NULL, NULL);

  graph_destroy(&graph);
  sll_clear(&paths);
  syn_buff_destroy(&buffer);
  return 0;
}
