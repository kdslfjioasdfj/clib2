#include "../../include/types/stack.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct clib2_types_stack_s {
  void *mem;
  size_t elem_size;
  size_t height;
};

clib2_types_stack_t *clib2_types_stack_init(size_t elem_size) {
  if (!elem_size)
    return NULL;
  clib2_types_stack_t *stack = malloc(sizeof(clib2_types_stack_t));
  if (!stack)
    return NULL;
  stack->height = 0;
  stack->elem_size = elem_size;
  stack->mem = NULL; // 0 height = 0 mem
  return stack;
}

void clib2_types_stack_free(clib2_types_stack_t *restrict *restrict stack) {
  if (!stack || !*stack)
    return;
  if ((*stack)->mem)
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

  if (!(stack->mem)) {
    // No prior elements
    stack->mem = malloc(stack->elem_size);
    if (!(stack->mem))
      return false;
    memcpy(stack->mem, in, stack->elem_size);
    stack->height++;
    return true;
  }

  // If we reach here, there were prior elements and we just have to push a new
  // item on top with realloc()
  void *nmem = realloc(stack->mem, (stack->height + 1) * stack->elem_size);
  if (!nmem)
    return false; // Could not allocate more memory, fail with memory untouched

  stack->height++;
  stack->mem = nmem; // Update memory
  memcpy(((uint8_t *)(stack->mem)) + ((stack->height - 1) * stack->elem_size),
         in, stack->elem_size);

  return true;
}

bool clib2_types_stack_pop(clib2_types_stack_t *restrict stack) {
  if (!stack || !(stack->mem))
    return false;

  if (stack->height == 1) {
    // Handle this edge case:
    /*
      SHORT:
      Becase we call realloc(stack->mem, (stack->height - 1) * stack->elem_size)

      LONG:
      This is an edge case because in the normal handling block, we use
      ((stack->height - 1) * stack->elem_size), which when stack->height == 1,
      gives us 0. This would make a realloc(stack->mem, 0) call, which is
      implementation-defined
    */
    free(stack->mem);
    stack->mem = NULL;
    stack->height = 0;
    return true;
  }

  void *nmem = realloc(stack->mem, (stack->height - 1) * stack->elem_size);
  if (!nmem)
    return false;

  stack->height--;
  stack->mem = nmem;

  return true;
}

bool clib2_types_stack_peek(const clib2_types_stack_t *restrict const stack,
                            void *restrict out) {
  if (!stack || !out || !(stack->mem) || !(stack->height))
    return false;

  memcpy(out,
         ((uint8_t *)(stack->mem)) + ((stack->height - 1) * stack->elem_size),
         stack->elem_size);

  return true;
}

size_t
clib2_types_stack_height(const clib2_types_stack_t *restrict const stack) {
  if (!stack)
    return 0;
  return stack->height;
}
