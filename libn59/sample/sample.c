/**
 * Command line RPN calculator based on libn59.
 *
 * Operations: + - / * ^ i^
 *             chs abs int frac
 *             x2 x1 vx ln log exp pow
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
  n_w = N_0;
}

static void handle_line(char *line) {
  n_err_t err = N_ERR_NONE;

  if (strcmp(line, "")  == 0) { push(); }

  if (strcmp(line, "deg")  == 0) { trig = N_DEG; }
  if (strcmp(line, "rad")  == 0) { trig = N_RAD; }
  if (strcmp(line, "grad") == 0) { trig = N_GRAD; }

  if (strcmp(line, "flt") == 0) { format = N_FLOAT; }
  if (strcmp(line, "eng") == 0) { format = N_ENG; }
  if (strcmp(line, "sci") == 0) { format = N_SCI; }

  if (strcmp(line, "f0") == 0) { fix = 0; }
  if (strcmp(line, "f1") == 0) { fix = 1; }
  if (strcmp(line, "f2") == 0) { fix = 2; }
  if (strcmp(line, "f3") == 0) { fix = 3; }
  if (strcmp(line, "f4") == 0) { fix = 4; }
  if (strcmp(line, "f5") == 0) { fix = 5; }
  if (strcmp(line, "f6") == 0) { fix = 6; }
  if (strcmp(line, "f7") == 0) { fix = 7; }
  if (strcmp(line, "f8") == 0) { fix = 8; }
  if (strcmp(line, "f9") == 0) { fix = 9; }

  if (strcmp(line, "x2")   == 0) { n_x = n_square(n_x, &err); }
  if (strcmp(line, "ix")   == 0) { n_x = n_1_x(n_x, &err); }
  if (strcmp(line, "vx")   == 0) { n_x = n_sqrt(n_x, &err); }
  if (strcmp(line, "ln")   == 0) { n_x = n_ln(n_x, &err); }
  if (strcmp(line, "log")  == 0) { n_x = n_log(n_x, &err); }
  if (strcmp(line, "iln")  == 0) { n_x = n_exp(n_x, &err); }
  if (strcmp(line, "ilog") == 0) { n_x = n_pow10(n_x, &err); }

  if (strcmp(line, "sin") == 0)  { n_x = n_sin(n_x, trig, &err); }
  if (strcmp(line, "cos") == 0)  { n_x = n_cos(n_x, trig, &err); }
  if (strcmp(line, "tan") == 0)  { n_x = n_tan(n_x, trig, &err); }
  if (strcmp(line, "asin") == 0) { n_x = n_asin(n_x, trig, &err); }
  if (strcmp(line, "acos") == 0) { n_x = n_acos(n_x, trig, &err); }
  if (strcmp(line, "atan") == 0) { n_x = n_atan(n_x, trig, &err); }

  if (strcmp(line, "chs") == 0)  { n_x = n_chs(n_x); }
  if (strcmp(line, "int") == 0)  { n_x = n_int(n_x); }
  if (strcmp(line, "frac") == 0) { n_x = n_frac(n_x); }
  if (strcmp(line, "abs") == 0)  { n_x = n_abs(n_x); }

  if (strcmp(line, "dms") == 0)  { n_x = n_dms(n_x, fix, format, &err); }
  if (strcmp(line, "idms") == 0) { n_x = n_idms(n_x, fix, format, &err); }

  if (strcmp(line, "pr") == 0) {
    n_t rho, theta;
    n_p_r(n_y, n_x, trig, &rho, &theta, &err);
    n_x = rho;
    n_y = theta;
  }
  if (strcmp(line, "rp") == 0) {
    n_t n1, n2;
    n_r_p(n_y, n_x, trig, &n1, &n2, &err);
    n_x = n2;
    n_y = n1;
  }

  if (strcmp(line, "+") == 0)  { eval_arith(n_plus); }
  if (strcmp(line, "-") == 0)  { eval_arith(n_minus); }
  if (strcmp(line, "*") == 0)  { eval_arith(n_times); }
  if (strcmp(line, "/") == 0)  { eval_arith(n_div); }
  if (strcmp(line, "^") == 0)  { eval_arith(n_pow); }
  if (strcmp(line, "i^") == 0) { eval_arith(n_ipow); }

  if (strcmp(line, "pi") == 0) {
    push();
    n_x = N_PI;
  }

  if (strcmp(line, "xy") == 0) {
    n_t tmp = n_x;
    n_x = n_y;
    n_y = tmp;
  }

  n_err_t err2;
  n_t n = n_s2n(line, &err2);
  if (err2 != N_ERR_DOMAIN) {
     n_y = n_x;
     n_x = n;
  }
}

int main(void) {
  char line[1000];
  int line_len = 0;
  line[0] = 0;

  prepare_screen();

  while (true) {
    update_screen(line);

    char c = getch();
    if (c == 127) {
      if (line_len > 0) {
        line[--line_len] = 0;
      }
    } else if (c == '\n') {
      handle_line(line);
      line[0] = 0;
      line_len = 0;
    } else if (line_len < LINE_LEN) {
      line[line_len++] = c;
      line[line_len] = 0;
    }
  }

  return 0;
}
