#include "xdisplay.h"

#include <assert.h>
#include <stdio.h>

static void input(xdisplay_t *d, char * input) {
  for (char *c = input; *c != '\0'; c++) {
    if (*c == 'k') {
      xdisplay_edit_init(d);
    } else if (*c == '-') {
      xdisplay_edit_chs(d);
    } else if (*c == '.') {
      xdisplay_edit_dot(d);
    } else if (*c == 'e') {
      xdisplay_mode_ee(d);
    } else if (*c == 'i') {
      xdisplay_mode_iee(d);
    } else if (*c == 'E') {
      xdisplay_mode_eng(d);
    } else if (*c == 'I') {
      xdisplay_mode_ieng(d);
    } else if (*c >= '0' && *c <= '9') {
      xdisplay_edit_digit(d, *c - '0');
    }
  }
}

static void test(char *chars) {
  xdisplay_t d;
  xdisplay_edit_init(&d);
  input(&d, chars);
  printf("%s\n", d.display);
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
  xdisplay_t d;
  xdisplay_edit_init(&d);
  xdisplay_update_x(&d, n_make(1e45));
  printf("%s\n", d.display);
  xdisplay_mode_ee(&d);
  xdisplay_edit_digit(&d, 9);
  xdisplay_edit_dot(&d);
  xdisplay_edit_digit(&d, 7);
  printf("%s\n", d.display);

  // Replace last visible digit of number.
  printf("\n");
  xdisplay_edit_init(&d);
  xdisplay_mode_fix(&d, 4);
  xdisplay_update_x(&d, n_make(12345.54321));
  printf("%s\n", d.display);
  xdisplay_mode_ee(&d);
  printf("%s\n", d.display);
  xdisplay_edit_digit(&d, 5);
  printf("%s\n", d.display);

  // Only keep visible digits of pi.
  printf("\n");
  for (int i = 0; i <= 9; i++) {
    xdisplay_edit_init(&d);
    xdisplay_mode_fix(&d, i);
    xdisplay_update_x(&d, N_PI);
    xdisplay_mode_ee(&d);
    xdisplay_mode_iee(&d);
    n_t X = xdisplay_resolve_edit(&d);
    char str[N_PRINT_MAX_SIZE];
    printf("%s : %s\n", n_print(X, str), d.display);
  }

  // '0 = +/-'.
  printf("\n");
  xdisplay_edit_init(&d);
  xdisplay_update_x(&d, N_0);
  xdisplay_edit_chs(&d);
  printf("%s\n", d.display);

  // 'Fix 0 9.9 EE 99 lnx'.
  printf("\n");
  xdisplay_edit_init(&d);
  xdisplay_mode_fix(&d, 0);
  input(&d, "9.9e99");
  n_t X = xdisplay_resolve_edit(&d);
  n_err_t err;
  X = n_ln(X, &err);
  xdisplay_update_x(&d, X);
  printf("%s\n", d.display);
  assert(!d.blink);

  return 0;
}
