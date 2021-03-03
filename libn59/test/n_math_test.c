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
  n_t n;
  n_t ns[] = { TEST_NUMBERS };

  fun_t funs[] = { n_square, n_sqrt, n_log, n_ln, n_1_over_x, n_exp, n_pow10 };
  char *ops[] = { "square", "sqrt", "log", "ln", "1/x" , "exp", "pow10" };

  for (int k = 0; k < N_ELEMS(funs); k++) {
    for (int i = 0; i < N_ELEMS(ns); i++) {
      n = funs[k](ns[i], &err);
      p(ns[i], n, err, ops[k]);
    }
    printf("\n\n\n");
  }

  fun2_t fun2s[] = { n_int, n_frac, n_abs, n_chs };
  char *ops2[] = { "int", "frac", "abs", "chs" };

  for (int k = 0; k < N_ELEMS(fun2s); k++) {
    for (int i = 0; i < N_ELEMS(ns); i++) {
      n = fun2s[k](ns[i]);
      p(ns[i], n, false, ops2[k]);
    }
    printf("\n\n\n");
  }

}
