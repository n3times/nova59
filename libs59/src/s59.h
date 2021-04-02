/**
 * s59.h
 */

#ifndef S59_H
#define S59_H

#include <stdbool.h>

#include "n59.h"


/******************************************************************************
 *
 *  TYPES.
 *
 ******************************************************************************/

typedef struct s_display_s {
  char d[14];
  bool edit_exp;
} s_display_t;

typedef struct s_aos_s {
  n_t hir[8];
  char ops[17];
} s_aos_t;

typedef struct s_program_s {
  char steps[1000];
  int step_count;
} s_program_t;

typedef bool s_err_t;


/******************************************************************************
 *
 *  PARAMS.
 *
 ******************************************************************************/

/**
 * Returns an integer that can be used as param for an operation that takes a
 * 1-digit param such as 'Fix'.
 *
 * This can be used to resolve indirect operations such as 'Fix Ind' that use
 * the value of regs[i] as parameter.
 *
 * If regs[i] >= 0, returns the last digit of the integral part of regs[i]. If
 * regs[i] < 0, returns 0.
 */
int s_param_get_ind_d(n_t *regs, int i, s_err_t *err);

/**
 * Returns an integer that can be used as param for an operation that takes a
 * 2-digit param such as 'STO'.
 *
 * This can be used to resolve indirect operations such as 'STO Ind' that use
 * the value of regs[i] as parameter.
 *
 * Only the integral part of regs[i] is considered. If regs[i] < 0, returns 0.
 * If regs[i] >= 100, sets error.
 */
int s_param_get_ind_dd(n_t *regs, int i, s_err_t *err);

/**
 * Returns an integer that can be used as param for an operation that takes a
 * 3-digit param such as 'GTO'.
 *
 * This can be used to resolve indirect operations such as 'GTO Ind' that use
 * the value of regs[i] as parameter.
 *
 * Only the integral part of regs[i] is considered. If regs[i] < 0, returns 0.
 * If regs[i] >= 1000, sets error.
 */
int s_param_get_ind_ddd(n_t *regs, int i, s_err_t *err);

/**
 * Returns the location just after a given label, within a given program.
 *
 * Sets error if there is no such label.
 *
 * Note that return value may be 1000 if label is at the very end of program.
 */
int s_param_get_label_address(int label, s_program_t *program, s_err_t *err);


/******************************************************************************
 *
 *  DISPLAY.
 *
 ******************************************************************************/

void s_display_init(s_display_t *display);

void s_display_digit(s_display_t *display, int digit);

void s_display_dot(s_display_t *display);

void s_display_chs(s_display_t *display);

void s_display_ee(s_display_t *display);

void s_display_iee(s_display_t *display);


/******************************************************************************
 *
 *  AOS.
 *
 ******************************************************************************/

void s_aos_left(s_aos_t *aos);
void s_aos_right(s_aos_t *aos);
void s_aos_equal(s_aos_t *aos);
void s_aos_plus(s_aos_t *aos);
void s_aos_minus(s_aos_t *aos);
void s_aos_times(s_aos_t *aos);
void s_aos_div(s_aos_t *aos);
void s_aos_pow(s_aos_t *aos);
void s_aos_ipow(s_aos_t *aos);


/******************************************************************************
 *
 *  REGISTERS.
 *
 ******************************************************************************/

void s_reg_rcl(int dd, n_t *regs, n_t *X, s_err_t *err);
void s_reg_sto(int dd, n_t *regs, n_t *X, s_err_t *err);

void s_reg_sum(int dd, n_t *regs, n_t *X, s_err_t *err);
void s_reg_isum(int dd, n_t *regs, n_t *X, s_err_t *err);

void s_reg_prd(int dd, n_t *regs, n_t *X, s_err_t *err);
void s_reg_iprd(int dd, n_t *regs, n_t *X, s_err_t *err);

void s_reg_exc(int dd, n_t *regs, n_t *X, s_err_t *err);

void s_reg_cms(n_t *regs);
void s_reg_xt(n_t *X, n_t *T);


/******************************************************************************
 *
 *  MATH.
 *
 ******************************************************************************/

void s_math_chs(n_t *X);
void s_math_abs(n_t *X);
void s_math_sign(n_t *X);
void s_math_int(n_t *X);
void s_math_frac(n_t *X);

void s_math_square(n_t *X, s_err_t *err);
void s_math_sqrt(n_t *X, s_err_t *err);
void s_math_1_x(n_t *X, s_err_t *err);

void s_math_ln(n_t *X, s_err_t *err);
void s_math_log(n_t *X, s_err_t *err);
void s_math_exp(n_t *X, s_err_t *err);
void s_math_pow10(n_t *X, s_err_t *err);

void s_math_sin(n_t *X, n_err_t trig, s_err_t *err);
void s_math_cos(n_t *X, n_err_t trig, s_err_t *err);
void s_math_tan(n_t *X, n_err_t trig, s_err_t *err);

void s_math_asin(n_t *X, n_err_t trig, s_err_t *err);
void s_math_acos(n_t *X, n_err_t trig, s_err_t *err);
void s_math_atan(n_t *X, n_err_t trig, s_err_t *err);

void s_math_dms(n_t *X, int fix, n_format_t format, s_err_t *err);
void s_math_idms(n_t *X, int fix, n_format_t format, s_err_t *err);

void s_math_p_r(n_t *X, n_t *T, n_trig_t trig, n_err_t *err);
void s_math_r_p(n_t *X, n_t *T, n_trig_t trig, n_err_t *err);

void s_math_pi(n_t *X);


/******************************************************************************
 *
 *  MODES.
 *
 ******************************************************************************/

void s_mode_deg(n_trig_t *trig);
void s_mode_rad(n_trig_t *trig);
void s_mode_grad(n_trig_t *trig);

void s_mode_fix(int *fix, int val);

void s_mode_ee(bool *ee);
void s_mode_iee(bool *ee);

void s_mode_eng(bool *eng);
void s_mode_ieng(bool *eng);


/******************************************************************************
 *
 *  FLAGS.
 *
 ******************************************************************************/

void s_flag_raise(int d, bool *flags, s_err_t *err);
void s_flag_clear(int d, bool *flags, s_err_t *err);


/******************************************************************************
 *
 *  FLOW.
 *
 ******************************************************************************/

void s_flow_gto(int ddd, int *pc, s_err_t *err);
void s_flow_sbr(int ddd, int *pc, s_err_t *err);
void s_flow_equal(int ddd, int *X, int *T, int *pc, s_err_t *err);
void s_flow_ge(int ddd, int *X, int *T, int *pc, s_err_t *err);
void s_flow_lt(int ddd, int *X, int *T, int *pc, s_err_t *err);
void s_flow_iff(int d, int ddd, bool *flags, int *pc, s_err_t *err);
void s_flow_iiff(int d, int ddd, bool *flags, int *pc, s_err_t *err);
void s_flow_dsz(int d, int ddd, n_t *regs, int *pc, s_err_t *err);
void s_flow_idsz(int d, int ddd, n_t *regs, int *pc, s_err_t *err);
void s_flow_cp(int *steps);

#endif  // S59_H
