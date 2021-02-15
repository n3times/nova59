#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "n.h"

#define ABS(x)  ( (x) < 0 ? (-x) : (x) )

int main() {
  char s[16];
  for (int i = 0; i < 1; i++) {
    for (int j = -99; j <= 99; j++) {
      sprintf(s, "%lld.%s%02d",
              (long long) pow(10, i), j < 0 ? "-" : " " ,ABS(j));
      bool err = false;
      n_t n = s2n(s, &err);
      printf("%s: %lld %d%s\n", s, n.mant, n.exp, err ? " ?" : "");
    }
  }
}
