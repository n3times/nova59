#include <stdio.h>

#include "n.h"

int main() {
  n_t n = { 1570796326795LL, 0 };  // 9 degrees
  printf("%lld %d\n", n.mant, n.exp);
  n = n_asin(n_acos(n_atan(n_tan(n_cos(n_sin(n, 
      NULL), NULL), NULL), NULL), NULL), NULL);
  printf("%f\n", n2d(n) * 100000000 * 180 / 3.141592656359);
}
