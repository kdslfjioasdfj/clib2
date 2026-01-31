#include "../../include/crypto/rng.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// DO NOT DO THIS, use another way to get seeds.
// THIS IS JUST FOR THE EXAMPLE
uint64_t seeds[2] = {102, 99};

void do_stuff(uint64_t random_number_u, int64_t random_number_i) {
  printf("Unsigned random value: %" PRIu64 "\n", random_number_u);
  printf("Signed random value: %" PRId64 "\n", random_number_i);
}

// This function needs random numbers
void some_function_that_needs_random_numbers(void) {
  clib2_crypto_rng_t *my_rng = clib2_crypto_rng_init(seeds[0], seeds[1]);
  if (!my_rng) {
    // Could not initialize an RNG
    exit(1); // Exit with some error
  }

  // Get the next random number
  clib2_crypto_rng_next(my_rng);

  // Get the values
  int64_t random_int = clib2_crypto_rng_yield_i32(my_rng);
  uint64_t random_uint = clib2_crypto_rng_yield_u32(my_rng);

  // Do something with the values
  do_stuff(random_uint, random_int);

  // Get the next random number
  clib2_crypto_rng_next(my_rng);

  // Get the values
  random_int = clib2_crypto_rng_yield_i32(my_rng);
  random_uint = clib2_crypto_rng_yield_u32(my_rng);

  // Do something with the new values
  do_stuff(random_uint, random_int);

  clib2_crypto_rng_free(&my_rng); // Destroy the RNG
}

int main(void) {
  some_function_that_needs_random_numbers();
  return 0;
}
