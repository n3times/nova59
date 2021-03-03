#include "n_test.h"

#include <stdio.h>

static void t1(n_t n) {
  double d = n_n2d(n);
  n_err_t err;
  n_t n2 = n_d2n(d, &err);
  char s[N_PRINT_MAX_SIZE];
  char s2[N_PRINT_MAX_SIZE];
  printf("%s  =>  %20.13e  =>  %s%s\n",
         n_print(n, s), d, n_print(n2, s2), err ? " ?" : "");
}

static void t2(double d) {
  n_err_t err;
  n_t n = n_d2n(d, &err);
  char s[N_PRINT_MAX_SIZE];
  double d2 = n_n2d(n);
  printf("%22.15e  =>  %s%s  =>  %22.15e\n",
         d, n_print(n, s), err ? " ?" : "  ", d2);
}

int main(void) {
  t1(N_0);
  t1(N_1);
  t1(N_PI);
  t1(N_INF);
  t1(N_EPS);
  t1(n_chs(N_1));
  t1(n_chs(N_PI));
  t1(n_chs(N_INF));
  t1(n_chs(N_EPS));

  printf("\n");

  t2(1);
  t2(9.999999999999e99);
  t2(1e100);
  t2(1e300);
  t2(1e-99);
  t2(9.999999999999e-100);
  t2(1e-300);
  t2(1.2345678901234567890);
  t2(9.9999999999999999999);
  t2(9.9999999999994);
  t2(9.99999999999949);
  t2(9.999999999999495);
  t2(9.9999999999995);
}
