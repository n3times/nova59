#include "s59.h"

#include <assert.h>


/******************************************************************************
 *
 *  HELPERS.
 *
 ******************************************************************************/

void s_reg_arithmetic(n_t (*op)(n_t, n_t, n_err_t *),
                      int dd, n_t *regs, n_t *X, s_err_t *err) {
  n_err_t n_err;
  regs[dd] = op(regs[dd], *X, &n_err);
  if (err) *err = n_err ? true : false;
}


/******************************************************************************
 *
 *  Implementation..
 *
 ******************************************************************************/

void s_reg_rcl(int dd, n_t *regs, n_t *X, s_err_t *err) {
  *X = regs[dd];
}

void s_reg_sto(int dd, n_t *regs, n_t *X, s_err_t *err) {
  regs[dd] = *X;
}

void s_reg_sum(int dd, n_t *regs, n_t *X, s_err_t *err) {
  s_reg_arithmetic(n_plus, dd, regs, X, err);
}

void s_reg_isum(int dd, n_t *regs, n_t *X, s_err_t *err) {
  s_reg_arithmetic(n_minus, dd, regs, X, err);
}

void s_reg_prd(int dd, n_t *regs, n_t *X, s_err_t *err) {
  s_reg_arithmetic(n_times, dd, regs, X, err);
}

void s_reg_iprd(int dd, n_t *regs, n_t *X, s_err_t *err) {
  s_reg_arithmetic(n_div, dd, regs, X, err);
}

void s_reg_exc(int dd, n_t *regs, n_t *X, s_err_t *err) {
  n_t tmp = *X;
  *X = regs[dd];
  regs[dd] = tmp;
}

void s_reg_cms(n_t *regs) {
  for (int i = 0; i < 100; i++) {
    regs[i] = N_0;
  }
}

void s_reg_xt(n_t *X, n_t *T) {
  n_t tmp = *X;
  *X = *T;
  *T = tmp;
}
