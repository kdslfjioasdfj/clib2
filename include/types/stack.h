/**
 * @file stack.h
 * @brief A stack type for C
 * @date 2025-12-26
 */

#ifndef CLIB2_TYPES_STACK_H
#define CLIB2_TYPES_STACK_H

#include "../shared.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief The stack struct, is opaque
 */
typedef struct clib2_types_stack_s clib2_types_stack_t;

/**
 * @brief Initialize a stack
 * @param elem_size The size of each element in the stack (homogeneous)
 * @return A pointer to the stack
 */
CLIB2_SHARED_PUBLIC clib2_types_stack_t *
clib2_types_stack_init(size_t elem_size);

/**
 * @brief This function frees the stack
 * @param stack The stack to free
 */
CLIB2_SHARED_PUBLIC void
clib2_types_stack_free(clib2_types_stack_t *restrict *restrict stack);

/**
 * @brief Fetch the size of each element in a stack
 * @param stack The stack to fetch size from
 * @return The size of the stack
 */
CLIB2_SHARED_PUBLIC size_t
clib2_types_stack_elemsize(const clib2_types_stack_t *restrict const stack);

/**
 * @brief Push data to the stack
 * @param stack The stack to push an element onto
 * @param in The data to copy into the new element. Copies in about
 * clib2_types_stack_elemsize( @p stack ) bytes into the stack from @p in
 * @return The success of the operation
 */
CLIB2_SHARED_PUBLIC bool
clib2_types_stack_push(clib2_types_stack_t *restrict stack,
                       const void *restrict const in);

/**
 * @brief Pop the top element of a stack
 * @param stack The stack to pop data from
 * @return The success of the operation
 */
CLIB2_SHARED_PUBLIC bool
clib2_types_stack_pop(clib2_types_stack_t *restrict stack);

/**
 * @brief Peek on the top element of a stack
 * @param stack The stack to peek data from
 * @param out The out pointer to write into. Copies in upto
 * clib2_types_stack_elemsize( @p stack ) bytes into @p out
 * @return The success of the operation
 */
CLIB2_SHARED_PUBLIC bool
clib2_types_stack_peek(const clib2_types_stack_t *restrict const stack,
                       void *restrict out);

/**
 * @brief Get the height (length) of a stack
 * @param stack The stack to fetch from
 * @return The height of the stack
 */
CLIB2_SHARED_PUBLIC size_t
clib2_types_stack_height(const clib2_types_stack_t *restrict const stack);

#endif // CLIB2_TYPES_STACK_H
