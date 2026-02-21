#include "../../include/crypto/crc32.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 8192

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "rb");
  if (!file) {
    perror("Failed to open file");
    return 1;
  }

  uint8_t buffer[BUFFER_SIZE];
  uint32_t crc_result;
  size_t bytes_read = 0;
  bool initialized = false;

  while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
    if (!initialized) {
      if (!clib2_crypto_crc32_hash(buffer, bytes_read, &crc_result)) {
        fprintf(stderr, "Initial CRC calculation failed\n");
        fclose(file);
        return 1;
      }
      initialized = true;
    } else {
      if (!clib2_crypto_crc32_update(buffer, bytes_read, crc_result,
                                     &crc_result)) {
        fprintf(stderr, "CRC update failed\n");
        fclose(file);
        return 1;
      }
    }
  }

  if (ferror(file)) {
    perror("Error reading file");
    fclose(file);
    return 1;
  }

  fclose(file);

  printf("%08X  %s\n", crc_result, argv[1]);

  return 0;
}