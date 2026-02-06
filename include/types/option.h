/**
 * @file option.h
 * @brief An optional type for C
 * @date 2026-02-05
 */

#ifndef CLIB2_TYPES_OPTION_H
#define CLIB2_TYPES_OPTION_H

#include "../shared.h"
#include <stdbool.h>

/**
 * @brief The option type
 * @details
 * Members:
 * - bool is_some: Whether val holds a value/is valid.
 * - void *val: The value stored
 * Notes:
 * If is_some == true, val contains the value
 * If is_some == false, val does not contain a value
 */
typedef struct clib2_types_option_s {
  bool is_some;
  void *val;
} clib2_types_option_t;

/**
 * @brief Get an option with value @p val and `self.is_some` set to true
 * @param val The value to put into the option
 * @return An option with the value @p val and `self.is_some` set to true
 */
CLIB2_SHARED_PUBLIC clib2_types_option_t clib2_types_option_some(void *val);

/**
 * @brief Get an option with no value
 * @return An option with no value (NULL by default) and `self.is_some` set to
 * false
 */
CLIB2_SHARED_PUBLIC clib2_types_option_t clib2_types_option_none(void);

/**
 * @brief Return the stored value or panic
 * @param opt The option to fetch from
 * @return The stored value
 * @note If @p opt.is_some does not store a value, the program crashes with
 * `abort()` immediately.
 */
CLIB2_SHARED_PUBLIC void *clib2_types_option_unwrap(clib2_types_option_t opt);

#endif // CLIB2_TYPES_OPTION_H
