#include "../../include/types/queue.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct clib2_types_queue_s {
  void *mem;
  size_t len;
  size_t elem_size;
};

clib2_types_queue_t *clib2_types_queue_init(size_t elem_size) {
  if (!elem_size)
    return NULL;
  clib2_types_queue_t *q = malloc(sizeof(clib2_types_queue_t));
  if (!q)
    return NULL;
  q->len = 0;
  q->mem = NULL;
  q->elem_size = elem_size;
  return q;
}

void clib2_types_queue_free(clib2_types_queue_t *restrict *restrict queue) {
  if (!queue || !*queue)
    return;
  free((*queue)->mem);
  free(*queue);
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

  // Get a new element
  void *nmem = realloc(queue->mem, queue->elem_size * (queue->len + 1));
  if (!nmem)
    return false;    // Error: Could not get more memory
  queue->mem = nmem; // Update the memory
  // Got new element
  // Copy in the data
  memcpy(((uint8_t *)(queue->mem)) + ((queue->len) * (queue->elem_size)), in,
         queue->elem_size);
  // Update length
  queue->len++;

  return true;
}

bool clib2_types_queue_dequeue(clib2_types_queue_t *restrict queue) {
  // Handles edge case: queue->len == 0
  if (!queue || !(queue->mem))
    return false;

  // Edge case: queue->len == 1
  if (queue->len == 1) {
    free(queue->mem);
    queue->mem = NULL;
    queue->len = 0;
    return true;
  }

  // Default case: queue->len > 1
  // This is here for atomicity (failure of realloc later on)
  void *buf = malloc(queue->elem_size);
  if (!buf)
    return false;

  // Copy the element into buf
  memcpy(buf, queue->mem, queue->elem_size);

  // Shift backwards by 1
  memmove(queue->mem, ((uint8_t *)(queue->mem)) + queue->elem_size,
          (queue->len - 1) * queue->elem_size);

  // TRY to remove the unused "slack" element
  void *nmem = realloc(queue->mem, queue->elem_size * (queue->len - 1));

  // If we failed to do that
  if (!nmem) {
    // Shift elements forwards by 1
    memmove(((uint8_t *)(queue->mem)) + queue->elem_size, queue->mem,
            (queue->len - 1) * queue->elem_size);

    // Copy in original element
    memcpy(queue->mem, buf, queue->elem_size);

    free(buf);
    return false;
  }

  // Update memory
  queue->mem = nmem;

  // Adjust length
  queue->len--;

  free(buf);
  return true;
}

bool clib2_types_queue_peek(const clib2_types_queue_t *restrict const queue,
                            void *restrict out) {
  // Handles edge case: queue->len == 0
  if (!queue || !out || !(queue->mem))
    return false;

  // Copies out first element
  memcpy(out, queue->mem, queue->elem_size);

  return true;
}

size_t clib2_types_queue_len(const clib2_types_queue_t *restrict const queue) {
  if (!queue)
    return 0;
  return queue->len;
}
