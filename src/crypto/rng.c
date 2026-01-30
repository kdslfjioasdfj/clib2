#include "../../include/crypto/rng.h"
#include <stdint.h>
#include <stdlib.h>

struct clib2_crypto_rng_s {
  uint64_t upper;
  uint64_t lower;
  uint32_t cur;
};

// The core implementation
static uint32_t next(uint64_t *upper, uint64_t *lower) {
  if (!upper || !lower)
    return 1;

  uint64_t s0 = *upper;
  uint64_t s1 = *lower;

  *upper = s1;
  s1 ^= s1 << 23;
  s1 ^= s0 ^ (s1 >> 17) ^ (s0 >> 26);
  *lower = s1;

  return (((uint32_t)((s0 + s1) & UINT32_MAX)));
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
  // I know this is valid but it is unavoidable
  if (!rng)
    return 0;

  return (int32_t)rng->cur;
}

uint32_t
clib2_crypto_rng_yield_u32(const clib2_crypto_rng_t *restrict const rng) {
  // I know this is valid but it is unavoidable
  if (!rng)
    return 0;

  return rng->cur;
}
