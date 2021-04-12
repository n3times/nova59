#include "s_display_x.h"

#include <stdio.h>

static void test(char *input) {
  s_display_x_t display_x;
  s_display_x_edit_init(&display_x);

  for (char *c = input; *c != '\0'; c++) {
    if (*c == 'k') {
      s_display_x_edit_init(&display_x);
    } else if (*c == '-') {
      s_display_x_edit_chs(&display_x);
    } else if (*c == '.') {
      s_display_x_edit_dot(&display_x);
    } else if (*c == 'e') {
      s_display_x_mode_ee(&display_x);
    } else if (*c == 'i') {
      s_display_x_mode_iee(&display_x);
    } else if (*c >= '0' && *c <= '9') {
      s_display_x_edit_digit(&display_x, *c - '0');
    }
  }

  printf("%s\n", display_x.display);
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
  s_display_x_t display_x;
  s_display_x_edit_init(&display_x);
  s_display_x_update_x(&display_x, n_make(1e45));
  printf("%s\n", display_x.display);
  s_display_x_mode_ee(&display_x);
  s_display_x_edit_digit(&display_x, 9);
  s_display_x_edit_dot(&display_x);
  s_display_x_edit_digit(&display_x, 7);
  printf("%s\n", display_x.display);

  // Replace last visible digit of number.
  printf("\n");
  s_display_x_edit_init(&display_x);
  s_display_x_mode_fix(&display_x, 4);
  s_display_x_update_x(&display_x, n_make(12345.54321));
  printf("%s\n", display_x.display);
  s_display_x_mode_ee(&display_x);
  printf("%s\n", display_x.display);
  s_display_x_edit_digit(&display_x, 5);
  printf("%s\n", display_x.display);

  // Only keep visible digits of pi.
  printf("\n");
  for (int i = 0; i <= 9; i++) {
    s_display_x_edit_init(&display_x);
    s_display_x_mode_fix(&display_x, i);
    s_display_x_update_x(&display_x, N_PI);
    s_display_x_mode_ee(&display_x);
    s_display_x_mode_iee(&display_x);
    n_t X;
    s_display_x_update_reg(&display_x, &X);
    char str[N_PRINT_MAX_SIZE];
    printf("%s : %s\n", n_print(X, str), display_x.display);
  }

  printf("\n");
  s_display_x_edit_init(&display_x);
  s_display_x_mode_fix(&display_x, 1);
  s_display_x_update_x(&display_x, n_make(-0.01));
  s_display_x_edit_chs(&display_x);
  printf("%s\n", display_x.display);

  return 0;
}
