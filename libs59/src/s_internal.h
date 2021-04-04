#include "s59.h"

#include <assert.h>

#define CHECK(X) {\
                   assert(X); \
                   if (!X) return; \
                 }

#define CHECK_D(d) {\
                     assert((d) >= 0 && (d) <= 9); \
                     if ((d) < 0 || (d) > 9) return; \
                   }

#define CHECK_DD(dd) {\
                       assert((dd) >= 0 && (dd) <= 99); \
                       if ((dd) < 0 || (dd) > 99) return; \
                     }

#define CHECK_REG(regs, dd) {\
                              CHECK(regs); \
                              CHECK_DD(dd); \
                            }

#define CHECK_FLAG(flags, d) {\
                               CHECK(flags); \
                               CHECK_D(d); \
                             }
