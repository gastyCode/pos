#include "../libgraph/graph.h"

void print_item(void* data, void* in, void* out, void* err) { printf("%d => ", *(int*)data); }

int main(int argc, char *argv[])
{
  graph* graph = (struct graph*)malloc(sizeof(struct graph));
  shortest_path* shortest_path = (struct shortest_path*)malloc(sizeof(struct shortest_path));
  graph_init_read(graph, "/home/shared/POS/digraph");

  dijkstra_init(shortest_path);
  dijkstra_find(graph, 7, 54, shortest_path);
  sll_for_each(shortest_path->path_, print_item, NULL, NULL, NULL);

  dijkstra_destroy(shortest_path);
  graph_destroy(graph);

  return 0;
}
