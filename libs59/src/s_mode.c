#include "s59.h"

#include <assert.h>

void s_mode_deg(n_trig_t *trig) {
  assert(trig);
  if (!trig) return;

  *trig = N_DEG;
}

void s_mode_rad(n_trig_t *trig) {
  assert(trig);
  if (!trig) return;

  *trig = N_RAD;
}

void s_mode_grad(n_trig_t *trig) {
  assert(trig);
  if (!trig) return;

  *trig = N_GRAD;
}

void s_mode_fix(int *fix, int val) {
  assert(fix);
  if (!fix) return;

  if (val < 0) val = 0;
  val = val % 10;

  *fix = val;
}

void s_mode_ee(bool *ee) {
  assert(ee);
  if (!ee) return;

  *ee = true;
}

void s_mode_iee(bool *ee) {
  assert(ee);
  if (!ee) return;

  *ee = false;
}

void s_mode_eng(bool *eng) {
  assert(eng);
  if (!eng) return;

  *eng = true;
}

void s_mode_ieng(bool *eng) {
  assert(eng);
  if (!eng) return;

  *eng = false;
}
