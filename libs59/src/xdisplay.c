#include "s_internal.h"

#include <assert.h>
#include <string.h>

#define CHECK_DISPLAY_EDIT(d) { CHECK(d); assert(check_xdisplay(d)); }


/******************************************************************************
 *
 *  HELPERS.
 *
 ******************************************************************************/

#if !NDEBUG
static bool is_digit(char c) {
  return c >= '0' && c <= '9';
}

static bool check_xdisplay(xdisplay_t * xdisplay) {
  char *d = xdisplay->display;

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
    if (xdisplay->mode == XDISPLAY_MODE_EDIT_EXP) return false;
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
 *
 * In case of overflow, sets 'blink' to true;
 */
static void update_display(xdisplay_t *d) {
  n_format_t format = d->eng ? N_ENG : d->ee ? N_SCI : N_FLOAT;
  n_err_t err;
  n_n2s(d->x, d->fix, format, d->display, &err);
  if (err) d->blink = true;
}


/******************************************************************************
 *
 *  EDITING.
 *
 ******************************************************************************/

void xdisplay_edit_init(xdisplay_t *d) {
  CHECK(d);

  xdisplay_edit_clear(d);
  d->eng = false;
  d->fix = 9;

  CHECK_DISPLAY_EDIT(d);
}

void xdisplay_edit_clear(xdisplay_t *d) {
  CHECK(d);

  d->display[0] = '0';
  d->display[1] = '\0';
  d->blink = false;
  d->mode = XDISPLAY_MODE_EDIT_MANT;
  d->x = N_0;
  d->ee = false;

  CHECK_DISPLAY_EDIT(d);
}

void xdisplay_edit_digit(xdisplay_t *xdisplay, int digit) {
  CHECK_D(digit);
  CHECK_DISPLAY_EDIT(xdisplay);
  char *d = xdisplay->display;

  if (xdisplay->mode == XDISPLAY_MODE_REG) {
    xdisplay->display[0] = '0' + digit;
    xdisplay->display[1] = '\0';
    xdisplay->mode = XDISPLAY_MODE_EDIT_MANT;
    return;
  }

  // Add digit to exponent.
  if (xdisplay->mode == XDISPLAY_MODE_EDIT_EXP) {
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

void xdisplay_edit_dot(xdisplay_t *xdisplay) {
  CHECK_DISPLAY_EDIT(xdisplay);
  char *d = xdisplay->display;

  if (xdisplay->mode == XDISPLAY_MODE_REG) {
    xdisplay->display[0] = '0';
    xdisplay->display[1] = '.';
    xdisplay->display[2] = '\0';
    xdisplay->mode = XDISPLAY_MODE_EDIT_MANT;
    return;
  }

  xdisplay->mode = XDISPLAY_MODE_EDIT_MANT;
  char *dot = strchr(d, '.');
  if (dot) return;
  insert_at_end_mant(d, '.');
}

void xdisplay_edit_chs(xdisplay_t *xdisplay) {
  CHECK_DISPLAY_EDIT(xdisplay);
  char *d = xdisplay->display;

  if (xdisplay->mode == XDISPLAY_MODE_EDIT_EXP) {
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


/******************************************************************************
 *
 *  MODES.
 *
 ******************************************************************************/

void xdisplay_mode_ee(xdisplay_t *xdisplay) {
  CHECK_DISPLAY_EDIT(xdisplay);
  char *d = xdisplay->display;

  xdisplay->ee = true;

  if (xdisplay->mode == XDISPLAY_MODE_EDIT_EXP) return;

  // Add exponent if missing.

  char *end_mant = get_end_mant(d);
  if (*end_mant != '\0') {
    xdisplay->mode = XDISPLAY_MODE_EDIT_EXP;
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
    xdisplay->mode = XDISPLAY_MODE_EDIT_EXP;
  } else {
    xdisplay->mode = XDISPLAY_MODE_EDIT_MANT;
  }
}

void xdisplay_mode_iee(xdisplay_t *xdisplay) {
  CHECK_DISPLAY_EDIT(xdisplay);

  xdisplay->ee = false;

  if (xdisplay->mode == XDISPLAY_MODE_REG) return;

  xdisplay->mode = XDISPLAY_MODE_EDIT_MANT;

  char *d = xdisplay->display;
  int len = strlen(d);
  if (len >= 3 && d[len - 3] == ' ' && d[len - 2] == '0' && d[len - 1] == '0') {
    d[len - 3] = '\0';
  }
}

void xdisplay_mode_eng(xdisplay_t *d) {
  d->eng = true;
  d->mode = XDISPLAY_MODE_REG;
  update_display(d);
}

void xdisplay_mode_ieng(xdisplay_t *d) {
  d->eng = false;
  d->mode = XDISPLAY_MODE_REG;
  update_display(d);
}

void xdisplay_mode_fix(xdisplay_t *d, int fix) {
  d->fix = fix;
  d->mode = XDISPLAY_MODE_REG;
  update_display(d);
}


/******************************************************************************
 *
 *  SYNCHRONIZATION DISPLAY X <=> REG_X X.
 *
 ******************************************************************************/

void xdisplay_update_x(xdisplay_t *d, n_t X) {
  d->mode = XDISPLAY_MODE_REG;
  d->x = X;
  update_display(d);
}

void xdisplay_set_blink(xdisplay_t *d) {
  d->blink = true;
}

n_t xdisplay_resolve_edit(xdisplay_t *d) {
  assert(d->mode != XDISPLAY_MODE_REG);

  n_err_t err;
  n_t X = n_s2n(d->display, &err);
  if (err) d->blink = true;

  return X;
}
