#include "n_internal.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>


/******************************************************************************
 *
 *  HELPERS.
 *
 ******************************************************************************/

static n_t keep_visible_digits(n_t n, int fix, n_format_t format,
                               n_err_t *err) {
  char str[N_N2S_MAX_SIZE];
  n_n2s(n, fix, format, str, err);  // May overflow.
  n_err_t no_err;
  n = n_s2n(str, &no_err);
  assert(no_err == N_ERR_NONE);
  return n;
}


/******************************************************************************
 *
 *  IMPLEMENTATION.
 *
 ******************************************************************************/

// D.MS to decimal degrees.
n_t n_dms(n_t n, int fix, n_format_t format, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  assert(fix >= 0 && fix <= 9);
  if (fix < 0) fix = 0;
  else if (fix > 9) fix = 9;

  // Only consider the digits visible on the display.
  n = keep_visible_digits(n, fix, format, err);

  if (n.exp >= 9) return n;  // Optimization.

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
  n_err_t no_err;
  n = n_d2n(res, &no_err);
  assert(no_err == N_ERR_NONE);

  return n;
}

// Decimal degrees to D.MS.
n_t n_idms(n_t n, int fix, n_format_t format, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  assert(fix >= 0 && fix <= 9);
  if (fix < 0) fix = 0;
  else if (fix > 9) fix = 9;

  // Only consider the digits visible on the display.
  n = keep_visible_digits(n, fix, format, err);

  if (n.exp >= 9) return n;  // Optimization.

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
  n_err_t err2;
  n = n_d2n(res, &err2);  // May underflow.
  if (err) {
    assert(*err == N_ERR_NONE || err2 == N_ERR_NONE);
    if (*err == N_ERR_NONE) *err = err2;
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
