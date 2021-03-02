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

double normalize_angle(double d, n_trig_t mode) {
  double period;
  if (ABS(d) >= POW10_13) return 0;
  switch (mode) {
    case N_RAD:  period = 2 * PI; break;
    case N_DEG:  period = 360;    break;
    case N_GRAD: period = 400;    break;
  }
  double normalized_d = d - floor(d/period) * period;
  assert(normalized_d >= 0);
  switch (mode) {
    case N_RAD:  assert(normalized_d < 2 * PI); break;
    case N_DEG:  assert(normalized_d < 360);    break;
    case N_GRAD: assert(normalized_d < 400);    break;
  }
  return normalized_d;
}

double convert_angle(double d, n_trig_t from, n_trig_t to) {
  if (from == to) return d;
  if (from == N_DEG)  d = d / 180 * PI;
  if (from == N_GRAD) d = d / 200 * PI;
  if (to   == N_DEG)  d = d / PI * 180;
  if (to   == N_GRAD) d = d / PI * 200;
  return (double) d;
}

static n_err_t is_n_right_angles(double d, n_trig_t mode, int n) {
  if (mode == N_DEG  &&   90 * n == d) return true;
  if (mode == N_GRAD &&  100 * n == d) return true;
  if (mode == N_RAD  && PI/2 * n == d) return true;
  return N_ERR_NONE;
}


/******************************************************************************
 *
 *  IMPLEMENTATION.
 *
 ******************************************************************************/

n_t n_sin(n_t n, n_trig_t mode, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  double d = n_n2d(n);
  d = normalize_angle(d, mode);

  if (d == 0) return N_0;
  if (is_n_right_angles(d, mode, 1)) return N_1;
  if (is_n_right_angles(d, mode, 2)) return N_0;
  if (is_n_right_angles(d, mode, 3)) return n_chs(N_1);

  d = convert_angle(d, mode, N_RAD);
  return n_d2n(sin(d), err);
}

n_t n_cos(n_t n, n_trig_t mode, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  double d = n_n2d(n);
  d = normalize_angle(d, mode);

  if (d == 0) return N_1;
  if (is_n_right_angles(d, mode, 1)) return N_0;
  if (is_n_right_angles(d, mode, 2)) return n_chs(N_1);
  if (is_n_right_angles(d, mode, 3)) return N_0;

  d = convert_angle(d, mode, N_RAD);
  return n_d2n(cos(d), err);
}

n_t n_tan(n_t n, n_trig_t mode, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  double d = n_n2d(n);
  d = normalize_angle(d, mode);

  if (d == 0) return N_0;
  if (is_n_right_angles(d, mode, 2)) return N_0;
  if (is_n_right_angles(d, mode, 1) ||
      is_n_right_angles(d, mode, 3)) {
    if (err) *err = N_ERR_OVERFLOW;
    return N_INF;
  }

  d = convert_angle(d, mode, N_RAD);
  return n_d2n(tan(d), err);
}

n_t n_asin(n_t n, n_trig_t mode, n_err_t *err) {
  double d = asin(n_n2d(n));
  if (ABS(d) > 1) {
    if (err) *err = N_ERR_DOMAIN;
    return n;
  }
  d = convert_angle(d, N_RAD, mode);
  return n_d2n(d, err);
}

n_t n_acos(n_t n, n_trig_t mode, n_err_t *err) {
  double d = acos(n_n2d(n));
  if (ABS(d) > 1) {
    if (err) *err = N_ERR_DOMAIN;
    return n;
  }
  d = convert_angle(d, N_RAD, mode);
  return n_d2n(d, err);
}

n_t n_atan(n_t n, n_trig_t mode, n_err_t *err) {
  double d = atan(n_n2d(n));
  d = convert_angle(d, N_RAD, mode);
  return n_d2n(d, err);
}
