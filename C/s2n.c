#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "n.h"

static n_t n_chs(n_t n) {
  n.mant = -n.mant;
  return n;
}

n_t s2n(char *s, bool *err) {
  n_t n = N_0;
  bool neg = false;
  int index_dot = -1;
  int index_end;
  if (err) *err = false;

  // Compute mantissa/exponent.
  for (int i = 0; ; i++) {
    if (s[i] == '\0') break;
    if (i == 0 && s[i] == '-') {
      neg = true;
      continue;
    }
    if (s[i] == ' ' && s[i + 1] == '?') break;
    if (s[i] == ' ' || s[i] == '-') {
      n.exp = (s[i + 1] - '0') * 10 + s[i + 2] - '0';
      if (s[i] == '-') n.exp = -n.exp;
      break;
    }
    if (s[i] == '.') {
      index_dot = i;
      continue;
    }
    n.mant = n.mant * 10 + s[i] - '0';
    index_end = i;
  }
  if (n.mant == 0) return N_0;

  // Normalize.
  while (n.mant < pow(10, 12)) {
    n.mant *= 10;
    n.exp -= 1;
  }
  if (neg) n.mant = -n.mant;
  if (index_dot == -1) index_dot = index_end;
  n.exp += 12 - index_end + index_dot;

  // Overflow/underflow.
  if (ABS(n.exp) > 99) {
    if (err) *err = true;
    n = n.exp < 0 ? N_EPS : N_INF;
    return n.mant < 0 ? n_chs(n) : n;
  }

  return n;
}
