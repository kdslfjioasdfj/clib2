/**
 * @file compatibility.h
 * @brief Compatibility functions for strings (e.g. strdup, strlcpy)
 * @date 2025-12-29
 */

#ifndef CLIB2_UTILS_STR_COMPATIBILITY_H
#define CLIB2_UTILS_STR_COMPATIBILITY_H

#include "../../shared.h"
#include <stddef.h>

/**
 * @brief Duplicate a string
 * @param str The string to duplicate
 * @return The pointer to the new string (on heap)
 * @note Like in classic POSIX strdup, you need to free the pointer returned
 * manually, it is not freed automatically
 */
CLIB2_SHARED_PUBLIC char *
clib2_utils_str_compatibility_strdup(const char *restrict const str);

/**
 * @brief Copy a string safely.
 * @param dest The destination to copy into
 * @param src The source to copy from
 * @param dstsz The amount of bytes to copy, including the null-terminator
 * @return The size of @p src (to check for truncation, refer to example)
 * @code {.c}
 * char *src = "Hello, World!"; // 13 character string
 * char dest[10]; // 10 characters only
 * size_t x = clib2_utils_str_compatibility_strlcpy(dest, src, sizeof(dest));
 * if (x >= sizeof(dest)) { // String was truncated!
 *   // NOTE: dest is null-terminated because sizeof(dest) > 0!
 *   // Handle the truncation issue
 * }
 * @endcode
 * @note This function guarantees null-termination of @p dest when @p dstsz > 0
 */
CLIB2_SHARED_PUBLIC size_t clib2_utils_str_compatibility_strlcpy(
    char *restrict dest, const char *restrict const src, size_t dstsz);

#endif // CLIB2_UTILS_STR_COMPATIBILITY_H
