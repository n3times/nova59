#include "n_test.h"

#include <math.h>
#include <stdio.h>

typedef n_t (*fun_t)(n_t n, n_err_t *err);
typedef n_t (*fun2_t)(n_t n);

static void p(n_t n, n_t res, n_err_t err, char *op) {
  char s[N_PRINT_MAX_SIZE];
  char s_res[N_PRINT_MAX_SIZE];
  printf("%s %s   =   %s%s\n",
         n_print(n, s),
         op,
         n_print(res, s_res),
         err ? "?" : "");
}

static void test_funs() {
  n_err_t err;
  n_t res;
  n_t ns[] = { TEST_NUMBERS_2 };

  fun_t funs[] = { n_square, n_sqrt, n_log, n_ln, n_1_x, n_exp, n_pow10 };
  char *fun_strs[] = { "square", "sqrt", "log", "ln", "1/x" , "exp", "pow10" };

  // Test all the functions for all the test numbers.
  for (int i = 0; i < N_ELEMS(funs); i++) {
    for (int j = 0; j < N_ELEMS(ns); j++) {
      res = funs[i](ns[j], &err);
      p(ns[j], res, err, fun_strs[i]);
    }
    printf("\n\n\n");
  }
}

static void test_funs_2() {
  n_t res;
  n_t ns[] = { TEST_NUMBERS_2 };

  fun2_t funs[] = { n_int, n_frac, n_abs, n_chs };
  char *ops[] = { "int", "frac", "abs", "chs" };

  // Test all the functions for all the test numbers.
  for (int i = 0; i < N_ELEMS(funs); i++) {
    for (int j = 0; j < N_ELEMS(ns); j++) {
      res = funs[i](ns[j]);
      p(ns[j], res, N_ERR_NONE, ops[i]);
    }
    printf("\n\n\n");
  }
}
static void test_int() {
  n_t res;

  for (int i = -1; i < 14; i++) {
    res = n_make(pow(10, i));
    p(res, n_int(res), N_ERR_NONE, "int");
  }
  for (int i = -2; i < 14; i++) {
    res = n_make(9.999999999999 * pow(10, i));
    p(res, n_int(res), N_ERR_NONE, "int");
  }
  for (int i = -1; i < 14; i++) {
    res = n_make(1.234567890123 * pow(10, i));
    p(res, n_int(res), N_ERR_NONE, "int");
  }
  for (int i = -1; i < 14; i++) {
    res = n_make(-1.234567890123 * pow(10, i));
    p(res, n_int(res), N_ERR_NONE, "int");
  }
}

static void test_frac() {
  n_t res;

  for (int i = -1; i < 14; i++) {
    res = n_make(pow(10, i));
    p(res, n_frac(res), N_ERR_NONE, "frac");
  }
  for (int i = -2; i < 14; i++) {
    res = n_make(9.999999999999 * pow(10, i));
    p(res, n_frac(res), N_ERR_NONE, "frac");
  }
  for (int i = -1; i < 14; i++) {
    res = n_make(1.234567890123 * pow(10, i));
    p(res, n_frac(res), N_ERR_NONE, "frac");
  }
  for (int i = -1; i < 14; i++) {
    res = n_make(-1.234567890123 * pow(10, i));
    p(res, n_frac(res), N_ERR_NONE, "frac");
  }
}

int main() {
  test_funs();

  test_funs_2();

  test_int();

  printf("\n");

  test_frac();
}
