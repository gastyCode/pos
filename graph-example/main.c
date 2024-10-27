#include "../libgraph/graph.h"

void print_item(void* data, void* in, void* out, void* err) { printf("%d\n", *(int*)data); }

int main(int argc, char *argv[])
{
  graph* graph;
  shortest_path* shortest_path;
  graph_init_read(graph, "/home/shared/POS/paluch-digraph");

  dijkstra_init(shortest_path);
  dijkstra_find(graph, 1, 3, shortest_path);
  sll_for_each(shortest_path->path_, print_item, NULL, NULL, NULL);

  dijkstra_destroy(shortest_path);

  return 0;
}
