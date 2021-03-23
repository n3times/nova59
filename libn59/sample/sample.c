/**
 * Command line RPN calculator based on libn59.
 *
 * Operations - first 2 characters of:
 *             + - / * ^ i^
 *             chs abs int frac
 *             xx ii v ln log iln ilog
 *             sin cos tan asin acos atan
 *             dms idms pr rp
 * Formats: flt sci eng f0-f9 deg rad grd
 * Stack:   xy
 * Numbers: 0-9 . ~
 *
 * UI shows input, number stack, modes.
 * Input can be edited with del key.
 *
 * Example: 2 ^ 3 - sin(4)
 *          2 newline 3 ^ 4 sin -
 */

#include "n59.h"

#include <assert.h>
#include <curses.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define LINE_LEN 16
#define DEL 127

/** Modes. */
static n_format_t format = N_FLOAT;
static int fix = 9;
static n_trig_t trig = N_DEG;

/** Stack. */
static n_t X = { 0, 0 };
static n_t Y = { 0, 0 };
static n_t Z = { 0, 0 };
static n_t T = { 0, 0 };

static bool blink = false;

static void prepare_screen() {
  initscr();
  noecho();

  // Modes.
  mvprintw(0, 5, "=======================================================");
  mvprintw(1, 5, "RPN-59");
  mvprintw(1, 43, "Fix");
  mvprintw(2, 5, "=======================================================");

  // Stack.
  mvprintw(3, 5, "T =");
  mvprintw(4, 5, "Z =");
  mvprintw(5, 5, "Y =");
  mvprintw(6, 5, "X =");
  mvprintw(7, 5, "=======================================================");

  // Display and Input.
  mvprintw(8,  5, "D =");
  mvprintw(9,  5, "=======================================================");
  mvprintw(10, 5, "I =");
  mvprintw(11, 5, "=======================================================");
}

static void update_screen(char *input) {
  // Modes.
  mvprintw(1, 47, "%d", fix);
  mvprintw(
    1, 51, "%s", format == N_FLOAT ? "FLT" : format == N_SCI ? "SCI" : "ENG");
  mvprintw(
    1, 57, "%s", trig == N_RAD ? "RAD" : trig == N_DEG ? "DEG" : "GRD");

  // Stack.
  char str[20];
  n_n2s(T, 9, N_FLOAT, str, NULL);
  mvprintw(3, 10, "%20s", str);
  mvprintw(3, 40, "%20s", n_print(T, str));
  n_n2s(Z, 9, N_FLOAT, str, NULL);
  mvprintw(4, 10, "%20s", str);
  mvprintw(4, 40, "%20s", n_print(Z, str));
  n_n2s(Y, 9, N_FLOAT, str, NULL);
  mvprintw(5, 10, "%20s", str);
  mvprintw(5, 40, "%20s", n_print(Y, str));
  n_n2s(X, 9, N_FLOAT, str, NULL);
  mvprintw(6, 10, "%20s", str);
  mvprintw(6, 40, "%20s", n_print(X, str));

  // Display and Input.
  n_err_t err;
  n_n2s(X, fix, format, str, &err);
  if (err) blink = true;
  attron(A_BOLD);
  if (blink) attron(A_BLINK);
  mvprintw(8, 10, "%20s", str);
  attroff(A_BOLD);
  if (blink) attroff(A_BLINK);
  mvprintw(10, 10, "%[ %16s ]\r", input);
}

static void push_X() {
  T = Z;
  Z = Y;
  Y = X;
}

static void eval_arithmetic_op(n_t (*opr)(n_t, n_t, n_err_t *)) {
  n_err_t err;
  X = opr(Y, X, &err);
  Y = Z;
  Z = T;
  if (err) blink = true;
}

static void eval_fun(n_t (*fun)(n_t, n_err_t *)) {
  n_err_t err;
  X = fun(X, &err);
  if (err) blink = true;
}

static void eval_trig(n_t (*fun)(n_t, n_trig_t, n_err_t *)) {
  n_err_t err;
  X = fun(X, trig, &err);
  if (err) blink = true;
}

