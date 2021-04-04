#include "s59.h"

#include <stdio.h>

static void test(char *input) {
  s_display_t display;
  s_display_init(&display);

  for (char *c = input; *c != '\0'; c++) {
    if (*c == 'k') {
      s_display_init(&display);
    } else if (*c == '-') {
      s_display_chs(&display);
    } else if (*c == '.') {
      s_display_dot(&display);
    } else if (*c == 'e') {
      s_display_ee(&display);
    } else if (*c == 'i') {
      s_display_iee(&display);
    } else if (*c >= '0' && *c <= '9') {
      s_display_digit(&display, *c - '0');
    }
  }

  printf("%s\n", display.d);
}

int main() {
  test("0");
  test("00");
  test("000");

  printf("\n");
  test("-");
  test("--");
  test("---");

  printf("\n");
  test("1");
  test("12");
  test("1234567890123");

  printf("\n");
  test(".");
  test(".1");
  test(".1234567890123");

  printf("\n");
  test("0.");
  test("0.123456789");
  test("0.123456789012");

  printf("\n");
  test("e");
  test("e1");
  test("e12");
  test("e123");

  printf("\n");
  test("e");
  test("e-");
  test("e--");
  test("e---");
  test("e---i-");

  printf("\n");
  test("12e34.56e78");

  printf("\n");
  test("12345678e");
  test("123456789e");
  test(".1234567e");
  test(".12345678e");
  test(".123456789e");

  printf("\n");
  test("1k");
  test("ek");
  test("1e-1k");

  return 0;
}
