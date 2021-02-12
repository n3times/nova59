#include <stdbool.h>
#include <math.h>

#include "n.h"

#define NULL 0
#define BIG_TRIG pow(10, 13)

// TODO: deal with errors.

n_t n_sin(n_t n, bool *err) {
  if (err) *err = false;
  double d = n2d(n);
  if (ABS(d) == M_PI) return N_0;
  if (ABS(d) >= BIG_TRIG) return N_0;
  return d2n(sin(d), err);
}

n_t n_cos(n_t n, bool *err) {
  if (err) *err = false;
  double d = n2d(n);
  if (ABS(d) == M_PI) return N_0;
  if (ABS(d) >= BIG_TRIG) return N_1;
  return d2n(cos(d), err);
}

n_t n_tan(n_t n, bool *err) {
  return d2n(tan(n2d(n)), err);
}

n_t n_asin(n_t n, bool *err) {
  return d2n(asin(n2d(n)), err);
}

n_t n_acos(n_t n, bool *err) {
  return d2n(acos(n2d(n)), err);
}

n_t n_atan(n_t n, bool *err) {
  return d2n(atan(n2d(n)), err);
}
