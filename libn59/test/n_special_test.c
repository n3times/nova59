#include "n_test.h"

#include <stdio.h>

static n_t N_0_5 =     {  5000000000000LL,  -1 };
static n_t N_0_5_NEG = { -5000000000000LL,  -1 };
static n_t N_X =       {  1420000000000LL,   0 };
static n_t N_Y =       {  1000000000000LL, -11 };

static void test_dms(n_t n) {
  n_err_t e;
  n_t res = n_dms(n, 9, N_FLOAT, &e);
  printf("%lld %d => %lld %d%s\n",
         n.mant, n.exp, res.mant, res.exp, e ? "?" : "");
}

static void test_idms(n_t n) {
  n_err_t e;
  n_t res = n_idms(n, 9, N_FLOAT, &e);
  printf("%lld %d => %lld %d%s\n",
         n.mant, n.exp, res.mant, res.exp, e ? "?" : "");
}

static void test_idms_dms(n_t n) {
  n_err_t e1, e2;
  n_t res = n_idms(n_dms(n, 9, N_FLOAT, &e1), 9, N_FLOAT, &e2);
  printf("%lld %d => %lld %d%s\n",
         n.mant, n.exp, res.mant, res.exp, e1 || e2 ? "?" : "");
}

static void test_dms_idms(n_t n) {
  n_err_t e1, e2;
  n_t res = n_dms(n_idms(n, 9, N_FLOAT, &e1), 9, N_FLOAT, &e2);
  printf("%lld %d => %lld %d%s\n",
         n.mant, n.exp, res.mant, res.exp, e1 || e2 ? "?" : "");
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
