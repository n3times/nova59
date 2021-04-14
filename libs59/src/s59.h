/**
 * s59.h
 */

#ifndef S59_H
#define S59_H

#include <stdbool.h>

#include "n59.h"
#include "s_err.h"
#include "xdisplay.h"


/******************************************************************************
 *
 *  TYPES.
 *
 ******************************************************************************/

typedef struct s_aos_s {
  n_t hir[8];
  char ops[17];
} s_aos_t;

typedef struct s_program_s {
  char steps[1000];
  int step_count;
} s_program_t;

typedef struct s_module_s {
  s_program_t program[100];
  int program_count;
} s_module_t;

typedef enum s_mode_e {
  MODE_EVAL,
  MODE_LRN,
  MODE_RUN,
  MODE_SST
} s_mode_t;

typedef enum s_opd_type_e {
  OPD_TYPE_UNK,  // Unknown, not yet determined.
  OPD_TYPE_IND,  // Indirect. 'GTO Ind 59', for example.
  OPD_TYPE_LBL,  // Label. 'GTO A', for example.
  OPD_TYPE_D,    // 1-digit operand. 'Fix 5' for example.
  OPD_TYPE_DD,   // 2-digit operand. 'STO 59' for example.
  OPD_TYPE_DDD,  // 3-digit operand. 'GTO 059' for example.
} s_opd_type_t;

typedef enum s_opn_status_e {
  OPN_PENDING,
  OPN_COMPLETE,
  OPN_CANCELED
} s_opn_status_t;

typedef struct s_opd_s {
  s_opd_type_t type;  // Indicates how to interpret the operand value.
  char value[4];      // Up to 3 digits: opcode for label, address or register..
  bool sbr;           // Operand is SBR <addr> w/ <addr> given by type and value
} s_opd_t;

typedef struct s_opn_s {
  s_opn_status_t status;
  bool opr_inv;
  char opr_key;
  s_opd_t opds[2];
} s_opn_t;


/******************************************************************************
 *
 *  REGISTERS.
 *
 ******************************************************************************/

/**
 * For register operators, 'regs' should have 100 elements.
 *
 * 'err' is set if dd is a not in 0..99 or if there is a overflow, underflow or
 * division by 0.
 */

/** Recall: X = regs[i]. */
void s_reg_rcl(int dd, n_t *regs, n_t *X, s_err_t *err);

/** Store: regs[i] = X. */
void s_reg_sto(int dd, n_t *regs, n_t *X, s_err_t *err);

/** Sum: regs[i] += X. */
void s_reg_sum(int dd, n_t *regs, n_t *X, s_err_t *err);

/** Inv sum: regs[i] -= X. */
void s_reg_isum(int dd, n_t *regs, n_t *X, s_err_t *err);

/** Product: regs[i] *= X. */
void s_reg_prd(int dd, n_t *regs, n_t *X, s_err_t *err);

/** Inv product: regs[i] /= X. */
void s_reg_iprd(int dd, n_t *regs, n_t *X, s_err_t *err);

/** Exchange: regs[i] <-> X. */
void s_reg_exc(int dd, n_t *regs, n_t *X, s_err_t *err);

/** Sets all registers to 0. */
void s_reg_cms(n_t *regs);

/** Exchange of X and T: X <-> T. */
void s_reg_xt(n_t *X, n_t *T);


/******************************************************************************
 *
 *  FLAGS.
 *
 ******************************************************************************/

/**
 * For flag operators, 'flags' should have 10 elements.
 */

/** flags[d] = true. */
void s_flag_raise(int d, bool *flags);

/** flags[d] = false. */
void s_flag_clear(int d, bool *flags);


/******************************************************************************
 *
 *  FLOW.
 *
 ******************************************************************************/

/**
 *
 */
void s_flow_gto(int ddd, int *pc, s_err_t *err);

void s_flow_sbr(int ddd, int *pc, s_err_t *err);

void s_flow_rtn(int *pc, s_err_t *err);

/**
 * Returns true if X = T.
 */
