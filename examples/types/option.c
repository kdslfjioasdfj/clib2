#include "../../include/types/option.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

clib2_types_option_t safe_malloc(size_t s) {
  if (!s)
    return clib2_types_option_none();
  void *mem = malloc(s);
  if (!mem)
    return clib2_types_option_none();
  return clib2_types_option_some(mem);
}

int main(void) {
  puts("Trying to allocate 32 bytes of memory:");
  clib2_types_option_t mem = safe_malloc(32);
  if (!mem.is_some) {
    fputs("Could not allocate 32 bytes of memory", stderr);
    return 1;
  } else {
    puts("Allocated 32 bytes of memory successfully!");
    uint8_t *ptr = clib2_types_option_unwrap(mem);
    puts("Using memory:");
    for (uint8_t i = 0; i < 32; i++)
      ptr[i] = i + 1;
    for (uint8_t i = 0; i < 32; i++)
      printf("[Index %u]: [Address %p], [Value %u]\n", i, &(ptr[i]), ptr[i]);
    free(ptr); // Or free(mem.val)
  }
  return 0;
}
