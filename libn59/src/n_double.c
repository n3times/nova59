#include "n_internal.h"

#include <math.h>

double n_n2d(n_t n) {
  NORMALIZE(n);

  double d = n.mant * pow(10, n.exp - 12);

  return d;
}

n_t n_d2n(double d, n_err_t *err) {
  if (err) *err = N_ERR_NONE;

  if (d == 0) return N_0;

  // Compute mantissa and exponent. Do round the mantissa to ensure that if
  // d = n_n2d(n) for some n, then n_d2n(d) = n.
  int exp = (int) floor(log10(ABS(d)));
  long long mant = (long long) (ABS(d) * pow(10, 12 - exp) + 0.499);
  if (mant >= POW10_13) {
    mant /= 10;
    exp += 1;
  }

  // Overflow.
  if (ABS(exp) >  99) {
    if (err) *err = exp < 0 ? N_ERR_UNDERFLOW : N_ERR_OVERFLOW;
    n_t n = exp < 0 ? N_EPS : N_INF;
    if (d < 0) n = n_chs(n);
    return n;
  }

  if (d < 0) mant = -mant;

  return (n_t) { mant, exp };
}
