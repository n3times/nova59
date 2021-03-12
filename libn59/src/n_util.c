#include "n_internal.h"

#include <assert.h>
#include <stdio.h>

n_t n_make(long long mant, int exp) {
  n_t n = { mant, exp };
  assert(n_is_number(n));

  return n;
}

bool n_equals(n_t n1, n_t n2) {
  return (n1.mant == n2.mant) && (n1.exp == n2.exp);
}

bool n_is_zero(n_t n) {
  return (n.mant == 0) && (n.exp == 0);
}

int n_cmp(n_t n1, n_t n2) {
  int n1_sign = (n1.mant > 0) - (n1.mant < 0);
  int n2_sign = (n2.mant > 0) - (n2.mant < 0);

  // Different signs.
  if (n1_sign > n2_sign) return 1;
  if (n1_sign < n2_sign) return -1;

  // Same sign.
  int ret;
  int e1 = n1.exp;
  int e2 = n2.exp;
  long long m1 = ABS(n1.mant);
  long long m2 = ABS(n2.mant);
  if (e1 > e2) {
    ret = 1;
  } else if (e1 < e2) {
    ret = -1;
  } else {
    if (m1 > m2) {
      ret = 1;
    } else if (m1 < m2) {
      ret = -1;
    } else {
      ret = 0;
    }
  }

  return n1_sign < 0 ? -ret : ret;
}

char *n_print(n_t n, char *str_out) {
  sprintf(str_out, "% 014lld% 03d", n.mant, n.exp);
  return str_out;
}
