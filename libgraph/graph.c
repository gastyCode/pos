#include "graph.h"

void graph_init(graph* graph, int n_vertex) {
  graph->n_vertex_ = n_vertex;
  graph->dist_matrix_ = malloc(n_vertex * sizeof(double*));
  for (size_t i = 0; i < n_vertex; i++) {
    graph->dist_matrix_[i] = malloc(n_vertex * sizeof(double));
    for (size_t j = 0; j < n_vertex; j++) {
      graph->dist_matrix_[i][j] = DIJKSTRA_INF;
    }
  }
}

void graph_destroy(graph* graph) {
  for (size_t i = 0; i < graph->n_vertex_; i++) {
    free(graph->dist_matrix_[i]);
  }
  free(graph->dist_matrix_);
}

_Bool graph_init_read(graph* graph, const char* path) {
  FILE* file = fopen(path, "r");
  if (file == NULL) {
    fprintf(stderr, "Failed to open the file: %s\n", path);
    return 0;
  }
  int n_vertex;
  fscanf(file, "%d", &n_vertex);
  graph_init(graph, n_vertex);
  int from, to;
  double distance;
  while (fscanf(file, "%d %d %lf", &from, &to, &distance) != EOF)  {
    graph->dist_matrix_[from][to] = distance;
  }
  fclose(file);
  return 1;
}

void graph_print(graph* graph) {
  for (size_t i = 0; i < graph->n_vertex_; i++) {
    for (size_t j = 0; j < graph->n_vertex_; j++) {
      if (graph->dist_matrix_[i][j] == DIJKSTRA_INF) {
        printf("%3c ", '-');
      }
      else {
        printf("%3.0lf ", graph->dist_matrix_[i][j]);
      }
    }
    puts("");
  }
}
