#include "../../include/crypto/crc32.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

void test_ieee_compliance(void) {
  const char buf[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
  uint32_t buf_checksum_crc32 = 0;
  bool buf_hash_ok =
      clib2_crypto_crc32_hash(buf, sizeof(buf), &buf_checksum_crc32);
  assert(buf_hash_ok && buf_checksum_crc32 == 0xCBF43926);

  uint32_t empty_hash = 0xFFFFFFFF;
  char in = '\0';
  bool empty = clib2_crypto_crc32_hash(&in, 0, &empty_hash);
  assert(empty && empty_hash == 0);
}

int main(void) {
  test_ieee_compliance();
  puts("Passed test[s]!");
  return 0;
}
