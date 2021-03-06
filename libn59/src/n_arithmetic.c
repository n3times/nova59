#include "n_internal.h"

#include <math.h>


/******************************************************************************
 *
 *  HELPERS.
 *
 ******************************************************************************/

/**
 * Given an arbitrary mantissa and exponent, not necessary within the ranges of
 * those of a TI-59 number, returns a TI-59 number, that is either N_0 or a
 * number whose mantissa has exactly 13 digits and whose exponent is in -99..99.
 */
static n_t normalize_number(long long mant, int exp, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  if (mant == 0) return N_0;

  // Make sure the mantissa has exactly 13 digits.
  while (ABS(mant) >= POW10_13) {
    mant /= 10;
    exp += 1;
  }
  while (ABS(mant) < POW10_12) {
    mant *= 10;
    exp -= 1;
  }

  // Overflow.
  if (ABS(exp) > 99) {
    if (err) {
      *err = exp < 0 ? N_ERR_UNDERFLOW : N_ERR_OVERFLOW;
    }
    if (exp < 0) {
      return mant < 0 ? n_chs(N_EPS) : N_EPS;
    } else {
      return mant < 0 ? n_chs(N_INF) : N_INF;
    }
  }

  return n_make(mant, exp);
}


/******************************************************************************
 *
 *  IMPLEMENTATION.
 *
 ******************************************************************************/

n_t n_plus(n_t n1, n_t n2, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  if (n_is_zero(n1)) return n2;
  if (n_is_zero(n2)) return n1;

  if (n1.exp < n2.exp) {
    n_t tmp = n1;
    n1 = n2;
    n2 = tmp;
  }

  // Optimization.
  if (n1.exp - n2.exp >= 13) return n1;

  // Align n1 and n2 digits as is basic addition.
  // Note that, as in TI-59, we do not round up:
  //       9999999999.999
  //     +           .0009
  //     = 9999999999.999
  while (n2.exp < n1.exp) {
    n2.exp += 1;
    n2.mant /= 10;
  }

  long long mant = n1.mant + n2.mant;

  n_t res = normalize_number(ABS(mant), n1.exp, err);
  if (mant < 0) res.mant = -res.mant;
  return res;
}

n_t n_minus(n_t n1, n_t n2, n_err_t *err) {
  return n_plus(n1, n_chs(n2), err);
}

n_t n_times(n_t n1, n_t n2, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  if (n_is_zero(n1) || n_is_zero(n2)) return N_0;

  bool neg = (n1.mant > 0) != (n2.mant > 0);
  n1 = n_abs(n1);
  n2 = n_abs(n2);

  // Perform double precision multiplication. The first 13 or 14 digits will be
  // stored in l and the last 12 in r.
  long long f = 1000000LL;
  long long l1 = n1.mant / f;
  long long r1 = n1.mant % f;
  long long l2 = n2.mant / f;
  long long r2 = n2.mant % f;
  long long l = l1 * l2;
  long long m = l1 * r2 + l2 * r1;
  long long r = r1 * r2;
  r = r + (m % f) * f;
  l = l + m / f + r / f / f;
  r = r % (f * f);

  // Normalize. Note that TI-59 does not appear to do any rounding.
  int exp = n1.exp + n2.exp;
  if (l >= 10000000000000LL) {  // 14 digits.
    l /= 10;
    exp += 1;
  }
  n_t res = normalize_number(l, exp, err);

  res = neg ? n_chs(res) : res;
  return res;
}

n_t n_div(n_t n1, n_t n2, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  double d1 = n_n2d(n1);
  double d2 = n_n2d(n2);

  // Division by 0.
  if (d2 == 0) {
    if (err) *err = N_ERR_DOMAIN;
    if (d1 == 0) {
      return N_1;
    } else {
      return (d1 > 0) ? N_INF : n_chs(N_INF);
    }
  }

  return n_d2n(d1/d2, err);
}

n_t n_pow(n_t n1, n_t n2, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  double d1 = n_n2d(n1);
  double d2 = n_n2d(n2);

  // Special cases: d1 or d2 are zero.
  if (d1 == 0 && d2 == 0) return N_1;
  if (d1 == 0 && d2 > 0) return N_0;
  if (d1 == 0 && d2 < 0) {
    if (err) *err = N_ERR_DOMAIN;
    return N_INF;
  }
  if (d2 == 0 && d1 > 0) return N_1;
  if (d2 == 0 && d1 < 0) {
    if (err) *err = N_ERR_DOMAIN;
    return N_1;
  }

  // d1 and d2 are nonzero.

  bool d1_neg = d1 < 0;
  if (d1_neg) {
    d1 = -d1;
  }

  // Overflow.
  double exp = (double) (d2 * log10(d1));
  if (exp >= 100) {
    if (err) *err = d1_neg ? N_ERR_DOMAIN : N_ERR_OVERFLOW;
    return N_INF;
  } else if (exp < -99) {
    if (err) *err = d1_neg ? N_ERR_DOMAIN : N_ERR_UNDERFLOW;
    return N_EPS;
  }

  n_t res = n_d2n(pow(d1, d2), err);
  if (err) {
    // Prioritize N_ERR_DOMAIN.
    if (d1_neg) *err = N_ERR_DOMAIN;
  }
  return res;
}

n_t n_ipow(n_t n1, n_t n2, n_err_t *err) {
  if (err) *err = N_ERR_NONE;

  // Note that some of these return values are somewhat arbitrary. We follow,
  // here, TI-59's conventions.
  if (n_is_zero(n2)) {
    if (err) *err = N_ERR_DOMAIN;
    if (n_is_zero(n1)) return N_1;  // 0 ipow 0 = 1.
    n1 = n_abs(n1);
    int cmp = n_cmp(n1, N_1);
    if (cmp == 0) {
      return N_1;                   // 1 ipow 0 = 1.
    } else if (cmp < 0) {
      return n_chs(N_INF);          // 0.5 ipow 0 = -inf, e.g.
    } else {
      return N_INF;                 // 2 ipow 0 = inf, e.g.
    }
  }

  return n_pow(n1, n_1_over_x(n2, NULL), err);
}
