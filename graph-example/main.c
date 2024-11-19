#include <pthread.h>
#include <time.h>
#include "../libgraph/graph.h"

typedef struct thread_data {
  graph* graph_;
  int from_;
  int to_;
  shortest_path shortest_path_;
} thread_data;

void print_item(void* data, void* in, void* out, void* err) { printf("%d => ", *(int*)data); }

void* compute_path(void* data) {
  thread_data* thread_data = data;

  dijkstra_find(thread_data->graph_, thread_data->from_, thread_data->to_, &thread_data->shortest_path_);
  
  return NULL;
}

void print_path(int from, int to, shortest_path* shortest_path) {
  printf("[ %d => %d ]: ", from, to);
  sll_for_each(shortest_path->path_, print_item, NULL, NULL, NULL);
  printf("| Length: %.2f\n", shortest_path->length_);
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
  graph_init_read(&graph, argv[1]);

  pthread_t threads[threads_count];
  thread_data data[threads_count];

  for (int i = 0; i < threads_count; i++) {
    data[i].graph_ = &graph;
    dijkstra_init(&data[i].shortest_path_);
    printf("From: ");
    scanf("%d", &data[i].from_);
    printf("To: ");
    scanf("%d", &data[i].to_);
    pthread_create(&threads[i], NULL, compute_path, &data[i]);
  }

  for (int i = 0; i < threads_count; i++) {
    pthread_join(threads[i], NULL);
    print_path(data[i].from_, data[i].to_, &data[i].shortest_path_);
    dijkstra_destroy(&data[i].shortest_path_);
  }
  graph_destroy(&graph);
  return 0;
}
