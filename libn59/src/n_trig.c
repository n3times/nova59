#include "n_internal.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>


/**
 * Implementation of trigonometric functions on TI-59 numbers.
 *
 * Note:
 *
 * We mimic TI-59 on very large angles and on multiples of right angles, when
 * computing sin, cos and tan.
 *
 * Angles >= 10^12, in absolute value, are reduced to 0 in any trig mode. For
 * example:
 *
 *   cos(10^20) = cos(0) = 1.
 *
 * Indeed 13 digits are not precise enough to represent meaningfully very large
 * angles, so TI-59 just reduces these angles to 0.
 *
 * For smaller numbers, multiples of a right angle are always calculated
 * accurately. For example, in DEG:
 *
 *   sin(1234567 * 90) = (-1000000000000, 0).
 *
 * In RAD, pi_2 = (1570796326795, 0) is considered to be the right angle and:
 *
 *   sin(1234567 * pi_2) = (-1000000000000, 0),
 *
 * Note that, in RAD, this is not totally accurate, since pi_2 is only an
 * approximation of pi/2.
 */


/******************************************************************************
 *
 *  HELPERS.
 *
 ******************************************************************************/

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

static bool is_multiple_of_right_angle(n_t n, n_trig_t mode, long long *ratio) {
  n_t right_angle;
  switch (mode) {
    case N_DEG:  right_angle = n_make(9000000000000LL, 1); break;
    case N_GRAD: right_angle = n_make(1000000000000LL, 2); break;
    case N_RAD:  right_angle = n_make(1570796326795LL, 0); break;
  }

  n_t n_ratio;
  if (!is_multiple(n, right_angle, &n_ratio)) return false;
  *ratio = n_ratio.mant;
  for (int i = 0; i < 12 - n_ratio.exp; i++) {
    *ratio /= 10;
  }
  return true;
}


/******************************************************************************
 *
 *  IMPLEMENTATION.
 *
 ******************************************************************************/

n_t n_sin(n_t n, n_trig_t mode, n_err_t *err) {
  assert(n_is_number(n));

  if (err) *err = N_ERR_NONE;
  if (n.exp >= 13) return N_0;

  long long t;
  if (is_multiple_of_right_angle(n, mode, &t)) {
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
  assert(n_is_number(n));

  if (err) *err = N_ERR_NONE;
  if (n.exp >= 13) return N_1;

  long long t;
  if (is_multiple_of_right_angle(n, mode, &t)) {
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
  assert(n_is_number(n));

  if (err) *err = N_ERR_NONE;
  if (n.exp >= 13) return N_0;

  long long t;
  if (is_multiple_of_right_angle(n, mode, &t)) {
    if (t % 2 == 0) return N_0;
    *err = N_ERR_DOMAIN;
    return N_INF;
  }

  double d = n_n2d(n);
  d = convert_angle(d, mode, N_RAD);
  return n_d2n(tan(d), err);
}

n_t n_asin(n_t n, n_trig_t mode, n_err_t *err) {
  assert(n_is_number(n));

  double d = n_n2d(n);

  if (ABS(d) > 1) {
    if (err) *err = N_ERR_DOMAIN;
    return n;
  }

  d = convert_angle(asin(d), N_RAD, mode);
  return n_d2n(d, err);
}

n_t n_acos(n_t n, n_trig_t mode, n_err_t *err) {
  assert(n_is_number(n));

  double d = n_n2d(n);

  if (ABS(d) > 1) {
    if (err) *err = N_ERR_DOMAIN;
    return n;
  }

  d = convert_angle(acos(d), N_RAD, mode);
  return n_d2n(d, err);
}

n_t n_atan(n_t n, n_trig_t mode, n_err_t *err) {
  assert(n_is_number(n));

  double d = n_n2d(n);

  d = convert_angle(atan(d), N_RAD, mode);
  return n_d2n(d, err);
}
