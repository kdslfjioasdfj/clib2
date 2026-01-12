#include "../../include/types/queue.h"
#include <asm-generic/errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct clib2_types_queue_s {
  void *mem;
  size_t elem_size;
  size_t length;
};

clib2_types_queue_t *clib2_types_queue_init(size_t elem_size) {
  if (!elem_size)
    return NULL;
  clib2_types_queue_t *queue = malloc(sizeof(clib2_types_queue_t));
  if (!queue)
    return NULL;
  queue->mem = NULL;
  queue->elem_size = elem_size;
  queue->length = 0;
  return queue;
}

void clib2_types_queue_free(clib2_types_queue_t *restrict *restrict queue) {
  if (!queue || !(*queue))
    return;
  if ((*queue)->mem)
    free((*queue)->mem);
  free(*queue);
  *queue = NULL;
}

size_t clib2_types_queue_elemsize(clib2_types_queue_t *restrict const queue) {
  if (!queue)
    return 0; // The queue needs to exist
  return queue->elem_size;
}

bool clib2_types_queue_enqueue(clib2_types_queue_t *restrict queue,
                               const void *restrict const in) {
  if (!queue || !in)
    return false; // Queue must exist

  // First enqueue operation
  if (!queue->mem) {
    queue->mem = malloc(queue->elem_size);
    if (!queue->mem)
      return false;
    memcpy(queue->mem, in, queue->elem_size);
    queue->length++;
    return true;
  }

  // Default: Append a new element
  void *mem = realloc(queue->mem, (queue->length + 1) * queue->elem_size);
  if (!mem)
    return false;

  queue->mem = mem;
  memcpy(((uint8_t *)(queue->mem)) + (queue->length * queue->elem_size), in,
         queue->elem_size);
  queue->length++;
  return true;
}

bool clib2_types_queue_dequeue(clib2_types_queue_t *restrict queue) {
  // NOTE: Handles edge case in which no elements are in the queue
  if (!queue || !queue->mem)
    return false; // Queue must exist and have elements

  // Edge case: Only 1 element in queue
  if (queue->length == 1) {
    free(queue->mem);
    queue->mem = NULL;
    queue->length = 0;
    return true;
  }

  // Default case: Multiple elements in queue
  void *buf = malloc(queue->elem_size);
  if (!buf)
    return false; // Allocation error, queue unchanged
  // Keep a backup for atomicity: IMPORTANT
  memcpy(buf, queue->mem, queue->elem_size);

  // Shift elements back 1 time
  memmove(queue->mem, ((uint8_t *)(queue->mem)) + queue->elem_size,
          queue->elem_size * (queue->length - 1));

  // realloc so we remove 1 element
  void *mem = realloc(queue->mem, queue->elem_size * (queue->length - 1));
  if (!mem) {
    // There was an error: could not reallocate
    // Shift elements back to original positions
    memmove(((uint8_t *)(queue->mem)) + queue->elem_size, queue->mem,
            queue->elem_size * (queue->length - 1));
    // Return to original state and fail
    memcpy(queue->mem, buf, queue->elem_size);
    free(buf);
    return false;
  }

  // If we're here, we managed to reduce one element. Adjust length.
  queue->length--;

  free(buf);
  return true;
}

bool clib2_types_queue_peek(const clib2_types_queue_t *restrict const queue,
                            void *restrict out) {
  // NOTE: Handles edge case where there are no elements in the queue
  if (!queue || !out || !queue->mem)
    return false;

  // Copy in data
  // Casted queue->mem to (const void *) to avoid accidental modification. There
  // is no RT slow-down.
  memcpy(out, (const void *)queue->mem, queue->elem_size);

  return true;
}

size_t clib2_types_queue_len(const clib2_types_queue_t *restrict const queue) {
  if (!queue)
    return 0; // Queue needs to exist
  return queue->length;
}