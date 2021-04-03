#include "s59.h"

#include <assert.h>

static void s_math_op(n_t (*op)(n_t), n_t *X) {
  assert(X);
  if (!X) return;

  *X = op(*X);
}

static void s_math_op2(n_t (*op)(n_t, n_err_t *), n_t *X, s_err_t *err) {
  assert(X);
  if (!X) return;

  n_err_t n_err;

  *X = op(*X, &n_err);

  if (err) *err = n_err ? true : false;
}

static void s_math_op_trig(n_t (*op)(n_t, n_trig_t trig, n_err_t *),
                           n_t *X, n_trig_t trig, s_err_t *err) {
  assert(X);
  if (!X) return;

  n_err_t n_err;

  *X = op(*X, trig, &n_err);

  if (err) *err = n_err ? true : false;
}

static void s_math_op_dms(n_t (*op)(n_t, int, n_format_t, n_err_t *), 
                          n_t *X, int fix, n_format_t format, s_err_t *err) {
  assert(X);
  if (!X) return;

  n_err_t n_err;

  *X = op(*X, fix, format, &n_err);

  if (err) *err = n_err ? true : false;
}

static void s_math_op_p_r(
    void (*op)(n_t, n_t, n_trig_t, n_t *, n_t *, n_err_t *), 
    n_t *X, n_t * T, n_trig_t trig, s_err_t *err) {
  assert(X);
  assert(T);
  if (!X) return;
  if (!T) return;

  n_err_t n_err;

  op(*X, *T, trig, X, T, &n_err);

  if (err) *err = n_err ? true : false;
}


/******************************************************************************
 *
 *  IMPLEMENTATION.
 *
 ******************************************************************************/

void s_math_chs(n_t *X) {
  s_math_op(n_chs, X);
}

void s_math_abs(n_t *X) {
  s_math_op(n_abs, X);
}

void s_math_sign(n_t *X) {
  s_math_op(n_sign, X);
}

void s_math_int(n_t *X) {
  s_math_op(n_int, X);
}

void s_math_frac(n_t *X) {
  s_math_op(n_frac, X);
}

void s_math_square(n_t *X, s_err_t *err) {
  s_math_op2(n_square, X, err);
}

void s_math_sqrt(n_t *X, s_err_t *err) {
  s_math_op2(n_sqrt, X, err);
}

void s_math_1_x(n_t *X, s_err_t *err) {
  s_math_op2(n_1_x, X, err);
}

void s_math_ln(n_t *X, s_err_t *err) {
  s_math_op2(n_ln, X, err);
}

void s_math_log(n_t *X, s_err_t *err) {
  s_math_op2(n_log, X, err);
}

void s_math_exp(n_t *X, s_err_t *err) {
  s_math_op2(n_exp, X, err);
}

void s_math_pow10(n_t *X, s_err_t *err) {
  s_math_op2(n_pow10, X, err);
}

/** Trigonometric functions. */

void s_math_sin(n_t *X, n_trig_t trig, s_err_t *err) {
  s_math_op_trig(n_sin, X, trig, err);
}

void s_math_cos(n_t *X, n_trig_t trig, s_err_t *err) {
  s_math_op_trig(n_cos, X, trig, err);
}

void s_math_tan(n_t *X, n_trig_t trig, s_err_t *err) {
  s_math_op_trig(n_tan, X, trig, err);
}

void s_math_asin(n_t *X, n_trig_t trig, s_err_t *err) {
  s_math_op_trig(n_asin, X, trig, err);
}

void s_math_acos(n_t *X, n_trig_t trig, s_err_t *err) {
  s_math_op_trig(n_acos, X, trig, err);
}

void s_math_atan(n_t *X, n_trig_t trig, s_err_t *err) {
  s_math_op_trig(n_atan, X, trig, err);
}

/** Special functions. */

void s_math_dms(n_t *X, int fix, n_format_t format, s_err_t *err) {
  s_math_op_dms(n_dms, X, fix, format, err);
}

void s_math_idms(n_t *X, int fix, n_format_t format, s_err_t *err) {
  s_math_op_dms(n_idms, X, fix, format, err);
}

void s_math_p_r(n_t *X, n_t *T, n_trig_t trig, s_err_t *err) {
  s_math_op_p_r(n_p_r, X, T, trig, err);
}

void s_math_r_p(n_t *X, n_t *T, n_trig_t trig, s_err_t *err) {
  s_math_op_p_r(n_r_p, X, T, trig, err);
}

/** Miscellaneous. */

void s_math_pi(n_t *X) {
  assert(X);
  if (!X) return;
  *X = N_PI;
}
