#include <stdio.h>

#include "n.h"

#define N_ELEMS(array)  ( sizeof(array) / sizeof((array)[0]) )

typedef n_t (*fun_t)(n_t, n_t, bool *);

static void p(n_t n1, n_t n2, n_t res, bool err, char *op) {
  printf("% 014lld% 03d %s % 014lld% 03d   =   % 014lld% 03d%s\n",
         n1.mant, n1.exp,
         op,
         n2.mant, n2.exp,
         res.mant, res.exp,
         err ? "?" : "");
}

int main() {
  bool err;
  n_t n;
  n_t ns[] = {
    N_0, N_1, N_PI, N_EPS, N_INF,
    n_chs(N_1), n_chs(N_PI), n_chs(N_EPS), n_chs(N_INF)
  };

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
}
