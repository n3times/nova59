#ifndef N_INTERNAL_H
#define N_INTERNAL_H

#include "n59.h"

#define ABS(x)     ( (x) <  0  ? (-x) : (x) )
#define MIN(x, y)  ( (x) < (y) ?  (x) : (y) )
#define MAX(x, y)  ( (x) > (y) ?  (x) : (y) )

#define NULL 0

#define POW10_12   1000000000000LL
#define POW10_13  10000000000000LL

/** Converts angle between 2 measuring units. */
double convert_angle(double d, n_trig_t from, n_trig_t to);

/**
 * Returns the most severe order between err1, err2.
 *
 * See n_err_t.
 */
n_err_t max_error(n_err_t err1, n_err_t err2);

#endif  // N_INTERNAL_H
