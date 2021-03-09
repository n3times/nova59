#include "n_internal.h"

#define PI 3.14159265359

double convert_angle(double d, n_trig_t from, n_trig_t to) {
  if (from == to) return d;

  // Convert to radians.
  if (from == N_DEG) d = d / 180 * PI;
  else if (from == N_GRAD) d = d / 200 * PI;

  // Convert from radians.
  if (to == N_DEG)  d = d / PI * 180;
  else if (to == N_GRAD) d = d / PI * 200;

  return (double) d;
}

n_err_t max_error(n_err_t err1, n_err_t err2) {
  return MAX(err1, err2);
}