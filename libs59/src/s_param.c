#include "s59.h"

#define IND 40
#define LBL 76

// 3: @
// 4: D+
// 5: D+ @
static int OP_LENGTH[] =
{
  1, 1, 1, 1, 1,   1, 1, 1, 1, 1,

  1, 1, 1, 1, 1,   1, 1, 1, 1, 1,  // 11, 12, 13, 14, 15   16, 17, 18, 19, 20
  1, 1, 1, 1, 1,   1, 1, 1, 1, 1,
  1, 1, 1, 1, 1,   2, 1, 1, 1, 1,
  1, 2, 2, 2, 1,   1, 1, 2, 2, 1,
  1, 1, 1, 1, 1,   1, 1, 4, 1, 1,
  3, 2, 2, 2, 1,   1, 3, 1, 2, 1,
  3, 2, 2, 2, 1,   1, 3, 1, 1, 1,
  1, 2, 2, 2, 1,   4, 5, 1, 1, 1,
  1, 1, 1, 1, 1,   1, 5, 1, 1, 1,
};

static int s_param_get_ind(int max_val, n_t *regs, int i, s_err_t *err) {
  if (i < 0 || i >= 100) {
    if (err) *err = true;
    return -1;
  }

  n_t n = regs[i];

  if (n_cmp(n, N_0) < 0) return 0;

  if (n_cmp(n, n_make(max_val)) > 0) {
    if (err) *err = true;
    return -1;
  }

  if (err) *err = false;
  return (int) n_n2d(n);
}

int s_param_get_ind_d(n_t *regs, int i, s_err_t *err) {
  if (i < 0 || i >= 100) {
    if (err) *err = true;
    return -1;
  }

  n_t n = regs[i];

  if (err) *err = false;

  if (n_cmp(n, N_0) < 0) return 0;

  if (n_cmp(n, n_make(1e12)) >= 0) return 0;

  return (long long) n_n2d(n) % 10;
}

int s_param_get_ind_dd(n_t *regs, int i, s_err_t *err) {
  return s_param_get_ind(99, regs, i, err);
}

int s_param_get_ind_ddd(n_t *regs, int i, s_err_t *err) {
  return s_param_get_ind(999, regs, i, err);
}

int s_param_get_label_address(int label, s_program_t *program, s_err_t *err) {
  char *steps = program->steps;

  for (int i = 0; i < program->step_count; ) {
    char op = steps[i];

    if (op == LBL) return i + 2;

    int len = OP_LENGTH[op / 10 + (op + 9) % 10];

    if (len <= 2) {
      i += len;
    } else if (len == 3) {  // Address = 0D DD | 40 DD | DD
      if (i + 1 < program->step_count) break;
      int next_step = steps[i + 1];
      bool lbl = next_step != IND && next_step / 10 != 0;
      i += lbl ? 2 : 3;
    } else if (len == 4) {  // 0D | IND DD
      if (i + 1 < program->step_count) break;
      bool ind = steps[i + 1] == IND;
      i += ind ? 3 : 2;
    } else if (len == 5) {  // (0D | IND DD) Address
      if (i + 1 < program->step_count) break;
      bool ind = steps[i + 1] == IND;
      i += ind ? 3 : 2;

      if (i + 1 < program->step_count) break;
      int next_step = steps[i + 1];
      bool lbl = next_step != IND && next_step / 10 != 0;
      i += lbl ? 2 : 3;
    }

    continue;
  }

  return -1;
}
