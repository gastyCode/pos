#include <pthread.h>
#include <time.h>
#include "../libgraph/graph.h"

typedef struct dijkstra_data {
  graph graph_;
  shortest_path shortest_path_;
  const char* file_path_;
} dijkstra_data;

void dijkstra_data_init(dijkstra_data* data, const char* file_path) {
  data->file_path_ = file_path; 
}

void print_item(void* data, void* in, void* out, void* err) { printf("%d => ", *(int*)data); }

void* run_dijkstra(void* data) {
  dijkstra_data dijkstra_data;
  dijkstra_data_init(&dijkstra_data, "/home/shared/POS/digraph");
  graph_init_read(&dijkstra_data.graph_, dijkstra_data.file_path_);

  dijkstra_init(&dijkstra_data.shortest_path_);
  int from = rand() % 100, to = rand() % 100;
  dijkstra_find(&dijkstra_data.graph_, from, to, &dijkstra_data.shortest_path_);

  printf("[ %d => %d ]: ", from, to);
  sll_for_each(dijkstra_data.shortest_path_.path_, print_item, NULL, NULL, NULL);
  printf("| Length: %.2f\n", dijkstra_data.shortest_path_.length_);

  dijkstra_destroy(&dijkstra_data.shortest_path_);
  graph_destroy(&dijkstra_data.graph_);

  return NULL;
}

int main(int argc, char *argv[])
{
  srand(time(NULL));

  int threads_count;
  printf("Insert number of paths: ");
  scanf("%d", &threads_count);

  pthread_t threads[threads_count];

  for (int i = 0; i < threads_count; i++) {
    pthread_create(&threads[i], NULL, run_dijkstra, NULL);
  }

  for (int i = 0; i < threads_count; i++) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}
