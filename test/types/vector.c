#include "../../include/types/vector.h"
#include <assert.h>
#include <stdio.h>

static clib2_types_vector_cmpres_t cmp(const void *const a,
                                       const void *const b);
static clib2_types_vector_cmpres_t cmp(const void *const a,
                                       const void *const b) {
  if (*(int *)a < *(int *)b)
    return CLIB2_TYPES_VECTOR_CMPRES_LT;
  else if (*(int *)a > *(int *)b)
    return CLIB2_TYPES_VECTOR_CMPRES_GT;
  else
    return CLIB2_TYPES_VECTOR_CMPRES_EQ;
}

void test_basics(void) {
  // Tests get, set, push, resize (everything else is a invariant)
  clib2_types_vector_t *vec = clib2_types_vector_init(sizeof(int), 2);
  assert(vec != NULL);
  int x = 0;
  clib2_types_vector_set(vec, &x, 0);
  int test1;
  clib2_types_vector_get(vec, &test1, 0);
  assert(test1 == 0);
  x = 33;
  clib2_types_vector_set(vec, &x, 0);
  int test2;
  clib2_types_vector_get(vec, &test2, 0);
  assert(test2 == 33);
  const int *const test3 = clib2_types_vector_getfast(vec, 0);
  assert(*test3 == 33); // Maybe ownership violation if not const int *const
  int y = 57;
  bool suc = clib2_types_vector_push(vec, &y);
  assert(suc);
  int test4;
  clib2_types_vector_get(vec, &test4, 2);
  assert(test4 == 57);
  clib2_types_vector_resize(vec, 1);
  int tbuf = -2;
  bool test5 = clib2_types_vector_get(vec, &tbuf, 2);
  assert(!test5);
  clib2_types_vector_resize(vec, 4);
  bool test6 = clib2_types_vector_set(vec, (const int *const)&tbuf, 3);
  int tbuf2;
  clib2_types_vector_get(vec, &tbuf2, 3);
  assert(test6 && tbuf2 == -2);
  clib2_types_vector_free(&vec);
}

void test_sort(void) {
  clib2_types_vector_t *vec = clib2_types_vector_init(sizeof(int), 10);
  assert(vec != NULL);
  for (int i = 0; i < 10; i++) {
    int x = 9 - i;
    clib2_types_vector_set(vec, &x, (size_t)i);
  }
  clib2_types_vector_sort(vec, cmp);
  for (int i = 0; i < 10; i++) {
    int x;
    clib2_types_vector_get(vec, &x, (size_t)i);
    assert(x == i);
  }
  clib2_types_vector_free(&vec);
}

int main(void) {
  test_basics();
  test_sort();
  puts("Passed test[s]!");
  return 0;
}
