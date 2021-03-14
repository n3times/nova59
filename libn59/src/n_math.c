#include "n_internal.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>

n_t n_chs(n_t n) {
  NORMALIZE(n);

  return n_make(-n.mant, n.exp);
}

n_t n_abs(n_t n) {
  NORMALIZE(n);

  return n_make(ABS(n.mant), n.exp);
}

n_t n_sign(n_t n) {
  NORMALIZE(n);

  if (n.mant > 0) {
    return N_1;
  } else if (n.mant < 0) {
    return n_chs(N_1);
  }
  return N_0;
}

n_t n_int(n_t n) {
  NORMALIZE(n);

  if (n.exp >= 12) return n;

  return n_d2n((long long)n_n2d(n), NULL);
}

n_t n_frac(n_t n) {
  NORMALIZE(n);

  return n_minus(n, n_int(n), NULL);
}

n_t n_square(n_t n, n_err_t *err) {
  NORMALIZE(n);

  return n_times(n, n, err);
}

n_t n_1_x(n_t n, n_err_t *err) {
  NORMALIZE(n);

  return n_div(N_1, n, err);
}

n_t n_sqrt(n_t n, n_err_t *err) {
  NORMALIZE(n);

  if (err) *err = N_ERR_NONE;

  if (n.mant < 0) {
    if (err) *err = N_ERR_DOMAIN;
    n = n_chs(n);
  }

  return n_d2n(sqrt(n_n2d(n)), NULL);
}

n_t n_ln(n_t n, n_err_t *err) {
  NORMALIZE(n);

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
  NORMALIZE(n);

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
  NORMALIZE(n);

  return n_pow(N_E, n, err);
}

n_t n_pow10(n_t n, n_err_t *err) {
  NORMALIZE(n);

  return n_pow(N_10, n, err);
}
