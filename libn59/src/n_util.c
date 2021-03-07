#include "n_internal.h"

#include <assert.h>
#include <stdio.h>

#if !NDEBUG
static bool n_is_number(long long mant, int exp) {
  return ( (mant == 0 && exp == 0) ||
           (ABS(mant) >= POW10_12 && ABS(mant) < POW10_13 && ABS(exp) <= 99) );
}
#endif

n_t n_make(long long mant, int exp) {
  assert(n_is_number(mant, exp));

  return (n_t) { mant, exp };
}

bool n_equals(n_t n1, n_t n2) {
  return (n1.mant == n2.mant) && (n1.exp == n2.exp);
}

bool n_is_zero(n_t n) {
  return (n.mant == 0) && (n.exp == 0);
}

int n_cmp(n_t n1, n_t n2) {
  double d1 = n_n2d(n1);
  double d2 = n_n2d(n2);

  if (d1 > d2) return 1;
  if (d1 < d2) return -1;
  return 0;
}

char *n_print(n_t n, char *str_out) {
  sprintf(str_out, "% 014lld% 03d", n.mant, n.exp);
  return str_out;
}
