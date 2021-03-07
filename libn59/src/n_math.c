#include "n_internal.h"

#include <stdbool.h>
#include <math.h>

n_t n_chs(n_t n) {
  return n_make(-n.mant, n.exp);
}

n_t n_abs(n_t n) {
  return n_make(ABS(n.mant), n.exp);
}

n_t n_sign(n_t n) {
  if (n.mant > 0) {
    return N_1;
  } else if (n.mant < 0) {
    return n_chs(N_1);
  }
  return N_0;
}

n_t n_int(n_t n) {
  if (n.exp >= 12) return n;

  return n_d2n((long long)n_n2d(n), NULL);
}

n_t n_frac(n_t n) {
  if (n.exp >= 12) return N_0;

  return n_minus(n, n_int(n), NULL);
}

n_t n_square(n_t n, n_err_t *err) {
  return n_times(n, n, err);
}

n_t n_1_over_x(n_t n, n_err_t *err) {
  return n_div(N_1, n, err);
}

n_t n_sqrt(n_t n, n_err_t *err) {
  if (err) *err = N_ERR_NONE;

  if (n.mant < 0) {
    if (err) *err = N_ERR_DOMAIN;
    n = n_chs(n);
  }

  return n_d2n(sqrt(n_n2d(n)), NULL);
}

n_t n_ln(n_t n, n_err_t *err) {
  if (err) *err = N_ERR_NONE;

  if (n_is_zero(n)) {
    if (err) *err = N_ERR_DOMAIN;
    return n_chs(N_INF);
  }

  if (n.mant < 0) {
    if (err) *err = N_ERR_DOMAIN;
    n = n_chs(n);
  }

  return n_d2n(log(n_n2d(n)), NULL);
}

n_t n_log(n_t n, n_err_t *err) {
  if (err) *err = N_ERR_NONE;

  if (n_is_zero(n)) {
    if (err) *err = N_ERR_DOMAIN;
    return n_chs(N_INF);
  }

  if (n.mant < 0) {
    if (err) *err = N_ERR_DOMAIN;
    n = n_chs(n);
  }

  return n_d2n(log10(n_n2d(n)), NULL);
}

n_t n_exp(n_t n, n_err_t *err) {
  double d = n_n2d(n);

  if (ABS(d) > 231) {
    if (err) {
      *err = d < 0 ? N_ERR_UNDERFLOW : N_ERR_OVERFLOW;
    }
    return d < 0 ? N_EPS : N_INF;
  }

  return n_d2n(exp(d), err);
}

n_t n_pow10(n_t n, n_err_t *err) {
  double d = n_n2d(n);

  if (ABS(d) > 100) {
    if (err) {
      *err = d < 0 ? N_ERR_UNDERFLOW : N_ERR_OVERFLOW;
    }
    return d < 0 ? N_EPS : N_INF;
  }

  return n_d2n(pow(10, d), err);
}
