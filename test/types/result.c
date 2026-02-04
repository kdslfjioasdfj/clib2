#include "../../include/types/result.h"
#include <assert.h>
#include <stdio.h>

const static int value_ok = 0;
const static int value_err = 1;

void test_ok(void) {
  clib2_types_result_t res = clib2_types_result_ok((void *)&value_ok);
  assert(res.ok && res.success == res.err &&
         res.success == (void *)(&value_ok));
}

void test_err(void) {
  clib2_types_result_t res = clib2_types_result_err((void *)&value_err);
  assert(!res.ok && res.err == res.success && res.err == (void *)(&value_err));
}

int main(void) {
  test_ok();
  test_err();
  puts("Passed test[s]!");
  return 0;
}
