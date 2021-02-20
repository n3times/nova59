#include "n_internal.h"

#include <math.h>
#include <stdio.h>


/******************************************************************************
 *
 *  HELPERS.
 *
 ******************************************************************************/

// Given a mantissa and an exponent with an arbitrary number of digits, returns
// a TI-59 number, that is either 0 or a number whose mantissa has exactly 13
// digits and whose exponent is in -99..99.
n_t normalize(long long mant, int exp, bool *err) {
  if (err) *err = false;
  if (mant == 0) return N_0;
  bool neg = mant < 0;
  if (neg) mant = -mant;
  while (mant >= POW10_13) {
    mant /= 10;
    exp += 1;
  }
  while (mant < POW10_12) {
    mant *= 10;
    exp -= 1;
  }
  if (neg) mant = -mant;
  if (ABS(exp > 99)) {
    if (err) *err = true;
    n_t n = exp < 0 ? N_EPS : N_INF;
    return neg ? n_chs(n) : n;
  }
  n_t n = { mant, exp };
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

  // Note that, as in the TI-59, we do not round up:
  //       9999999999.999
  //     +           .0009
  //     = 9999999999.999
  if (n1.exp - n2.exp >= 13) return n1;

  while (n2.exp < n1.exp) {
    n2.exp += 1;
    n2.mant /= 10;
  }
  return normalize(n1.mant + n2.mant, n1.exp, err);
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
  double exp = (double) (d2 * log10(d1));
  if (exp >= 100) {
    if (err) *err = true;
    return N_INF;
  } else if (exp < -99) {
    if (err) *err = true;
    return N_EPS;
  }

  bool err2;
  n_t res = d2n(pow(d1, d2), &err2);
  if (err) *err = *err || err2;
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
  return n_pow(n1, n_1_x(n2, NULL), err);
}
