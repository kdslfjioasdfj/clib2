#include "../../include/types/option.h"
#include <assert.h>
#include <stdio.h>

void test_some(void) {
  int x = 2;
  clib2_types_option_t opt = clib2_types_option_some(&x);
  assert(opt.is_some && *(int *)opt.val == 2);
}

void test_none(void) {
  clib2_types_option_t opt = clib2_types_option_none();
  assert(!opt.is_some && !opt.val);
}

// Should not abort
void test_abort_some(void) {
  int x = 2;
  clib2_types_option_t opt = clib2_types_option_some(&x);
  assert(clib2_types_option_unwrap(opt) == &x);
}

void test_abort_none(void) {
  clib2_types_option_t opt = clib2_types_option_none();
  clib2_types_option_unwrap(opt); // Aborts program
}

int main(void) {
  test_some();
  test_none();
  test_abort_some();
  // Uncomment to test:
  // test_abort_none();
  puts("Passed test[s]!");
  return 0;
}
