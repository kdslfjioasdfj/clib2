#include "../../../include/utils/io/logger.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

const size_t extra_fmt_chars = 7;

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

static inline void mk_fmt(char *restrict out, const char *title,
                          size_t title_len, const char *description,
                          size_t description_len, const char *stringified_level,
                          size_t stringified_level_len) {
  out[0] = '[';
  out++;
  memcpy(out, stringified_level, stringified_level_len);
  out += stringified_level_len;
  out[0] = ']';
  out[1] = ' ';
  out += 2;
  memcpy(out, title, title_len);
  out += title_len;
  out[0] = ' ';
  out[1] = '-';
  out[2] = ' ';
  out += 3;
  memcpy(out, description, description_len);
  out[description_len] = '\n';
  out[description_len + 1] = '\0';
}

clib2_utils_io_logger_result_t
clib2_utils_io_logger_log(FILE *restrict file, const char *restrict const title,
                          const char *restrict const description,
                          clib2_utils_io_logger_level_t level) {
  if (!file || !title || !description)
    return CLIB2_UTILS_IO_LOGGER_RESULT_INVALID;
  const char *stringified_level = stringify_level(level);
  const size_t title_len = strlen(title), description_len = strlen(description),
               stringified_level_len = strlen(stringified_level),
               total_bytes_needed = title_len + description_len +
                                    stringified_level_len + extra_fmt_chars + 1;
  char *buf = malloc(total_bytes_needed);
  if (!buf)
    return CLIB2_UTILS_IO_LOGGER_RESULT_MEM;

  mk_fmt(buf, title, title_len, description, description_len, stringified_level,
         stringified_level_len);

  size_t written = fwrite(buf, sizeof(char), total_bytes_needed - 1, file);
  if (written < (total_bytes_needed - 1)) {
    free(buf);
    return CLIB2_UTILS_IO_LOGGER_RESULT_WRITE;
  }

  free(buf);
  return CLIB2_UTILS_IO_LOGGER_RESULT_SUCCESS;
}
