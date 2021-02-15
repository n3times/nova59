#include "n_internal.h"

#include <math.h>
#include <stdbool.h>

n_t N_100 = { 1000000000000LL, 2 };
n_t N_60 = { 6000000000000LL, 1 };
n_t N_36 = { 3600000000000LL, 1 };
n_t N_06 = { 6000000000000LL, -1 };

n_t n_dms(n_t n, int fix, notation_t notation, bool *err) {
  // Normalize.
  char str[16];
  n2s(n, fix, notation, str);
  n = s2n(str, err);

  if (n.exp >= 10) return n;

  bool neg = n.mant < 0;
  if (neg) n.mant = -n.mant;
  n_t h = n_int(n);
  n_t x = n_times(n_frac(n), N_100, 0);
  n_t m = n_div(n_int(x), N_60, 0);
  n_t s = n_div(n_frac(x), N_36, 0);
  n_t res = n_plus(n_plus(h, m, 0), s, 0);
  if (neg) res.mant = -res.mant;
  return res;
}

n_t n_idms(n_t n, int fix, notation_t notation, bool *err) {
  // Normalize.
  char str[16];
  n2s(n, fix, notation, str);
  n = s2n(str, err);

  if (n.exp >= 10) return n;

  bool neg = n.mant < 0;
  if (neg) n.mant = -n.mant;
  n_t h = n_int(n);
  n_t x = n_times(n_frac(n), N_100, 0);
  n_t m = n_div(n_int(n_times(n_int(x), N_06, 0)), N_100, 0);
  n_t s = N_0;///n_div(n_frac(x), N_36, 0);
  n_t res = n_plus(n_plus(h, m, 0), s, 0);
  if (neg) res.mant = -res.mant;
  return res;
}

void n_p_r(n_t rho, n_t theta, n_t *x_out, n_t *y_out, trig_t mode, bool *err) {
  double d_rho = n2d(rho);
  double d_theta = n2d(theta);
  d_theta = normalize_angle(d_theta, mode);
  d_theta = convert_trig_mode(d_theta, mode, RAD);
  double x = d_rho * sin(d_theta);
  double y = d_rho * cos(d_theta);
  *x_out = d2n(x, err);
  *y_out = d2n(y, err);
}

void n_r_p(n_t x, n_t y, n_t *rho_out, n_t *theta_out, trig_t mode, bool *err) {
  double d_x = n2d(x);
  double d_y = n2d(y);
  double d_rho;
  double d_theta;
  if (d_x == 0) {
    if (d_y == 0) d_theta =  M_PI / 4;
    if (d_y > 0)  d_theta =  M_PI / 2;
    if (d_y < 0)  d_theta = -M_PI / 2;
  } else {
    d_theta = atan(d_y/d_x);  // -pi/2 .. pi/2
    if (d_x < 0) d_theta += M_PI;
  }
  // theta in -pi/2 .. 3pi/2
  d_rho = sqrt(d_x * d_x + d_y * d_y);
  d_theta = convert_trig_mode(d_theta, RAD, mode);
  *rho_out = d2n(d_rho, err);
  *theta_out = d2n(d_theta, err);
}
