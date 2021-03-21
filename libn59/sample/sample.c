/**
 * Command line RPN calculator based on libn59.
 *
 * Operations: + - / * ^ i^
 *             chs abs int frac
 *             xx ii v ln log iln ilog
 *             sin cos tan asin acos atan
 *             dms idms pr rp
 * Formats: flt sci eng f0-f9 deg rad grad
 * Stack:   clr
 * Numbers: 0-9 . ~
 *
 * UI shows current line, number stack, last operations.
 * Current line can be edited with del key.
 *
 * Example: 2 ^ 3 - sin(4)
 *          2 newline 3 ^ 4 sin -
 *          stack: 2, 2 2, 2
 */

#include "n59.h"

#include <assert.h>
#include <curses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 16

static n_format_t format = N_FLOAT;
static int fix = 9;
static n_trig_t trig = N_DEG;

static n_t n_x = { 0, 0 };
static n_t n_y = { 0, 0 };
static n_t n_z = { 0, 0 };
static n_t n_w = { 0, 0 };

static void prepare_screen() {
  initscr();
  noecho();

  mvprintw(0, 5, "CLI-59");
  mvprintw(0, 30, "Fix");
  mvprintw(2, 5, "O =");

  mvprintw(4, 5, "T =");
  mvprintw(5, 5, "Z =");
  mvprintw(6, 5, "Y =");
  mvprintw(7, 5, "X =");

  mvprintw(9, 5, "D =");
  mvprintw(11, 5, "I =");
}

static void update_screen(char *line) {
  mvprintw(0, 34, "%d", fix);
  mvprintw(
    0, 40, "%s", format == N_FLOAT ? "FLT" : format == N_SCI ? "SCI" : "ENG");
  mvprintw(
    0, 48, "%s", trig == N_RAD ? "RAD" : trig == N_DEG ? "DEG" : "GRAD");

  char str[20];
  n_n2s(n_w, 9, N_FLOAT, str, NULL);
  mvprintw(4, 10, "%20s", str);
  mvprintw(4, 40, "%20s", n_print(n_w, str));
  n_n2s(n_z, 9, N_FLOAT, str, NULL);
  mvprintw(5, 10, "%20s", str);
  mvprintw(5, 40, "%20s", n_print(n_z, str));
  n_n2s(n_y, 9, N_FLOAT, str, NULL);
  mvprintw(6, 10, "%20s", str);
  mvprintw(6, 40, "%20s", n_print(n_y, str));
  n_n2s(n_x, 9, N_FLOAT, str, NULL);
  mvprintw(7, 10, "%20s", str);
  mvprintw(7, 40, "%20s", n_print(n_x, str));

  n_n2s(n_x, fix, format, str, NULL);
  mvprintw(9, 10, "%20s", str);
  mvprintw(11, 10, "%[ %16s ]\r", line);
}

static void push() {
  n_w = n_z;
  n_z = n_y;
  n_y = n_x;
}

static void eval_arith(n_t (*opr)(n_t, n_t, n_err_t *)) {
  n_x = opr(n_y, n_x, NULL);
  n_y = n_z;
  n_z = n_w;
  ///n_w = N_0;
}

