/**
 * @file crc32.h
 * @brief IEEE CRC32 implemented in C
 * @date 2026-02-21
 */

#ifndef CLIB2_CRYPTO_CRC32_H
#define CLIB2_CRYPTO_CRC32_H

#include "../shared.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Compute the IEEE CRC32 checksum of a memory buffer.
 * @param data The input buffer to checksum. Must not be NULL.
 * @param len The length of the buffer @p data
 * @param out The output of the checksum. Unmodified on failure. Must not be
 * NULL.
 * @return The success of the operation
 * @warning This is not a cryptographically secure checksum
 */
CLIB2_SHARED_PUBLIC bool clib2_crypto_crc32_hash(const void *restrict data,
                                                 size_t len,
                                                 uint32_t *restrict out);

/**
 * @brief Update a CRC32 checksum with new bytes
 * @param data The new data to account for. Must not be NULL.
 * @param len The length of the buffer @p data
 * @param prev_crc32 The previous checksum to update
 * @param out The output of the checksum. Unmodified on failure. Must not be
 * NULL.
 * @return The success of the operation
 * @warning This is not a cryptographically secure checksum
 */
CLIB2_SHARED_PUBLIC bool clib2_crypto_crc32_update(const void *restrict data,
                                                   size_t len,
                                                   uint32_t prev_crc32,
                                                   uint32_t *out);

#endif // CLIB2_CRYPTO_CRC32_H
