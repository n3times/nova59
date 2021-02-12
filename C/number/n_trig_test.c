#include <stdio.h>

#include "n.h"

int main() {
  n_t n = { 9000000000000LL, 0 };
  printf("%lld %d\n", n.mant, n.exp);
  n = n_asin(n_acos(n_atan(n_tan(n_cos(n_sin(n, DEG,
      NULL), DEG, NULL), DEG, NULL), DEG, NULL), DEG, NULL), DEG, NULL);
  printf("%f\n", n2d(n));
}
