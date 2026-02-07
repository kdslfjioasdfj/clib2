#include "../../include/types/queue.h"
#include "../../include/shared.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct clib2_types_queue_s {
  void *mem;        // Backing memory of the queue
  size_t elem_size; // The size of each element in the queue
  size_t head;      // The index of the head of the queue
  size_t tail;      // The index of the tail of the queue
  size_t capacity;  // The amount of elements in the queue
  size_t used;      // The amount of USED spaces in the queue
};

static bool queue_grow(clib2_types_queue_t *queue) {
  size_t new_capacity;
  if (mul_overflow(queue->capacity, 2, &new_capacity))
    return false;

  size_t new_bytes;
  if (mul_overflow(new_capacity, queue->elem_size, &new_bytes))
    return false;

  void *new_mem = malloc(new_bytes);
  if (!new_mem)
    return false;

  for (size_t i = 0; i < queue->used; ++i) {
    size_t src_index = (queue->head + i) % queue->capacity;

    size_t src_offset;
    if (mul_overflow(src_index, queue->elem_size, &src_offset)) {
      free(new_mem);
      return false;
    }

    size_t dst_offset;
    if (mul_overflow(i, queue->elem_size, &dst_offset)) {
      free(new_mem);
      return false;
    }

    memcpy((uint8_t *)new_mem + dst_offset, (uint8_t *)queue->mem + src_offset,
           queue->elem_size);
  }

  free(queue->mem);
  queue->mem = new_mem;
  queue->capacity = new_capacity;
  queue->head = 0;
  queue->tail = queue->used;

  return true;
}

clib2_types_queue_t *clib2_types_queue_init(size_t elem_size) {
  if (!elem_size)
    return NULL;
  clib2_types_queue_t *queue = malloc(sizeof(clib2_types_queue_t));
  if (!queue)
    return NULL;
  // Starting with 1 element because it makes enforcing invariants easier
  queue->mem = malloc(elem_size);
  if (!(queue->mem)) {
    free(queue);
    return NULL;
  }
  queue->elem_size = elem_size;
  queue->capacity = 1;
  queue->used = 0;
  queue->head = 0;
  queue->tail = 0;
  return queue;
}

void clib2_types_queue_free(clib2_types_queue_t *restrict *restrict queue) {
  // NOTE: !(*queue) is only evaluated if queue != NULL
  if (!queue || !(*queue))
    return;
  free((*queue)->mem);
  free((*queue));
  *queue = NULL;
}

size_t clib2_types_queue_elemsize(clib2_types_queue_t *restrict const queue) {
  if (!queue)
    return 0;
  return queue->elem_size;
}

bool clib2_types_queue_enqueue(clib2_types_queue_t *restrict queue,
                               const void *restrict const in) {
  if (!queue || !in)
    return false;

  if (queue->used == queue->capacity) {
    if (!queue_grow(queue))
      return false;
  }

  size_t offset;
  if (mul_overflow(queue->tail, queue->elem_size, &offset))
    return false;

  memcpy((uint8_t *)queue->mem + offset, in, queue->elem_size);

  queue->tail = (queue->tail + 1) % queue->capacity;
  queue->used++;

  return true;
}

bool clib2_types_queue_dequeue(clib2_types_queue_t *restrict queue) {
  if (!queue || queue->used == 0)
    return false;

  queue->head = (queue->head + 1) % queue->capacity;
  queue->used--;

  return true;
}

bool clib2_types_queue_peek(const clib2_types_queue_t *restrict const queue,
                            void *restrict out) {
  if (!queue || !out || queue->used == 0)
    return false;

  size_t offset;
  if (mul_overflow(queue->head, queue->elem_size, &offset))
    return false;

  memcpy(out, (uint8_t *)queue->mem + offset, queue->elem_size);
  return true;
}

size_t clib2_types_queue_len(const clib2_types_queue_t *restrict const queue) {
  if (!queue)
    return 0;
  return queue->used;
}
