#include "../../include/crypto/rng.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

void test_basics(void) {
  // Seed values
  uint64_t seed1 = 1234567890123456;
  uint64_t seed2 = 6543210987654321;

  // Initialize the RNG
  clib2_crypto_rng_t *rng = clib2_crypto_rng_init(seed1, seed2);
  assert(rng != NULL); // Ensure the RNG was initialized successfully

  // Fetch initial random values
  int32_t first_int = clib2_crypto_rng_yield_i32(rng);
  uint32_t first_uint = clib2_crypto_rng_yield_u32(rng);

  // Generate the next random number and verify if state has changed
  clib2_crypto_rng_next(rng);

  int32_t second_int = clib2_crypto_rng_yield_i32(rng);
  uint32_t second_uint = clib2_crypto_rng_yield_u32(rng);

  // Ensure that the second values are different from the first values (state
  // change)
  assert(first_int != second_int); // Ensure the integer value changes
  assert(first_uint !=
         second_uint); // Ensure the unsigned integer value changes

  // Free the RNG instance
  clib2_crypto_rng_free(&rng);
  assert(rng == NULL); // Ensure the RNG is freed and pointer is NULL
}

int main(void) {
  test_basics();
  puts("Passed test[s]!");
  return 0;
}
