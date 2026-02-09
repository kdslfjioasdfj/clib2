#include "../../../include/utils/str/extra.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#define BUF_SZ 68

char buf[BUF_SZ] = {0};

bool get_int64_from_stdio(int64_t *out) {
  puts("Enter an integer:");
  if (!fgets(buf, BUF_SZ, stdin))
    return false;

  size_t len = strlen(buf);
  if (len && buf[len - 1] == '\n')
    buf[len - 1] = '\0';

  return clib2_utils_str_extra_atoi64(buf, out);
}

int main(void) {
  int64_t x = 0;
  bool success = get_int64_from_stdio(&x);
  if (!success) {
    puts("You did not enter a valid integer.");
    return 1;
  }
  printf("You entered: %" PRIi64 "\n", x);
  return 0;
}
