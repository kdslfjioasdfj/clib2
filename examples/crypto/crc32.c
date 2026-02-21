#include "../../include/crypto/crc32.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

void *read_full_file(FILE *restrict f, size_t *restrict out) {
  if (!f || !out)
    return NULL;

  long pos = ftell(f);
  if (pos == -1L)
    return NULL;

  if (fseek(f, 0, SEEK_END) != 0)
    return NULL;
  long len = ftell(f);
  if (len == -1L)
    return NULL;

  if (fseek(f, 0, SEEK_SET) != 0)
    return NULL;

  void *buf = malloc((size_t)len);
  if (!buf)
    return NULL;

  size_t read = fread(buf, 1, (size_t)len, f);
  if (read != (size_t)len) {
    free(buf);
    return NULL;
  }

  *out = (size_t)len;
  return buf;
}

int main(int argc, char **argv) {
  if (argc == 1) {
    fputs("No filename given", stderr);
    return 1;
  }
  FILE *f = fopen(argv[1], "rb");
  if (!f) {
    fprintf(stderr, "Could not open file for reading: %s\n", argv[1]);
    return 1;
  }
  size_t bufsz;
  void *buf = read_full_file(f, &bufsz);
  fclose(f);
  if (!buf) {
    fputs("Could not read full file into memory", stderr);
    return 1;
  }
  uint32_t hash = 0xFFFFFFFF;
  bool success = clib2_crypto_crc32_hash(buf, bufsz, &hash);
  if (!success) {
    fputs("Could not hash file", stderr);
    free(buf);
    return 1;
  }
  printf("CRC32 hash of file: 0x%08" PRIX32 "\n", hash);
  free(buf);
  return 0;
}
