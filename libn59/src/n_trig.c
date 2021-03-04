#include "n_internal.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>

#define PI 3.14159265359

/******************************************************************************
 *
 *  HELPERS.
 *
 ******************************************************************************/

double convert_angle(double d, n_trig_t from, n_trig_t to) {
  if (from == to) return d;
  if (from == N_DEG)  d = d / 180 * PI;
  if (from == N_GRAD) d = d / 200 * PI;
  if (to   == N_DEG)  d = d / PI * 180;
  if (to   == N_GRAD) d = d / PI * 200;
  return (double) d;
}

/** Returns true if there is an integer n3 such that n1 = n2 * n3. */
static bool is_multiple(n_t n1, n_t n2, n_t *n3) {
  n_err_t err;
  n_t f = n_int(n_div(n1, n2, &err));
  if (err) return false;

  n_t f1 = f;
  n_t p = n_times(n2, f1, &err);
  if (err) return false;
  if (n_equals(p, n1)) { *n3 = f1; return true; }

  f1 = n_plus(f, N_1, &err);
  if (err) return false;
  p = n_times(n2, f1, &err);
  if (err) return false;
  if (n_equals(p, n1)) { *n3 = f1; return true; }

  f1 = n_minus(f, N_1, &err);
  if (err) return false;
  p = n_times(n2, f1, &err);
  if (err) return false;
  if (n_equals(p, n1)) { *n3 = f1; return true; }

  return false;
}

static bool is_t_right_angles(n_t n, n_trig_t mode, long long *t) {
  n_t N_90      = n_make(9000000000000LL, 1);
  n_t N_100     = n_make(1000000000000LL, 2);
  n_t N_HALF_PI = n_div(N_PI, n_make(2000000000000LL, 0), NULL);
  n_t f;
  switch (mode) {
    case N_DEG:  f = N_90;      break;
    case N_GRAD: f = N_100;     break;
    case N_RAD:  f = N_HALF_PI; break;
  }
  n_t nt;
  if (!is_multiple(n, f, &nt)) return false;
  *t = nt.mant;
  for (int i = 0; i < 12 - nt.exp; i++) {
    *t /= 10;
  }
  return true;
}

/******************************************************************************
 *
 *  IMPLEMENTATION.
 *
 ******************************************************************************/

n_t n_sin(n_t n, n_trig_t mode, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  if (n.exp >= 13) return N_0;

  long long t;
  if (is_t_right_angles(n, mode, &t)) {
    switch(t % 4) {
      case -3: return N_1;
      case -2: return N_0;
      case -1: return n_chs(N_1);
      case  0: return N_0;
      case  1: return N_1;
      case  2: return N_0;
      case  3: return n_chs(N_1);
      default: assert(false);
    }
  }

  double d = n_n2d(n);
  d = convert_angle(d, mode, N_RAD);
  return n_d2n(sin(d), err);
}

n_t n_cos(n_t n, n_trig_t mode, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  if (n.exp >= 13) return N_1;

  long long t;
  if (is_t_right_angles(n, mode, &t)) {
    switch(ABS(t % 4)) {
      case 0: return N_1;
      case 1: return N_0;
      case 2: return n_chs(N_1);
      case 3: return N_0;
      default: assert(false);
    }
  }

  double d = n_n2d(n);
  d = convert_angle(d, mode, N_RAD);
  return n_d2n(cos(d), err);
}

n_t n_tan(n_t n, n_trig_t mode, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  if (n.exp >= 13) return N_0;

  long long t;
  if (is_t_right_angles(n, mode, &t)) {
    if (t % 2 == 0) return N_0;
    *err = N_ERR_DOMAIN;
    return N_INF;
  }

  double d = n_n2d(n);
  d = convert_angle(d, mode, N_RAD);
  return n_d2n(tan(d), err);
}

n_t n_asin(n_t n, n_trig_t mode, n_err_t *err) {
  double d = n_n2d(n);
  if (ABS(d) > 1) {
    if (err) *err = N_ERR_DOMAIN;
    return n;
  }
  d = asin(d);
  d = convert_angle(d, N_RAD, mode);
  return n_d2n(d, err);
}

n_t n_acos(n_t n, n_trig_t mode, n_err_t *err) {
  double d = n_n2d(n);
  if (ABS(d) > 1) {
    if (err) *err = N_ERR_DOMAIN;
    return n;
  }
  d = acos(d);
  d = convert_angle(d, N_RAD, mode);
  return n_d2n(d, err);
}

n_t n_atan(n_t n, n_trig_t mode, n_err_t *err) {
  double d = atan(n_n2d(n));
  d = convert_angle(d, N_RAD, mode);
  return n_d2n(d, err);
}
