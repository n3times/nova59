#include "n_internal.h"

#include <math.h>

double n2d(n_t n) {
  return (double)n.mant * pow(10, n.exp - 12);
}

n_t d2n(double d, bool *err) {
  if (err) *err = false;
  if (d == 0) return N_0;
  n_t n;
  n.exp = (int) floor(log10(ABS(d)));
  n.mant = (long long) (ABS(d) * pow(10, 12 - n.exp) + 0.5);
  if (ABS(n.exp) >  99) {
    if (err) *err = true;
    n = n.exp < 0 ? N_EPS : N_INF;
  }
  if (d < 0) n.mant = -n.mant;
  return n;
}
