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

// Returns angle in period interval [0, period[ for mode.
double normalize_angle(double d, n_trig_t mode) {
  double period;
  if (ABS(d) >= POW10_13) return 0;
  switch (mode) {
    case N_RAD:  period = 2 * PI; break;
    case N_DEG:  period = 360;    break;
    case N_GRAD: period = 400;    break;
  }
  double angle = (d/period - floor(d/period)) * period;
  switch (mode) {
    case N_RAD:  assert(angle < 2 * PI); break;
    case N_DEG:  assert(angle < 360);    break;
    case N_GRAD: assert(angle < 400);    break;
  }
  return angle;
}

double convert_trig_mode(double d, n_trig_t from, n_trig_t to) {
  if (from == to) return d;
  if (from == N_DEG)  d = d / 180 * PI;
  if (from == N_GRAD) d = d / 200 * PI;
  if (to   == N_DEG)  d = d / PI * 180;
  if (to   == N_GRAD) d = d / PI * 200;
  return (double) d;
}


/******************************************************************************
 *
 *  IMPLEMENTATION.
 *
 ******************************************************************************/

n_t n_sin(n_t n, n_trig_t mode, bool *err) {
  if (err) *err = false;
  double d = n_n2d(n);
  d = normalize_angle(d, mode);

  if (d == 0) return N_0;
  if (mode == N_DEG  && d == 180) return N_0;
  if (mode == N_GRAD && d == 200) return N_0;
  if (mode == N_RAD  && d == PI)  return N_0;

  d = convert_trig_mode(d, mode, N_RAD);
  return n_d2n(sin(d), err);
}

n_t n_cos(n_t n, n_trig_t mode, bool *err) {
  if (err) *err = false;
  double d = n_n2d(n);
  d = normalize_angle(d, mode);

  if (mode == N_DEG   && (d ==  90  || d == 270))    return N_0;
  if (mode == N_GRAD  && (d == 100  || d == 300))    return N_0;
  if (mode == N_RAD   && (d == PI/2 || d == 3*PI/2)) return N_0;

  d = convert_trig_mode(d, mode, N_RAD);
  return n_d2n(cos(d), err);
}

n_t n_tan(n_t n, n_trig_t mode, bool *err) {
  if (err) *err = false;
  double d = n_n2d(n);
  d = normalize_angle(d, mode);

  if (d == 0) return N_0;
  if (mode == N_DEG  && d == 180) return N_0;
  if (mode == N_GRAD && d == 200) return N_0;
  if (mode == N_RAD  && d == PI)  return N_0;
  if (mode == N_DEG  && (d == 90 || d == 270)) {
    if (err) *err = true;
    return N_INF;
  }
  if (mode == N_GRAD  && (d == 100 || d == 300)) {
    if (err) *err = true;
    return N_INF;
  }
  if (mode == N_RAD && (d == PI/2 || d == 3*PI/2)) {
    if (err) *err = true;
    return N_INF;
  }

  d = convert_trig_mode(d, mode, N_RAD);
  return n_d2n(tan(d), err);
}

n_t n_asin(n_t n, n_trig_t mode, bool *err) {
  double d = asin(n_n2d(n));
  if (ABS(d) > 1) {
    if (err) *err = true;
    return n;
  }
  d = convert_trig_mode(d, N_RAD, mode);
  return n_d2n(d, err);
}

n_t n_acos(n_t n, n_trig_t mode, bool *err) {
  double d = acos(n_n2d(n));
  if (ABS(d) > 1) {
    if (err) *err = true;
    return n;
  }
  d = convert_trig_mode(d, N_RAD, mode);
  return n_d2n(d, err);
}

n_t n_atan(n_t n, n_trig_t mode, bool *err) {
  double d = atan(n_n2d(n));
  d = convert_trig_mode(d, N_RAD, mode);
  return n_d2n(d, err);
}
