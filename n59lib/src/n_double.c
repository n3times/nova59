#include "n_internal.h"

#include <math.h>

double n_n2d(n_t n) {
  return n.mant * pow(10, n.exp - 12);
}

n_t n_d2n(double d, bool *err) {
  if (err) *err = false;
  if (d == 0) return N_0;
  int exp = (int) floor(log10(ABS(d)));
  long long mant = (long long) (ABS(d) * pow(10, 12 - exp) + 0.5);

  if (mant >= POW10_13) {
    mant /= 10;
    exp += 1;
  }
  if (ABS(exp) >  99) {
    if (err) *err = true;
    n_t n = exp < 0 ? N_EPS : N_INF;
    if (d < 0) n = n_chs(n);
    return n;
  }
  if (d < 0) mant = -mant;

  return n_make(mant, exp);
}
