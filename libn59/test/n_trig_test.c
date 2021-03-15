#include "n_test.h"

#include <stdio.h>

#define PI 3.141592653590

typedef n_t (*trig_fun_t)(n_t, n_trig_t mode, n_err_t *err);

static void t(n_t n, n_trig_t mode, char *str, n_t (op)(n_t, n_trig_t, n_err_t *)) {
  n_err_t err;
  n_t res = op(n, mode, &err);
  char s[N_PRINT_MAX_SIZE];
  char s_res[N_PRINT_MAX_SIZE];
  printf("%4s %s => %s%s\n",
         str, n_print(n, s), n_print(res, s_res), err ? " ?" : "");
}

static void test_forensics() {
  n_trig_t D = N_DEG;
  n_t res = n_asin(n_acos(n_atan(n_tan(n_cos(n_sin(n_make(9), D, 0), D,
      0), D, 0), D, 0), D, 0), D, 0);
  char str[N_PRINT_MAX_SIZE];
  printf("Forensics:\n  asin(acos(atan(tan(cos(sin(9)))))) = %s\n\n\n",
         n_print(res, str));
}

int main() {
  test_forensics();

  n_t n;
  trig_fun_t trig_funs[] = { n_sin, n_cos, n_tan, n_asin, n_acos, n_atan };
  char *trig_fun_strs[] = { "sin", "cos", "tan", "asin", "acos", "atan" };

  printf("DEGREES: -720 .. -90 -45 0 45 90 .. 720\n");
  printf("=======================================\n\n");
  for (int k = 0; k < N_ELEMS(trig_funs) / 2; k++) {
    for (int i = -16; i < 16; i += 1) {
      n = n_make(i * 45);
      t(n, N_DEG, trig_fun_strs[k], trig_funs[k]);
    }
    printf("\n");
  }
  printf("\n");

  printf("GRADIANS -800 .. -100 -50 0 50 100 .. 800\n");
  printf("=========================================\n\n");
  for (int k = 0; k < N_ELEMS(trig_funs) / 2; k++) {
    for (int i = -16; i < 16; i += 1) {
      n = n_make(i * 50);
      t(n, N_GRAD, trig_fun_strs[k], trig_funs[k]);
    }
    printf("\n");
  }
  printf("\n");

  printf("RADIANS -4PI .. -PI/2 -PI/4 0 PI/4 PI/2 .. 4PI\n");
  printf("==============================================\n\n");
  for (int k = 0; k < N_ELEMS(trig_funs) / 2; k++) {
    for (int i = -16; i < 16; i += 1) {
      n = n_times(n_div(N_PI, n_make(2), NULL), n_make(i / 2), NULL);
      n = n_plus(n, n_times(n_div(N_PI, n_make(4), NULL),
                            n_make(i % 2), NULL), NULL);
      t(n, N_RAD, trig_fun_strs[k], trig_funs[k]);
    }
    printf("\n");
  }
  printf("\n");

  n_t ns[] = { TEST_NUMBERS };
  n_trig_t modes[] = { N_DEG, N_GRAD, N_RAD };
  char *mode_strs[] = { "Deg ", "Grad", "Rad "};
  for (int j = 0; j < N_ELEMS(modes); j++) {
    printf("%s: 0 1 pi eps inf -1 -pi -eps -inf\n", mode_strs[j]);
    printf("=====================================\n\n");
    for (int k = 0; k < N_ELEMS(trig_funs); k++) {
      for (int i = 0; i < N_ELEMS(ns); i++) {
        t(ns[i], modes[j], trig_fun_strs[k], trig_funs[k]);
      }
      printf("\n");
    }
    printf("\n");
  }

  printf("Values around pi and pi/2 RADIANS\n");
  printf("=================================\n\n");
  n_t ns2[] = {
                // Around pi.
                n_make(3.141592),
                n_make(3.1415926),
                n_make(3.141592653589),
                n_make(3.141592653590),
                n_make(3.141592653591),
                n_make(3.1415927),
                n_make(3.141593),

                // Around pi/2.
                n_make(1.570796),
                n_make(1.5707963),
                n_make(1.570796326794),
                n_make(1.570796326795),
                n_make(1.570796326796),
                n_make(1.5707964),
                n_make(1.570797),
              };
  for (int i = 0; i < N_ELEMS(trig_funs) / 2; i++) {
    for (int j = 0; j < N_ELEMS(ns2); j++) {
      t(ns2[j], N_RAD, trig_fun_strs[i], trig_funs[i]);
    }
    printf("\n");
  }
  printf("\n");

  n_trig_t trig_modes[] = { N_DEG, N_GRAD, N_RAD };
  for (int i = 0; i < N_ELEMS(trig_modes); i++) {
    printf("\n%s: input -1.0 -0.9 -0.8 ... 1.0\n\n", mode_strs[i]);
    for (int j = -10; j <= 10; j++) {
      t(n_make(j / 10.), trig_modes[i], "asin", n_asin);
    }
    printf("\n");
    for (int j = -10; j <= 10; j++) {
      t(n_make(j / 10.), trig_modes[i], "acos", n_acos);
    }
    printf("\n");
    for (int j = -10; j <= 10; j++) {
      t(n_make(j / 10.), trig_modes[i], "atan", n_atan);
    }
    printf("\n");
  }
}
