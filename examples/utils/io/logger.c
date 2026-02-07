#include "../../../include/utils/io/logger.h"
#include <stdio.h>
#include <stdlib.h>

static const char *log_filename = "example.ignore.log.txt";
FILE *f;
int was_init = 0;
int glob_ret = 0;

void init_logging(void) {
  f = fopen(log_filename, "w");
  if (!f)
    abort();
  fclose(f);
  f = fopen(log_filename, "a");
  if (!f)
    abort();
  was_init = 1;
}

void close_logging(void) {
  if (!was_init)
    abort();
  fclose(f);
}

void function_that_needs_to_log_something(int a) {
  if (a == 0)
    return; // Success
  else {
    // Failure: Log stuff
    clib2_utils_io_logger_result_t res = CLIB2_UTILS_IO_LOGGER_RESULT_SUCCESS;
    res = clib2_utils_io_logger_log(f, "function_that_needs_to_log_something",
                                    "Value did not match expected 0",
                                    CLIB2_UTILS_IO_LOGGER_LEVEL_INFO);
    if (res == CLIB2_UTILS_IO_LOGGER_RESULT_WRITE)
      abort();
    res = clib2_utils_io_logger_log(f, "function_that_needs_to_log_something",
                                    "Pass proper value next time",
                                    CLIB2_UTILS_IO_LOGGER_LEVEL_WARNING);
    if (res == CLIB2_UTILS_IO_LOGGER_RESULT_WRITE)
      abort();
    res = clib2_utils_io_logger_log(f, "function_that_needs_to_log_something",
                                    "Proper value is only 0",
                                    CLIB2_UTILS_IO_LOGGER_LEVEL_DEBUG);
    if (res == CLIB2_UTILS_IO_LOGGER_RESULT_WRITE)
      abort();
    res = clib2_utils_io_logger_log(f, "function_that_needs_to_log_something",
                                    "Exiting with error",
                                    CLIB2_UTILS_IO_LOGGER_LEVEL_ERROR);
    glob_ret = 1;
  }
}

int main(void) {
  init_logging();
  function_that_needs_to_log_something(0);
  function_that_needs_to_log_something(11);
  close_logging();
  return glob_ret; // NB: ALWAYS RETURNS 1
}
