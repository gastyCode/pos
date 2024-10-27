#include <libdijkstra/dijkstra.h>
#include <stdlib.h>
#include <stdio.h>

void graph_init(graph* graph, int n_vertex);
void graph_destroy(graph* graph);
_Bool graph_init_read(graph* graph, const char* path);
void graph_print(graph* graph);
