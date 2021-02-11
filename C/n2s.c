#include "stdio.h"
#include "math.h"
#include "stdbool.h"

#define ABS(x)          ( (x) < 0 ? (-x) : (x) )
#define MIN(x, y)       ( (x) < (y) ? (x) : (y) )
#define MAX(x, y)       ( (x) > (y) ? (x) : (y) )
#define N_ELEMS(array)  ( sizeof(array) / sizeof((array)[0]) )

typedef struct n_s {
  long long mant;
  int exp;
} n_t;

typedef enum mode_e {
  FLOAT,
  EE,
  ENG
} mode_t;

static n_t N_0 = { 0LL, 0 };

static void get_display(bool neg, long long mant, int int_len, int frac_len,
                        int exp, bool err, char *str_out) {
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
  if (err) { str_out[i++] = ' '; str_out[i++] = '?'; }
  str_out[i++] = '\0';
}

// n is a TI-59 number.
// fix in 0..9.
// mode in { FLOAT, EE, ENG).
// str_out should have at least 16 characters.
// 
// Returns representation of number as string:
// - optional "-"
// - followed by decimal number, with one "."
// - optionally followed by exponent: " " and 2 digits
// - optionally followed by overflow: " ?".
void n2s(n_t n, int fix, mode_t mode, char *str_out) {
  bool neg = n.mant < 0;
  long long mant = ABS(n.mant);
  int exp = n.exp;

  bool is_big = exp >= 10; // || (exp == 9 && mant >= pow(10, 13) - 500);
  bool is_small = exp <= -12 || (exp == -11 && mant < 5 * pow(10, 12));
  bool is_exp = mode != FLOAT || is_big || (is_small && fix == 9);

  if (is_small && fix != 9 && mode == FLOAT) {
    return n2s(N_0, fix, FLOAT, str_out);
  }

  // Compute actual mantissa.
  int mant_len = is_exp ? 8 : 10;
  int int_len = 0;
  if (is_exp) {
    int_len = 1;
    if (mode == ENG) {
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
      n_t n_1 = { pow(10, 12) * (neg ? -1 : 1), n.exp + 1 };
      return n2s(n_1, fix, mode, str_out);
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
  bool err = false;
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
    err = true;
  }

  get_display(neg, mant, int_len, frac_len, is_exp ? exp : -100, err, str_out);
}


int main(void) {
  long long mants[] = { 
                         1000000000000LL,
                         9999999999999LL,
                         9999999999499LL,
                         9999999999500LL,
                         9999999949999LL,
                         9999999950000LL,
                         1234123412341LL,
                        -1234123412341LL,
                         5678956789567LL,
                        -5678956789567LL,
                         5000000000000LL,
                         4999999999999LL,
                                     0LL,
                   };
  int exps[] = {
      -99, -98, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1,
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 98, 99
  };
  int fixes[] = { 0, 5, 8, 9 };
  mode_t modes[] = { FLOAT, EE, ENG };

  for (int i = 0; i < N_ELEMS(mants); i++) {
    printf("==========================================================\n\n");
    printf("mantissa = %lld\n\n", mants[i]);
    for (int j = 0; j < N_ELEMS(modes); j++) {
      for (int k = 0; k < N_ELEMS(fixes); k++) {
        printf("=== mode %s | fix %d ===\n\n",
               j == FLOAT ? "FLOAT" : j == EE ? "EE" : "ENG", fixes[k]);
        for (int l = 0; l < N_ELEMS(exps); l++) {
          if (mants[i] == 0 && exps[l]) continue;
          n_t n = { mants[i], exps[l] };
          char str[16];
          n2s(n, fixes[k], modes[j], (char *)&str);
          printf("%s\n", str);
        }
        printf("\n");
      }
    }
  }
}
