#include "../../include/types/stack.h"
#include <stdbool.h>
#include <stdio.h>

static bool matches(char open, char close) {
  return (open == '(' && close == ')') || (open == '{' && close == '}') ||
         (open == '[' && close == ']');
}

bool braces_balanced(const char *s) {
  clib2_types_stack_t *stack = clib2_types_stack_init(sizeof(char));

  if (!stack)
    return false;

  for (; *s; s++) {
    if (*s == '(' || *s == '{' || *s == '[') {
      clib2_types_stack_push(stack, s);
    } else if (*s == ')' || *s == '}' || *s == ']') {
      char top;

      if (clib2_types_stack_height(stack) == 0) {
        clib2_types_stack_free(&stack);
        return false;
      }

      clib2_types_stack_peek(stack, &top);
      clib2_types_stack_pop(stack);

      if (!matches(top, *s)) {
        clib2_types_stack_free(&stack);
        return false;
      }
    }
  }

  bool ok = (clib2_types_stack_height(stack) == 0);
  clib2_types_stack_free(&stack);
  return ok;
}

int main(void) {
  const char *test = "{[()]}";

  printf("Balanced: %s\n", braces_balanced(test) ? "yes" : "no");

  return 0;
}
