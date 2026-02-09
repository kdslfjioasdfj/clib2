/**
 * @file extra.h
 * @brief Extra, non-standard string helpers
 * @date 2026-02-08
 */

#ifndef CLIB2_UTILS_STR_EXTRA_H
#define CLIB2_UTILS_STR_EXTRA_H

#include "../../shared.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Convert an ASCII string into an int64_t integer
 * @param ascii The string to convert to an integer
 * @param out Where the value will be stored on success (will not be modified on
 * failure)
 * @return The success of the operation
 * @details
 * - Base prefixes are parsed in this order (1 to 4, ascending):
 * 1. Hexadecimal prefix check: If the string starts with "0x" or "0X", it is
 * treated as a hexadecimal value
 * 2. Binary prefix check: If the string starts with "0b" or "0B", it is treated
 * as a binary value
 * 3. Octal prefix check: If the string starts with "0o", "0O" or "0" it is
 * treated as an octal value
 * 4. Default to decimal: If the string starts with none of these, it is treated
 * as a decimal value
 * - Sign character check is done in this order (1 to 3, ascending):
 * 1. Negative prefix check: If the string starts with "-", it is treated as a
 * negative value
 * 2. Positive prefix check: If the string starts with "+", it is treated as a
 * positive value
 * 3. Default to positive: If the string does not start with either prefix, it
 * is treated as a positive value
 * - Parsing slack (whitespace tolerance, string consumption):
 * 1. The function does NOT allow leading or trailing whitespace
 * 2. The function reads the entire string assuming there is no overflow. If any
 * overflow is detected, the function fails, returning false and leaving @p out
 * unchanged. Same condition is true with underflows.
 * - VERY IMPORTANT
 * 1. Sign is parsed BEFORE base prefix
 */
CLIB2_SHARED_PUBLIC bool
clib2_utils_str_extra_atoi64(const char *restrict ascii,
                             int64_t *restrict const out);

/**
 * @brief Convert an ASCII string into an uint64_t integer
 * @param ascii The string to convert to an integer
 * @param out Where the value will be stored on success (will not be modified on
 * failure)
 * @return The success of the operation
 * @details
 * - Base prefixes are parsed in this order (1 to 4, ascending):
 * 1. Hexadecimal prefix check: If the string starts with "0x" or "0X", it is
 * treated as a hexadecimal value
 * 2. Binary prefix check: If the string starts with "0b" or "0B", it is treated
 * as a binary value
 * 3. Octal prefix check: If the string starts with "0o", "0O" or "0" it is
 * treated as an octal value
 * 4. Default to decimal: If the string starts with none of these, it is treated
 * as a decimal value
 * - Sign character check is done in this order (1 to 3, ascending):
 * 1. Negative prefix check: If the string starts with "-", it is treated as an
 * error. The function returns false and @p out is unchanged
 * 2. Positive prefix check: If the string starts with "+", it is treated as a
 * positive value
 * 3. Default to positive: If the string does not start with either prefix, it
 * is treated as a positive value
 * - Parsing slack (whitespace tolerance, string consumption):
 * 1. The function does NOT allow leading or trailing whitespace
 * 2. The function reads the entire string assuming there is no overflow. If any
 * overflow is detected, the function fails, returning false and leaving @p out
 * unchanged. Underflows are impossible.
 * - VERY IMPORTANT
 * 1. Sign is parsed BEFORE base prefix
 * 2. "-0" is INVALID
 */
CLIB2_SHARED_PUBLIC bool
clib2_utils_str_extra_atou64(const char *restrict ascii,
                             uint64_t *restrict const out);

#endif // CLIB2_UTILS_STR_EXTRA_H
