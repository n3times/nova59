#include "n_internal.h"

#include <math.h>


/******************************************************************************
 *
 *  IMPLEMENTATION.
 *
 ******************************************************************************/

/**
 * Adds 2 numbers.
 *
 * If both numbers are of the same sign, the 13 digits of the result are
 * guaranteed to be accurate (with no rounding up). If they have different
 * signs, the result may be off by 1.
 *
 * This appears to be same algorithm TI-59 uses.
 *
 * Note that unless both numbers have same exp, this method truncates one of the
 * 2 numbers before adding or substracting. For example:
 *
 *     10000000000000      <=  has 13 digits.
 *  +               1.9    <=  gets truncated down to 1, before addition.
 *  -------------------
 *     10000000000001
 *
 * even if 1000000000002 (13 digits) would be more accurate.
 */
n_t n_plus(n_t n1, n_t n2, n_err_t *err) {
  NORMALIZE(n1);
  NORMALIZE(n2);

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

/** See n_plus. */
n_t n_minus(n_t n1, n_t n2, n_err_t *err) {
  return n_plus(n1, n_chs(n2), err);
}

/**
 * Multiplies 2 numbers.
 *
 * The 13 digits of the result are guaranteed to be accurate (with no rounding
 * up).
 *
 * TI-59 appears to follow the same strategy most of the time. But if all the
 * digits of both numbers are significant, TI-59 appears to truncate the last
 * digit of at least one of the numbers, before multiplication. In this case,
 * TI-59 gets results slightly less accurate than this method.
 */
n_t n_times(n_t n1, n_t n2, n_err_t *err) {
  NORMALIZE(n1);
  NORMALIZE(n2);

  if (err) *err = N_ERR_NONE;

  if (n_is_zero(n1) || n_is_zero(n2)) return N_0;

  long long m1 = ABS(n1.mant);
  long long m2 = ABS(n2.mant);

  // Perform double precision multiplication. The first 13 or 14 digits will be
  // stored in 'l' and the last 12 in 'r'.
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

  // Normalize. We could consider 'r' and possibly round up but TI-59 does not
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

n_t n_div(n_t n1, n_t n2, n_err_t *err) {
  NORMALIZE(n1);
  NORMALIZE(n2);

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
  NORMALIZE(n1);
  NORMALIZE(n2);

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

  if (d1 < 0) {
    if (err) *err = N_ERR_DOMAIN;
    d1 = ABS(d1);
  }

  // Overflow.
  double exp = (double) (d2 * log10(d1));
  if (exp >= 100) {
    if (err) *err = max_error(*err, N_ERR_OVERFLOW);
    return N_INF;
  } else if (exp < -99) {
    if (err) *err = max_error(*err, N_ERR_UNDERFLOW);
    return N_EPS;
  }

  n_err_t err2;
  n_t res = n_d2n(pow(d1, d2), &err2);
  if (err) *err = max_error(*err, err2);

  return res;
}

n_t n_ipow(n_t n1, n_t n2, n_err_t *err) {
  NORMALIZE(n1);
  NORMALIZE(n2);

  if (err) *err = N_ERR_NONE;

  double d1 = n_n2d(n1);
  double d2 = n_n2d(n2);

  // Special cases: d1 or d2 are zero.
  // Note that some of these return values are somewhat arbitrary. We follow,
  // here, TI-59's conventions.
  if (d2 == 0) {
    if (err) *err = N_ERR_DOMAIN;
  }
  if (d2 == 0 && d1 == 0) return N_1;
  if (d2 == 0 && ABS(d1) > 1) return N_INF;
  if (d2 == 0 && ABS(d1) < 1) return n_chs(N_INF);
  if (d2 == 0 && ABS(d1) == 1) return N_1;
  if (d1 == 0 && d2 > 0) return N_0;
  if (d1 == 0 && d2 < 0) {
    if (err) *err = N_ERR_DOMAIN;
    return N_INF;
  }

  if (d1 == 1) return N_1;

  n_err_t err1, err2;
  n_t res = n_pow(n1, n_1_x(n2, &err1), &err2);
  if (err) *err = max_error(err1, err2);

  return res;
}
