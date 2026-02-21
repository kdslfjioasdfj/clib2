#include "../../include/crypto/crc32.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 1024 // 1 KB

int main(int argc, char **argv) {
  if (argc < 2) {
    fputs("No filename given\n", stderr);
    return 1;
  }

  FILE *f = fopen(argv[1], "rb");
  if (!f) {
    fprintf(stderr, "Could not open file for reading: %s\n", argv[1]);
    return 1;
  }

  // Incremental hash
  uint32_t hash_incremental = 0xFFFFFFFF;

  // For one-shot hash, read entire file
  if (fseek(f, 0, SEEK_END) != 0) {
    fputs("Failed to seek file end\n", stderr);
    fclose(f);
    return 1;
  }
  long len = ftell(f);
  if (len == -1L) {
    fputs("Failed to get file size\n", stderr);
    fclose(f);
    return 1;
  }
  if (fseek(f, 0, SEEK_SET) != 0) {
    fputs("Failed to seek file start\n", stderr);
    fclose(f);
    return 1;
  }

  void *full_buf = malloc((size_t)len);
  if (!full_buf) {
    fputs("Memory allocation failed\n", stderr);
    fclose(f);
    return 1;
  }

  size_t total_read = 0;
  while (!feof(f)) {
    uint8_t chunk[CHUNK_SIZE];
    size_t n = fread(chunk, 1, CHUNK_SIZE, f);
    if (n == 0 && ferror(f)) {
      fputs("Error reading file\n", stderr);
      free(full_buf);
      fclose(f);
      return 1;
    }

    // Incremental hash update
    if (!clib2_crypto_crc32_update(chunk, n, hash_incremental,
                                   &hash_incremental)) {
      fputs("Incremental hash failed\n", stderr);
      free(full_buf);
      fclose(f);
      return 1;
    }

    // Copy into full buffer for one-shot hash
    if (total_read + n > (size_t)len) {
      fputs("Read size mismatch\n", stderr);
      free(full_buf);
      fclose(f);
      return 1;
    }
    memcpy((uint8_t *)full_buf + total_read, chunk, n);
    total_read += n;
  }

  fclose(f);

  // Compute full hash
  uint32_t hash_full = 0;
  if (!clib2_crypto_crc32_hash(full_buf, (size_t)len, &hash_full)) {
    fputs("One-shot hash failed\n", stderr);
    free(full_buf);
    return 1;
  }

  printf("Incremental CRC32: 0x%08" PRIX32 "\n", hash_incremental);
  printf("Full CRC32       : 0x%08" PRIX32 "\n", hash_full);

  // Verify they match
  if (hash_incremental != hash_full) {
    fputs("Hashes do not match!\n", stderr);
    free(full_buf);
    return 1;
  }

  free(full_buf);
  return 0;
}