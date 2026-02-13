#include "../../../include/utils/str/strbuilder.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_init_and_free(void) {
  clib2_utils_str_strbuilder_t *b = clib2_utils_str_strbuilder_init();

  assert(b != NULL);
  assert(clib2_utils_str_strbuilder_len(b) == 0);

  clib2_utils_str_strbuilder_free(&b);
  assert(b == NULL);
}

static void test_basic_append(void) {
  clib2_utils_str_strbuilder_t *b = clib2_utils_str_strbuilder_init();
  assert(b);

  assert(clib2_utils_str_strbuilder_append(b, "Hello"));
  assert(clib2_utils_str_strbuilder_append(b, " "));
  assert(clib2_utils_str_strbuilder_append(b, "World"));

  assert(clib2_utils_str_strbuilder_len(b) == 11);

  char buf[32];
  size_t written = clib2_utils_str_strbuilder_copy(b, buf, sizeof(buf));

  assert(written == 11);
  assert(strcmp(buf, "Hello World") == 0);

  clib2_utils_str_strbuilder_free(&b);
}

static void test_growth(void) {
  clib2_utils_str_strbuilder_t *b = clib2_utils_str_strbuilder_init();
  assert(b);

  for (int i = 0; i < 100; ++i)
    assert(clib2_utils_str_strbuilder_append(b, "A"));

  assert(clib2_utils_str_strbuilder_len(b) == 100);

  char buf[128];
  size_t written = clib2_utils_str_strbuilder_copy(b, buf, sizeof(buf));

  assert(written == 100);
  for (int i = 0; i < 100; ++i)
    assert(buf[i] == 'A');
  assert(buf[100] == '\0');

  clib2_utils_str_strbuilder_free(&b);
}

static void test_clear(void) {
  clib2_utils_str_strbuilder_t *b = clib2_utils_str_strbuilder_init();
  assert(b);

  assert(clib2_utils_str_strbuilder_append(b, "Test"));
  assert(clib2_utils_str_strbuilder_len(b) == 4);

  clib2_utils_str_strbuilder_clear(b);
  assert(clib2_utils_str_strbuilder_len(b) == 0);

  char buf[8];
  size_t written = clib2_utils_str_strbuilder_copy(b, buf, sizeof(buf));

  assert(written == 0);
  assert(buf[0] == '\0');

  clib2_utils_str_strbuilder_free(&b);
}

static void test_truncation(void) {
  clib2_utils_str_strbuilder_t *b = clib2_utils_str_strbuilder_init();
  assert(b);

  assert(clib2_utils_str_strbuilder_append(b, "ABCDEFGHIJ"));
  assert(clib2_utils_str_strbuilder_len(b) == 10);

  char buf[5]; // room for 4 chars + '\0'
  size_t written = clib2_utils_str_strbuilder_copy(b, buf, sizeof(buf));

  assert(written == 4);
  assert(strcmp(buf, "ABCD") == 0);

  clib2_utils_str_strbuilder_free(&b);
}

static void test_copy_edge_cases(void) {
  clib2_utils_str_strbuilder_t *b = clib2_utils_str_strbuilder_init();
  assert(b);

  assert(clib2_utils_str_strbuilder_append(b, "Hi"));

  // dstsz == 0
  char dummy[1];
  size_t written = clib2_utils_str_strbuilder_copy(b, dummy, 0);
  assert(written == 0);

  // dstsz == 1
  char small[1];
  written = clib2_utils_str_strbuilder_copy(b, small, sizeof(small));
  assert(written == 0);
  assert(small[0] == '\0');

  clib2_utils_str_strbuilder_free(&b);
}

static void test_null_tolerance(void) {
  // append(NULL, ...)
  assert(!clib2_utils_str_strbuilder_append(NULL, "x"));

  clib2_utils_str_strbuilder_t *b = clib2_utils_str_strbuilder_init();
  assert(b);

  assert(!clib2_utils_str_strbuilder_append(b, ""));
  assert(!clib2_utils_str_strbuilder_append(b, NULL));

  assert(clib2_utils_str_strbuilder_len(NULL) == 0);

  assert(clib2_utils_str_strbuilder_copy(NULL, NULL, 0) == 0);

  clib2_utils_str_strbuilder_free(&b);
}

int main(void) {
  test_init_and_free();
  test_basic_append();
  test_growth();
  test_clear();
  test_truncation();
  test_copy_edge_cases();
  test_null_tolerance();

  puts("Passed test[s]!");
  return 0;
}
