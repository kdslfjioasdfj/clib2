#include "../../../include/utils/str/extra.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

void test_i64_zero(void) {
  int64_t v = 123;
  assert(clib2_utils_str_extra_atoi64("0", &v));
  assert(v == 0);
}

void test_i64_negative_zero(void) {
  int64_t v = 123;
  assert(clib2_utils_str_extra_atoi64("-0", &v));
  assert(v == 0);
}

void test_i64_int64_max(void) {
  int64_t v = 0;
  assert(clib2_utils_str_extra_atoi64("9223372036854775807", &v));
  assert(v == INT64_MAX);
}

void test_i64_int64_min(void) {
  int64_t v = 0;
  assert(clib2_utils_str_extra_atoi64("-9223372036854775808", &v));
  assert(v == INT64_MIN);
}

void test_i64_positive_overflow(void) {
  int64_t v = 42;
  assert(!clib2_utils_str_extra_atoi64("9223372036854775808", &v));
  assert(v == 42);
}

void test_i64_negative_overflow(void) {
  int64_t v = 42;
  assert(!clib2_utils_str_extra_atoi64("-9223372036854775809", &v));
  assert(v == 42);
}

void test_i64_hex_min(void) {
  int64_t v = 0;
  assert(clib2_utils_str_extra_atoi64("-0x8000000000000000", &v));
  assert(v == INT64_MIN);
}

void test_i64_hex_overflow(void) {
  int64_t v = 7;
  assert(!clib2_utils_str_extra_atoi64("0x8000000000000000", &v));
  assert(v == 7);
}

void test_u64_zero(void) {
  uint64_t v = 123;
  assert(clib2_utils_str_extra_atou64("0", &v));
  assert(v == 0);
}

void test_u64_uint64_max(void) {
  uint64_t v = 0;
  assert(clib2_utils_str_extra_atou64("18446744073709551615", &v));
  assert(v == UINT64_MAX);
}

void test_u64_overflow(void) {
  uint64_t v = 9;
  assert(!clib2_utils_str_extra_atou64("18446744073709551616", &v));
  assert(v == 9);
}

void test_u64_negative_rejected(void) {
  uint64_t v = 99;
  assert(!clib2_utils_str_extra_atou64("-1", &v));
  assert(v == 99);
}

void test_u64_negative_zero_rejected(void) {
  uint64_t v = 99;
  assert(!clib2_utils_str_extra_atou64("-0", &v));
  assert(v == 99);
}

void test_binary_parse(void) {
  int64_t v = 0;
  assert(clib2_utils_str_extra_atoi64("0b101010", &v));
  assert(v == 42);
}

void test_octal_parse(void) {
  int64_t v = 0;
  assert(clib2_utils_str_extra_atoi64("052", &v));
  assert(v == 42);
}

void test_octal_invalid_digit(void) {
  int64_t v = 123;
  assert(!clib2_utils_str_extra_atoi64("09", &v));
  assert(v == 123);
}

void test_hex_invalid_digit(void) {
  int64_t v = 123;
  assert(!clib2_utils_str_extra_atoi64("0xFG", &v));
  assert(v == 123);
}

void test_leading_whitespace_rejected(void) {
  int64_t v = 1;
  assert(!clib2_utils_str_extra_atoi64(" 1", &v));
  assert(v == 1);
}

void test_trailing_whitespace_rejected(void) {
  int64_t v = 1;
  assert(!clib2_utils_str_extra_atoi64("1 ", &v));
  assert(v == 1);
}

void test_trailing_junk_rejected(void) {
  int64_t v = 1;
  assert(!clib2_utils_str_extra_atoi64("123abc", &v));
  assert(v == 1);
}

void test_empty_string_rejected(void) {
  int64_t v = 5;
  assert(!clib2_utils_str_extra_atoi64("", &v));
  assert(v == 5);
}

void test_prefix_only_rejected(void) {
  int64_t v = 5;
  assert(!clib2_utils_str_extra_atoi64("-", &v));
  assert(!clib2_utils_str_extra_atoi64("+", &v));
  assert(!clib2_utils_str_extra_atoi64("0x", &v));
  assert(v == 5);
}

int main(void) {
  test_i64_zero();
  test_i64_negative_zero();
  test_i64_int64_max();
  test_i64_int64_min();
  test_i64_positive_overflow();
  test_i64_negative_overflow();
  test_i64_hex_min();
  test_i64_hex_overflow();
  test_u64_zero();
  test_u64_uint64_max();
  test_u64_overflow();
  test_u64_negative_rejected();
  test_u64_negative_zero_rejected();
  test_binary_parse();
  test_octal_parse();
  test_octal_invalid_digit();
  test_hex_invalid_digit();
  test_leading_whitespace_rejected();
  test_trailing_whitespace_rejected();
  test_trailing_junk_rejected();
  test_empty_string_rejected();
  test_prefix_only_rejected();
  puts("Passed test[s]!");
  return 0;
}
