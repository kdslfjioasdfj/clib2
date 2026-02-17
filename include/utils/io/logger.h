/**
 * @file logger.h
 * @brief A human-readable event logger
 * @date 2026-02-07
 */

#ifndef CLIB2_UTILS_IO_LOGGER_H
#define CLIB2_UTILS_IO_LOGGER_H

#include "../../shared.h"
#include <stdio.h>

/**
 * @brief The level to log
 */
typedef enum clib2_utils_io_logger_level_e {
  CLIB2_UTILS_IO_LOGGER_LEVEL_INFO = 0,    ///< Generic info
  CLIB2_UTILS_IO_LOGGER_LEVEL_DEBUG = 1,   ///< Debug info
  CLIB2_UTILS_IO_LOGGER_LEVEL_WARNING = 2, ///< Warning info
  CLIB2_UTILS_IO_LOGGER_LEVEL_ERROR = 3,   ///< Error message
} clib2_utils_io_logger_level_t;

typedef enum clib2_utils_io_logger_result_e {
  CLIB2_UTILS_IO_LOGGER_RESULT_SUCCESS = 0, ///< Successful log write
  CLIB2_UTILS_IO_LOGGER_RESULT_INVALID = 1, ///< Recieved invalid parameter
  CLIB2_UTILS_IO_LOGGER_RESULT_WRITE = 3,   ///< Failure in write
} clib2_utils_io_logger_result_t;

/**
 * @brief Log to a file
 * @param file The log file to log into. Must be writable.
 * @param title The title of the error
 * @param description The description (brief) of the error
 * @param level The level of the error
 * @return The success of the operation
 * @warning If the write to the file fails, the file has been changed with a
 * potentially incomplete log
 */
CLIB2_SHARED_PUBLIC clib2_utils_io_logger_result_t
clib2_utils_io_logger_log(FILE *restrict file, const char *restrict const title,
                          const char *restrict const description,
                          clib2_utils_io_logger_level_t level);

#endif // CLIB2_UTILS_IO_LOGGER_H
