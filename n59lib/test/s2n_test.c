#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "n.h"

#define ABS(x)  ( (x) < 0 ? (-x) : (x) )

static void t(char *s) {
  bool err = false;
  n_t n = s2n(s, &err);
  printf("%20s: % 14lld%s%02d%s\n",
         s, n.mant, n.exp < 0 ? "-" : " ", ABS(n.exp), err ? " ?" : "");
}

int main() {
  char s[16];
  for (int i = 0; i < 1; i++) {
    for (int j = -99; j <= 99; j++) {
      sprintf(s, "%lld.%s%02d",
              (long long) pow(10, i), j < 0 ? "-" : " " ,ABS(j));
      t(s);
    }
  }
  t("0");
  t("1");
  t("1234567890");
  t("1234567890123");
  t("0.");
  t("123.");
  t("0.0");
  t("0.000");
  t("0.0001");
  t("-0.0001");
  t("1 99");
  t("1-99");
  t("123.456 03");
  t("123.456 3");
  t("123.456-3");
  t("123.456-03");
  t("123.456 100");
  t("123.456-110");
  t("123.456  1");
  t("123.456 -1");
  t(".1 0");
}
