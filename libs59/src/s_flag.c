#include "s59.h"

#include "s_internal.h"

#include <assert.h>

static void s_flag_set(int d, bool *flags, bool val) {
  CHECK_FLAG(flags, d)

  flags[d] = val;
}

void s_flag_raise(int d, bool *flags) {
  s_flag_set(d,flags, true);
}

void s_flag_clear(int d, bool *flags) {
  s_flag_set(d,flags, false);
}
