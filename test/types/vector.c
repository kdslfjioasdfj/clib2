#include "../../include/types/vector.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int x, y;
} point_t;

void test_basics(void) {
  clib2_types_vector_t *v = clib2_types_vector_init(sizeof(int), 2);
  assert(v != NULL);
  assert(clib2_types_vector_len(v) == 0);
  assert(clib2_types_vector_elemsize(v) == sizeof(int));

  int a = 10, b = 20;
  assert(clib2_types_vector_push(v, &a));
  assert(clib2_types_vector_push(v, &b));
  assert(clib2_types_vector_len(v) == 2);

  int out;
  assert(clib2_types_vector_get(v, &out, 0));
  assert(out == 10);
  assert(clib2_types_vector_get(v, &out, 1));
  assert(out == 20);

  int c = 42;
  assert(clib2_types_vector_set(v, &c, 1));
  assert(clib2_types_vector_get(v, &out, 1));
  assert(out == 42);

  assert(clib2_types_vector_resize(v, 1));
  assert(clib2_types_vector_len(v) == 1);

  assert(clib2_types_vector_resize(v, 4));
  assert(clib2_types_vector_len(v) == 4);

  int d = 99;
  assert(clib2_types_vector_push(v, &d));
  assert(clib2_types_vector_len(v) == 5);

  clib2_types_vector_free(&v);
  assert(v == NULL);

  clib2_types_vector_t *svec = clib2_types_vector_init(sizeof(point_t), 1);
  assert(svec != NULL);
  point_t p = {1, 2};
  assert(clib2_types_vector_push(svec, &p));
  point_t pout;
  assert(clib2_types_vector_get(svec, &pout, 0));
  assert(pout.x == 1 && pout.y == 2);

  clib2_types_vector_free(&svec);
}

clib2_types_vector_cmpres_t cmp_int(const void *a, const void *b) {
  int x = *(const int *)a;
  int y = *(const int *)b;
  if (x == y)
    return CLIB2_TYPES_VECTOR_CMPRES_EQ;
  return (x < y) ? CLIB2_TYPES_VECTOR_CMPRES_LT : CLIB2_TYPES_VECTOR_CMPRES_GT;
}

void test_sort(void) {
  clib2_types_vector_t *v = clib2_types_vector_init(sizeof(int), 5);
  assert(v != NULL);

  int vals[] = {5, 2, 4, 1, 3};
  for (size_t i = 0; i < 5; i++) {
    assert(clib2_types_vector_push(v, &vals[i]));
  }

  assert(clib2_types_vector_sort(v, cmp_int));

  int out;
  for (int i = 0; i < 5; i++) {
    assert(clib2_types_vector_get(v, &out, (size_t)i));
    assert(out == i + 1);
  }

  clib2_types_vector_free(&v);
}

int main(void) {
  test_basics();
  test_sort();
  puts("Passed test[s]!");
  return 0;
}
