#include "../../include/types/stack.h"
#include <assert.h>
#include <stdio.h>

void test_basics(void) {
  // Tests everything
  clib2_types_stack_t *stack = clib2_types_stack_init(sizeof(int));
  assert(stack != NULL);

  int x = 22;
  clib2_types_stack_push(stack, &x);
  int y;
  clib2_types_stack_peek(stack, &y);
  assert(x == y);
  int z = 12;
  clib2_types_stack_push(stack, &z);
  int x2;
  clib2_types_stack_peek(stack, &x2);
  assert(x2 == z); // This simultaneously asserts x2 != y
  clib2_types_stack_pop(stack);
  int y2;
  clib2_types_stack_peek(stack, &y2);
  assert(y2 == y);

  size_t esz = clib2_types_stack_elemsize(stack);
  assert(esz == sizeof(int));

  size_t hsz = clib2_types_stack_height(stack);
  assert(hsz == 1);

  clib2_types_stack_free(&stack);
}

int main(void) {
  test_basics();
  puts("Passed test[s]!");
  return 0;
}
