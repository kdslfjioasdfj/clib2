#include "../../include/types/arena.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct clib2_types_arena_s {
  void *mem;
  size_t total_size;
  size_t align_by; // If zero, treat it as no alignment
  size_t used;
};

clib2_types_arena_t *clib2_types_arena_init(size_t sz, size_t align,
                                            bool zeroed) {
  if (!sz)
    return NULL;

  clib2_types_arena_t *arena = malloc(sizeof(clib2_types_arena_t));
  if (!arena)
    return NULL;

  if (zeroed)
    arena->mem = calloc(1, sz);
  else
    arena->mem = malloc(sz);
  if (!(arena->mem)) {
    free(arena);
    return NULL;
  }
  arena->align_by = align;
  arena->used = 0;
  arena->total_size = sz;

  return arena;
}

void clib2_types_arena_destroy(clib2_types_arena_t *restrict *restrict arena) {
  if (!arena || !(*arena))
    return;
  free((*arena)->mem);
  free(*arena);
  *arena = NULL;
}

void *clib2_types_arena_alloc(clib2_types_arena_t *restrict arena, size_t sz) {
  if (!sz || !arena)
    return NULL;
  uintptr_t cur_addr = (uintptr_t)arena->mem + arena->used;
  size_t offset = 0;
  if (arena->align_by > 1) {
    size_t rem = cur_addr % arena->align_by;
    if (rem != 0)
      offset = arena->align_by - rem;
  } else {
    // arena->align_by is 1 or 0, ignore alignment
    if (sz > arena->total_size - arena->used)
      return NULL;
    void *ptr = (uint8_t *)arena->mem + arena->used;
    arena->used += sz;
    return ptr;
  }
  if (sz + offset > arena->total_size - arena->used)
    return NULL;
  void *ptr = (uint8_t *)arena->mem + arena->used + offset;
  arena->used += sz + offset;
  return ptr;
}

void clib2_types_arena_reset(clib2_types_arena_t *restrict arena) {
  if (!arena)
    return;
  arena->used = 0;
}
