/**
 * @file arena.h
 * @brief A memory arena
 * @date 2026-01-15
 */

#ifndef CLIB2_TYPES_ARENA_H
#define CLIB2_TYPES_ARENA_H

#include "../shared.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief The arena type
 */
typedef struct clib2_types_arena_s clib2_types_arena_t;

/**
 * @brief Initialize an arena
 * @param sz The size of the arena
 * @param align The alignment to be followed
 * @param zeroed Whether the memory should be zeroed out ONCE at initialization
 * @return The pointer to the arena
 * @note If @p align == 0 || @p align == 1, the allocations do not follow any
 * alignment and might cause improper alignment.
 */
CLIB2_SHARED_PUBLIC clib2_types_arena_t *
clib2_types_arena_init(size_t sz, size_t align, bool zeroed);

/**
 * @brief Destroy an arena (deallocate it and invalidate all elements)
 * @param arena The arena to free
 */
CLIB2_SHARED_PUBLIC void
clib2_types_arena_destroy(clib2_types_arena_t *restrict *restrict arena);

/**
 * @brief Allocate memory from the arena
 * @param arena The arena to allocate from
 * @param sz The size the block of memory should (at least) be. The amount of
 * accessible memory could be greater due to alignment, but no promises are
 * made about out-of-bounds access.
 * @return The pointer to use
 */
CLIB2_SHARED_PUBLIC void *
clib2_types_arena_alloc(clib2_types_arena_t *restrict arena, size_t sz);

/**
 * @brief Reset the usage counter of an arena to 0
 * @param arena The arena to reset
 * @details This function resets the internal usage counter of an arena to 0.
 * This means it will go back to 0 and start giving earlier addresses; previous
 * allocations are still valid but may overlap with newer ones.
 */
CLIB2_SHARED_PUBLIC void
clib2_types_arena_reset(clib2_types_arena_t *restrict arena);

#endif // CLIB2_TYPES_ARENA_H
