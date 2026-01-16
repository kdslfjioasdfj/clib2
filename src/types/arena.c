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

static inline size_t align_offset(size_t wanted, size_t align_by) {
  if (align_by <= 1)
    return 0; // No alignment needed

  size_t aligned_size = (wanted + align_by - 1) / align_by * align_by;
  return aligned_size -
         wanted; // How much we need to round up to reach aligned_size
}

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

  // Calculate how much we need to round up the size for alignment
  size_t offset = align_offset(sz, arena->align_by);

  // Check if there’s enough space in the arena
  if ((sz + offset) > (arena->total_size - arena->used))
    return NULL;

  // The next available pointer, accounting for the alignment offset
  void *next = ((uint8_t *)(arena->mem)) + arena->used + offset;

  // Update arena usage to include the size and the offset
  arena->used += sz + offset;

  return next;
}

void clib2_types_arena_reset(clib2_types_arena_t *restrict arena) {
  if (!arena)
    return;
  arena->used = 0;
}
