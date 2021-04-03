#include "s59.h"

#include <assert.h>

static void s_flag_set(int d, bool *flags, bool val) {
  assert(flags);
  if (!flags) return;

  if (d < 0) d = 0;
  d = d % 10;

  flags[d] = val;
}

void s_flag_raise(int d, bool *flags) {
  s_flag_set(d,flags, true);
}

void s_flag_clear(int d, bool *flags) {
  s_flag_set(d,flags, false);
}
