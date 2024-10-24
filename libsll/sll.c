#include "sll.h"

sll_node* create_node(sll* this, void* data) {
  sll_node* node = malloc(sizeof(sll_node));
  node->data_ = malloc(this->dataSize_);
  node->next_ = NULL;
  memcpy(node->data_, data, this->dataSize_);
  return node;
}

void destroy_node(sll_node* node) {
  free(node->data_);
  free(node);
}

sll_node* get_node(sll* this, size_t index) {
  sll_node* node = this->head_;
  for (int i = 0;i < index;i++) {
    node = node->next_;
  }
  return node;
}

void sll_init(sll* this, size_t dataSize) {
  this->head_ = NULL;
  this->tail_ = NULL;
  this->size_ = 0;
  this->dataSize_ = dataSize;
}

void sll_clear(sll* this) {
  while (this->head_ != NULL) {
    sll_node* next = this->head_->next_;
    destroy_node(this->head_);
    this->head_ = next;
  }
  this->head_ = NULL;
  this->size_ = 0;
}

size_t sll_get_size(const sll* this) {
  return this->size_;
}

_Bool sll_get(const sll* this, size_t index, void* out) {
  if (index >= sll_get_size(this)) {
    return 0;
  }
  sll_node* node = get_node(this, index);
  memcpy(out, node->data_, this->dataSize_);
  return 1;
}

_Bool sll_set(const sll* this, size_t index, void* data) {
  if (index >= sll_get_size(this)) {
    return 0;
  }
  sll_node* node = get_node(this, index);
  memcpy(node->data_, data, this->dataSize_);
  return 1;
}

_Bool sll_add(sll* this, void* data) {
  sll_node* newNode = create_node(this, data);
  sll_node** nextPtr = (sll_get_size(this) == 0) ? &(this->head_) : &(this->tail_->next_);
  *nextPtr = newNode;
  this->tail_ = newNode;
  ++(this->size_);
  return 1;
}

_Bool sll_insert(sll* this, size_t index, void* data, size_t size) {
  if (index > sll_get_size(this)) {
    return 0;
  }
  if (index == sll_get_size(this)) {
    return sll_add(this, data);
  }
  sll_node* newNode = create_node(this, data);
  sll_node** nextPtr = &(this->head_);
  for (int i = 0;i < index;i++) {
    nextPtr = &((*nextPtr)->next_);
  }
  newNode->next_ = *nextPtr;
  *nextPtr = newNode;
  ++(this->size_);
  return 1;
}

_Bool sll_remove(sll* this, size_t index) {
  if (index >= sll_get_size(this)) {
    return 0;
  }
  --(this->size_);
  sll_node* node = NULL;
  sll_node* prevNode = NULL;
  sll_node** nextPtr = &(this->head_);
  for (int i = 0; i < index; i++) {
    prevNode = *nextPtr;
    nextPtr = &((*nextPtr)->next_);
  }
  if (index == sll_get_size(this) - 1) {
    this->tail_ = prevNode;
  }
  node = *nextPtr;
  *nextPtr = (*nextPtr)->next_;
  destroy_node(node);
  return 1;
}

void sll_for_each(
  sll* this,
  void(*process_item)(void*, void*, void*, void*),
  void* in, void* out, void* err
) {
  sll_node* node = this->head_;
  while (node != NULL) {
    process_item(node->data_, in, out, err);
    node = node->next_;
  }
}