#include <stdio.h>

#include "n.h"

int main() {
  n_t n1 = { 8000000000000LL, -11 };
  n_t n2 = { 7000000000000LL,   0 };
  ///printf("%f\n", n2d(n_plus(n1, n2, NULL)));
  ///printf("%f\n", n2d(n_minus(n1, n2, NULL)));
  ///printf("%f\n", n2d(n_minus(n2, n1, NULL)));
  ///printf("%f\n", n2d(n_times(n1, n2, NULL)));
  ///printf("%f\n", n2d(n_div(n1, n2, NULL)));

  n_t min = n_minus(n1, n2, NULL);

  printf("%lld %d\n", min.mant, min.exp);
  printf("%f\n", n2d(min));
  printf("%e\n", n2d(min));
}
