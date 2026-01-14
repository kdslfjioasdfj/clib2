#include "../../include/crypto/rng.h"
#include <stdint.h>
#include <stdlib.h>

static const uint64_t non0const = 0xC0015EDF00D53141;

struct clib2_crypto_rng_s {
  uint64_t upper;
  uint64_t lower;
  uint32_t cur;
};

// The core implementation
static uint32_t next(uint64_t *upper, uint64_t *lower) {
  if (!upper || !lower)
    return 1;

  uint64_t s0 = (*upper) ? *upper : non0const;
  uint64_t s1 = (*lower) ? *lower : non0const;

  *upper = s1;
  s1 ^= s1 << 23;
  s1 ^= s0 ^ (s1 >> 17) ^ (s0 >> 26);
  *lower = s1;

  // The return is a bit different from normal XorShift128+.
  // This shouldn't hurt XorShift128+'s benefits, just differentiate.
  return (~((uint32_t)((s0 + s1) & UINT32_MAX)));
}

clib2_crypto_rng_t *clib2_crypto_rng_init(uint64_t seed1, uint64_t seed2) {
  clib2_crypto_rng_t *rng = malloc(sizeof(clib2_crypto_rng_t));
  if (!rng)
    return NULL;

  rng->cur = 0;
  rng->upper = seed1;
  rng->lower = seed2;

  return rng;
}

void clib2_crypto_rng_free(clib2_crypto_rng_t *restrict *restrict rng) {
  if (!rng || !*rng)
    return;
  free(*rng);
  *rng = NULL;
}

void clib2_crypto_rng_next(clib2_crypto_rng_t *restrict rng) {
  if (!rng)
    return;

  // NOTE: This line changes every element in rng
  rng->cur = next(&(rng->upper), &(rng->lower));
}

int32_t
clib2_crypto_rng_yield_i32(const clib2_crypto_rng_t *restrict const rng) {
  // 0 is not valid RNG but how is the user supposed to know that?
  if (!rng)
    return 0;

  return (int32_t)rng->cur;
}

uint32_t
clib2_crypto_rng_yield_u32(const clib2_crypto_rng_t *restrict const rng) {
  // 0 is not valid RNG but how is the user supposed to know that?
  if (!rng)
    return 0;

  return rng->cur;
}
