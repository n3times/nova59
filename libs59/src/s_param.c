#include "s59.h"

#define FIX  58
#define IND  40
#define INV  22
#define INV2 27
#define LBL  76
#define SBR  71

/**
 * Map between a step code, see as operator, and the type of the operands.
 * 1 - No operand.
 * 2 - D or DD operand.
 * 3 - Address operand.
 * 4 - D or Ind DD operand.
 * 5 - D or Ind DD operand followed by address operand.
 */
static int OP_TYPE[] =
{
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 2, 1, 1, 1,
  1, 1, 2, 2, 2, 1, 1, 1, 2, 2,
  1, 1, 1, 1, 1, 1, 1, 1, 4, 1,
  1, 3, 2, 2, 2, 1, 1, 3, 1, 2,
  1, 3, 2, 2, 2, 1, 1, 3, 1, 1,
  1, 1, 2, 2, 2, 1, 4, 5, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 5, 1, 1,
};


/******************************************************************************
 *
 *  IMPLEMENTATION.
 *
 ******************************************************************************/

int s_param_get_ind_operand(
    s_opd_type_t opd_type, n_t *regs, int i, s_err_t *err) {
  if (err) *err = false;

  if (i < 0 || i >= 100) {
    if (err) *err = true;
    return -1;
  }

  n_t n = regs[i];
  if (n_cmp(n, N_0) < 0) return 0;

  if (opd_type == OPD_TYPE_D) {
    if (n_cmp(n, n_make(1e12)) >= 0) return 0;
    return (long long) n_n2d(n) % 10;
  }

  int max_value = opd_type == OPD_TYPE_DD ? 99 : 999;
  if (n_cmp(n, n_make(max_value)) > 0) {
    if (err) *err = true;
    return -1;
  }

  return (int) n_n2d(n);
}

int s_param_get_label_address(char label, s_program_t *program, s_err_t *err) {
  char *steps = program->steps;

  for (int i = 0; i < program->step_count - 1; ) {
    char step = steps[i];
    char next_step = steps[i + 1];

    if (step == LBL && next_step == label) return i + 2;

    bool inv = step == INV || step == INV2;
    if (inv && (next_step == FIX || next_step == SBR)) {
      i += 2;
      continue;
    }

    int type = OP_TYPE[(int) step];

    if (type == 1) {
      // No operands.
      i += 1;
    } else if (type == 2) {
      // D or DD operand.
      i += 2;
    } else if (type == 3) {
      // Address operand.
      bool lbl = next_step != IND && next_step / 10 != 0;
      i += lbl ? 2 : 3;
    } else if (type == 4) {
      // D or Ind DD operand.
      bool ind = next_step == IND;
      i += ind ? 3 : 2;
    } else if (type == 5) {
      // D or Ind DD operand followed by address operand.
      bool ind = next_step == IND;
      i += ind ? 3 : 2;

      if (i + 1 >= program->step_count) break;
      next_step = steps[i + 1];
      bool lbl = next_step != IND && next_step / 10 != 0;
      i += lbl ? 2 : 3;
    }

    continue;
  }

  return -1;
}
