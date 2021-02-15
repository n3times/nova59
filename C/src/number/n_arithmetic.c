#include "n_internal.h"

#include <math.h>
#include <stdio.h>


/******************************************************************************
 *
 *  HELPERS.
 *
 ******************************************************************************/

n_t normalize(n_t n, bool *err) {
  if (err) *err = false;
  if (n.mant == 0) return N_0;
  bool neg = n.mant < 0;
  if (neg) n.mant = -n.mant;
  while (n.mant >= POW10_13) {
    n.mant /= 10;
    n.exp += 1;
  }
  while (n.mant < POW10_12) {
    n.mant *= 10;
    n.exp -= 1;
  }
  if (neg) n.mant = -n.mant;
  if (ABS(n.exp > 99)) {
    if (err) *err = true;
    n = n.exp < 0 ? N_EPS : N_INF;
    return neg ? n_chs(n) : n;
  }
  return n;
}


/******************************************************************************
 *
 *  IMPLEMENTATION.
 *
 ******************************************************************************/

n_t n_plus(n_t n1, n_t n2, bool *err) {
  if (err) *err = false;
  if (n1.mant == 0) return n2;
  if (n2.mant == 0) return n1;
  if (n1.exp < n2.exp) {
    n_t tmp = n1;
    n1 = n2;
    n2 = tmp;
  }
  if (n2.exp + 12 < n1.exp) return n1;
  while (n2.exp < n1.exp) {
    n2.exp += 1;
    n2.mant /= 10;
  }
  n_t n = { n1.mant + n2.mant, n1.exp };
  return normalize(n, err);
}

n_t n_minus(n_t n1, n_t n2, bool *err) {
  return n_plus(n1, n_chs(n2), err);
}

n_t n_times(n_t n1, n_t n2, bool *err) {
  return d2n(n2d(n1) * n2d(n2), err);
}

n_t n_div(n_t n1, n_t n2, bool *err) {
  double d1 = n2d(n1);
  double d2 = n2d(n2);
  if (d2 != 0) return d2n(d1/d2, err);
  if (err) *err = true;
  if (d1 == 0) return N_1;
  return (d1 > 0) ? N_INF : n_chs(N_INF);
}

n_t n_pow(n_t n1, n_t n2, bool *err) {
  double d1 = n2d(n1);
  double d2 = n2d(n2);
  if (err) *err = false;

  if (d1 == 0) {
    if (d2 == 0) {
      return N_1;
    } else if (d2 > 0) {
      return N_0;
    } else {
      if (err) *err = true;
      return N_INF;
    }
  }
  if (d1 < 0) {
    if (err) *err = true;
    d1 = -d1;
  }
  int exp = (int) (d2 * log10(d1));
  if (exp > 99) {
    if (err) *err = true;
    return N_INF;
  } else if (exp < -99) {
    if (err) *err = true;
    return N_EPS;
  }

  bool err2;
  n_t res = d2n(pow(d1, d2), &err2);
  if (err) *err = *err | err2;
  return res;
}

n_t n_ipow(n_t n1, n_t n2, bool *err) {
  double d1 = n2d(n1);
  double d2 = n2d(n2);
  if (err) *err = false;
  if (d2 == 0) {
    if (err) *err = true;
    d1 = ABS(d1);
    if (d1 == 0 || d1 == 1) {
      return N_1;
    } else if (d1 < 1) {
      return n_chs(N_INF);
    }
    return N_INF;
  }
  return n_pow(n1, n_1_x(n2, 0), err);
}
