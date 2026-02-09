#include "../../../include/utils/str/extra.h"
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

// u64 helpers
static inline bool u64_does_mul_overflow(uint64_t a, uint64_t b) {
  if (a == 0 || b == 0)
    return false;
  return a > UINT64_MAX / b;
}

static inline bool u64_does_add_overflow(uint64_t a, uint64_t b) {
  return a > UINT64_MAX - b;
}

// i64 helpers
static inline uint64_t i64_to_positive(int64_t a) {
  if (a > 0)
    return (uint64_t)a;
  else if (a == INT64_MIN)
    return ((uint64_t)INT64_MAX) + 1;
  return (uint64_t)(-a);
}

static inline bool i64_does_mul_overflow(int64_t a, int64_t b) {
  // Multiplication by 0 is always safe
  if (a == 0 || b == 0)
    return false;

  uint64_t ua = i64_to_positive(a);
  uint64_t ub = i64_to_positive(b);

  // If |a| > INT64_MAX / |b| → overflow
  uint64_t limit =
      (a < 0 || b < 0) ? ((uint64_t)INT64_MAX + 1) : (uint64_t)INT64_MAX;

  return ua > limit / ub;
}

static inline bool i64_does_add_overflow(int64_t a, int64_t b) {
  if (b > 0)
    return a > INT64_MAX - b;
  else if (b < 0)
    return a < INT64_MIN - b;
  return false;
}

// String helpers
static inline int str_digit_value(char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  return -1;
}

bool clib2_utils_str_extra_atoi64(const char *restrict ascii,
                                  int64_t *restrict const out) {
  if (!ascii || !*ascii)
    return false;

  if (ascii[0] == '0' && ascii[1] == '\0') {
    *out = 0;
    return true;
  }

  const char *p = ascii;
  bool negative = false;
  if (*p == '-') {
    negative = true;
    ++p;
    if (p[0] == '0' && p[1] == '\0') {
      *out = 0;
      return true;
    }
  } else if (*p == '+') {
    ++p;
    if (p[0] == '0' && p[1] == '\0') {
      *out = 0;
      return true;
    }
  }

  if (!*p)
    return false;

  int base = 10;
  if (p[0] == '0') {
    if (p[1] == 'x' || p[1] == 'X') {
      base = 16;
      p += 2;
    } else if (p[1] == 'b' || p[1] == 'B') {
      base = 2;
      p += 2;
    } else if (p[1] == 'o' || p[1] == 'O') {
      base = 8;
      p += 2;
    } else {
      base = 8;
      p += 1;
    }
  }

  if (!*p)
    return false;

  int64_t value = 0;

  for (; *p; ++p) {
    int d = str_digit_value(*p);
    if (d < 0 || d >= base)
      return false;

    if (i64_does_mul_overflow(value, base))
      return false;

    int64_t mul = value * base;

    int64_t add = negative ? -d : d;
    if (i64_does_add_overflow(mul, add))
      return false;

    value = mul + add;
  }

  *out = value;
  return true;
}

bool clib2_utils_str_extra_atou64(const char *restrict ascii,
                                  uint64_t *restrict const out) {
  if (!ascii || !*ascii)
    return false;

  if (ascii[0] == '0' && ascii[1] == '\0') {
    *out = 0;
    return true;
  }

  const char *p = ascii;

  if (*p == '-') {
    return false;
  } else if (*p == '+') {
    ++p;
    if (p[0] == '0' && p[1] == '\0') {
      *out = 0;
      return true;
    }
  }

  if (!*p)
    return false;

  int base = 10;
  if (p[0] == '0') {
    if (p[1] == 'x' || p[1] == 'X') {
      base = 16;
      p += 2;
    } else if (p[1] == 'b' || p[1] == 'B') {
      base = 2;
      p += 2;
    } else if (p[1] == 'o' || p[1] == 'O') {
      base = 8;
      p += 2;
    } else {
      base = 8;
      p += 1;
    }
  }

  if (!*p)
    return false;

  uint64_t value = 0;

  for (; *p; ++p) {
    int d = str_digit_value(*p);
    if (d < 0 || d >= base)
      return false;

    if (u64_does_mul_overflow(value, (uint64_t)base))
      return false;

    uint64_t mul = value * (uint64_t)base;

    if (u64_does_add_overflow(mul, (uint64_t)d))
      return false;

    value = mul + (uint64_t)d;
  }

  *out = value;
  return true;
}
