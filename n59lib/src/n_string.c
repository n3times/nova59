#include "n_internal.h"

#include <math.h>
#include <stdbool.h>


/******************************************************************************
 *
 *  HELPERS.
 *
 ******************************************************************************/

static void get_display(bool neg, long long mant, int int_len, int frac_len,
                        int exp, char *str_out) {
  int i = 0;
  if (neg) str_out[i++] = '-';
  for (int j = 0; j < int_len + frac_len; j++) {
    if (j == 0 && int_len == 0) str_out[i++] = '.';
    int digit = mant / (long long)pow(10, int_len + frac_len - 1 - j) % 10;
    str_out[i++] = '0' + digit;
    if (j + 1 == int_len) str_out[i++] = '.';
  }
  if (ABS(exp) <= 99) {
    str_out[i++] = exp < 0 ? '-' : ' ';
    exp = ABS(exp);
    str_out[i++] = '0' + exp / 10;
    str_out[i++] = '0' + exp % 10;
  }
  str_out[i++] = '\0';
}


/******************************************************************************
 *
 *  IMPLEMENTATION.
 *
 ******************************************************************************/

/** Number to string. */
void n2s(n_t n, int fix, format_t format, char *str_out, bool *err) {
  bool neg = n.mant < 0;
  long long mant = ABS(n.mant);
  int exp = n.exp;
  if (err) *err = false;

  bool is_big = exp >= 10; // || (exp == 9 && mant >= POW10_13 - 500);
  bool is_small = exp <= -12 || (exp == -11 && mant < 5 * POW10_12);
  bool is_exp = format != FLOAT || is_big || (is_small && fix == 9);

  if (is_small && fix != 9 && format == FLOAT) {
    return n2s(N_0, fix, FLOAT, str_out, err);
  }

  // Compute actual mantissa.
  int mant_len = is_exp ? 8 : 10;
  int int_len = 0;
  if (is_exp) {
    int_len = 1;
    if (format == ENG) {
      int new_exp = (int)(3 * floor(exp / 3.0));
      int_len = 1 + exp - new_exp;
      exp = new_exp;
    }
  } else {
    int_len = MAX(0, exp + 1);
  }
  int frac_len = mant_len - int_len;
  if (fix != 9) {
    frac_len = MIN(frac_len, fix);
    mant_len = int_len + frac_len;
  }
  int keep = mant_len;
  if (!is_exp && exp < 0) keep = frac_len + exp + 1;
  mant = mant / pow(10, 13 - keep);
  bool round = ABS(n.mant) / (long long)pow(10, 12 - keep) % 10 >= 5;
  if (round) {
    mant += 1;
    if (mant >= pow(10, mant_len)) {
      n_t n_1 = n_make(POW10_12 * (neg ? -1 : 1), n.exp + 1);
      return n2s(n_1, fix, format, str_out, err);
    }
  }

  // Remove trailing 0's.
  if (mant != 0 && fix == 9) {
    while (mant % 10 == 0 && frac_len > 0) {
      mant /= 10;
      frac_len -= 1;
      mant_len -= 1;
    }
  }

  // Add a leading 0 if possible.
  if (int_len == 0 && mant_len < 10) {
    int_len += 1;
    mant_len += 1;
  }

  // Deal with edge cases.
  if (mant == 0) {
    // 0.
    neg = false;
    int_len = 1;
    frac_len = 0;
    if (fix < 9) frac_len = is_exp ? MIN(fix, 7) : fix;
    exp = 0;
  } else if (exp == 100 || (exp == 99 && int_len > 1)) {
    // Overflow.
    int_len = 1;
    frac_len = MIN(fix, 7);
    mant = (int) pow(10, 1 + frac_len) - 1;
    exp = 99;
    if (err) *err = true;
  }

  get_display(neg, mant, int_len, frac_len, is_exp ? exp : -100, str_out);
}

/** String to number. */
n_t s2n(char *s, bool *err) {
  n_t n = N_0;
  bool neg = false;
  bool neg_exp = false;
  int index_dot = -1;
  int index_end;
  if (err) *err = false;
  bool in_exp = false;

  // Extract mantissa and exponent.
  for (int i = 0; ; i++) {
    if (s[i] == '\0') break;
    if (s[i] == '?') break;
    if (in_exp) {
      if (s[i] == ' ')  continue;
      if (s[i] == '-')  {
        neg_exp = !neg_exp;
        continue;
      }
      n.exp = n.exp * 10 + s[i] - '0';
      continue;
    }
    if (i == 0 && s[i] == '-') {
      neg = true;
      continue;
    }
    if (s[i] == ' ' || s[i] == '-') {
      in_exp = true;
      neg_exp = s[i] == '-';
      continue;
    }
    if (s[i] == '.') {
      index_dot = i;
      continue;
    }
    n.mant = n.mant * 10 + s[i] - '0';
    index_end = i;
  }
  if (n.mant == 0) return N_0;
  if (neg_exp) n.exp = -n.exp;

  // Normalize.
  while (n.mant < POW10_12) {
    n.mant *= 10;
    n.exp -= 1;
  }
  if (neg) n.mant = -n.mant;
  if (index_dot == -1) index_dot = index_end;
  if (index_dot > index_end) index_dot = index_end;
  n.exp += 12 - index_end + index_dot;

  // Overflow/underflow.
  if (ABS(n.exp) > 99) {
    if (err) *err = true;
    n = n.exp < 0 ? N_EPS : N_INF;
    return n.mant < 0 ? n_chs(n) : n;
  }

  return n;
}
