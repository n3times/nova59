#include "n_test.h"

#include <math.h>
#include <stdio.h>

static void p(n_t n, n_t res, n_err_t err, char *op) {
  char s[N_PRINT_MAX_SIZE];
  char s_res[N_PRINT_MAX_SIZE];

  printf("%s %s    =   %s%s\n",
         n_print(n, s),
         op,
         n_print(res, s_res),
         err ? "?" : "");
}

static void test_funs() {
  typedef n_t (*fun_t)(n_t n, n_err_t *err);

  n_err_t err;
  n_t res;
  n_t ns[] = { TEST_NUMBERS_2 };
  fun_t funs[] = { n_square, n_sqrt, n_log, n_ln, n_1_x, n_exp, n_pow10 };
  char *fun_strs[] =
      { "  ^2", "  vx", " log", "  ln", " 1/x" , " exp", " ^10" };

  // Test all the functions for all the test numbers.
  for (int i = 0; i < N_ELEMS(funs); i++) {
    for (int j = 0; j < N_ELEMS(ns); j++) {
      res = funs[i](ns[j], &err);
      p(ns[j], res, err, fun_strs[i]);
    }
    printf("\n\n");
  }
}

static void test_funs_2() {
  typedef n_t (*fun_t)(n_t n);

  n_t res;
  n_t ns[] = { TEST_NUMBERS_2 };
  fun_t funs[] = { n_int, n_frac, n_abs, n_chs };
  char *ops[] = { " int", " frc", " abs", " chs" };

  // Test all the functions for all the test numbers.
  for (int i = 0; i < N_ELEMS(funs); i++) {
    for (int j = 0; j < N_ELEMS(ns); j++) {
      res = funs[i](ns[j]);
      p(ns[j], res, N_ERR_NONE, ops[i]);
    }
    printf("\n\n");
  }
}

static void test_int() {
  n_t res;

  for (int i = -1; i < 14; i++) {
    res = n_make(pow(10, i));
    p(res, n_int(res), N_ERR_NONE, " int");
  }
  printf("\n");

  for (int i = -2; i < 14; i++) {
    res = n_make(9.999999999999 * pow(10, i));
    p(res, n_int(res), N_ERR_NONE, " int");
  }
  printf("\n");

  for (int i = -1; i < 14; i++) {
    res = n_make(1.234567890123 * pow(10, i));
    p(res, n_int(res), N_ERR_NONE, " int");
  }
  printf("\n");

  for (int i = -1; i < 14; i++) {
    res = n_make(-1.234567890123 * pow(10, i));
    p(res, n_int(res), N_ERR_NONE, " int");
  }
  printf("\n");
}

static void test_frac() {
  n_t res;

  for (int i = -1; i < 14; i++) {
    res = n_make(pow(10, i));
    p(res, n_frac(res), N_ERR_NONE, " frc");
  }
  printf("\n");

  for (int i = -2; i < 14; i++) {
    res = n_make(9.999999999999 * pow(10, i));
    p(res, n_frac(res), N_ERR_NONE, " frc");
  }
  printf("\n");

  for (int i = -1; i < 14; i++) {
    res = n_make(1.234567890123 * pow(10, i));
    p(res, n_frac(res), N_ERR_NONE, " frc");
  }
  printf("\n");

  for (int i = -1; i < 14; i++) {
    res = n_make(-1.234567890123 * pow(10, i));
    p(res, n_frac(res), N_ERR_NONE, " frc");
  }
}

static void test_sqrt() {
  n_t n_1_plus_eps  = n_make(1.000000000001);
  n_t n_1_minus_eps = n_make(.9999999999999);

  p(n_1_plus_eps,  n_sqrt(n_1_plus_eps, NULL),  N_ERR_NONE, "sqrt");
  p(n_1_minus_eps, n_sqrt(n_1_minus_eps, NULL), N_ERR_NONE, "sqrt");
}

int main() {
  test_funs();

  test_funs_2();

  test_int();

  test_frac();

  printf("\n");
  test_sqrt();
}
