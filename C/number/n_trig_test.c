#include <stdio.h>

#include "n.h"

int main() {
  n_t n = {-7110000000000LL, 2 };
  printf("%lld %d\n", n.mant, n.exp);
  n = n_asin(n_acos(n_atan(n_tan(n_cos(n_sin(n, DEG,
      NULL), DEG, NULL), DEG, NULL), DEG, NULL), DEG, NULL), DEG, NULL);
  printf("%f\n", n2d(n));
  trig_t mode = GRAD;
  for (double i = -800; i < 800; i += 50) {
    printf("%f sin %e\n", i, n2d(n_sin(d2n(i, NULL), mode, NULL)));
    printf("%f cos %e\n", i, n2d(n_cos(d2n(i, NULL), mode, NULL)));
    printf("%f tan %e\n", i, n2d(n_tan(d2n(i, NULL), mode, NULL)));
  }
  n_t n2 = { 3141592653590LL, 0 };
  mode = RAD;
  printf("%lld sin %e\n", n2.mant, n2d(n_sin(n2, mode, NULL)));
  printf("%lld cos %e\n", n2.mant, n2d(n_cos(n2, mode, NULL)));
  printf("%lld tan %e\n", n2.mant, n2d(n_tan(n2, mode, NULL)));
  n2.mant /= 2;
  printf("%lld sin %e\n", n2.mant, n2d(n_sin(n2, mode, NULL)));
  printf("%lld cos %e\n", n2.mant, n2d(n_cos(n2, mode, NULL)));
  printf("%lld tan %e\n", n2.mant, n2d(n_tan(n2, mode, NULL)));
}
