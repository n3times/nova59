#include "s59.h"

#include <stdio.h>

static void test(char *input) {
  s_display_x_t display_x;
  s_display_x_init(&display_x);

  for (char *c = input; *c != '\0'; c++) {
    if (*c == 'k') {
      s_display_x_init(&display_x);
    } else if (*c == '-') {
      s_display_x_chs(&display_x);
    } else if (*c == '.') {
      s_display_x_dot(&display_x);
    } else if (*c == 'e') {
      s_display_x_ee(&display_x);
    } else if (*c == 'i') {
      s_display_x_iee(&display_x);
    } else if (*c >= '0' && *c <= '9') {
      s_display_x_digit(&display_x, *c - '0');
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

  printf("\n");
  s_display_x_t display_x;
  s_display_x_set_with_x(&display_x, n_make(1e45), 2, N_FLOAT, NULL);
  s_display_x_ee(&display_x);
  s_display_x_digit(&display_x, 9);
  printf("%s\n", display_x.display);

  return 0;
}
