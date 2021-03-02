#include "n_test.h"

#include <stdio.h>

#define PI_MANT 3141592653590LL
#define PI 3.141592653590

typedef n_t (*trig_fun_t)(n_t, n_trig_t mode, n_err_t *err);

static void t(n_t n, n_trig_t mode, char *str, n_t (op)(n_t, n_trig_t, n_err_t *)) {
  n_t res = op(n, mode, NULL);
  printf("%4s % 014lld% 03d => % 014lld% 03d\n",
         str, n.mant, n.exp, res.mant, res.exp);
}

int main() {
  // Forensics.
  n_t n = n_make(9000000000000LL, 0);
  n_t res = n_asin(n_acos(n_atan(n_tan(n_cos(n_sin(n, N_DEG, NULL), N_DEG,
      NULL), N_DEG, NULL), N_DEG, NULL), N_DEG, NULL), N_DEG, NULL);
  printf("Forensics as(ac(at(t(c(s(9)))))):\n%lld %d => %lld %d\n\n",
         n.mant, n.exp, res.mant, res.exp);

  trig_fun_t funs[] = { n_sin, n_cos, n_tan, n_asin, n_acos, n_atan };
  char *ops[] = { "sin", "cos", "tan", "asin", "acos", "atan" };

  printf("DEGREES: -720 .. -90 -45 0 45 90 .. 720\n");
  printf("=======================================\n\n");
  for (int k = 0; k < N_ELEMS(funs) / 2; k++) {
    for (int i = -16; i < 16; i += 1) {
      n = n_d2n(i * 45, NULL);
      t(n, N_DEG, ops[k], funs[k]);
    }
    printf("\n");
  }
  printf("\n");

  printf("GRADIANS -800 .. -100 -50 0 50 100 .. 800\n");
  printf("=========================================\n\n");
  for (int k = 0; k < N_ELEMS(funs) / 2; k++) {
    for (int i = -16; i < 16; i += 1) {
      n = n_d2n(i * 50, NULL);
      t(n, N_GRAD, ops[k], funs[k]);
    }
    printf("\n");
  }
  printf("\n");

  printf("RADIANS -4PI .. -PI/2 -PI/4 0 PI/4 PI/2 .. 4PI\n");
  printf("==============================================\n\n");
  for (int k = 0; k < N_ELEMS(funs) / 2; k++) {
    for (int i = -16; i < 16; i += 1) {
      double d = (i/2)*PI/2 + (i%2)*PI/4;
      t(n_d2n(d, NULL), N_RAD, ops[k], funs[k]);
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
    for (int k = 0; k < N_ELEMS(funs); k++) {
      for (int i = 0; i < N_ELEMS(ns); i++) {
        t(ns[i], modes[j], ops[k], funs[k]);
      }
      printf("\n");
    }
    printf("\n");
  }
}
