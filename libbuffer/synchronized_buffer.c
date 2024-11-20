#include "synchronized_buffer.h"

void syn_buff_init(synchronized_buffer * this, size_t capacity) {
  buffer_init(&this->buff_, capacity);
  pthread_mutex_init(&this->mutPC_, NULL);
  pthread_cond_init(&this->consume_, NULL);
  pthread_cond_init(&this->produce_, NULL);
}

void syn_buff_destroy(synchronized_buffer * this) {
  buffer_destroy(&this->buff_);
  pthread_mutex_destroy(&this->mutPC_);
  pthread_cond_destroy(&this->consume_);
  pthread_cond_destroy(&this->produce_);
}

void syn_buff_push(synchronized_buffer * this, const coordinates * input) {
  pthread_mutex_lock(&this->mutPC_);
  while (this->buff_.size_ == this->buff_.capacity_) {
    pthread_cond_wait(&this->produce_, &this->mutPC_);
  }
  buffer_push(&this->buff_, input);
  pthread_cond_signal(&this->consume_);
  pthread_mutex_unlock(&this->mutPC_);
}
void syn_buff_pop(synchronized_buffer * this, coordinates * output) {
  pthread_mutex_lock(&this->mutPC_);
  while (this->buff_.size_ == 0) {
    pthread_cond_wait(&this->consume_, &this->mutPC_);
  }
  buffer_pop(&this->buff_, output);
  pthread_cond_signal(&this->produce_);
  pthread_mutex_unlock(&this->mutPC_);
}

_Bool syn_buff_is_empty(synchronized_buffer * this) {
  _Bool result;
  pthread_mutex_lock(&this->mutPC_);
  result = this->buff_.size_ == 0;
  pthread_mutex_unlock(&this->mutPC_);
  return result;
}
