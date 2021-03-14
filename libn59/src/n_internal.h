#ifndef N_INTERNAL_H
#define N_INTERNAL_H

#include "n59.h"

#include <assert.h>

#define NULL 0

#define PI 3.14159265359

#define POW10_12   1000000000000LL
#define POW10_13  10000000000000LL

#define ABS(x)     ((x) <  0  ? (-x) : (x))
#define MIN(x, y)  ((x) < (y) ?  (x) : (y))
#define MAX(x, y)  ((x) > (y) ?  (x) : (y))

#define NORMALIZE(n) n = normalize_number(n.mant, n.exp, NULL);

/**
 * Given an arbitrary mantissa and exponent, not necessary within the ranges of
 * those of a TI-59 number, returns a TI-59 number, that is either N_0 or a
 * number whose mantissa has exactly 13 digits and whose exponent is in -99..99.
 */
n_t normalize_number(long long mant, int exp, n_err_t *err);

/** Converts angle from one measuring unit to another. */
double convert_angle(double d, n_trig_t from, n_trig_t to);

/**
 * Returns the most severe error between err1, err2.
 *
 * See n_err_t.
 */
n_err_t max_error(n_err_t err1, n_err_t err2);

#endif  // N_INTERNAL_H
