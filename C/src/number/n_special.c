#include "n_internal.h"

#include <math.h>
#include <stdbool.h>

n_t n_dms(n_t n, bool *err) {
  if (n.exp >= 12) return n;
  double d  = n2d(n);
  bool neg = d < 0;
  d = ABS(d);
  long long h = (long long) d;
  double frac = d - h;
  double m = (int) (frac * 100);
  double s = (frac * 100 - m) * 100;
  double res = h + m / 60 + s / 60 / 60;
  return d2n(neg ? -res : res, err);
}

n_t n_idms(n_t n, bool *err) {
  if (n.exp >= 12) return n;
  double d  = n2d(n);
  bool neg = d < 0;
  d = ABS(d);
  long long h = (long long) d;
  double frac = d - h;
  double m = (int) (frac * 60);
  double s = (frac * 60 - m) * 60;
  double res = h + m / 100 + s / 100 / 100;
  return d2n(neg ? -res : res, err);
}

void n_p_r(n_t rho, n_t theta, n_t *x_out, n_t *y_out, trig_t mode, bool *err) {
  double d_rho = n2d(rho);
  double d_theta = n2d(theta);
  d_theta = normalize_angle(d_theta, mode);
  d_theta = scale_angle(d_theta, mode, RAD);
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
  d_theta = scale_angle(d_theta, RAD, mode);
  *rho_out = d2n(d_rho, err);
  *theta_out = d2n(d_theta, err);
}
