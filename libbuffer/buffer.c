#include "buffer.h"

void buffer_init(buffer * this, size_t capacity) {
  this->capacity_ = capacity;
  this->data_ = calloc(capacity, sizeof(coordinates));
  this->size_ = 0;
  this->in_ = 0;
  this->out_ = 0;
}

void buffer_destroy(buffer * this) {
  free(this->data_);
  this->data_ = NULL;
}

void buffer_push(buffer * this, const coordinates * input) {
  this->data_[this->in_++] = *input;
  this->size_++;
  this->in_ %= this->capacity_;
}

void buffer_pop(buffer * this, coordinates * output) {
  *output = this->data_[this->out_++];
  this->size_--;
  this->out_ %= this->capacity_;
}
