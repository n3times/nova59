#include "s_internal.h"

#include <string.h>


/******************************************************************************
 *
 *  HELPERS.
 *
 ******************************************************************************/

static char *get_end_mant(char *start_mant) {
  int len = strlen(start_mant);
  if (len <= 3) return start_mant + len;
  char *c = start_mant + len - 3;
  if (*c == ' ' || *c == '-') return c;
  return start_mant + len;
}

static void insert_at_end_mant(char *start_mant, char c) {
  char *end_mant = get_end_mant(start_mant);

  // Move exponent.
  memmove(end_mant + 1, end_mant, strlen(end_mant) + 1);

  *end_mant = c;
}


/******************************************************************************
 *
 *  IMPLEMENTATION.
 *
 ******************************************************************************/

void s_display_init(s_display_t *display) {
  CHECK(display);

  display->d[0] = '0';
  display->d[1] = '\0';
  display->edit_exp = false;
}

void s_display_digit(s_display_t *display, int digit) {
  CHECK(display);
  CHECK_D(digit);
  char *d = display->d;

  // Add digit to exponent.
  if (display->edit_exp) {
    int len = strlen(d);
    d[len - 2] = d[len - 1];
    d[len - 1] = '0' + digit;
    return;
  }

  // Add digit to mantissa.

  char *start = *d == '-' ? d + 1 : d;
  char *end = get_end_mant(d);

  // Case "0" or "-0".
  if (*start == '0' && end == start + 1) {
    *start = '0' + digit;
    return;
  }

  // Count mantissa digits.
  int dig_count = 0;
  for (char *p = start; p != end; p++) {
    if (p == end) break;
    if (*p == '.') continue;
    dig_count += 1;
  }

  // See if there is room for digit.
  bool exp = *end != '\0';
  if (dig_count == (exp ? 8 : 10)) {
    if (*start == '0' && start[1] == '.') {
      // Remove 0.
      memmove(start, start + 1, strlen(start + 1) + 1);
    } else {
      // No room for digit.
      return;
    }
  }

  insert_at_end_mant(d, '0' + digit);
}

void s_display_dot(s_display_t *display) {
  CHECK(display);
  char *d = display->d;

  if (display->edit_exp) {
    display->edit_exp = false;
  }
  char *dot = strchr(d, '.');
  if (dot) return;
  insert_at_end_mant(d, '.');
}

void s_display_chs(s_display_t *display) {
  CHECK(display);
  char *d = display->d;

  if (display->edit_exp) {
    char *sign = d + strlen(d) - 3;
    *sign = *sign == '-' ? ' ' : '-';
  } else {
    bool neg = *d == '-';
    if (neg) {
      memmove(d, d + 1, strlen(d + 1) + 1);
    } else {
      memmove(d + 1, d, strlen(d) + 1);
      d[0] = '-';
    }
  }
}

void s_display_ee(s_display_t *display) {
  CHECK(display);
  char *d = display->d;

  if (display->edit_exp) return;
  display->edit_exp = true;

  // Add exponent if missing.

  char *end_mant = get_end_mant(d);
  if (*end_mant != '\0') return;

  int dig_count = 0;
  for (char *c = d; *c != '\0'; c++) {
    if (*c >= '0' && *c <= '9') dig_count += 1;
  }

  if (dig_count == 9) {
    int i = 0;
    if (d[0] == '-') i = 1;
    if (d[i] == '0' && d[i + 1] == '.') {
      memmove(d + i, d + i + 1, strlen(d+ i + 1) + 1);
      dig_count = 8;
    }
  }

  if (dig_count <= 8) {
    insert_at_end_mant(d, ' ');
    insert_at_end_mant(d, '0');
    insert_at_end_mant(d, '0');
  }
}

void s_display_iee(s_display_t *display) {
  CHECK(display);
  if (display->edit_exp) {
    display->edit_exp = false;
  }

  char *d = display->d;
  int len = strlen(d);
  if (len >= 3 && d[len - 3] == ' ' && d[len - 2] == '0' && d[len - 1] == '0') {
    d[len - 3] = '\0';
  }
}
