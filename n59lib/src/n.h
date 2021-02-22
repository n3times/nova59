#ifndef N_H
#define N_H

#include <stdbool.h>


/******************************************************************************
 *
 *  TYPES.
 *
 ******************************************************************************/

/**
 * TI-59 number.
 *
 * Non zero numbers have
 * - a mantissa: a positive or negative integer, with exactly 13 digits.
 * - an exponent: an integer in -99..99.
 *
 * 0 is represented as { 0, 0 }.
 */
typedef struct n_s {
  long long mant;
  int exp;
} n_t;

/** Display format. */
typedef enum format_e {
  FLOAT,  // Float if possible, scientific for small and big numbers.
  SCI,    // Scientific notation.
  ENG     // Engineering notation.
} format_t;

/** Trigonometric mode. */
typedef enum trig_e {
  RAD,  // Radians.
  DEG,  // Degrees.
  GRAD  // Gradians.
} trig_t;


/******************************************************************************
 *
 *  CONSTANTS AND UTILITY METHODS.
 *
 ******************************************************************************/

extern n_t N_0;    // 0.
extern n_t N_1;    // 1.
extern n_t N_PI;   // Pi.
extern n_t N_INF;  // Infinity, largest number.
extern n_t N_EPS;  // Epsilon, smallest positive number.


/** Preferred method to make a TI-59 number. */
n_t n_make(long long mant, int exp);


/******************************************************************************
 *
 *  NOTE ON ERRORS.
 *
 *  For functions, if "err_out" parameter is non null, it will be set to true
 *  if an error occurs, and false otherwise.
 *  An error occurs when some parameter is not in the function domain ("square
 *  root of -1") or because of underflow or overflow ("square of 10^80").
 *
 ******************************************************************************/


/******************************************************************************
 *
 *  ARITHMETIC OPERATORS.
 *
 ******************************************************************************/

/** Addition. */
n_t n_plus(n_t n1, n_t n2, bool *err_out);

/** Substraction. */
n_t n_minus(n_t n1, n_t n2, bool *err_out);

/** Multiplication. */
n_t n_times(n_t n1, n_t n2, bool *err_out);

/** Division. */
n_t n_div(n_t n1, n_t n2, bool *err_out);

/** Exponentiation. */
n_t n_pow(n_t n1, n_t n2, bool *err_out);

/** Root extraction. */
n_t n_ipow(n_t n1, n_t n2, bool *err_out);


/******************************************************************************
 *
 *  MATH FUNCTIONS.
 *
 ******************************************************************************/

/** -n. */
n_t n_chs(n_t n);

/** |n|. */
n_t n_abs(n_t n);

/** 0, 1 or -1, depending on whether n == 0, n > 0 or n < 0. */
n_t n_sign(n_t n);

/** The integral part of n: n_int(-3.6) == -3, n_int(3.6) == 3. */
n_t n_int(n_t n);

/** The fractional part of n: n_frac(-3.6) == -0.6, n_frac(3.6) == 0.6. */
n_t n_frac(n_t n);

/** n * n. */
n_t n_square(n_t n, bool *err_out);

/** 1/n. */
n_t n_1_x(n_t n, bool *err_out);

/** Square root of n. */
n_t n_sqrt(n_t n, bool *err_out);

/** Natural log. */
n_t n_ln(n_t n, bool *err_out);

/** Log in base 10. */
n_t n_log(n_t n, bool *err_out);

/** e^n. */
n_t n_exp(n_t n, bool *err_out);

/** 10^n. */
n_t n_pow10(n_t n, bool *err_out);


/******************************************************************************
 *
 *  TRIGONOMETRIC FUNCTIONS.
 *
 ******************************************************************************/

/** sine. */
n_t n_sin(n_t n, trig_t mode, bool *err_out);

/** cosine. */
n_t n_cos(n_t n, trig_t mode, bool *err_out);

/** tangent. */
n_t n_tan(n_t n, trig_t mode, bool *err_out);

/** arcsine. */
n_t n_asin(n_t n, trig_t mode, bool *err_out);

/** arccosine. */
n_t n_acos(n_t n, trig_t mode, bool *err_out);

/** arctangent. */
n_t n_atan(n_t n, trig_t mode, bool *err_out);


/******************************************************************************
 *
 *  SPECIAL FUNCTIONS.
 *
 ******************************************************************************/

/** Converts degrees/minutes/seconds to decimal degrees. */
n_t n_dms(n_t n, int fix, format_t format, bool *err_out);

/** Converts decimal degrees to degrees/minutes/seconds. */
n_t n_idms(n_t n, int fix, format_t format, bool *err_out);

/** Converts polar coordinates to rectangular coordinates. */
void n_p_r(n_t rho, n_t theta, n_t *x_out, n_t *y_out, trig_t mode, bool *err_out);

/** Converts rectangular coordinates to polar coordinates. */
void n_r_p(n_t x, n_t y, n_t *rho_out, n_t *theta_out, trig_t mode, bool *err_out);


/******************************************************************************
 *
 *  CONVERSION WITH STRINGS.
 *
 ******************************************************************************/

/**
 * Number to string.
 *
 * This is the number as it would appear on the display with leading and
 * trailing spaces trimmed:
 * - "-" if negative number
 * - followed by digits and exactly 1 "."
 * - optionally followed by exponent: " " or "-", and exactly 2 digits
 *
 * String 'str_out' must have at least 16 characters.
 * Sets error if overflow.
 */
void n2s(n_t n, int fix, format_t format, char *str_out, bool *err_out);

/**
 * String to number.
 *
 * String should have:
 * - a sequence of digits, optionally starting with "-" and with at most 1 ".".
 * - an optional exponent: a sequence of digits, optionally preceded by "-".
 * - an optional "?".
 * In addition there can be 1 or more spaces before, after or between
 * components.
 *
 * Sets error if under/overflow.
 */
n_t s2n(char *s, bool *err_out);


/******************************************************************************
 *
 *  CONVERSION WITH DOUBLES.
 *
 ******************************************************************************/

/** Number to double. */
double n2d(n_t n);

/** Double to number. Sets error if under/overflow. */
n_t d2n(double d, bool *err_out);

#endif  // N_H
