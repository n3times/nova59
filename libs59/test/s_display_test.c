#include "s59.h"

#include <stdio.h>

int main() {
  s_display_t display;
  s_display_init(&display);
  s_display_dot(&display);
  for (int i = 0; i < 20; i++)
    s_display_digit(&display, 1);
  s_display_ee(&display);
  printf("%s\n", display.d);
  return 0;
}
