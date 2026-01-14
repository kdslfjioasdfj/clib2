/**
 * @file rng.h
 * @brief A random number generator
 * @warning NO GUARANTEE ON CRYPTOGRAPHIC SECURITY
 * @note There are no promises on what the PRNG generator does.
 * @date 2026-01-14
 */

#ifndef CLIB2_CRYPTO_RNG_H
#define CLIB2_CRYPTO_RNG_H

#include "../shared.h"
#include <stdint.h>

/**
 * @brief The RNG type that stores intermediate state
 */
typedef struct clib2_crypto_rng_s clib2_crypto_rng_t;

/**
 * @brief Initialize RNG state
 * @param seed1 The first 64-bits of the seed
 * @param seed2 The second 64-bits of the seed
 * @return A pointer to the RNG state
 */
CLIB2_SHARED_PUBLIC clib2_crypto_rng_t *clib2_crypto_rng_init(uint64_t seed1,
                                                              uint64_t seed2);

/**
 * @brief Free a RNG instance
 * @param rng The RNG instance to free
 */
CLIB2_SHARED_PUBLIC void
clib2_crypto_rng_free(clib2_crypto_rng_t *restrict *restrict rng);

/**
 * @brief Generate and store the next random integer in the RNG state
 * @param rng The RNG state to randomize
 */
CLIB2_SHARED_PUBLIC void
clib2_crypto_rng_next(clib2_crypto_rng_t *restrict rng);

/**
 * @brief Get the value stored in state as a 32-bit signed integer
 * @param rng The RNG state to fetch from
 * @return The value stored in state as a 32-bit signed integer
 */
CLIB2_SHARED_PUBLIC int32_t
clib2_crypto_rng_yield_i32(const clib2_crypto_rng_t *restrict const rng);

/**
 * @brief Get the value stored in state as a 32-bit unsigned integer
 * @param rng The RNG state to fetch from
 * @return The value stored in state as a 32-bit unsigned integer
 */
CLIB2_SHARED_PUBLIC uint32_t
clib2_crypto_rng_yield_u32(const clib2_crypto_rng_t *restrict const rng);

#endif // CLIB2_CRYPTO_RNG_H
