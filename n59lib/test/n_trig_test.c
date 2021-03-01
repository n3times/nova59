#include <stdio.h>

#include "n.h"

#define PI_MANT 3141592653590LL
#define PI 3.141592653590
#define N_ELEMS(array)  ( sizeof(array) / sizeof((array)[0]) )

typedef n_t (*fun_t)(n_t, n_trig_t mode, bool *err);

static void t(char *mode_str, n_t n, n_trig_t mode, char *str, n_t (op)(n_t, n_trig_t, bool *)) {
  n_t res = op(n, mode, NULL);
  printf("%s %s % 12f => % 014lld% 03d\n", mode_str, str, n_n2d(n), res.mant, res.exp);
}

int main() {
  // Forensics.
  n_t n = n_make(9000000000000LL, 0);
  n_t res = n_asin(n_acos(n_atan(n_tan(n_cos(n_sin(n, N_DEG, NULL), N_DEG,
      NULL), N_DEG, NULL), N_DEG, NULL), N_DEG, NULL), N_DEG, NULL);
  printf("Forensics asin(acos(atan(tan(cos(sin(9)))))):\n%lld %d => %lld %d\n\n",
     n.mant, n.exp, res.mant, res.exp);

  fun_t funs[] = { n_sin, n_cos, n_tan };
  char *ops[] = { "sin", "cos", "tan" };

  for (int k = 0; k < N_ELEMS(funs); k++) {
    for (double i = -720; i < 720; i += 45) {
      t("Deg: ", n_d2n(i, NULL), N_DEG, ops[k], funs[k]);
    }
    printf("\n");
  }

  for (int k = 0; k < N_ELEMS(funs); k++) {
    for (double i = -800; i < 800; i += 50) {
      t("Grad:", n_d2n(i, NULL), N_GRAD, ops[k], funs[k]);
    }
    printf("\n");
  }

  for (int k = 0; k < N_ELEMS(funs); k++) {
    for (int i = -16; i < 16; i += 1) {
      double d = (i/2)*PI/2 + (i%2)*PI/4;
      t("Rad: ", n_d2n(d, NULL), N_RAD, ops[k], funs[k]);
    }
    printf("\n");
  }
}
