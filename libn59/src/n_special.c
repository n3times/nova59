#include "n_internal.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>

n_t n_dms(n_t n, int fix, n_format_t format, n_err_t *err) {
  assert(fix >= 0 && fix <= 9);
  if (fix < 0) fix = 0;
  else if (fix > 9) fix = 9;

  // Only consider the digits visible on the display.
  char str[N_N2S_MAX_SIZE];
  n_err_t err1, err2, err3;
  n_n2s(n, fix, format, str, &err1);
  assert(err1 == N_ERR_NONE || err1 == N_ERR_OVERFLOW);
  n = n_s2n(str, &err2);
  assert(err2 == N_ERR_NONE);

  // Optimization.
  if (n.exp >= 9) return n;

  // Split n into hours, minutes and seconds.
  long long mant = ABS(n.mant);
  long long h = (long long) (mant / pow(10, 12 - n.exp));
  mant -= h * pow(10, 12 - n.exp);
  int m = (int) (mant / pow(10, 10 - n.exp));
  mant -= m * pow(10, 10 - n.exp);
  double s = mant / pow(10,  8 - n.exp);

  // Scale and re-combine hours, minutes and seconds.
  double res = h + m/60. + s/3600.;

  // Convert result to number.
  if (n.mant < 0) res = -res;
  n = n_d2n(res, &err3);
  assert(err3 == N_ERR_NONE);
  if (err) *err = err1;

  return n;
}

n_t n_idms(n_t n, int fix, n_format_t format, n_err_t *err) {
  assert(fix >= 0 && fix <= 9);
  if (fix < 0) fix = 0;
  else if (fix > 9) fix = 9;

  // Only consider the digits visible on the display.
  char str[N_N2S_MAX_SIZE];
  n_err_t err1, err2, err3;
  n_n2s(n, fix, format, str, &err1);
  assert(err1 == N_ERR_NONE || err1 == N_ERR_OVERFLOW);
  n = n_s2n(str, &err2);
  assert(err2 == N_ERR_NONE);

  // Optimization.
  if (n.exp >= 9) return n;

  // Get hours, minutes and seconds from decimal degrees.
  double d = ABS(n_n2d(n));
  long long h = (long long) d;
  double total_s = (d - h) * 3600;
  int m = (int) (total_s / 60);
  double s = total_s - m * 60;

  // Combine hours, minutes and seconds into result.
  double res = h + m / 100. + s / 10000;

  // Convert result to number.
  if (n.mant < 0) res = -res;
  n = n_d2n(res, &err3);
  assert(err3 == N_ERR_NONE || err3 == N_ERR_UNDERFLOW);
  assert(err1 == N_ERR_NONE || err3 == N_ERR_NONE);
  if (err) {
    if (err1 != N_ERR_NONE) *err = err1;
    else *err = err3;
  }

  return n;
}

void n_p_r(n_t n_rho, n_t n_theta, n_trig_t mode,
           n_t *n_x_out, n_t *n_y_out, n_err_t *err) {
  double d_rho = n_n2d(n_rho);
  double d_theta = n_n2d(n_theta);

  d_theta = convert_angle(d_theta, mode, N_RAD);

  *n_x_out = n_d2n(d_rho * sin(d_theta), err);
  *n_y_out = n_d2n(d_rho * cos(d_theta), err);
}

void n_r_p(n_t n_x, n_t n_y, n_trig_t mode,
           n_t *n_rho_out, n_t *n_theta_out, n_err_t *err) {
  double d_x = n_n2d(n_x);
  double d_y = n_n2d(n_y);
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
  d_theta = convert_angle(d_theta, N_RAD, mode);

  *n_rho_out = n_d2n(d_rho, err);
  *n_theta_out = n_d2n(d_theta, err);
}
