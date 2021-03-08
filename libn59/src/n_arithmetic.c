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

// Gives 13 accurate digits (no rounding up) when adding 2 numbers of the same
// sign. This appears to be TI-59's algorithm.
//
// This method may truncate one of the 2 numbers before performing addition or
// substraction. For example:
//   10000000000000      <=  has 13 digits.
// +              1.9    <=  gets truncated down to 1, before addition.
// ------------------
//   10000000000001
// even if 1000000000002 would be more accurate.
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

  // Align n1 and n2 digits as in basic addition.
  while (n2.exp < n1.exp) {
    n2.exp += 1;
    n2.mant /= 10;
  }

  long long mant = n1.mant + n2.mant;

  n_t res = normalize_number(ABS(mant), n1.exp, err);
  if (mant < 0) res.mant = -res.mant;
  return res;
}

// See n_plus.
n_t n_minus(n_t n1, n_t n2, n_err_t *err) {
  return n_plus(n1, n_chs(n2), err);
}

// Gives 13 accurate digits (no rounding up) when multiplying 2 numbers.
//
// TI-59 appears to perform a similar strategy (no rounding up) getting all 13
// digits right most of the time. When all the 13 digits are significant, TI-59
// appears to truncate the last digit before multiplication, getting a less
// accurate result than we do.
n_t n_times(n_t n1, n_t n2, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  if (n_is_zero(n1) || n_is_zero(n2)) return N_0;

  long long m1 = ABS(n1.mant);
  long long m2 = ABS(n2.mant);

  // Perform double precision multiplication. The first 13 or 14 digits will be
  // stored in l and the last 12 in r.
  long long f = 1000000LL;
  long long l1 = m1 / f;
  long long r1 = m1 % f;
  long long l2 = m2 / f;
  long long r2 = m2 % f;
  long long l = l1 * l2;
  long long m = l1 * r2 + l2 * r1;
  long long r = r1 * r2;
  r = r + (m % f) * f;
  l = l + m / f + r / f / f;
  r = r % (f * f);

  // Normalize. We could consider r and possibly round up but TI-59 does not
  // seem to do that.
  int exp = n1.exp + n2.exp;
  if (l >= 10000000000000LL) {  // 14 digits.
    l /= 10;
    exp += 1;
  }
  n_t res = normalize_number(l, exp, err);

  bool neg = (n1.mant > 0) != (n2.mant > 0);
  res = neg ? n_chs(res) : res;
  return res;
}

#if 0
// Alternative division with no rounding. Doesn't look like TI-59 does this.
n_t n_div2(n_t n1, n_t n2, n_err_t *err) {
  if (err) *err = N_ERR_NONE;

  // n1 or n2 is zero.
  if (n_is_zero(n1) && n_is_zero(n2)) {
    if (err) *err = N_ERR_DOMAIN;
    return N_1;
  } else if (n_is_zero(n1)) {
    return N_0;
  } else if (n_is_zero(n2)) {
    return (n_cmp(n1, N_0) > 0) ? N_INF : n_chs(N_INF);
  }

  long long m1 = ABS(n1.mant);
  long long m2 = ABS(n2.mant);
  long long div = 0;
  int exp = n1.exp - n2.exp;
  if (m1 < m2) m1 *= 10;
  int i = 0;
  while (i < 14) {
    if (m1 >= m2) {
      m1 = m1 - m2;
      div += 1;
    } else {
      i += 1;
      m1 *= 10;
      div *= 10;
    }
  }

  n_t res = normalize_number(div, exp, err);
  bool neg = (n1.mant > 0) != (n2.mant > 0);
  res = neg ? n_chs(res) : res;
  return res;
}
#endif

n_t n_div(n_t n1, n_t n2, n_err_t *err) {
  if (err) *err = N_ERR_NONE;
  double d1 = n_n2d(n1);
  double d2 = n_n2d(n2);

  // Division by 0.
  if (d2 == 0) {
    if (err) *err = N_ERR_DOMAIN;
    if (d1 == 0) {
      return N_1;
    } else if (d1 > 0) {
      return N_INF;
    } else {
      return n_chs(N_INF);
    }
  }

  return n_d2n(d1 / d2, err);
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
  d1 = ABS(d1);

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
