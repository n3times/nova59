#include "n_test.h"

#include <stdio.h>

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

static void test_p_r(n_t rho, n_t theta, n_trig_t mode) {
  n_t x, y;
  n_err_t e;
  n_p_r(rho, theta, mode, &x, &y, &e);
  char s1[N_PRINT_MAX_SIZE];
  char s2[N_PRINT_MAX_SIZE];
  char s3[N_PRINT_MAX_SIZE];
  char s4[N_PRINT_MAX_SIZE];
  printf("%s  %s   =>  %s  %s%s\n",
         n_print(rho, s1), n_print(theta, s2),
         n_print(x, s3), n_print(y, s4),
         e ? "?" : "");
}

static void test_r_p(n_t x, n_t y, n_trig_t mode) {
  n_t rho, theta;
  n_err_t e;
  n_r_p(x, y, mode, &rho, &theta, &e);
  char s1[N_PRINT_MAX_SIZE];
  char s2[N_PRINT_MAX_SIZE];
  char s3[N_PRINT_MAX_SIZE];
  char s4[N_PRINT_MAX_SIZE];
  printf("%s  %s   =>  %s  %s%s\n",
         n_print(x, s1), n_print(y, s2),
         n_print(rho, s3), n_print(theta, s4),
         e ? "?" : "");
}

static void test_polar_rect() {
  for (int theta = 0; theta <= 360; theta += 10) {
    test_p_r(N_1, n_make(theta), N_DEG);
  }
}

static void test_rect_polar() {
  for (int theta = 0; theta <= 360; theta += 10) {
    n_t x, y;
    n_err_t e;
    n_p_r(N_1, n_make(theta), N_DEG, &x, &y, &e);
    test_r_p(x, y, N_DEG);
  }
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

  printf("\n[P -> R]  rho  theta  =>  x  y\n");
  test_polar_rect();

  printf("\n[R -> P]  x  y  =>  rho  theta\n");
  test_rect_polar();
}
