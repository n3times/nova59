#include "n_internal.h"

#include <stdbool.h>
#include <math.h>

#define NULL 0

n_t n_chs(n_t n) {
  n.mant = -n.mant;
  return n;
}

n_t n_abs(n_t n) {
  n.mant = ABS(n.mant);
  return n;
}

n_t n_sign(n_t n) {
  if (n.mant > 0) return N_1;
  else if (n.mant < 0) return n_chs(N_1);
  return N_0;
}

n_t n_int(n_t n) {
  if (n.exp >= 12) return n;
  return d2n((long long)n2d(n), NULL);
}

n_t n_frac(n_t n) {
  if (n.exp >= 12) return N_0;
  double d = n2d(n);
  return d2n(d - (long long)d, NULL);
}

n_t n_square(n_t n, bool *err) {
  double d = n2d(n);
  return d2n(d * d, err);
}

n_t n_1_x(n_t n, bool *err) {
  if (n.mant == 0) {
    if (err) *err = true;
    return N_INF;
  }
  return d2n(n2d(n), err);
}

n_t n_sqrt(n_t n, bool *err) {
  if (err) *err = false;
  if (n.mant < 0) {
    if (err) *err = true;
    n.mant = -n.mant;
  }
  return d2n(sqrt(n2d(n)), NULL);
}

n_t n_ln(n_t n, bool *err) {
  if (err) *err = false;
  if (n.mant == 0) {
    if (err) *err = true;
    return n_chs(N_INF);
  }
  if (n.mant < 0) {
    if (err) *err = true;
    n.mant = -n.mant;
  }
  return d2n(log(n2d(n)), NULL);
}

n_t n_log(n_t n, bool *err) {
  if (*err) *err = false;
  if (n.mant == 0) {
    if (*err) *err = true;
    return n_chs(N_INF);
  }
  if (n.mant < 0) {
    if (*err) *err = true;
    n.mant = -n.mant;
  }
  return d2n(log10(n2d(n)), NULL);
}

n_t n_exp(n_t n, bool *err) {
  double d = n2d(n);
  if (ABS(d) > 231) {
    if (*err) *err = true;
    return d < 0 ? N_EPS : N_INF;
  }
  return d2n(exp(d), err);
}

n_t n_pow10(n_t n, bool *err) {
  double d = n2d(n);
  if (ABS(d) > 100) {
    if (*err) *err = true;
    return d < 0 ? N_EPS : N_INF;
  }
  return d2n(pow(10, d), err);
}
