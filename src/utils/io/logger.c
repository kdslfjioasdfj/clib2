#include "../../../include/utils/io/logger.h"

static inline const char *stringify_level(clib2_utils_io_logger_level_t level) {
  switch (level) {
  case CLIB2_UTILS_IO_LOGGER_LEVEL_INFO:
    return "INFO";
  case CLIB2_UTILS_IO_LOGGER_LEVEL_DEBUG:
    return "DEBUG";
  case CLIB2_UTILS_IO_LOGGER_LEVEL_WARNING:
    return "WARNING";
  case CLIB2_UTILS_IO_LOGGER_LEVEL_ERROR:
    return "ERROR";
  default:
    return "UNKNOWN LEVEL";
  }
}

clib2_utils_io_logger_result_t
clib2_utils_io_logger_log(FILE *restrict file, const char *restrict const title,
                          const char *restrict const description,
                          clib2_utils_io_logger_level_t level) {
  if (!file || !title || !description)
    return CLIB2_UTILS_IO_LOGGER_RESULT_INVALID;

  if (fprintf(file, "[%s] %s - %s\n", stringify_level(level), title,
              description) < 0 ||
      ferror(file) || fflush(file) == EOF)
    return CLIB2_UTILS_IO_LOGGER_RESULT_WRITE;

  return CLIB2_UTILS_IO_LOGGER_RESULT_SUCCESS;
}
