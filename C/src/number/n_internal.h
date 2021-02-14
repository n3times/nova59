#ifndef N_INTERNAL_H
#define N_INTERNAL_H

#include "n.h"

#define ABS(x)     ( (x) < 0 ? (-x) : (x) )
#define MIN(x, y)  ( (x) < (y) ? (x) : (y) )
#define MAX(x, y)  ( (x) > (y) ? (x) : (y) )

#define POW10_12  1000000000000LL
#define POW10_13  10000000000000LL

#endif  // N_INTERNAL_H
