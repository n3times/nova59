#include "n_internal.h"

#include <math.h>
#include <stdbool.h>

#include <stdio.h>

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

static bool is_digit(char c) {
  return c >= '0' && c <= '9';
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

n_t s2n(char *s, bool *err) {
  enum state_e {
    START,
    N_INT,
    N_FRAC,
    POST_N,
    EXP,
    POST_EXP
  };
  enum state_e state = START;
  bool format_err = false;
  long long n = -1;
  int exp = 0;
  bool has_exp = false;
  bool neg_n = false;
  bool neg_exp = false;
  int index_dot = -1;
  int index_end = 0;

  // Extract mantissa and exponent.
  for (int i = 0; ; i++) {
    char c = s[i];
    if (c == '\0') break;
    switch(state) {
      case START:
        if (c == ' ') continue;
        if (c == '-') {
          neg_n = true;
          state = N_INT;
        } else if (c == '.') {
          index_dot = i;
          state = N_FRAC;
        } else if (is_digit(c)) {
          state = N_INT;
          n = c - '0';
        } else { format_err = true; }
        break;
      case N_INT:
      case N_FRAC:
        if (c == ' ') {
          state = POST_N;
        } else if (c == '-') {
          neg_exp = true;
          state = EXP;
        } else if (is_digit(c)) {
          if (n == -1) n = 0;
          if (n < POW10_12) n = 10 * n + c - '0';
          else exp += 1;
          index_end = i;
        } else if (state == N_INT && c == '.') {
          index_dot = i;
          state = N_FRAC;
        } else { format_err = true; }
        break;
      case POST_N:
        if (c == ' ') continue;
        if (c == '-') {
          neg_exp = true;
          state = EXP;
        } else if (is_digit(c)) {
          has_exp = true;
          exp = c - '0';
          state = EXP;
        } else { format_err = true; }
        break;
      case EXP:
	if (c == ' ') {
          state = POST_EXP;
        } else if (is_digit(c)) {
          has_exp = true;
          if (exp < 100000000) exp = 10 * exp + c - '0';
        } else { format_err = true; }
        break;
      case POST_EXP:
        if (c == ' ') continue;
        format_err = true;
    }
    if (format_err) break;
  }
  if (n == -1) format_err = true;
  else if (!has_exp && neg_exp) format_err = true;
  if (format_err) {
    if (err) *err = true;
    return N_0;
  }

  if (n == 0) return N_0;
  if (neg_exp) exp = -exp;

  // Normalize.
  while (n < POW10_12) {
    n *= 10;
    exp -= 1;
  }
  if (neg_n) n = -n;
  if (index_dot == -1) index_dot = index_end;
  if (index_dot > index_end) index_dot = index_end;
  exp += 12 - index_end + index_dot;

  // Overflow/underflow.
  if (ABS(exp) > 99) {
    if (err) *err = true;
    n_t ret = exp < 0 ? N_EPS : N_INF;
    return n < 0 ? n_chs(ret) : ret;
  }

///printf("=> %lld %d\n", n, exp);
  return n_make(n, exp);
}
