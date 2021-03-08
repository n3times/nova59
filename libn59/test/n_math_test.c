#include "n_test.h"

#include <stdio.h>

typedef n_t (*fun_t)(n_t, n_err_t *err);
typedef n_t (*fun2_t)(n_t);

static void p(n_t n, n_t res, n_err_t err, char *op) {
  char s[N_PRINT_MAX_SIZE];
  char s_res[N_PRINT_MAX_SIZE];
  printf("%s %s   =   %s%s\n",
         n_print(n, s),
         op,
         n_print(res, s_res),
         err ? "?" : "");
}

int main() {
  n_err_t err;
  n_t res;
  n_t ns[] = { TEST_NUMBERS, N_10, N_E };

  fun_t funs[] = { n_square, n_sqrt, n_log, n_ln, n_1_over_x, n_exp, n_pow10 };
  char *fun_strs[] = { "square", "sqrt", "log", "ln", "1/x" , "exp", "pow10" };

  // Test all the functions for all the test numbers.
  for (int i = 0; i < N_ELEMS(funs); i++) {
    for (int j = 0; j < N_ELEMS(ns); j++) {
      res = funs[i](ns[j], &err);
      p(ns[j], res, err, fun_strs[i]);
    }
    printf("\n\n\n");
  }

  fun2_t fun2s[] = { n_int, n_frac, n_abs, n_chs };
  char *ops2[] = { "int", "frac", "abs", "chs" };

  for (int k = 0; k < N_ELEMS(fun2s); k++) {
    for (int i = 0; i < N_ELEMS(ns); i++) {
      res = fun2s[k](ns[i]);
      p(ns[i], res, N_ERR_NONE, ops2[k]);
    }
    printf("\n\n\n");
  }

  // n_int.
  for (int i = -1; i < 14; i++) {
    res = n_make(1000000000000LL, i);
    p(res, n_int(res), N_ERR_NONE, "int");
  }
  for (int i = -2; i < 14; i++) {
    res = n_make(9999999999999LL, i);
    p(res, n_int(res), N_ERR_NONE, "int");
  }
  for (int i = -1; i < 14; i++) {
    res = n_make(1234567890123LL, i);
    p(res, n_int(res), N_ERR_NONE, "int");
  }
  for (int i = -1; i < 14; i++) {
    res = n_make(-1234567890123LL, i);
    p(res, n_int(res), N_ERR_NONE, "int");
  }

  printf("\n");

  // n_frac.
  for (int i = -1; i < 14; i++) {
    res = n_make(1000000000000LL, i);
    p(res, n_frac(res), N_ERR_NONE, "frac");
  }
  for (int i = -2; i < 14; i++) {
    res = n_make(9999999999999LL, i);
    p(res, n_frac(res), N_ERR_NONE, "frac");
  }
  for (int i = -1; i < 14; i++) {
    res = n_make(1234567890123LL, i);
    p(res, n_frac(res), N_ERR_NONE, "frac");
  }
  for (int i = -1; i < 14; i++) {
    res = n_make(-1234567890123LL, i);
    p(res, n_frac(res), N_ERR_NONE, "frac");
  }
}