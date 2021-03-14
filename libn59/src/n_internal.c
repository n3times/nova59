#include "n_internal.h"

#include <math.h>

n_t normalize_number(long long mant, int exp, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  if (mant == 0) return N_0;

  // Make sure the mantissa has exactly 13 digits.
  while (ABS(mant) >= POW10_13) {
    mant /= 10;
    if (exp < 100) exp += 1;
  }
  while (ABS(mant) < POW10_12) {
    mant *= 10;
    if (exp > -100) exp -= 1;
  }

  // Overflow.
  if (ABS(exp) > 99) {
    if (err) {
      *err = exp < 0 ? N_ERR_UNDERFLOW : N_ERR_OVERFLOW;
    }

    if (exp < 0) {
      return mant < 0 ? n_chs(N_EPS) : N_EPS;
    } else {
      return mant < 0 ? n_chs(N_INF) : N_INF;
    }
  }

  return (n_t) { mant, exp };
}

double convert_angle(double d, n_trig_t from, n_trig_t to) {
  if (from == to) return d;

  // Convert to radians.
  if (from == N_DEG) d = d / 180 * M_PI;
  else if (from == N_GRAD) d = d / 200 * M_PI;

  // Convert from radians.
  if (to == N_DEG)  d = d / M_PI * 180;
  else if (to == N_GRAD) d = d / M_PI * 200;

  return (double) d;
}

n_err_t max_error(n_err_t err1, n_err_t err2) {
  return MAX(err1, err2);
}
