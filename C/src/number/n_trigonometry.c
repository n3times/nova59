#include "n_internal.h"

#include <stdbool.h>
#include <math.h>

#define PI 3.14159265359

// Returns angle in period interval [0, period[ for mode.
static double normalize(double d, trig_t mode) {
  double period;
  if (ABS(d) >= POW10_13) return 0;
   switch (mode) {
    case RAD:  period = 2 * PI; break;
    case DEG:  period = 360;    break;
    case GRAD: period = 400;;   break;
  }
  return (d/period - floor(d/period)) * period;
}

static double scale(double d, trig_t from, trig_t to) {
  if (from == to) return d;
  if (from == DEG)  d = d / 180 * PI;
  if (from == GRAD) d = d / 200 * PI;
  if (to == DEG)    d = d / PI * 180;
  if (to == GRAD)   d = d / PI * 200;
  return (double) d;
}

n_t n_sin(n_t n, trig_t mode, bool *err) {
  if (err) *err = false;
  double d = n2d(n);
  d = normalize(d, mode);

  if (d == 0) return N_0;
  if (mode == DEG  && d == 180) return N_0;
  if (mode == GRAD && d == 200) return N_0;
  if (mode == RAD  && d == PI)  return N_0;

  d = scale(d, mode, RAD);
  return d2n(sin(d), err);
}

n_t n_cos(n_t n, trig_t mode, bool *err) {
  if (err) *err = false;
  double d = n2d(n);
  d = normalize(d, mode);

  if (mode == DEG   && (d ==  90  || d == 270))    return N_0;
  if (mode == GRAD  && (d == 100  || d == 300))    return N_0;
  if (mode == RAD   && (d == PI/2 || d == 3*PI/2)) return N_0;

  d = scale(d, mode, RAD);
  return d2n(cos(d), err);
}

n_t n_tan(n_t n, trig_t mode, bool *err) {
  if (err) *err = false;
  double d = n2d(n);
  d = normalize(d, mode);

  if (d == 0) return N_0;
  if (mode == DEG  && d == 180) return N_0;
  if (mode == GRAD && d == 200) return N_0;
  if (mode == RAD  && d == PI)  return N_0;
  if (mode == DEG  && (d == 90 || d == 270)) {
    if (err) *err = true;
    return N_INF;
  }
  if (mode == GRAD  && (d == 100 || d == 300)) {
    if (err) *err = true;
    return N_INF;
  }
  if (mode == RAD && (d == PI/2 || d == 3*PI/2)) {
    if (err) *err = true;
    return N_INF;
  }

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
