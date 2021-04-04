#include "s59.h"

#include "s_internal.h"

void s_mode_deg(n_trig_t *trig) {
  CHECK(trig);

  *trig = N_DEG;
}

void s_mode_rad(n_trig_t *trig) {
  CHECK(trig);

  *trig = N_RAD;
}

void s_mode_grad(n_trig_t *trig) {
  CHECK(trig);

  *trig = N_GRAD;
}

void s_mode_fix(int *fix, int d) {
  CHECK(fix);
  CHECK_D(d);

  *fix = d;
}

void s_mode_ee(bool *ee) {
  CHECK(ee);

  *ee = true;
}

void s_mode_iee(bool *ee) {
  CHECK(ee);

  *ee = false;
}

void s_mode_eng(bool *eng) {
  CHECK(eng);

  *eng = true;
}

void s_mode_ieng(bool *eng) {
  CHECK(eng);

  *eng = false;
}
