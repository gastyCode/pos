#include <pthread.h>
#include <time.h>
#include "../libgraph/graph.h"
#include "../libsll/sll.h"

typedef struct thread_data {
  graph* graph_;
  int from_;
  int to_;
  sll* paths_;
  pthread_mutex_t* mutex_;
} thread_data;

void* compute_path(void* data) {
  thread_data* thread_data = data;
  shortest_path shortest_path;
  dijkstra_init(&shortest_path);
  dijkstra_find(thread_data->graph_, thread_data->from_, thread_data->to_, &shortest_path);

  pthread_mutex_lock(thread_data->mutex_);
  sll_add(thread_data->paths_, &shortest_path);
  pthread_mutex_unlock(thread_data->mutex_);
  
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

  srand(time(NULL));

  int threads_count;
  printf("Insert number of paths: ");
  scanf("%d", &threads_count);

  graph graph;
  sll paths;
  pthread_mutex_t mutex;
  graph_init_read(&graph, argv[1]);
  sll_init(&paths, sizeof(shortest_path));
  pthread_mutex_init(&mutex, NULL);

  pthread_t threads[threads_count];
  thread_data data[threads_count];

  for (int i = 0; i < threads_count; i++) {
    data[i].graph_ = &graph;
    data[i].paths_ = &paths;
    data[i].mutex_ = &mutex;
    printf("From: ");
    scanf("%d", &data[i].from_);
    printf("To: ");
    scanf("%d", &data[i].to_);
    pthread_create(&threads[i], NULL, compute_path, &data[i]);
  }

  for (int i = 0; i < threads_count; i++) {
    pthread_join(threads[i], NULL);
  }

  sll_for_each(&paths, &print_whole_path, NULL, NULL, NULL);
  sll_for_each(&paths, &clear_path, NULL, NULL, NULL);

  graph_destroy(&graph);
  sll_clear(&paths);
  pthread_mutex_destroy(&mutex);
  return 0;
}
