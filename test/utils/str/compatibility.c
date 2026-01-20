#include "../../../include/utils/str/compatibility.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_strdup(void) {
  const char *const src = "Hello, World!\n";
  char *ptr = clib2_utils_str_compatibility_strdup(src);
  assert(ptr != NULL);
  assert(strcmp(ptr, src) == 0);
  free(ptr);
}

void test_strlcpy(void) {
  const char *const src = "Hello!\n";

  // More-than-necessary case
  char dest1[10];
  clib2_utils_str_compatibility_strlcpy(dest1, src, sizeof(dest1));
  assert(strcmp(dest1, src) == 0);

  // Truncation case
  char dest2[7];
  clib2_utils_str_compatibility_strlcpy(dest2, src, sizeof(dest2));
  assert(strcmp(dest2, "Hello!") == 0);

  // Exactly-enough case
  char dest3[8];
  clib2_utils_str_compatibility_strlcpy(dest3, src, sizeof(dest3));
  assert(strcmp(dest3, src) == 0);
}

int main(void) {
  puts("Passed test[s]!");
  test_strdup();
  test_strlcpy();
  return 0;
}
