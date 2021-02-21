#include "n_internal.h"

#include <assert.h>

n_t n_make(long long mant, int exp) {
  assert( (mant == 0 && exp == 0) ||
          (ABS(mant) >= POW10_12 && ABS(mant) < POW10_13) );
  n_t n = { mant, exp };
  return n;
}
