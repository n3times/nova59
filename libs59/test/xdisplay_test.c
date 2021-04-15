#include "xdisplay.h"

#include <assert.h>
#include <stdio.h>

static void input(xdisplay_t *x, char * input) {
  for (char *c = input; *c != '\0'; c++) {
    if (*c == 'k') {
      xdisplay_init(x);
    } else if (*c == '-') {
      xdisplay_chs(x);
    } else if (*c == '.') {
      xdisplay_dot(x);
    } else if (*c == 'e') {
      xdisplay_ee(x);
    } else if (*c == 'i') {
      xdisplay_iee(x);
    } else if (*c == 'E') {
      xdisplay_eng(x);
    } else if (*c == 'I') {
      xdisplay_ieng(x);
    } else if (*c >= '0' && *c <= '9') {
      xdisplay_digit(x, *c - '0');
    }
  }
}

static void test(char *chars) {
  xdisplay_t x;
  xdisplay_init(&x);
  input(&x, chars);
  printf("%s\n", x.display);
}

int main() {
  // "0".
  test("0");
  test("00");
  test("000");
  test("--");
  test("0ei");
  test("-0ei-");
  test("e-ie-i");
  test("1.23e45k");
  test("k");

  // "-0".
  printf("\n");
  test("-");
  test("---");
  test("-0ei");
  test("0-");

  // "0.".
  printf("\n");
  test(".");
  test("..");
  test("0.");
  test("0...--");

  // "-1.2".
  printf("\n");
  test("-1.2");
  test("1.2-");
  test("1-.2");
  test("1.-2");
  test("-1..2");
  test("-1.2.");


  // ".9999999999".
  printf("\n");
  test(".9999999999");
  test("0.9999999999");
  test("000.9999999999");
  test("0.999999999999");
  test(".999999999e9");
  test(".9999999999e");

  // "9999999999".
  printf("\n");
  test("9999999999");
  test("99999999999999");
  test("9999999999e12");
  test("9999999999e12i45");

  // "1 12".
  printf("\n");
  test("1e12");
  test("1ee12");
  test("e12i1");
  test("e11i1e12");
  test("e11i1e1341312");

  // "1234.5678-59".
  printf("\n");
  test("1234.5678e-59");
  test("e-59i1234.5678");
  test("e-59i1234e.5678");

  // "0 00".
  printf("\n");
  test("e");
  test("e--");
  test("-e-i-e-");
  test("0e00");
  test("e0");

  // "-9999999999-99"
  printf("\n");
  test("-99999999.e-99");
  test("99999999e99-.-");
  test("-99999999.e-99i9999");

  // "-.9999999999-99"
  printf("\n");
  test("-.99999999e-99");
  test(".99999999e99-.-");

  // Edit number.
  printf("\n");
  xdisplay_t x;
  xdisplay_init(&x);
  xdisplay_update_reg_x(&x, n_make(1e45));
  printf("%s\n", x.display);
  xdisplay_ee(&x);
  xdisplay_digit(&x, 9);
  xdisplay_dot(&x);
  xdisplay_digit(&x, 7);
  printf("%s\n", x.display);

  // Replace last visible digit of number.
  printf("\n");
  xdisplay_init(&x);
  xdisplay_fix(&x, 4);
  xdisplay_update_reg_x(&x, n_make(12345.54321));
  printf("%s\n", x.display);
  xdisplay_ee(&x);
  printf("%s\n", x.display);
  xdisplay_digit(&x, 5);
  printf("%s\n", x.display);

  // Only keep visible digits of pi.
  printf("\n");
  for (int i = 0; i <= 9; i++) {
    xdisplay_init(&x);
    xdisplay_fix(&x, i);
    xdisplay_update_reg_x(&x, N_PI);
    xdisplay_ee(&x);
    xdisplay_iee(&x);
    n_t X = xdisplay_resolve_edit(&x);
    char str[N_PRINT_MAX_SIZE];
    printf("%s : %s\n", n_print(X, str), x.display);
  }

  // '0 = +/-'.
  printf("\n");
  xdisplay_init(&x);
  xdisplay_update_reg_x(&x, N_0);
  xdisplay_chs(&x);
  printf("%s\n", x.display);

  // 'Fix 0 9.9 EE 99 lnx'.
  printf("\n");
  xdisplay_init(&x);
  xdisplay_fix(&x, 0);
  input(&x, "9.9e99");
  n_t X = xdisplay_resolve_edit(&x);
  n_err_t err;
  X = n_ln(X, &err);
  xdisplay_update_reg_x(&x, X);
  printf("%s\n", x.display);
  assert(!x.blinking);

  return 0;
}
