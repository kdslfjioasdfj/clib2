#include "../../../include/utils/str/compatibility.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  const char *s = "Hey, look! This is a cool string!";
  char *s2 = clib2_utils_str_compatibility_strdup(s);
  if (!s2)
    return 1;
  printf("Original string: \"%s\"\nDuplicated string: \"%s\"\nOriginal string "
         "pointer: %p\nDuplicated string pointer: %p\n",
         s, s2, (void *)s, (void *)s2);
  printf("Using clib2_utils_str_compatibility_strlcpy now...\n");
  char *orig = "Hello, World!";
  char extra_space[32];
  char not_enough_space[10];
  char perfect_space[15];

  clib2_utils_str_compatibility_strlcpy(extra_space, orig, sizeof(extra_space));
  clib2_utils_str_compatibility_strlcpy(not_enough_space, orig,
                                        sizeof(not_enough_space));
  clib2_utils_str_compatibility_strlcpy(perfect_space, orig,
                                        sizeof(perfect_space));

  printf("Original string: \"%s\"\nExtra space: \"%s\"\nNot enough space: "
         "\"%s\"\nPerfect amount of space: \"%s\"\n",
         orig, extra_space, not_enough_space, perfect_space);

  free(s2);
  return 0;
}