/** Returns true if 'input' was handled. */
static bool handle_fun(char *input) {
  assert(input != NULL);

  n_err_t err = N_ERR_NONE;

  if (strcmp(input, "de") == 0) { trig = N_DEG;  return true; }
  if (strcmp(input, "ra") == 0) { trig = N_RAD;  return true; }
  if (strcmp(input, "gr") == 0) { trig = N_GRAD; return true; }

  if (strcmp(input, "fl") == 0) { format = N_FLOAT; return true; }
  if (strcmp(input, "en") == 0) { format = N_ENG;   return true; }
  if (strcmp(input, "sc") == 0) { format = N_SCI;   return true; }

  if (input[0] == 'f' && input[1] >= '0' && input[1] <= '9' &&
      input[2] == '\0') {
    fix = input[1] - '0';
    return true;
  }

  if (strcmp(input, "xx") == 0) { eval_fun(n_square); return true; }
  if (strcmp(input, "ii") == 0) { eval_fun(n_1_x);    return true; }
  if (strcmp(input, "v")  == 0) { eval_fun(n_sqrt);   return true; }
  if (strcmp(input, "ln") == 0) { eval_fun(n_ln);     return true; }
  if (strcmp(input, "lo") == 0) { eval_fun(n_log);    return true; }
  if (strcmp(input, "ex") == 0) { eval_fun(n_exp);    return true; }
  if (strcmp(input, "po") == 0) { eval_fun(n_pow10);  return true; }

  if (strcmp(input, "si") == 0) { eval_trig(n_sin);  return true; }
  if (strcmp(input, "co") == 0) { eval_trig(n_cos);  return true; }
  if (strcmp(input, "ta") == 0) { eval_trig(n_tan);  return true; }
  if (strcmp(input, "as") == 0) { eval_trig(n_asin); return true; }
  if (strcmp(input, "ac") == 0) { eval_trig(n_acos); return true; }
  if (strcmp(input, "at") == 0) { eval_trig(n_atan); return true; }

  if (strcmp(input, "ch") == 0) { X = n_chs(X);  return true; }
  if (strcmp(input, "in") == 0) { X = n_int(X);  return true; }
  if (strcmp(input, "fr") == 0) { X = n_frac(X); return true; }
  if (strcmp(input, "ab") == 0) { X = n_abs(X);  return true; }

  if (strcmp(input, "dm") == 0)  {
    X = n_dms(X, fix, format, &err);
    if (err) blink = true;
    return true;
  }
  if (strcmp(input, "id") == 0) {
    X = n_idms(X, fix, format, &err);
    if (err) blink = true;
    return true;
  }
  if (strcmp(input, "pr") == 0) {
    n_t rho, theta;
    n_p_r(Y, X, trig, &rho, &theta, &err);
    if (err) blink = true;
    X = rho;
    Y = theta;
    return true;
  }
  if (strcmp(input, "rp") == 0) {
    n_t n1, n2;
    n_r_p(Y, X, trig, &n1, &n2, &err);
    if (err) blink = true;
    X = n2;
    Y = n1;
    return true;
  }

  if (strcmp(input, "+")  == 0) { eval_arithmetic_op(n_plus);  return true; }
  if (strcmp(input, "~")  == 0) { eval_arithmetic_op(n_minus); return true; }
  if (strcmp(input, "*")  == 0) { eval_arithmetic_op(n_times); return true; }
  if (strcmp(input, "/")  == 0) { eval_arithmetic_op(n_div);   return true; }
  if (strcmp(input, "^")  == 0) { eval_arithmetic_op(n_pow);   return true; }
  if (strcmp(input, "i^") == 0) { eval_arithmetic_op(n_ipow);  return true; }

  if (strcmp(input, "pi") == 0) {
    push_X();
    X = N_PI;
    return true;
  }

  if (strcmp(input, "xy") == 0) {
    n_t tmp = X;
    X = Y;
    Y = tmp;
    return true;
  }

  return false;
}

/** Returns true if 'input' was handled. */
static bool handle_num(char *input) {
  n_err_t err;
  n_t n = n_s2n(input, &err);

  if (err != N_ERR_DOMAIN) {
    T = Z;
    Z = Y;
    Y = X;
    X = n;
    return true;
  }

  return false;
}

static bool is_numeric(char c) {
  return strchr("0123456789.- ", c);
}

static bool is_char(char c) {
  return is_numeric(c) || (c >= 'a' && c <= 'z') || strchr("/^*~+\n\x7f", c); 
}

int main(void) {
  typedef enum parse_state_e {
    PARSE_START,
    PARSE_NUM,
    PARSE_FUN,
  } parse_state_t;

  parse_state_t state = PARSE_START;
  char input[100];
  input[0] = 0;

  prepare_screen();

  while (true) {
    update_screen(input);

    char c = getch();

    if (!is_char(c)) continue;

    if (c == 'k') {
      blink = false;
      continue;
    }

    if (c == DEL) {
      int input_len = strlen(input);
      if (input_len > 0) { input[--input_len] = 0; }
      if (input_len== 0) { state = PARSE_START; }
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
      switch(state) {
        case PARSE_START:
          push_X();
          break;
        case PARSE_FUN:
          break;
        case PARSE_NUM:
          if (handle_num(input)) {
            state = PARSE_START;
            input[0] = 0;
          }
          break;
      }
      continue;
    }

    if (strlen(input) >= LINE_LEN) { continue; }

    if (state == PARSE_NUM && !is_numeric(c)) {
      if (handle_num(input)) {
        input[0] = 0;
        state = PARSE_FUN;
      } else {
        continue;
      }
    }

    int input_len = strlen(input);
    input[input_len] = c;
    input[input_len + 1] = 0;
    if (state == PARSE_FUN && handle_fun(input)) {
      state = PARSE_START;
      input[0] = 0;
    }
  }

  return 0;
}
