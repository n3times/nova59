/**
 * RPN-59.
 *
 * Command line RPN calculator based on libn59.
 *
 * Commands are 1 or 2 letters, typically the beginning of the name of the
 * function or operator.
 *
 * Math: + ~ (minus) / * ^ i^ (root extraction)
 *       ch's ab's in't fr'ac
 *       xx (x^2) ii (1/x) vv (square root)
 *       ln lo'g ex'p po'w
 *       si'n co's ta'n as'in ac'os at'an
 *       dm's id'ms pr (polar->rect) rp
 * Formats: fl'oat sc'i en'g f0-f9 de'g ra'd gr'ad
 * Stack: xy
 *
 * Numbers are entered using characters in "0123456789.- ".
 *
 * Example: to compute '2 ^ 3 - sin(4)', type '2 newline 3 ^ 4 si -'.
 */

#include "n59.h"

#include <assert.h>
#include <ctype.h>
#include <curses.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define LINE_LEN 16

#define DEL 127

/** State. */
static n_t X;
static n_t Y;
static n_t Z;
static n_t T;
static int fix = 9;
static n_format_t format = N_FLOAT;
static n_trig_t trig = N_DEG;
static bool blink = false;
static char input[100];

static char *p_n(n_t n, char *str) {
  n_n2s(n, 9, N_FLOAT, str, NULL);
  return str;
}

static void prepare_screen() {
  initscr();
  noecho();
}

static void update_screen() {
  char str1[20];
  char str2[20];
  n_err_t err;

  mvprintw(0,  5, "=======================================================");
  mvprintw(1,  5, "RPN-59");
  mvprintw(1, 43, "Fix %d   %s   %s",
                   fix,
                   format == N_FLOAT ? "FLT" : format == N_SCI ? "SCI" : "ENG",
                   trig == N_RAD ? "RAD" : trig == N_DEG ? "DEG" : "GRD");
  mvprintw(2,  5, "-------------------------------------------------------");
  mvprintw(3,  5, "T    %20s   %20s", p_n(T, str1), n_print(T, str2));
  mvprintw(4,  5, "Z    %20s   %20s", p_n(Z, str1), n_print(Z, str2));
  mvprintw(5,  5, "Y    %20s   %20s", p_n(Y, str1), n_print(Y, str2));
  mvprintw(6,  5, "X    %20s   %20s", p_n(X, str1), n_print(X, str2));
  mvprintw(7,  5, "-------------------------------------------------------");

  // Display.
  n_n2s(X, fix, format, str1, &err);
  if (err) blink = true;
  int attr = A_STANDOUT | (blink ? A_BLINK : 0);
  attron(attr);
  mvprintw(8, 16, "%14s ", str1);
  attroff(attr);

  mvprintw(9,  5, "-------------------------------------------------------");
  mvprintw(10, 13, "%16s%s\r", input, strlen(input) == 16 ? " " : "_");
  mvprintw(11, 5, "=======================================================");
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

/**
 * Computes function if 'input' is the name of a function.
 *
 * Returns true if 'input' is the name of a function.
 */
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
  if (strcmp(input, "vv") == 0) { eval_fun(n_sqrt);   return true; }
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

/**
 * Pushes input into the stack if it's a number.
 *
 * Returns true if 'input' is a number, false otherwise.
 */
static bool handle_number(char *input) {
  n_err_t err;
  n_t n = n_s2n(input, &err);

  bool is_a_num = err != N_ERR_DOMAIN;

  if (is_a_num) {
    push_X();
    X = n;
  }

  return is_a_num;
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
  input[0] = 0;

  prepare_screen();

  while (true) {
    update_screen();

    char c = getch();

    blink = false;
    c = tolower(c);

    if (!is_char(c)) continue;

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
          if (handle_number(input)) {
            state = PARSE_START;
            input[0] = 0;
          }
          break;
      }
      continue;
    }

    if (state == PARSE_NUM && !is_numeric(c)) {
      if (handle_number(input)) {
        input[0] = 0;
        state = PARSE_FUN;
      } else {
        continue;
      }
    }

    if (strlen(input) >= LINE_LEN) continue;

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
