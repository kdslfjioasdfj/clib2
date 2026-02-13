/**
 * @file strbuilder.h
 * @brief A string builder type in C
 * @date 2026-02-12
 */

#ifndef CLIB2_UTILS_STR_STRBUILDER_H
#define CLIB2_UTILS_STR_STRBUILDER_H

#include "../../shared.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief The string builder type
 */
typedef struct clib2_utils_str_strbuilder_s clib2_utils_str_strbuilder_t;

/**
 * @brief Initialize a string builder
 * @return The valid string builder or NULL
 */
CLIB2_SHARED_PUBLIC clib2_utils_str_strbuilder_t *
clib2_utils_str_strbuilder_init(void);

/**
 * @brief Free a string builder
 * @param builder The string builder to free
 */
CLIB2_SHARED_PUBLIC void clib2_utils_str_strbuilder_free(
    clib2_utils_str_strbuilder_t *restrict *restrict builder);

/**
 * @brief Append a string to the string builder
 * @param builder The string builder to append to
 * @param str The string to append to the builder. Fails if given an empty
 * string. Assumes this string is null-terminated.
 * @return The success of the append.
 * @note Handles memory allocation for the caller.
 */
CLIB2_SHARED_PUBLIC bool clib2_utils_str_strbuilder_append(
    clib2_utils_str_strbuilder_t *restrict builder,
    const char *restrict const str);

/**
 * @brief Get the length of the string currently stored in the string builder
 * @param builder The string builder to fetch from
 * @return The length of the string. Note that 0 is an ambiguous value: if @p
 * builder == NULL, then the function returns 0. Check the validity of the
 * pointer before calling this function on it. Does not include null-terminator
 * (strlen semantics)
 */
CLIB2_SHARED_PUBLIC size_t clib2_utils_str_strbuilder_len(
    const clib2_utils_str_strbuilder_t *restrict const builder);

/**
 * @brief Clear a string builder
 * @param builder The string builder to clear
 */
CLIB2_SHARED_PUBLIC void clib2_utils_str_strbuilder_clear(
    clib2_utils_str_strbuilder_t *restrict builder);

/**
 * @brief Copy out the string stored in the string builder
 * @param builder The builder to copy out of
 * @param dst The buffer to copy into.
 * @param dstsz The total amount of writable bytes at @p dst
 * @return The amount of bytes written to @p dst NOT including null-terminator
 * @note Will always null-terminate @p dst if @p dstsz >= 1
 */
CLIB2_SHARED_PUBLIC size_t clib2_utils_str_strbuilder_copy(
    const clib2_utils_str_strbuilder_t *restrict builder, char *restrict dst,
    size_t dstsz);

#endif // CLIB2_UTILS_STR_STRBUILDER_H
