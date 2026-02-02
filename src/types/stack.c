#include "../../include/types/stack.h"
#include "../../include/shared.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct clib2_types_stack_s {
  void *mem;        // Backing memory of the array
  size_t cap;       // Capacity of the array
  size_t used;      // The amount of used elements
  size_t elem_size; // The size of each element in the stack
};

clib2_types_stack_t *clib2_types_stack_init(size_t elem_size) {
  if (!elem_size)
    return NULL;
  clib2_types_stack_t *stack = malloc(sizeof(clib2_types_stack_t));
  if (!stack)
    return NULL;
  // Starting with 1 element because it makes enforcing elements easier
  stack->mem = malloc(elem_size);
  if (!(stack->mem)) {
    free(stack);
    return NULL;
  }
  stack->elem_size = elem_size;
  stack->cap = 1;
  stack->used = 0; // No elements used yet
  return stack;
}

void clib2_types_stack_free(clib2_types_stack_t *restrict *restrict stack) {
  // NOTE: !(*stack) is only evaluated if stack != NULL
  if (!stack || !(*stack))
    return;
  free((*stack)->mem);
  free(*stack);
  *stack = NULL;
}

size_t
clib2_types_stack_elemsize(const clib2_types_stack_t *restrict const stack) {
  if (!stack)
    return 0;
  return stack->elem_size;
}

bool clib2_types_stack_push(clib2_types_stack_t *restrict stack,
                            const void *restrict const in) {
  if (!stack || !in)
    return false;

  // Double size if space runs out
  if (stack->used == stack->cap) {
    size_t new_cap;
    if (mul_overflow(2, stack->cap, &new_cap))
      return false; // The stack was NOT changed
    size_t desired_bytes;
    if (mul_overflow(new_cap, stack->elem_size, &desired_bytes))
      return false; // The stack was NOT changed
    void *new_mem = realloc(stack->mem, desired_bytes);
    if (!new_mem)
      return false; // The stack was NOT changed
    stack->mem = new_mem;
    stack->cap = new_cap;
  }

  // Copy in the new element
  /*
  Note that this is safe because:
  During the check for sufficient space, we checked if
  (stack->cap*2)*stack->elem_size was safe, meaning that as long as
  stack->used<=capacity (it is), then (stack->used*stack->elem_size) is not an
  overflow by guarantee.
  */
  memcpy(((uint8_t *)(stack->mem)) + (stack->used * stack->elem_size), in,
         stack->elem_size);
  stack->used++;

  return true;
}

bool clib2_types_stack_pop(clib2_types_stack_t *restrict stack) {
  // NOTE: !(stack->used) is evaluated when stack != NULL
  if (!stack || !(stack->used))
    return false;

  stack->used--;

  return true;
}

bool clib2_types_stack_peek(const clib2_types_stack_t *restrict const stack,
                            void *restrict out) {
  // NOTE: !(stack->used) is only evaluated if stack != NULL
  if (!stack || !out || !(stack->used))
    return false;

  // (stack->used * stack->elem_size) is safe because
  // (stack->cap*stack->elem_size) is safe and checked in clib2_types_stack_push
  memcpy(out,
         ((uint8_t *)(stack->mem)) + ((stack->used - 1) * stack->elem_size),
         stack->elem_size);

  return true;
}

size_t
clib2_types_stack_height(const clib2_types_stack_t *restrict const stack) {
  if (!stack)
    return 0;
  return stack->used;
}
