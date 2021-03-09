#include "n_test.h"

#include <stdio.h>

#define MAX(x, y)  ( (x) > (y) ?  (x) : (y) )

static n_t N_0_5 =     {  5000000000000LL,  -1 };
static n_t N_0_5_NEG = { -5000000000000LL,  -1 };
static n_t N_X =       {  1420000000000LL,   0 };
static n_t N_Y =       {  1000000000000LL, -11 };

static void p(n_t n, n_t res, n_err_t e) {
  char str_n[N_PRINT_MAX_SIZE];
  char str_res[N_PRINT_MAX_SIZE];
  printf("%s => %s%s\n",
         n_print(n, str_n), n_print(res, str_res), e ? "?" : "");
}

static void test_dms(n_t n) {
  n_err_t e;
  n_t res = n_dms(n, 9, N_FLOAT, &e);
  p(n, res, e);
}

static void test_idms(n_t n) {
  n_err_t e;
  n_t res = n_idms(n, 9, N_FLOAT, &e);
  p(n, res, e);
}

static void test_idms_dms(n_t n) {
  n_err_t e1, e2;
  n_t res = n_idms(n_dms(n, 9, N_FLOAT, &e1), 9, N_FLOAT, &e2);
  p(n, res, MAX(e1, e2));
}

static void test_dms_idms(n_t n) {
  n_err_t e1, e2;
  n_t res = n_dms(n_idms(n, 9, N_FLOAT, &e1), 9, N_FLOAT, &e2);
  p(n, res, MAX(e1, e2));
}

int  main() {
  n_t ns[] = { N_0, N_1, N_PI, N_EPS, N_INF, N_0_5, N_0_5_NEG, N_X, N_Y };
  printf("dms\n");
  for (int i = 0; i < N_ELEMS(ns); i++) { test_dms(ns[i]); }
  printf("\nidms\n");
  for (int i = 0; i < N_ELEMS(ns); i++) { test_idms(ns[i]); }
  printf("\nidms dms\n");
  for (int i = 0; i < N_ELEMS(ns); i++) { test_idms_dms(ns[i]); }
  printf("\ndms idms\n");
  for (int i = 0; i < N_ELEMS(ns); i++) { test_dms_idms(ns[i]); }
}
