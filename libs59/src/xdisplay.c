#include "s_internal.h"

#include <assert.h>
#include <string.h>

#define CHECK_XDISPLAY(x) { CHECK(x); assert(check_xdisplay(x)); }


/******************************************************************************
 *
 *  HELPERS.
 *
 ******************************************************************************/

#if !NDEBUG
static bool is_digit(char c) {
  return c >= '0' && c <= '9';
}

static bool check_xdisplay(xdisplay_t * x) {
  char *d = x->display;

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
    if (x->mode == XDISPLAY_MODE_EDIT_EXP) return false;
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

/**
 * Updates the display taking into account 'x', 'fix', 'ee' and 'eng'.
 */
static void update_display(xdisplay_t *x) {
  n_format_t format = x->eng ? N_ENG : x->ee ? N_SCI : N_FLOAT;
  n_err_t err;

  n_n2s(x->reg_x, x->fix, format, x->display, &err);
  x->mode = XDISPLAY_MODE_REG;
  x->overflow = err;
}


/******************************************************************************
 *
 *  IMPLEMENTATION.
 *
 ******************************************************************************/

void xdisplay_init(xdisplay_t *x) {
  CHECK(x);

  x->display[0] = '0';
  x->display[1] = '\0';
  x->mode = XDISPLAY_MODE_EDIT_MANT;
  x->overflow = false;

  x->reg_x = N_0;
  x->ee = false;
  x->eng = false;
  x->fix = 9;

  CHECK_XDISPLAY(x);
}

void xdisplay_clear(xdisplay_t *x) {
  CHECK_XDISPLAY(x);

  x->display[0] = '0';
  x->display[1] = '\0';
  x->mode = XDISPLAY_MODE_EDIT_MANT;
  x->overflow = false;

  x->reg_x = N_0;
  x->ee = false;

  CHECK_XDISPLAY(x);
}

void xdisplay_clear_entry(xdisplay_t *x) {
  CHECK_XDISPLAY(x);

  if (x->mode != XDISPLAY_MODE_REG) {
    x->display[0] = '0';
    x->display[1] = '\0';
    x->mode = XDISPLAY_MODE_EDIT_MANT;
    x->overflow = false;

    x->reg_x = N_0;
  }

  CHECK_XDISPLAY(x);
}

void xdisplay_digit(xdisplay_t *x, int digit) {
  CHECK_XDISPLAY(x);
  CHECK_D(digit);
  char *d = x->display;

  if (x->mode == XDISPLAY_MODE_REG) {
    x->display[0] = '0' + digit;
    x->display[1] = '\0';
    x->mode = XDISPLAY_MODE_EDIT_MANT;
    return;
  }

  // Add digit to exponent.
  if (x->mode == XDISPLAY_MODE_EDIT_EXP) {
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

void xdisplay_dot(xdisplay_t *x) {
  CHECK_XDISPLAY(x);
  char *d = x->display;

  if (x->mode == XDISPLAY_MODE_REG) {
    x->display[0] = '0';
    x->display[1] = '.';
    x->display[2] = '\0';
    x->mode = XDISPLAY_MODE_EDIT_MANT;
    return;
  }

  x->mode = XDISPLAY_MODE_EDIT_MANT;
  char *dot = strchr(d, '.');
  if (dot) return;
  insert_at_end_mant(d, '.');
}

void xdisplay_chs(xdisplay_t *x) {
  CHECK_XDISPLAY(x);
  char *d = x->display;

  if (x->mode == XDISPLAY_MODE_EDIT_EXP) {
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

  CHECK_XDISPLAY(x);
}

void xdisplay_ee(xdisplay_t *x) {
  CHECK_XDISPLAY(x);
  char *d = x->display;

  x->ee = true;

  if (x->mode == XDISPLAY_MODE_EDIT_EXP) return;

  // Add exponent if missing.

  char *end_mant = get_end_mant(d);
  if (*end_mant != '\0') {
    x->mode = XDISPLAY_MODE_EDIT_EXP;
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
    x->mode = XDISPLAY_MODE_EDIT_EXP;
  } else {
    x->mode = XDISPLAY_MODE_EDIT_MANT;
  }
}

void xdisplay_iee(xdisplay_t *x) {
  CHECK_XDISPLAY(x);

  x->ee = false;

  if (x->mode == XDISPLAY_MODE_REG) return;

  x->mode = XDISPLAY_MODE_EDIT_MANT;

  char *d = x->display;
  int len = strlen(d);
  if (len >= 3 && d[len - 3] == ' ' && d[len - 2] == '0' && d[len - 1] == '0') {
    d[len - 3] = '\0';
  }

  CHECK_XDISPLAY(x);
}

void xdisplay_eng(xdisplay_t *x) {
  CHECK_XDISPLAY(x);

  x->eng = true;

  update_display(x);

  CHECK_XDISPLAY(x);
}

void xdisplay_ieng(xdisplay_t *x) {
  CHECK_XDISPLAY(x);

  x->eng = false;

  update_display(x);

  CHECK_XDISPLAY(x);
}

void xdisplay_fix(xdisplay_t *x, int fix) {
  CHECK_XDISPLAY(x);

  x->fix = fix;

  update_display(x);

  CHECK_XDISPLAY(x);
}

void xdisplay_update_reg_x(xdisplay_t *x, n_t X) {
  CHECK_XDISPLAY(x);

  x->reg_x = X;

  update_display(x);

  CHECK_XDISPLAY(x);
}

void xdisplay_resolve_edit(xdisplay_t *x, n_err_t *err) {
  assert(x->mode != XDISPLAY_MODE_REG);

  x->reg_x = n_s2n(x->display, err);

  update_display(x);
}
