#ifndef SLL_H
#define SLL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct sll_node {
  struct sll_node* next_;
  void* data_;
} sll_node;

typedef struct sll {
  struct sll_node* head_;
  struct sll_node* tail_;
  size_t size_;
  size_t dataSize_;
} sll;

void sll_init(sll* this, size_t dataSize);
void sll_clear(sll* this);
size_t sll_get_size(const sll* this);
_Bool sll_get(const sll* this, size_t index, void* out);
_Bool sll_add(sll* this, void* data);
_Bool sll_set(const sll* this, size_t index, void* data);
_Bool sll_insert(sll* this, size_t index, void* data, size_t size);
_Bool sll_remove(sll* this, size_t index);
void sll_for_each(
  sll* this,
  void(*process_item)(void*, void*, void*, void*),
  void* in, void* out, void* err
);

#endif // !SLL_H

