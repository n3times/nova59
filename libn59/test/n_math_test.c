#include "n_test.h"

#include <stdio.h>

typedef n_t (*fun_t)(n_t, n_err_t *err);
typedef n_t (*fun2_t)(n_t);

static void p(n_t n, n_t res, n_err_t err, char *op) {
  printf("% 014lld% 03d %s   =   % 014lld% 03d%s\n",
         n.mant, n.exp,
         op,
         res.mant, res.exp,
         err ? "?" : "");
}

int main() {
  n_err_t err;
  n_t n;
  n_t ns[] = {
    N_0, N_1, N_PI, N_EPS, N_INF,
    n_chs(N_1), n_chs(N_PI), n_chs(N_EPS), n_chs(N_INF)
  };

  fun_t funs[] = { n_square, n_sqrt, n_log, n_ln, n_1_x, n_exp, n_pow10 };
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
