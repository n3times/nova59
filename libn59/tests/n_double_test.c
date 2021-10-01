#include "n_test.h"

#include <assert.h>
#include <stdio.h>

static void test_d2n_n2d_is_identity() {
  n_t numbers[] = { TEST_NUMBERS_2 };

  for (int i = 0; i < N_ELEMS(numbers); i++) {
    n_t n1 = numbers[i];
    double d = n_n2d(n1);
    n_err_t err;
    n_t n2 = n_d2n(d, &err);

    assert(n_equals(n1, n2));

    char s1[N_PRINT_MAX_SIZE];
    char s2[N_PRINT_MAX_SIZE];
    printf("%s  =>  %20.13e  =>  %s%s\n",
           n_print(n1, s1), d, n_print(n2, s2), err ? " ?" : "");
  }
}

static void t_d2n(double d) {
  n_err_t err;
  n_t n = n_d2n(d, &err);
  char s[N_PRINT_MAX_SIZE];
  double d2 = n_n2d(n);

  printf("%22.15e  =>  %s%s  =>  %22.15e\n",
         d, n_print(n, s), err ? " ?" : "  ", d2);
}

static void test_d2n() {
  t_d2n(1);                      // N_1.
  t_d2n(9.999999999999e99);      // N_INF.
  t_d2n(1e100);                  // Overflow.
  t_d2n(1e300);                  // Overflow.
  t_d2n(1e-99);                  // N_EPS.
  t_d2n(9.999999999999e-100);    // Underflow.
  t_d2n(1e-300);                 // Underflow.
  t_d2n(1.2345678901234567890);  // Round down.
  t_d2n(9.9999999999999);        // Round up.
  t_d2n(9.9999999999994);        // Round down.
}

int main(void) {
  test_d2n_n2d_is_identity();
  printf("\n");
  test_d2n();
}
