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

typedef enum s_opd_type_e {
  OPD_TYPE_D,    // 1-digit operand. 'Fix' for example.
  OPD_TYPE_DD,   // 2-digit operand. 'STO' for example.
  OPD_TYPE_DDD,  // 3-digit operand. 'GTO' for example.
} s_opd_type_t;

typedef bool s_err_t;


/******************************************************************************
 *
 *  PARAMS.
 *
 ******************************************************************************/

/**
 * Returns the actual operand of an indirect operator.
 *
 * For example s_param_get_ind_operand(OPD_TYPE_DDD, regs, 59, &err) returns the
 * actual address of 'GTO Ind 59' or 'SBR Ind 59'.
 *
 * A negative regs[i] is interpreted as '0' and the fractional part of regs[i]
 * is discarded.
 *
 * If opd_type is OPD_TYPE_D, returns regs[i] % 10.
 *
 * Sets err if 'i' is out of bounds or regs[i] is too large for the param type.
 */
int s_param_get_ind_operand(
    s_opd_type_t opd_type, n_t *regs, int i, s_err_t *err);

/**
 * Returns the address immediately after a given label, within a given program.
 * This can be used to determine the actual operand of an address operator such
 * as GTO.
 *
 * For example s_param_get_label_address(11, program, &err) returns '59' for the
 * following program:
 *
 *   056  ..  ...
 *   057  76  Lbl
 *   058  11  A
 *   059  ..  ...
 *
 * and therefore 'GTO A' is equivalent to 'GTO 059'.
 *
 * If the same label appears multiple times in the program, only the first
 * one is considered.
 *
 * Sets err if there is no such label.
 */
int s_param_get_label_address(char label, s_program_t *program, s_err_t *err);


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
