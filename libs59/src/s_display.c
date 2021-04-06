#include "s_internal.h"

#include <assert.h>
#include <string.h>

#define CHECK_DISPLAY_EDIT(d) { CHECK(d); assert(check_display_X(d)); }


/******************************************************************************
 *
 *  HELPERS.
 *
 ******************************************************************************/

#if !NDEBUG
static bool is_digit(char c) {
  return c >= '0' && c <= '9';
}

static bool check_display_X(s_display_X_t * display_X) {
  char *d = display_X->display;

  // Should not start with "0d" or "-0d".
  if (*d == '-') d += 1;
  if (*d == '0' && is_digit(*(d + 1))) return false;

  // One dot and up to 8 or 10 digits.
  int digit_count = 0;
  bool has_dot = false;
  while (*d == '.' || is_digit(*d)) {
    if (*d == '.') {
      if (has_dot) return false;
      has_dot = true;
    } else {
      digit_count += 1;
    }
    d += 1;
  }

  // Has no exp.
  bool is_exp = *d != '\0';
  if (!is_exp) {
    if (digit_count > 10) return false;
    if (display_X->editing_exp) return false;
    return true;
  }

  // Has exp.
  if (digit_count > 8) return false;
  if (strlen(d) != 3) return false;
  if (*d != ' ' && *d != '-') return false;
  if (!is_digit(*(d + 1))) return false;
  if (!is_digit(*(d + 2))) return false;
  return true;
}
#endif

/** Returns the location of the first character after the mantissa. */
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

void s_display_X_init(s_display_X_t *display_X) {
  CHECK(display_X);

  display_X->display[0] = '0';
  display_X->display[1] = '\0';
  display_X->editing_exp = false;

  CHECK_DISPLAY_EDIT(display_X);
}

void s_display_X_digit(s_display_X_t *display_X, int digit) {
  CHECK_D(digit);
  CHECK_DISPLAY_EDIT(display_X);
  char *d = display_X->display;

  // Add digit to exponent.
  if (display_X->editing_exp) {
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

void s_display_X_dot(s_display_X_t *display_X) {
  CHECK_DISPLAY_EDIT(display_X);
  char *d = display_X->display;

  if (display_X->editing_exp) {
    display_X->editing_exp = false;
  }
  char *dot = strchr(d, '.');
  if (dot) return;
  insert_at_end_mant(d, '.');
}

void s_display_X_chs(s_display_X_t *display_X) {
  CHECK_DISPLAY_EDIT(display_X);
  char *d = display_X->display;

  if (display_X->editing_exp) {
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

void s_display_X_ee(s_display_X_t *display_X) {
  CHECK_DISPLAY_EDIT(display_X);
  char *d = display_X->display;

  if (display_X->editing_exp) return;

  // Add exponent if missing.

  char *end_mant = get_end_mant(d);
  if (*end_mant != '\0') {
    display_X->editing_exp = true;
    return;
  }

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
    display_X->editing_exp = true;
  }
}

void s_display_X_iee(s_display_X_t *display_X) {
  CHECK_DISPLAY_EDIT(display_X);

  if (display_X->editing_exp) {
    display_X->editing_exp = false;
  }

  char *d = display_X->display;
  int len = strlen(d);
  if (len >= 3 && d[len - 3] == ' ' && d[len - 2] == '0' && d[len - 1] == '0') {
    d[len - 3] = '\0';
  }
}
