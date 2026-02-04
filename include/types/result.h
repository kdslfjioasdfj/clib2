/**
 * @file result.h
 * @brief A result type which can hold an error or a value
 * @date 2026-02-04
 */

#ifndef CLIB2_TYPES_RESULT_H
#define CLIB2_TYPES_RESULT_H

#include "../shared.h"
#include <stdbool.h>

/**
 * @brief The result type
 * @details
 * Members:
 * - bool ok: Whether the object holds an error value or a success value.
 * - void *success: The success value specified if ok == true.
 * - void *err: The error value specified if ok == false.
 * Notes:
 * 1. If ok == false, success is not specified (it is an error).
 * 2. If ok == true, err is not specified (it is a success value).
 * 3. success and error are not owned by the object or library - the user needs
 * to free them manually.
 */
typedef struct clib2_types_result_s {
  bool ok;
  union {
    void *success;
    void *err;
  };
} clib2_types_result_t;

/**
 * @brief Set the success value to @p success and set `self.ok` to true
 * @param success The success value
 * @return The result object with @p as the success value and `self.ok` as true.
 */
CLIB2_SHARED_PUBLIC clib2_types_result_t clib2_types_result_ok(void *success);

/**
 * @brief Set the error value to @p err and set `self.ok` to false
 * @param err The error value
 * @return The result object with @p as the error value and `self.ok` as false.
 */
CLIB2_SHARED_PUBLIC clib2_types_result_t clib2_types_result_err(void *err);

#endif // CLIB2_TYPES_RESULT_H