bool s_flow_equal(int ddd, int *X, int *T, int *pc, s_err_t *err);

/**
 * Returns true if X >= T.
 */
bool s_flow_ge(int ddd, int *X, int *T, int *pc, s_err_t *err);

/**
 * Returns true if X < T.
 */
bool s_flow_lt(int ddd, int *X, int *T, int *pc, s_err_t *err);

/**
 * Returns true if flags[d] is true.
 */
bool s_flow_iff(int d, int ddd, bool *flags, int *pc, s_err_t *err);

/**
 * Returns true if flags[d] is false.
 */
bool s_flow_iiff(int d, int ddd, bool *flags, int *pc, s_err_t *err);

/**
 * Decrements d[z] and returns true if d[z] != 0.
 *
 * If d[z] > 1, d[z] = d[z] - 1.
 * If d[z] < -1, d[z] = d[z] + 1.
 * If |d[z]| <= 1, d[z] = 0.
 */
bool s_flow_dsz(int d, int ddd, n_t *regs, int *pc, s_err_t *err);

/**
 * Decrements d[z] and returns true if d[z] = 0.
 *
 * If d[z] > 1, d[z] = d[z] - 1.
 * If d[z] < -1, d[z] = d[z] + 1.
 * If |d[z]| <= 1, d[z] = 0.
 */
bool s_flow_idsz(int d, int ddd, n_t *regs, int *pc, s_err_t *err);

/** Sets all steps to 0. */
void s_flow_cp(int *steps);


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

void s_math_sin(n_t *X, n_trig_t trig, s_err_t *err);
void s_math_cos(n_t *X, n_trig_t trig, s_err_t *err);
void s_math_tan(n_t *X, n_trig_t trig, s_err_t *err);

void s_math_asin(n_t *X, n_trig_t trig, s_err_t *err);
void s_math_acos(n_t *X, n_trig_t trig, s_err_t *err);
void s_math_atan(n_t *X, n_trig_t trig, s_err_t *err);

void s_math_dms(n_t *X, int fix, n_format_t format, s_err_t *err);
void s_math_idms(n_t *X, int fix, n_format_t format, s_err_t *err);

/**
 * Converts polar coordinates into rectangular coordinates.
 *
 * Input: (X, T) = (theta, rho).
 * Output: (X, T) = (y, x).
 */
void s_math_p_r(n_t *X, n_t *T, n_trig_t trig, s_err_t *err);

/**
 * Converts rectangular coordinates into polar coordinates.
 *
 * Input: (X, T) = (y, x).
 * Output: (X, T) = (theta, rho).
 */
void s_math_r_p(n_t *X, n_t *T, n_trig_t trig, s_err_t *err);

/** X = N_PI. */
void s_math_pi(n_t *X);


/******************************************************************************
 *
 *  TRIG.
 *
 ******************************************************************************/

/** trig = N_DEG. */
void s_mode_deg(n_trig_t *trig);

/** trig = N_RAD. */
void s_mode_rad(n_trig_t *trig);

/** trig = N_GRAD. */
void s_mode_grad(n_trig_t *trig);


/******************************************************************************
 *
 *  PARAMS.
 *
 ******************************************************************************/

/**
 * Returns the actual operand of an indirect operator.
 *
 * For example s_param_get_ind_operand(OPD_TYPE_DDD, regs[59], &err) returns the
 * actual address of 'GTO Ind 59' or 'SBR Ind 59'.
 *
 * A negative n is interpreted as '0' and the fractional part of n is discarded.
 *
 * If opd_type is OPD_TYPE_D, returns n % 10.
 *
 * Sets err if n is too large for the param type.
 */
int s_param_get_ind_operand(s_opd_type_t opd_type, n_t n, s_err_t *err);

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
 *  PARSING.
 *
 ******************************************************************************/

void s_parse_init_opn(s_opn_t *opn);

/**
 * Given a pending operation, updates its state.
 */
void s_parse_opn(s_opn_t *opn, s_mode_t mode, int key);

#endif  // S59_H
