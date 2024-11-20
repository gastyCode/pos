#include <pthread.h>
#include <time.h>
#include <stdatomic.h>
#include "../libgraph/graph.h"
#include "../libsll/sll.h"
#include "../libbuffer/synchronized_buffer.h"

typedef struct shared_data {
  int count_;
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
  for (int i = 0; i < consumer_data->shared_->count_; i++) {
    dijkstra_init(&shortest_path);
    syn_buff_pop(consumer_data->shared_->input_, &coords);
    dijkstra_find(consumer_data->graph_, coords.from_, coords.to_, &shortest_path);
    sll_add(consumer_data->paths_, &shortest_path);
  }
}

void* consume(void* data) {
  compute_path(data);
  return NULL;
}

void get_user_input(void* data) {
  producer_data* producer_data = data;
  coordinates coords;
  for (int i = 0; i < producer_data->shared_->count_; i++) {
    printf("Insert from and to: ");
    scanf("%d %d", &coords.from_, &coords.to_);
    syn_buff_push(producer_data->shared_->input_, &coords);
  }
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

  int count;
  printf("Insert number of paths: ");
  scanf("%d", &count);

  graph graph;
  sll paths;
  synchronized_buffer buffer;
  graph_init_read(&graph, argv[1]);
  sll_init(&paths, sizeof(shortest_path));
  syn_buff_init(&buffer, 10);

  pthread_t thread;
  shared_data shared = { count, &buffer };
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