/** Returns true if 'line' was handled. */
static bool handle_fun(char *line) {
  assert(line != NULL);

  n_err_t err = N_ERR_NONE;

  if (strcmp(line, "de") == 0) { trig = N_DEG;  return true; }
  if (strcmp(line, "ra") == 0) { trig = N_RAD;  return true; }
  if (strcmp(line, "gr") == 0) { trig = N_GRAD; return true; }

  if (strcmp(line, "fl") == 0) { format = N_FLOAT; return true; }
  if (strcmp(line, "en") == 0) { format = N_ENG;   return true; }
  if (strcmp(line, "sc") == 0) { format = N_SCI;   return true; }

  if (line[0] == 'f' && line[1] >= '0' && line[1] <= '9' && line[2] == '\0') {
    fix = line[1] - '0';
    return true;
  }

  if (strcmp(line, "xx")  == 0) { n_x = n_square(n_x, &err); return true; }
  if (strcmp(line, "ii")  == 0) { n_x = n_1_x(n_x, &err);    return true; }
  if (strcmp(line, "v")   == 0) { n_x = n_sqrt(n_x, &err);   return true; }
  if (strcmp(line, "ln")  == 0) { n_x = n_ln(n_x, &err);     return true; }
  if (strcmp(line, "lo")  == 0) { n_x = n_log(n_x, &err);    return true; }
  if (strcmp(line, "iln") == 0) { n_x = n_exp(n_x, &err);    return true; }
  if (strcmp(line, "ilo") == 0) { n_x = n_pow10(n_x, &err);  return true; }

  if (strcmp(line, "si") == 0) { n_x = n_sin(n_x, trig, &err);  return true; }
  if (strcmp(line, "co") == 0) { n_x = n_cos(n_x, trig, &err);  return true; }
  if (strcmp(line, "ta") == 0) { n_x = n_tan(n_x, trig, &err);  return true; }
  if (strcmp(line, "as") == 0) { n_x = n_asin(n_x, trig, &err); return true; }
  if (strcmp(line, "ac") == 0) { n_x = n_acos(n_x, trig, &err); return true; }
  if (strcmp(line, "at") == 0) { n_x = n_atan(n_x, trig, &err); return true; }

  if (strcmp(line, "ch") == 0) { n_x = n_chs(n_x);  return true; }
  if (strcmp(line, "in") == 0) { n_x = n_int(n_x);  return true; }
  if (strcmp(line, "fr") == 0) { n_x = n_frac(n_x); return true; }
  if (strcmp(line, "ab") == 0) { n_x = n_abs(n_x);  return true; }

  if (strcmp(line, "dm") == 0)  {
    n_x = n_dms(n_x, fix, format, &err);
    return true;
  }
  if (strcmp(line, "idm") == 0) {
    n_x = n_idms(n_x, fix, format, &err);
    return true;
  }
  if (strcmp(line, "pr") == 0) {
    n_t rho, theta;
    n_p_r(n_y, n_x, trig, &rho, &theta, &err);
    n_x = rho;
    n_y = theta;
    return true;
  }
  if (strcmp(line, "rp") == 0) {
    n_t n1, n2;
    n_r_p(n_y, n_x, trig, &n1, &n2, &err);
    n_x = n2;
    n_y = n1;
    return true;
  }

  if (strcmp(line, "+")  == 0) { eval_arith(n_plus);  return true; }
  if (strcmp(line, "~")  == 0) { eval_arith(n_minus); return true; }
  if (strcmp(line, "*")  == 0) { eval_arith(n_times); return true; }
  if (strcmp(line, "/")  == 0) { eval_arith(n_div);   return true; }
  if (strcmp(line, "^")  == 0) { eval_arith(n_pow);   return true; }
  if (strcmp(line, "i^") == 0) { eval_arith(n_ipow);  return true; }

  if (strcmp(line, "pi") == 0) {
    push();
    n_x = N_PI;
    return true;
  }

  if (strcmp(line, "xy") == 0) {
    n_t tmp = n_x;
    n_x = n_y;
    n_y = tmp;
    return true;
  }

  return false;
}

/** Returns true if 'line' was handled. */
static bool handle_num(char *line) {
  n_err_t err2;
  n_t n = n_s2n(line, &err2);

  if (err2 != N_ERR_DOMAIN) {
    n_w = n_z;
    n_z = n_y;
    n_y = n_x;
    n_x = n;
    return true;
  }

  return false;
}

static bool is_numeric(char c) {
  return (c == '.' || (c >= '0' && c <= '9') || c == '-' || c == ' ');
}

int main(void) {
  typedef enum parse_state_e {
    PARSE_START,
    PARSE_NUM,
    PARSE_FUN,
  } parse_state_t;

  parse_state_t state = PARSE_START;
  char line[1000];
  int line_len = 0;
  line[0] = 0;

  prepare_screen();

  while (true) {
    update_screen(line);

    char c = getch();

    if (c == 127) {
      if (line_len > 0) { line[--line_len] = 0; }
      if (line_len == 0) { state = PARSE_START; }
      continue;
    }
    if (state == PARSE_START) {
      if (c == ' ') continue;
      if (is_numeric(c)) {
        state = PARSE_NUM;
      } else if (c != '\n') {
        state = PARSE_FUN;
      }
    }
    if (c == '\n') {
      if (state == PARSE_START) {
        push();
        continue;
      }
      if (state == PARSE_FUN) continue;

      handle_num(line);

      state = PARSE_START;
      line[0] = 0;
      line_len = 0;
      continue;
    }

    if (line_len < LINE_LEN) {
      if (state == PARSE_NUM && !is_numeric(c)) {
        if (handle_num(line)) {
          line_len = 0;
          state = PARSE_FUN;
        } else {
          continue;
        }
      }
      line[line_len++] = c;
      line[line_len] = 0;
      if (state == PARSE_FUN && handle_fun(line)) {
        state = PARSE_START;
        line[0] = 0;
        line_len = 0;
      }
    }
  }

  return 0;
}
