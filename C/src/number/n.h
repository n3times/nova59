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
 * For non zero numbers:
 * - mantissa is a positive or negative integer, with exactly 13 digits.
 * - exponent is in -99..99.
 *
 * 0 is represented as { 0, 0 }.
 */
typedef struct n_s {
  long long mant;
  int exp;
} n_t;

/** Number notation. */
typedef enum notation_e {
  FLOAT,  // Float if possible, scientific for small and big numbers.
  SCI,    // Scientific notation.
  ENG     // Engineering notation.
} notation_t;

/** Trigonometric mode. */
typedef enum trig_e {
  RAD,  // Radians.
  DEG,  // Degrees.
  GRAD  // Gradians.
} trig_t;


/******************************************************************************
 *
 *  CONSTANTS.
 *
 ******************************************************************************/

extern n_t N_0;    // 0.
extern n_t N_1;    // 1.
extern n_t N_PI;   // Pi.
extern n_t N_INF;  // Infinity, largest number.
extern n_t N_EPS;  // Epsilon, smallest positive number.


/******************************************************************************
 *
 *  NOTE ON ERRORS.
 *
 *  For functions, if "err" parameter is non null, it will be set to true if an
 *  error occurs, and false otherwise.
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
n_t n_plus(n_t n1, n_t n2, bool *err);

/** Substraction. */
n_t n_minus(n_t n1, n_t n2, bool *err);

/** Multiplication. */
n_t n_times(n_t n1, n_t n2, bool *err);

/** Division. */
n_t n_div(n_t n1, n_t n2, bool *err);

/** Exponentiation. */
n_t n_pow(n_t n1, n_t n2, bool *err);

/** Root extraction. */
n_t n_ipow(n_t n1, n_t n2, bool *err);


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
n_t n_square(n_t n, bool *err);

/** 1/n. */
n_t n_1_x(n_t n, bool *err);

/** Square root of n. */
n_t n_sqrt(n_t n, bool *err);

/** Natural log. */
n_t n_ln(n_t n, bool *err);

/** Log in base 10. */
n_t n_log(n_t n, bool *err);

/** e^n. */
n_t n_exp(n_t n, bool *err);

/** 10^n. */
n_t n_pow10(n_t n, bool *err);


/******************************************************************************
 *
 *  TRIGONOMETRIC FUNCTIONS.
 *
 ******************************************************************************/

/** sine. */
n_t n_sin(n_t n, trig_t mode, bool *err);

/** cosine. */
n_t n_cos(n_t n, trig_t mode, bool *err);

/** tangent. */
n_t n_tan(n_t n, trig_t mode, bool *err);

/** arcsine. */
n_t n_asin(n_t n, trig_t mode, bool *err);

/** arccosine. */
n_t n_acos(n_t n, trig_t mode, bool *err);

/** arctangent. */
n_t n_atan(n_t n, trig_t mode, bool *err);


/******************************************************************************
 *
 *  SPECIAL FUNCTIONS.
 *
 ******************************************************************************/

/** Converts degrees/minutes/seconds to decimal degrees. */
n_t n_dms(n_t n, int fix, notation_t notation, bool *err);

/** Converts decimal degrees to degrees/minutes/seconds. */
n_t n_idms(n_t n, int fix, notation_t notation, bool *err);

/** Converts polar coordinates to rectangular coordinates. */
void n_p_r(n_t rho, n_t theta, n_t *x_out, n_t *y_out, trig_t mode, bool *err);

/** Converts rectangular coordinates to polar coordinates. */
void n_r_p(n_t x, n_t y, n_t *rho_out, n_t *theta_out, trig_t mode, bool *err);


/******************************************************************************
 *
 *  CONVERSION WITH STRINGS.
 *
 *  String representation of numbers:
 *  - optional "-"
 *  - followed by digits and at most 1 "."
 *  - optionally followed by exponent: " " or "-", and 2 digits
 *  - optionally followed by overflow: " ?".
 *
 ******************************************************************************/

/** Number to string. String 'str_out' must have at least 16 characters. */
void n2s(n_t n, int fix, notation_t notation, char *str_out);

/** String to number. Sets error if under/overflow. */
n_t s2n(char *s, bool *err);


/******************************************************************************
 *
 *  CONVERSION WITH DOUBLES.
 *
 ******************************************************************************/

/** Number to double. */
double n2d(n_t n);

/** Double to number. Sets error if under/overflow. */
n_t d2n(double d, bool *err);

#endif  // N_H
