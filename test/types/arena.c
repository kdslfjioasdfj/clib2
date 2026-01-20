#include "../../include/types/arena.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

void test_basics(void) {
  // This tests:
  // 1. If allocations are of sufficient size
  // 2. Consistent error behavior
  // 3. Consistent init/destroy behavior

  clib2_types_arena_t *a =
      clib2_types_arena_init(sizeof(int) * 8, sizeof(int), true);
  assert(a != NULL);

  volatile int *ints2 = clib2_types_arena_alloc(a, sizeof(int) * 2);
  assert(ints2 != NULL);

  // Use ints2; if it segfaults, there was an error
  ints2[0] = 67;
  ints2[1] = 0x67;

  int *not_enough_memory = clib2_types_arena_alloc(a, sizeof(int) * 3000);
  assert(not_enough_memory == NULL);

  volatile int *remainder = clib2_types_arena_alloc(a, sizeof(int) * 6);
  assert(remainder != NULL);

  // Use remainder; if it segfaults, there was an error
  for (int i = 0; i < 6; i++)
    remainder[i] = i + 1;

  int *not_enough_memory2 = clib2_types_arena_alloc(a, sizeof(int));
  assert(not_enough_memory2 == NULL);

  clib2_types_arena_destroy(&a);
  assert(a == NULL);
}

void test_aligned(void) {
  // This solely exists to see if memory is aligned.
  clib2_types_arena_t *a =
      clib2_types_arena_init(sizeof(int) * 8, sizeof(int), true);
  assert(a != NULL);

  int *mem1 = clib2_types_arena_alloc(a, (sizeof(int) * 4) - 1);
  assert(mem1 != NULL);
  mem1[1] = 0xFF; // This would be out-of-bounds, but because of alignment, it
                  // shouldn't cause a segfault

  uint8_t *mem2 = clib2_types_arena_alloc(a, sizeof(int));
  assert(mem2 != NULL);
  assert(mem2[0] == 0);

  clib2_types_arena_destroy(&a);
}

int main(void) {
  test_basics();
  test_aligned();
  puts("Passed test[s]!");
  return 0;
}
