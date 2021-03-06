#include "n_test.h"

#include <assert.h>
#include <stdio.h>

typedef n_t (*fun_t)(n_t, n_t, n_err_t *);

static void p(n_t n1, n_t n2, n_t res, n_err_t err, char *op) {
  char s1[N_PRINT_MAX_SIZE];
  char s2[N_PRINT_MAX_SIZE];
  char s_res[N_PRINT_MAX_SIZE];
  printf("%s %s %s   =   %s%s\n",
         n_print(n1, s1),
         op,
         n_print(n2, s2),
         n_print(res, s_res),
         err ? "?" : "");
}

int main() {
  n_err_t err;
  n_t n;
  n_t ns[] = { TEST_NUMBERS };

  fun_t funs[] = { n_plus, n_minus, n_times, n_div, n_pow, n_ipow };
  char *ops[] = { "+", "-", "*", "/", "^", "v" };

  for (int k = 0; k < N_ELEMS(funs); k++) {
    for (int i = 0; i < N_ELEMS(ns); i++) {
      for (int j = 0; j < N_ELEMS(ns); j++) {
        n = funs[k](ns[i], ns[j], &err);
        p(ns[i], ns[j], n, err, ops[k]);
      }
      printf("\n");
    }
    printf("\n\n\n");
  }

  // Truncation in addition.

  n_t n1 = n_make(9999999999999LL, 13);
  n_t n2 = n_make(9999999999999LL, 0);
  n_t res = n_plus(n1, n2, NULL);
  assert(n_equals(res, n1));  // Note that n2 is ignored
  p(n1, n2, res, N_ERR_NONE, "+");

  // Truncation in substraction.

  n1 = n_make(1000000000000LL, 13);
  n2 = n_make(1000000000000LL, 0);
  res = n_minus(n1, n2, NULL);
  assert(n_equals(res, n1));  // Note that n2 is ignored
  p(n1, n2, res, N_ERR_NONE, "-");

  // No rounding in multiplication.

  n1 = n_make(1111111111111LL, 0);
  n2 = n_make(9900000000000LL, 1);
  res = n_times(n1, n2, NULL);
  assert(n_equals(res, n_make(1099999999999LL, 2)));
  p(n1, n2, res, N_ERR_NONE, "*");

  res = N_1;
  for (int i = 0; i < 100; i++) {
    res = n_times(res, N_PI, NULL);
  }
  p(N_PI, N_PI, res, N_ERR_NONE, "?");

  // No rounding in division.
  n1 = n_make(7000000000000LL, 0);
  n2 = n_make(9000000000000LL, 0);
  res = n_div(n1, n2, NULL);
  //assert(n_equals(res, n_make(7777777777777LL, -1)));
  p(n1, n2, res, N_ERR_NONE, "/");

  n1 = n_make(9999999999999LL, 0);
  res = n_times(n1, n1, NULL);
  p(n1, n1, res, N_ERR_NONE, "*");



  res = N_0;
  for (int i = 0; i < 100; i++) {
    res = n_plus(res, N_PI, NULL);
  }
  p(N_PI, N_PI, res, N_ERR_NONE, "?");
}
