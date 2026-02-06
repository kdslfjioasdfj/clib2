#include "../../include/crypto/rng.h"
#include <stdint.h>
#include <stdlib.h>

// Uses: SipHash-DRBG

struct clib2_crypto_rng_s {
  uint64_t upper;
  uint64_t lower;
  uint64_t counter;
  uint32_t cur;
};

//! ONLY WORKS ON 64-BIT INTEGERS
#define ROTL(x, b) (((x) << (b)) | ((x) >> (64 - (b))))

// This is SipHash
static uint64_t siphash24_u64(uint64_t msg, uint64_t k0, uint64_t k1) {
  uint64_t v0 = 0x736f6d6570736575ULL ^ k0;
  uint64_t v1 = 0x646f72616e646f6dULL ^ k1;
  uint64_t v2 = 0x6c7967656e657261ULL ^ k0;
  uint64_t v3 = 0x7465646279746573ULL ^ k1;
  v3 ^= msg;
  for (int i = 0; i < 2; i++) {
    v0 += v1;
    v1 = ROTL(v1, 13);
    v1 ^= v0;
    v0 = ROTL(v0, 32);
    v2 += v3;
    v3 = ROTL(v3, 16);
    v3 ^= v2;
    v0 += v3;
    v3 = ROTL(v3, 21);
    v3 ^= v0;
    v2 += v1;
    v1 = ROTL(v1, 17);
    v1 ^= v2;
    v2 = ROTL(v2, 32);
  }
  v0 ^= msg;
  v2 ^= 0xff;
  for (int i = 0; i < 4; i++) {
    v0 += v1;
    v1 = ROTL(v1, 13);
    v1 ^= v0;
    v0 = ROTL(v0, 32);
    v2 += v3;
    v3 = ROTL(v3, 16);
    v3 ^= v2;
    v0 += v3;
    v3 = ROTL(v3, 21);
    v3 ^= v0;
    v2 += v1;
    v1 = ROTL(v1, 17);
    v1 ^= v2;
    v2 = ROTL(v2, 32);
  }
  return v0 ^ v1 ^ v2 ^ v3;
}

// The core implementation
// counter is volatile because I don't want `*counter += 1` to be optimized out
static uint32_t next(uint64_t *upper, uint64_t *lower,
                     volatile uint64_t *counter) {
  if (!upper || !lower)
    return 0; // This is an unavoidable compromise
  uint64_t hash = siphash24_u64(*counter, *upper, *lower);
  *counter += 1;
  return (hash & UINT32_MAX);
}

clib2_crypto_rng_t *clib2_crypto_rng_init(uint64_t seed1, uint64_t seed2) {
  clib2_crypto_rng_t *rng = malloc(sizeof(clib2_crypto_rng_t));
  if (!rng)
    return NULL;

  rng->cur = 0;
  rng->upper = seed1;
  rng->lower = seed2;
  rng->counter = 0;

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
  rng->cur = next(&(rng->upper), &(rng->lower), &(rng->counter));
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
