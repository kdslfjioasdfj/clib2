#include "../../../include/utils/str/strbuilder.h"
#include "../../../include/shared.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define STARTING_BUFFER 16

struct clib2_utils_str_strbuilder_s {
  char *buf;
  size_t cap;  // Total allocated characters
  size_t used; // Used characters NOT including null-terminator
};

static inline bool
try_grow_buf(clib2_utils_str_strbuilder_t *restrict builder) {
  size_t desired_bytes;
  if (clib2_shared_mul_overflow_size_t(builder->cap, 2, &desired_bytes))
    return false;
  char *new_mem = realloc(builder->buf, desired_bytes);
  if (!new_mem)
    return false;
  builder->buf = new_mem;
  builder->cap = desired_bytes;
  return true;
}

static inline bool add_overflow(size_t a, size_t b, size_t *restrict out) {
  if (a > SIZE_MAX - b) {
    *out = SIZE_MAX;
    return true;
  }
  *out = a + b;
  return false;
}

clib2_utils_str_strbuilder_t *clib2_utils_str_strbuilder_init(void) {
  clib2_utils_str_strbuilder_t *builder =
      malloc(sizeof(clib2_utils_str_strbuilder_t));
  if (!builder)
    return NULL;
  builder->buf = malloc(STARTING_BUFFER);
  if (!(builder->buf)) {
    free(builder);
    return NULL;
  }
  builder->cap = STARTING_BUFFER;
  builder->used = 0;
  return builder;
}

void clib2_utils_str_strbuilder_free(
    clib2_utils_str_strbuilder_t *restrict *restrict builder) {
  if (!builder || !(*builder))
    return;
  free((*builder)->buf);
  free(*builder);
  *builder = NULL;
}

bool clib2_utils_str_strbuilder_append(
    clib2_utils_str_strbuilder_t *restrict builder,
    const char *restrict const str) {
  if (!builder || !str || !(*str))
    return false;

  size_t len = strlen(str);

  size_t required;
  if (add_overflow(builder->used, len, &required))
    return false;

  while (required > builder->cap)
    if (!try_grow_buf(builder))
      return false;

  memcpy(builder->buf + builder->used, str, len);
  builder->used += len;

  return true;
}

CLIB2_SHARED_PUBLIC size_t clib2_utils_str_strbuilder_len(
    const clib2_utils_str_strbuilder_t *restrict const builder) {
  if (!builder)
    return 0;
  return builder->used;
}

void clib2_utils_str_strbuilder_clear(
    clib2_utils_str_strbuilder_t *restrict builder) {
  if (!builder)
    return;
  builder->used = 0;
}

size_t clib2_utils_str_strbuilder_copy(
    const clib2_utils_str_strbuilder_t *restrict builder, char *restrict dst,
    size_t dstsz) {
  if (!dst || !builder || !dstsz)
    return 0;
  size_t copy_in_bytes =
      (builder->used >= dstsz - 1) ? (dstsz - 1) : builder->used;
  memcpy(dst, builder->buf, copy_in_bytes);
  dst[copy_in_bytes] = '\0'; // Null-terminate the string. DOES NOT influence
                             // return, just keeps promise.
  return copy_in_bytes;
}
