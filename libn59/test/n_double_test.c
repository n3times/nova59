#include "n_test.h"

#include <stdio.h>

static void t_n2d(n_t n) {
  double d = n_n2d(n);
  n_err_t err;
  n_t n2 = n_d2n(d, &err);
  char s[N_PRINT_MAX_SIZE];
  char s2[N_PRINT_MAX_SIZE];
  printf("%s  =>  %20.13e  =>  %s%s\n",
         n_print(n, s), d, n_print(n2, s2), err ? " ?" : "");
}

static void t_d2n(double d) {
  n_err_t err;
  n_t n = n_d2n(d, &err);
  char s[N_PRINT_MAX_SIZE];
  double d2 = n_n2d(n);
  printf("%22.15e  =>  %s%s  =>  %22.15e\n",
         d, n_print(n, s), err ? " ?" : "  ", d2);
}

int main(void) {
  t_n2d(N_0);
  t_n2d(N_1);
  t_n2d(N_PI);
  t_n2d(N_INF);
  t_n2d(N_EPS);
  t_n2d(n_chs(N_1));
  t_n2d(n_chs(N_PI));
  t_n2d(n_chs(N_INF));
  t_n2d(n_chs(N_EPS));

  printf("\n");

  t_d2n(1);
  t_d2n(9.999999999999e99);
  t_d2n(1e100);
  t_d2n(1e300);
  t_d2n(1e-99);
  t_d2n(9.999999999999e-100);
  t_d2n(1e-300);
  t_d2n(1.2345678901234567890);
  t_d2n(9.9999999999999999999);
  t_d2n(9.9999999999994);
  t_d2n(9.99999999999949);
  t_d2n(9.999999999999495);
  t_d2n(9.9999999999995);
}
