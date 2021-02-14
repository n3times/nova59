#include <stdio.h>

#include "n.h"

int main() {
  n_t n = {1234567890123LL, 13};
  printf("%lld %d\n", n.mant, n.exp);
  printf("%f\n", n2d(n_sqrt(n, NULL)));
}
