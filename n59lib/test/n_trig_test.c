#include <stdio.h>

#include "n.h"

int main() {
  // Forensics.
  n_t n = n_make(9000000000000LL, 0);
  n_t res = n_asin(n_acos(n_atan(n_tan(n_cos(n_sin(n, N_DEG, NULL), N_DEG,
      NULL), N_DEG, NULL), N_DEG, NULL), N_DEG, NULL), N_DEG, NULL);
  printf("Forensics asin(acos(atan(tan(cos(sin(9)))))):\n%lld %d => %lld %d\n\n",
     n.mant, n.exp, res.mant, res.exp);

  n_trig_t mode = N_GRAD;
  for (double i = -800; i < 800; i += 50) {
    printf("%f sin %e\n", i, n_n2d(n_sin(n_d2n(i, NULL), mode, NULL)));
    printf("%f cos %e\n", i, n_n2d(n_cos(n_d2n(i, NULL), mode, NULL)));
    printf("%f tan %e\n", i, n_n2d(n_tan(n_d2n(i, NULL), mode, NULL)));
  }
  n_t n2 = n_make(3141592653590LL, 0);
  mode = N_RAD;
  printf("%lld sin %e\n", n2.mant, n_n2d(n_sin(n2, mode, NULL)));
  printf("%lld cos %e\n", n2.mant, n_n2d(n_cos(n2, mode, NULL)));
  printf("%lld tan %e\n", n2.mant, n_n2d(n_tan(n2, mode, NULL)));
  n2.mant /= 2;
  printf("%lld sin %e\n", n2.mant, n_n2d(n_sin(n2, mode, NULL)));
  printf("%lld cos %e\n", n2.mant, n_n2d(n_cos(n2, mode, NULL)));
  printf("%lld tan %e\n", n2.mant, n_n2d(n_tan(n2, mode, NULL)));
}
