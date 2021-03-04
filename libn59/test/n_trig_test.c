#include "n_test.h"

#include <stdio.h>

#define PI 3.141592653590

typedef n_t (*trig_fun_t)(n_t, n_trig_t mode, n_err_t *err);

static void t(n_t n, n_trig_t mode, char *str, n_t (op)(n_t, n_trig_t, n_err_t *)) {
  n_err_t err;
  n_t res = op(n, mode, &err);
  char s[N_PRINT_MAX_SIZE];
  char s_res[N_PRINT_MAX_SIZE];
  printf("%4s %s => %s%s\n", str, n_print(n, s), n_print(res, s_res), err ? " ?" : "");
}

int main() {
  // Forensics.
  n_t nine = n_make(9000000000000LL, 0);
  n_t res = n_asin(n_acos(n_atan(n_tan(n_cos(n_sin(nine, N_DEG, NULL), N_DEG,
      NULL), N_DEG, NULL), N_DEG, NULL), N_DEG, NULL), N_DEG, NULL);
  printf("Forensics as(ac(at(t(c(s(9)))))):\n%lld %d => %lld %d\n\n",
         nine.mant, nine.exp, res.mant, res.exp);

  n_t n;
  trig_fun_t trig_funs[] = { n_sin, n_cos, n_tan, n_asin, n_acos, n_atan };
  char *trig_fun_strs[] = { "sin", "cos", "tan", "asin", "acos", "atan" };

  printf("DEGREES: -720 .. -90 -45 0 45 90 .. 720\n");
  printf("=======================================\n\n");
  for (int k = 0; k < N_ELEMS(trig_funs) / 2; k++) {
    for (int i = -16; i < 16; i += 1) {
      n = n_d2n(i * 45, NULL);
      t(n, N_DEG, trig_fun_strs[k], trig_funs[k]);
    }
    printf("\n");
  }
  printf("\n");

  printf("GRADIANS -800 .. -100 -50 0 50 100 .. 800\n");
  printf("=========================================\n\n");
  for (int k = 0; k < N_ELEMS(trig_funs) / 2; k++) {
    for (int i = -16; i < 16; i += 1) {
      n = n_d2n(i * 50, NULL);
      t(n, N_GRAD, trig_fun_strs[k], trig_funs[k]);
    }
    printf("\n");
  }
  printf("\n");

  printf("RADIANS -4PI .. -PI/2 -PI/4 0 PI/4 PI/2 .. 4PI\n");
  printf("==============================================\n\n");
  for (int k = 0; k < N_ELEMS(trig_funs) / 2; k++) {
    for (int i = -16; i < 16; i += 1) {
      double d = (i/2)*PI/2 + (i%2)*PI/4;
      t(n_d2n(d, NULL), N_RAD, trig_fun_strs[k], trig_funs[k]);
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

  n_t ns2[] = {
                n_make(3141592000000LL, 0),
                n_make(3141592600000LL, 0),
                n_make(3141592653589LL, 0),
                n_make(3141592653590LL, 0),
                n_make(3141592653591LL, 0),
                n_make(3141592700000LL, 0),
                n_make(3141593000000LL, 0),

                n_make(1570796000000LL, 0),
                n_make(1570796300000LL, 0),
                n_make(1570796326794LL, 0),
                n_make(1570796326795LL, 0),
                n_make(1570796326796LL, 0),
                n_make(1570796400000LL, 0),
                n_make(1570797000000LL, 0),
              };
  printf("Values around pi and pi/2 RADIANS\n");
  printf("=================================\n\n");
  for (int k = 0; k < N_ELEMS(trig_funs) / 2; k++) {
    for (int i = 0; i < N_ELEMS(ns2); i++) {
      t(ns2[i], N_RAD, trig_fun_strs[k], trig_funs[k]);
    }
    printf("\n");
  }
  printf("\n");
}
