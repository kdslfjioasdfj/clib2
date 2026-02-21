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

void test_incremental_ieee_compliance(void) {
  const char buf1[4] = {'1', '2', '3', '4'};
  const char buf2[5] = {'5', '6', '7', '8', '9'};
  const char total[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

  uint32_t hash_incremental = 0;
  bool buf1_hash_ok =
      clib2_crypto_crc32_hash(buf1, sizeof(buf1), &hash_incremental);
  bool buf2_hash_increment_ok = clib2_crypto_crc32_update(
      buf2, sizeof(buf2), hash_incremental, &hash_incremental);
  assert(buf1_hash_ok && buf2_hash_increment_ok);

  uint32_t hash_total = 0;
  bool total_hash_ok =
      clib2_crypto_crc32_hash(total, sizeof(total), &hash_total);
  assert(total_hash_ok && hash_incremental == hash_total &&
         hash_total == 0xCBF43926);
}

int main(void) {
  test_ieee_compliance();
  test_incremental_ieee_compliance();
  puts("Passed test[s]!");
  return 0;
}
