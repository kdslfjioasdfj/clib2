#include "../../include/types/vector.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct clib2_types_vector_s {
  void *mem;        // The internal memory backing
  size_t len;       // Amount of elements
  size_t elem_size; // Size of each element
};

clib2_types_vector_t *clib2_types_vector_init(size_t elem_size,
                                              size_t initial_len) {
  if (!elem_size || !initial_len)
    return NULL;
  clib2_types_vector_t *vec = malloc(sizeof(clib2_types_vector_t));
  if (!vec)
    return NULL;
  vec->mem = malloc(elem_size * initial_len);
  if (!(vec->mem)) {
    free(vec);
    return NULL;
  }
  vec->len = initial_len;
  vec->elem_size = elem_size;
  return vec;
}

void clib2_types_vector_free(clib2_types_vector_t *restrict *restrict vec) {
  if (!vec || !(*vec))
    return;
  free((*vec)->mem);
  free(*vec);
  *vec = NULL; // No UAF
}

size_t
clib2_types_vector_elemsize(const clib2_types_vector_t *restrict const vec) {
  if (!vec)
    return 0; // It needs to exist to have an element size
  return vec->elem_size;
}

bool clib2_types_vector_set(clib2_types_vector_t *restrict vec,
                            const void *restrict const data, size_t idx) {
  if (!vec || !data)
    return false;
  if (idx >= vec->len)
    return false; // Out-Of-Bounds access

  // Copy in data
  memcpy(((uint8_t *)(vec->mem)) + (idx * vec->elem_size), data,
         vec->elem_size);

  return true;
}

bool clib2_types_vector_get(const clib2_types_vector_t *restrict const vec,
                            void *restrict out, size_t idx) {
  if (!vec || !out)
    return false;
  if (idx >= vec->len)
    return false;

  memcpy(out, ((uint8_t *)(vec->mem)) + (idx * vec->elem_size), vec->elem_size);

  return true;
}

size_t clib2_types_vector_len(const clib2_types_vector_t *restrict const vec) {
  if (!vec)
    return 0; // You need a vector to have elements
  return vec->len;
}

bool clib2_types_vector_push(clib2_types_vector_t *restrict vec,
                             const void *const restrict in) {
  if (!vec)
    return false;
  void *new_mem =
      realloc(vec->mem, (vec->len * vec->elem_size) + vec->elem_size);
  if (!new_mem)
    return false;

  vec->mem = new_mem;
  vec->len++;

  if (!in)
    return true;

  memcpy(((uint8_t *)(vec->mem)) + ((vec->len - 1) * vec->elem_size), in,
         vec->elem_size);

  return true;
}

bool clib2_types_vector_resize(clib2_types_vector_t *restrict vec,
                               size_t size) {
  if (!vec || !size)
    return false;

  void *nmem = realloc(vec->mem, size * vec->elem_size);
  if (!nmem)
    return false;  // Operation failed, vec->mem is untouched
  vec->mem = nmem; // Operation succeeded, resized block
  vec->len = size;

  return true;
}

// DO NOT USE THIS UNLESS IN PERFORMANCE-CRITICAL SITUATIONS:
void *clib2_types_vector_getfast(clib2_types_vector_t *restrict vec,
                                 size_t idx) {
  if (!vec || idx >= vec->len)
    return NULL;
  return ((uint8_t *)(vec->mem)) + (idx * vec->elem_size);
}

bool clib2_types_vector_sort(clib2_types_vector_t *restrict vec,
                             clib2_types_vector_cmp_t cmp) {
  if (!vec || !cmp)
    return false;

  const size_t len = vec->len;
  const size_t elem_size = vec->elem_size;

  if (len < 2)
    return true;

  void *tmp = malloc(elem_size);
  if (!tmp)
    return false;

  for (size_t i = 0; i < len - 1; ++i) {
    size_t min_idx = i;

    uint8_t *min_elem = (uint8_t *)vec->mem + (min_idx * elem_size);

    for (size_t j = i + 1; j < len; ++j) {
      uint8_t *cur = (uint8_t *)vec->mem + (j * elem_size);

      if (cmp(cur, min_elem) == CLIB2_TYPES_VECTOR_CMPRES_LT) {
        min_idx = j;
        min_elem = cur;
      }
    }

    if (min_idx != i) {
      uint8_t *a = (uint8_t *)vec->mem + (i * elem_size);
      uint8_t *b = (uint8_t *)vec->mem + (min_idx * elem_size);

      memcpy(tmp, a, elem_size);
      memcpy(a, b, elem_size);
      memcpy(b, tmp, elem_size);
    }
  }

  free(tmp);
  return true;
}
