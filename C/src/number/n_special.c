#include "n_internal.h"

#include <stdbool.h>

n_t n_dms(n_t n, bool *err) {
  if (n.exp >= 12) return n;
  double d  = n2d(n);
  bool neg = d < 0;
  d = ABS(d);
  long long h = (long long) d;
  double frac = d - h;
  double m = (int) (frac * 100);
  double s = (frac * 100 - m) * 100;
  double res = h + m / 60 + s / 60 / 60;
  return d2n(neg ? -res : res, err);
}

n_t n_idms(n_t n, bool *err) {
  return N_0;
}

void n_p_r(n_t rho, n_t theta, n_t *x_out, n_t *y_out, trig_t mode, bool *err) {
}

void n_r_p(n_t x, n_t y, n_t *rho_out, n_t *theta_out, trig_t mode, bool *err) {}
