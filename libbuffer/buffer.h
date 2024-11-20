#include <stdlib.h>

typedef struct coordinates {
  int from_;
  int to_;
} coordinates;

typedef struct buffer {
  coordinates * data_;
  size_t capacity_;
  size_t size_;
  size_t in_;
  size_t out_;
} buffer;

void buffer_init(buffer * this, size_t capacity);
void buffer_destroy(buffer * this);
void buffer_push(buffer * this, const coordinates * input);
void buffer_pop(buffer * this, coordinates * output);
