#include "../../../include/utils/str/strbuilder.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Weekday names (not shorthand)
static const char *WEEKDAYS[] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                                 "Thursday", "Friday", "Saturday"};

static bool append_two_digits(clib2_utils_str_strbuilder_t *builder,
                              int value) {
  char buf[3];
  buf[0] = (char)('0' + (value / 10));
  buf[1] = '0' + (value % 10);
  buf[2] = '\0';
  return clib2_utils_str_strbuilder_append(builder, buf);
}

static bool append_four_digits(clib2_utils_str_strbuilder_t *builder,
                               int value) {
  char buf[5];
  buf[0] = '0' + (value / 1000 % 10);
  buf[1] = '0' + (value / 100 % 10);
  buf[2] = '0' + (value / 10 % 10);
  buf[3] = '0' + (value % 10);
  buf[4] = '\0';
  return clib2_utils_str_strbuilder_append(builder, buf);
}

static bool format_date(clib2_utils_str_strbuilder_t *builder,
                        struct tm *tm_info) {
  if (!append_four_digits(builder, tm_info->tm_year + 1900))
    return false;
  if (!clib2_utils_str_strbuilder_append(builder, ":"))
    return false;
  if (!append_two_digits(builder, tm_info->tm_mon + 1))
    return false;
  if (!clib2_utils_str_strbuilder_append(builder, ":"))
    return false;
  if (!append_two_digits(builder, tm_info->tm_mday))
    return false;
  return true;
}

static bool format_weekday(clib2_utils_str_strbuilder_t *builder,
                           struct tm *tm_info) {
  int wday = tm_info->tm_wday;
  if (wday < 0 || wday > 6)
    return false;
  return clib2_utils_str_strbuilder_append(builder, WEEKDAYS[wday]);
}

static char *build_time_string(time_t t) {
  struct tm *tm_info = localtime(&t); // ISO C11, cross-platform
  if (!tm_info)
    return NULL;

  clib2_utils_str_strbuilder_t *builder = clib2_utils_str_strbuilder_init();
  if (!builder)
    return NULL;

  bool ok = clib2_utils_str_strbuilder_append(builder, "Time: {") &&
            format_date(builder, tm_info) &&
            clib2_utils_str_strbuilder_append(builder, "}, Day = {") &&
            format_weekday(builder, tm_info) &&
            clib2_utils_str_strbuilder_append(builder, "}");

  if (!ok) {
    clib2_utils_str_strbuilder_free(&builder);
    return NULL;
  }

  size_t len = clib2_utils_str_strbuilder_len(builder);
  char *result = malloc(len + 1);
  if (result)
    clib2_utils_str_strbuilder_copy(builder, result, len + 1);

  clib2_utils_str_strbuilder_free(&builder);
  return result;
}

int main(void) {
  time_t now = time(NULL);
  char *formatted = build_time_string(now);
  if (!formatted) {
    fprintf(stderr, "Failed to build time string.\n");
    return 1;
  }

  printf("%s\n", formatted);
  free(formatted);
  return 0;
}
