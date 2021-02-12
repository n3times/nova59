#include <math.h>

#include "n.h"

double n2d(n_t n) {
  return (double)n.mant * pow(10, n.exp - 12);
}
