#include <stdlib.h>
#include <pthread.h>
#include "buffer.h"

typedef struct synchronized_buffer {
  buffer buff_;
  pthread_mutex_t mutPC_;
  pthread_cond_t consume_;
  pthread_cond_t produce_;
} synchronized_buffer;

void syn_buff_init(synchronized_buffer * this, size_t capacity);
void syn_buff_destroy(synchronized_buffer * this);
void syn_buff_push(synchronized_buffer * this,
const coordinates * input);
void syn_buff_pop(synchronized_buffer * this, coordinates * output);
_Bool syn_buff_is_empty(synchronized_buffer * this);
