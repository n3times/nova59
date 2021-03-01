#ifndef N_INTERNAL_H
#define N_INTERNAL_H

#include "n59.h"

#define ABS(x)     ( (x) <  0  ? (-x) : (x) )
#define MIN(x, y)  ( (x) < (y) ?  (x) : (y) )
#define MAX(x, y)  ( (x) > (y) ?  (x) : (y) )

#define NULL 0

#define POW10_12   1000000000000LL
#define POW10_13  10000000000000LL

/**
 * Returns angle in interval:
 * - [ 0,  360 [ for mode DEG
 * - [ 0,  400 [ for mode GRAD
 * - [ 0, 2*PI [ for mode RAD
 */
double normalize_angle(double d, n_trig_t mode);

/** Converts angle between 2 measuring units. */
double convert_angle(double d, n_trig_t from, n_trig_t to);
 
#endif  // N_INTERNAL_H
