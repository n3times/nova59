#include "s59.h"

#define FIX  58
#define IND  40
#define INV  22
#define INV2 27
#define LBL  76
#define SBR  71

#define OPD_NONE          1
#define OPD_D_DD          2
#define OPD_ADDRESS       3
#define OPD_D_IND         4
#define OPD_D_IND_ADDRESS 5

/** Map between a step code, see as operator, and the type of the operands. */
static int OPD_TYPE[] =
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

int s_param_get_ind_operand(s_opd_type_t opd_type, n_t n, s_err_t *err) {
  if (err) *err = false;

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

    int opd_type = OPD_TYPE[(int) step];

    if (opd_type == OPD_NONE) {
      i += 1;
    } else if (opd_type == OPD_D_DD) {
      i += 2;
    } else if (opd_type == OPD_ADDRESS) {
      bool lbl = next_step != IND && next_step / 10 != 0;
      i += lbl ? 2 : 3;
    } else if (opd_type == OPD_D_IND) {
      bool ind = next_step == IND;
      i += ind ? 3 : 2;
    } else if (opd_type == OPD_D_IND_ADDRESS) {
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
