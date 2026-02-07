#include "../../../include/utils/io/logger.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *tmp_file = "test_log.tmp";

static char *read_temp_file(void) {
  FILE *f = fopen(tmp_file, "r");
  assert(f && "Failed to open temp file for reading");

  fseek(f, 0, SEEK_END);
  long len = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *buf = malloc((unsigned long)len + 1);
  assert(buf && "Failed to allocate memory for reading temp file");

  size_t n = fread(buf, 1, (unsigned long)len, f);
  assert(n == (size_t)len && "Failed to read all bytes from temp file");

  buf[len] = '\0';
  fclose(f);
  return buf;
}

static void cleanup(void) { remove(tmp_file); }

void test_info_log(void) {
  FILE *f = fopen(tmp_file, "w+");
  assert(f && "Failed to open temp file for writing");

  clib2_utils_io_logger_result_t res = clib2_utils_io_logger_log(
      f, "Module", "Info message", CLIB2_UTILS_IO_LOGGER_LEVEL_INFO);
  fclose(f);

  assert(res == CLIB2_UTILS_IO_LOGGER_RESULT_SUCCESS);

  char *contents = read_temp_file();
  assert(strcmp(contents, "[INFO] Module - Info message\n") == 0);
  free(contents);
  cleanup();
}

void test_debug_log(void) {
  FILE *f = fopen(tmp_file, "w+");
  assert(f && "Failed to open temp file for writing");

  clib2_utils_io_logger_result_t res = clib2_utils_io_logger_log(
      f, "Vector", "Debugging", CLIB2_UTILS_IO_LOGGER_LEVEL_DEBUG);
  fclose(f);

  assert(res == CLIB2_UTILS_IO_LOGGER_RESULT_SUCCESS);

  char *contents = read_temp_file();
  assert(strcmp(contents, "[DEBUG] Vector - Debugging\n") == 0);
  free(contents);
  cleanup();
}

void test_warning_log(void) {
  FILE *f = fopen(tmp_file, "w+");
  assert(f && "Failed to open temp file for writing");

  clib2_utils_io_logger_result_t res = clib2_utils_io_logger_log(
      f, "Memory", "Almost full", CLIB2_UTILS_IO_LOGGER_LEVEL_WARNING);
  fclose(f);

  assert(res == CLIB2_UTILS_IO_LOGGER_RESULT_SUCCESS);

  char *contents = read_temp_file();
  assert(strcmp(contents, "[WARNING] Memory - Almost full\n") == 0);
  free(contents);
  cleanup();
}

void test_error_log(void) {
  FILE *f = fopen(tmp_file, "w+");
  assert(f && "Failed to open temp file for writing");

  clib2_utils_io_logger_result_t res = clib2_utils_io_logger_log(
      f, "IO", "Something went wrong", CLIB2_UTILS_IO_LOGGER_LEVEL_ERROR);
  fclose(f);

  assert(res == CLIB2_UTILS_IO_LOGGER_RESULT_SUCCESS);

  char *contents = read_temp_file();
  assert(strcmp(contents, "[ERROR] IO - Something went wrong\n") == 0);
  free(contents);
  cleanup();
}

int main(void) {
  test_info_log();
  test_debug_log();
  test_warning_log();
  test_error_log();
  puts("Passed test[s]!");
  return 0;
}
