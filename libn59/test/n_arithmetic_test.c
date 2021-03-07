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
  n_t res;
  n_t ns[] = { TEST_NUMBERS };

  fun_t oprs[] = { n_plus, n_minus, n_times, n_div, n_pow, n_ipow };
  char *opr_strs[] = { "+", "-", "*", "/", "^", "v" };

  // Test all the arithmetic operators for all the test number combinations.
  for (int i = 0; i < N_ELEMS(oprs); i++) {
    for (int j = 0; j < N_ELEMS(ns); j++) {
      for (int k = 0; k < N_ELEMS(ns); k++) {
        res = oprs[i](ns[j], ns[k], &err);
        p(ns[j], ns[k], res, err, opr_strs[i]);
      }
      printf("\n");
    }
    printf("\n\n\n");
  }

  // TI-59 accurate addition.
  n_t n1 = n_make(9999999999999LL, 13);
  n_t n2 = n_make(9999999999999LL, 0);
  res = n_plus(n1, n2, NULL);
  assert(n_equals(res, n1));  // Note that n2 is ignored
  p(n1, n2, res, N_ERR_NONE, "+");

  // TI-59 accurate substraction.
  n1 = n_make(1000000000000LL, 13);
  n2 = N_1;
  res = n_minus(n1, n2, NULL);
  assert(n_equals(res, n1));  // Note that n2 is ignored
  p(n1, n2, res, N_ERR_NONE, "-");

  // TI-59 accurate multiplication.
  n1 = n_make(1111111111111LL, 0);
  n2 = n_d2n(99, NULL);
  res = n_times(n1, n2, NULL);
  assert(n_equals(res, n_make(1099999999999LL, 2)));  // And not 101.
  p(n1, n2, res, N_ERR_NONE, "*");

  // TI-59 accurate multiplication.
  n1 = n_make(9999999999999LL, 0);
  res = n_times(n1, n1, NULL);
  p(n1, n1, res, N_ERR_NONE, "*");

  // TI-59 accurate addition: pi*100 using addition.
  res = N_0;
  for (int i = 0; i < 100; i++) {
    res = n_plus(res, N_PI, NULL);
  }
  p(N_PI, N_PI, res, N_ERR_NONE, "?");

  // TI-59 accurate multiplication: pi^100 using multiplication.
  res = N_1;
  for (int i = 0; i < 100; i++) {
    res = n_times(res, N_PI, NULL);
  }
  p(N_PI, N_PI, res, N_ERR_NONE, "?");

  // TI-59 accurate multiplication: 69! (See Pgm 16, from Master Library).
  res = N_1;
  for (int i = 69; i >= 1; i--) {
    res = n_times(res, n_d2n(i, NULL), NULL);
  }
  assert(n_equals(res, n_make(1711224524264LL, 98)));
  p(n_d2n(69, NULL), N_1, res, N_ERR_NONE, "!");
}
