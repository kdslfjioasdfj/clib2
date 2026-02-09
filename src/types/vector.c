#include "../../include/types/vector.h"
#include "../../include/shared.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct clib2_types_vector_s {
  void *mem;        // The backing memory of the vector
  size_t len;       // Used length
  size_t cap;       // Total capacity
  size_t elem_size; // Size of each element
};

clib2_types_vector_t *clib2_types_vector_init(size_t elem_size,
                                              size_t initial_len) {
  if (!elem_size || !initial_len)
    return NULL;
  clib2_types_vector_t *vec = malloc(sizeof(clib2_types_vector_t));
  if (!vec)
    return NULL;
  size_t bytes_needed;
  if (clib2_shared_mul_overflow_size_t(elem_size, initial_len, &bytes_needed))
    // elem_size * initial_len is an overflow
    return NULL;
  vec->mem = malloc(bytes_needed);
  if (!(vec->mem)) {
    // Could not obtain backing memory
    free(vec);
    return NULL;
  }
  vec->cap = initial_len;
  vec->len = 0;
  vec->elem_size = elem_size;
  return vec;
}

void clib2_types_vector_free(clib2_types_vector_t *restrict *restrict vec) {
  // If !vec, !(*vec) is never evaluated: no UB
  if (!vec || !(*vec))
    return;
  free((*vec)->mem);
  free(*vec);
  *vec = NULL;
}

size_t
clib2_types_vector_elemsize(const clib2_types_vector_t *restrict const vec) {
  if (!vec)
    return 0; // 0 is never a valid elem_size, so this can safely mean an error.
  return vec->elem_size;
}

bool clib2_types_vector_set(clib2_types_vector_t *restrict vec,
                            const void *restrict const data, size_t idx) {
  // Making sure all parameters are valid
  // idx >= vec->len is only evaluated if vec != NULL
  if (!vec || !data || idx >= vec->len)
    return false;

  size_t pos;
  if (clib2_shared_mul_overflow_size_t(idx, vec->elem_size, &pos))
    return false; // The vector was NOT changed
  memcpy(((uint8_t *)(vec->mem)) + pos, data, vec->elem_size);

  return true;
}

bool clib2_types_vector_get(const clib2_types_vector_t *restrict const vec,
                            void *restrict out, size_t idx) {
  // idx >= vec->len is only evaluated if vec != NULL
  if (!vec || !out || idx >= vec->len)
    return false;

  size_t pos;
  if (clib2_shared_mul_overflow_size_t(idx, vec->elem_size, &pos))
    return false; // The vector was NOT changed
  memcpy(out, ((uint8_t *)(vec->mem)) + pos, vec->elem_size);

  return true;
}

size_t clib2_types_vector_len(const clib2_types_vector_t *restrict const vec) {
  if (!vec)
    return 0; // 0 is NEVER a valid length, hence it can be an error value
  return vec->len;
}

bool clib2_types_vector_push(clib2_types_vector_t *restrict vec,
                             const void *const restrict in) {
  if (!vec)
    return false;

  if (vec->cap > vec->len) {
    if (in) {
      size_t pos;
      if (clib2_shared_mul_overflow_size_t(vec->elem_size, vec->len, &pos))
        return false; // The vector was NOT changed
      memcpy(((uint8_t *)(vec->mem)) + pos, in, vec->elem_size);
    }
    vec->len++;
  } else {
    size_t new_cap;      // Ideally vec->cap * 2
    size_t desired_size; // Ideally new_cap * vec->elem_size
    if (clib2_shared_mul_overflow_size_t(vec->cap, 2, &new_cap) ||
        clib2_shared_mul_overflow_size_t(new_cap, vec->elem_size, &desired_size))
      // NOTE: Both new_cap and desired_size should be initialized now
      return false; // The vector was NOT changed
    // Now, actually try to resize:
    void *new_mem = realloc(vec->mem, desired_size);
    if (!new_mem)
      return false; // The vector was NOT changed
    vec->mem = new_mem;
    vec->cap = new_cap;
    if (in)
      memcpy(((uint8_t *)(vec->mem)) + (vec->elem_size * vec->len), in,
             vec->elem_size); // In this case, vec->elem_size * vec->len is
                              // guaranteed not to be an overflow because of all
                              // the previous checks.
    vec->len++;
  }

  return true;
}

bool clib2_types_vector_resize(clib2_types_vector_t *restrict vec,
                               size_t size) {
  if (!vec || !size)
    return false;
  if (vec->len == size)
    return true;
  if (vec->cap > size || vec->len > size) {
    vec->len = size;
    return true;
  }
  size_t desired_size;
  if (clib2_shared_mul_overflow_size_t(size, vec->elem_size, &desired_size))
    return false; // The vector was NOT changed
  void *new_mem = realloc(vec->mem, desired_size);
  if (!new_mem)
    return false; // The vector was NOT changed
  vec->len = size;
  vec->cap = size;
  vec->mem = new_mem;
  return true;
}

bool clib2_types_vector_sort(clib2_types_vector_t *restrict vec,
                             clib2_types_vector_cmp_t cmp) {
  if (!vec || !cmp)
    return false;

  const size_t len = vec->len;
  const size_t elem_size = vec->elem_size;

  if (len < 2) // Only one element
    return true;

  void *tmp = malloc(elem_size);
  if (!tmp)
    return false;

  uint8_t *data = (uint8_t *)vec->mem;

  // I used selection sort because it's quite simple
  for (size_t i = 0; i < len - 1; i++) {
    size_t min_idx = i;

    for (size_t j = i + 1; j < len; j++) {
      if (cmp(data + j * elem_size, data + min_idx * elem_size) ==
          CLIB2_TYPES_VECTOR_CMPRES_LT) {
        min_idx = j;
      }
    }

    if (min_idx != i) {
      memcpy(tmp, data + i * elem_size, elem_size);
      memcpy(data + i * elem_size, data + min_idx * elem_size, elem_size);
      memcpy(data + min_idx * elem_size, tmp, elem_size);
    }
  }

  free(tmp);
  return true;
}
