#include "n_test.h"

#include <assert.h>
#include <stdio.h>

/* Arithmetic operator. */
typedef n_t (*opr_t)(n_t, n_t, n_err_t *);

/* Prints operation. */
static void p(n_t n1, n_t n2, n_t res, n_err_t err, char *opr) {
  char s1[N_PRINT_MAX_SIZE];
  char s2[N_PRINT_MAX_SIZE];
  char s_res[N_PRINT_MAX_SIZE];
  printf("%s %s %s   =   %s%s\n",
         n_print(n1, s1),
         opr,
         n_print(n2, s2),
         n_print(res, s_res),
         err ? "?" : "");
}

static void test_all_operators() {
  n_err_t err;
  n_t res;

  n_t numbers[] = { TEST_NUMBERS };
  opr_t oprs[] = { n_plus, n_minus, n_times, n_div, n_pow, n_ipow };
  char *opr_strs[] = { "+", "-", "*", "/", "^", "v" };

  for (int i = 0; i < N_ELEMS(oprs); i++) {
    for (int j = 0; j < N_ELEMS(numbers); j++) {
      for (int k = 0; k < N_ELEMS(numbers); k++) {
        res = oprs[i](numbers[j], numbers[k], &err);
        p(numbers[j], numbers[k], res, err, opr_strs[i]);
      }
      printf("\n");
    }
    printf("\n\n\n");
  }
}

static void test_multiplication() {
  n_t n1, n2, res;

  // Multiplication truncates result to 13 digits with no rounding. Same as
  // TI-59.
  n1 = n_make(1.111111111111);
  n2 = n_make(99);
  res = n_times(n1, n2, NULL);
  assert(n_equals(res, n_make(109.9999999999)));  // And not 101.
  p(n1, n2, res, N_ERR_NONE, "*");

  // Multiplication always gives 13 accurate digits, unlike TI-59.
  // TI-59 gets 99.99999999989 while we get 99.99999999998.
  n1 = n_make(9.999999999999);
  res = n_times(n1, n1, NULL);
  p(n1, n1, res, N_ERR_NONE, "*");

  // Same accuracy as TI-59 for mantissas with less than 13 significant digits:
  // pi^100 using multiplication. Note that pi = (3141592653590, 0) has 12
  // significant digits, its mantissa ending in 0.
  res = N_1;
  for (int i = 0; i < 100; i++) {
    res = n_times(res, N_PI, NULL);
  }
  p(N_PI, N_PI, res, N_ERR_NONE, "?");

  // Same accuracy as TI-59 for mantissas with less than 13 significant digits:
  // 69! (See Pgm 16, from Master Library).
  res = N_1;
  for (int i = 69; i >= 1; i--) {
    res = n_times(res, n_make(i), NULL);
  }
  assert(n_equals(res, n_make(1.711224524264e98)));
  p(n_make(69), N_1, res, N_ERR_NONE, "!");
}

static void test_addition() {
  n_t n1, n2, res;

  // Number truncation before addition. Same as TI-59.
  n1 = n_make(9.999999999999e13);
  n2 = n_make(9.999999999999);
  res = n_plus(n1, n2, NULL);
  assert(n_equals(res, n1));  // Note that n2 is ignored
  p(n1, n2, res, N_ERR_NONE, "+");

  // Number truncation before substraction. Same as TI-59.
  n1 = n_make(1e13);
  n2 = N_1;
  res = n_minus(n1, n2, NULL);
  assert(n_equals(res, n1));  // Note that n2 is ignored
  p(n1, n2, res, N_ERR_NONE, "-");

  // Addition has same accuracy as TI-59: pi*100 using addition.
  res = N_0;
  for (int i = 0; i < 100; i++) {
    res = n_plus(res, N_PI, NULL);
  }
  p(N_PI, N_PI, res, N_ERR_NONE, "?");
}

int main() {
  test_all_operators();
  test_addition();
  test_multiplication();
}
