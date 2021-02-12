#include <stdbool.h>
#include <math.h>

#include "n.h"

#define NULL 0
#define BIG_TRIG pow(10, 13)
#define PI 3.14159265358979323846

static n_t PI_RAD  = { 3141592653590LL, 0 };
static n_t PI_DEG  = { 1800000000000LL, 2 };
static n_t PI_GRAD = { 2000000000000LL, 2 };

static bool is_pi(n_t n, trig_t mode) {
  n_t pi = PI_RAD;
  if (mode == DEG)  pi = PI_DEG;
  if (mode == GRAD) pi = PI_GRAD;
  return n.mant == pi.mant && n.exp == pi.exp;
}

// TODO: deal with errors.

static double scale(double d, trig_t from, trig_t to) {
  if (from == to) return d;
  if (from == DEG)  d = d / 180 * PI;
  if( from == GRAD) d = d / 200 * PI;
  if (to == DEG)    d = d / PI * 180;
  if (to == GRAD)   d = d / PI * 200;
  return (double) d;
}

n_t n_sin(n_t n, trig_t mode, bool *err) {
  if (err) *err = false;
  if (is_pi(n_abs(n, NULL), mode)) return N_0;
  double d = n2d(n);
  d = scale(d, mode, RAD);
  if (ABS(d) >= BIG_TRIG) return N_0;
  return d2n(sin(d), err);
}

n_t n_cos(n_t n, trig_t mode, bool *err) {
  if (err) *err = false;
  double d = n2d(n);
  d = scale(d, mode, RAD);
  if (ABS(d) == M_PI) return N_0;
  if (ABS(d) >= BIG_TRIG) return N_1;
  return d2n(cos(d), err);
}

n_t n_tan(n_t n, trig_t mode, bool *err) {
  double d = n2d(n);
  d = scale(d, mode, RAD);
  return d2n(tan(d), err);
}

n_t n_asin(n_t n, trig_t mode, bool *err) {
  double d = asin(n2d(n));
  if (ABS(d) > 1) {
    if (err) *err = true;
    return n;
  }
  d = scale(d, RAD, mode);
  return d2n(d, err);
}

n_t n_acos(n_t n, trig_t mode, bool *err) {
  double d = acos(n2d(n));
  if (ABS(d) > 1) {
    if (err) *err = true;
    return n;
  }
  d = scale(d, RAD, mode);
  return d2n(d, err);
}

n_t n_atan(n_t n, trig_t mode, bool *err) {
  double d = atan(n2d(n));
  d = scale(d, RAD, mode);
  return d2n(d, err);
}
