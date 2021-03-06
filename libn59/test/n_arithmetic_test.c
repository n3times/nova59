#include "n_test.h"

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

  // No rounding.
  n_t n1 = n_make(1000000000000L, 1);
  n_t n2 = n_make(9000000000000L, 0);
  n_t n3 = n_make(9900000000000L, 1);
  n_t res = n_div(n1, n2, NULL);

  p(n1, n2, res, N_ERR_NONE, "/"); 
  p(res, n3, n_times(res, n3, NULL), N_ERR_NONE, "*"); 
}
