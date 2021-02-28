#include "n_internal.h"

#include <assert.h>

#if !NDEBUG
static bool n_is_number(long long mant, int exp) {
  return ( (mant == 0 && exp == 0) ||
           (ABS(mant) >= POW10_12 && ABS(mant) < POW10_13) );
}
#endif

n_t n_make(long long mant, int exp) {
  assert(n_is_number(mant, exp));
  n_t n = { mant, exp };
  return n;
}

bool n_equals(n_t n1, n_t n2) {
  return (n1.mant == n2.mant) && (n1.exp == n2.exp);
}

bool n_is_zero(n_t n) {
  return (n.mant == 0) && (n.exp == 0);
}
