/**
 * n59.h
 *
 * Header file for libn59, a library that implements arithmetic operators and
 * mathematical functions on TI-59 numbers.
 */


#ifndef N59_H
#define N59_H

#include <stdbool.h>

/** The size of the string that needs to be allocated when calling n_n2s. */
#define N_N2S_MAX_SIZE 14

/** The size of the string that needs to be allocated when calling n_print. */
#define N_PRINT_MAX_SIZE 18

/**
 * Note: Most functions have an 'err_out' parameter. If non null, it is set to
 * true if an error occurs, and to false otherwise. An error occurs when some
 * parameter is not in the function domain ("square root of -1") or because of
 * underflow or overflow ("square of 10^80"). In a TI-59, the display would
 * blink in case of error.
 */


/******************************************************************************
 *
 *  TYPES.
 *
 ******************************************************************************/

/**
 * TI-59 number.
 *
 * For nonzero numbers:
 * - the mantissa is a positive or negative integer, with exactly 13 digits.
 * - the exponent is an integer in -99..99.
 *
 * 0 is represented as { 0, 0 }.
 *
 * Note: Prefer n_make(mant, exp) to directly modifying mant and exp.
 */
typedef struct n_s {
  long long mant;
  int exp;
} n_t;

/** Display formats. */
typedef enum n_format_e {
  N_FLOAT,  // Float if possible, scientific for small and big numbers.
  N_SCI,    // Scientific notation.
  N_ENG     // Engineering notation.
} n_format_t;

/** Trigonometric modes. */
typedef enum n_trig_e {
  N_RAD,  // Radians.
  N_DEG,  // Degrees.
  N_GRAD  // Gradians.
} n_trig_t;

/**
 * Errors.
 *
 * Used as an out parameter by most functions from the library. If 'err_out' is
 * null, it is ignored. Otherwise, it is set to one of the values in the enum.
 */
typedef enum n_err_e {
  N_ERR_NONE = 0,   // No error.
  N_ERR_DOMAIN,     // Input not in the domain of the function.
  N_ERR_UNDERFLOW,  // Value too small, in absolute value, to be stored.
  N_ERR_OVERFLOW    // Value too big, in absolute value, to be displayed/stored.
} n_err_t;


/******************************************************************************
 *
 *  CONSTANTS.
 *
 ******************************************************************************/

extern n_t N_0;    // 0.
extern n_t N_1;    // 1.
extern n_t N_PI;   // pi.
extern n_t N_INF;  // infinity, largest number.
extern n_t N_EPS;  // epsilon, smallest positive number.

extern n_t N_10;   // 10.
extern n_t N_E;    // e.

/******************************************************************************
 *
 *  UTILITY METHODS.
 *
 ******************************************************************************/

/**
 * Returns a TI-59 number with the specified mantissa and exponent.
 *
 * Asserts that either mant and exp are zero or mant has 13 digits and exp is in
 * -99..99.
 */
n_t n_make(long long mant, int exp);

/** Returns true if n1 and n2 are equal. */
bool n_equals(n_t n1, n_t n2);

/** Returns true if n is zero. */
bool n_is_zero(n_t n);

/** Returns 1, 0 or -1, depending on whether n1 > n2, n1 == n2 or n1 < n2. */
int n_cmp(n_t n1, n_t n2);

/**
 * Returns a representation of n as a string for debugging purposes.
 *
 * For example n_print(N_PI, str) returns " 3141592653590 00".
 * Note that this method returns all the 13 digits of the mantissa, unlike
 * n_n2s which returns only the digits on the display.
 *
 * String 'str_out' must be of size at least 'N_PRINT_MAX_SIZE'. For convenience
 * str_out is also returned directly by this function.
 */
char *n_print(n_t n, char *str_out);


/******************************************************************************
 *
 *  ARITHMETIC OPERATORS.
 *
 ******************************************************************************/

/** Addition. */
n_t n_plus(n_t n1, n_t n2, n_err_t *err_out);

/** Substraction. */
n_t n_minus(n_t n1, n_t n2, n_err_t *err_out);

/** Multiplication. */
n_t n_times(n_t n1, n_t n2, n_err_t *err_out);

/** Division. */
n_t n_div(n_t n1, n_t n2, n_err_t *err_out);

/** Exponentiation. */
n_t n_pow(n_t n1, n_t n2, n_err_t *err_out);

/** Root extraction. */
n_t n_ipow(n_t n1, n_t n2, n_err_t *err_out);


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

/** n^2. */
n_t n_square(n_t n, n_err_t *err_out);

/** 1/n. */
n_t n_1_over_x(n_t n, n_err_t *err_out);

/** Square root of n. */
n_t n_sqrt(n_t n, n_err_t *err_out);

/** Natural log. */
n_t n_ln(n_t n, n_err_t *err_out);

/** Log in base 10. */
n_t n_log(n_t n, n_err_t *err_out);

/** e^n. */
n_t n_exp(n_t n, n_err_t *err_out);

/** 10^n. */
n_t n_pow10(n_t n, n_err_t *err_out);


/******************************************************************************
 *
 *  TRIGONOMETRIC FUNCTIONS.
 *
 ******************************************************************************/

/** sine. */
n_t n_sin(n_t n, n_trig_t mode, n_err_t *err_out);

/** cosine. */
n_t n_cos(n_t n, n_trig_t mode, n_err_t *err_out);

/** tangent. */
n_t n_tan(n_t n, n_trig_t mode, n_err_t *err_out);

/** arcsine. Range is -90..90 (in DEG mode). */
n_t n_asin(n_t n, n_trig_t mode, n_err_t *err_out);

/** arccossine. Range is 0..180 (in DEG mode). */
n_t n_acos(n_t n, n_trig_t mode, n_err_t *err_out);

/** arctangent. Range is -90..90 (in DEG mode). */
n_t n_atan(n_t n, n_trig_t mode, n_err_t *err_out);


/******************************************************************************
 *
 *  SPECIAL FUNCTIONS.
 *
 ******************************************************************************/

/**
 * Converts degrees/minutes/seconds to decimal degrees.
 *
 * Only the digits of n that are visible on TI-59 display are considered
 * for the conversion. For example, if format is FLOAT and fix is 2, 3.1549 will
 * be first trimmed down to 3.15 (3 hours and 15 minutes) and then converted to
 * 3.25.
 *
 * Note: fix must be in 0..9.
 */
n_t n_dms(n_t n, int fix, n_format_t format, n_err_t *err_out);

/**
 * Converts decimal degrees to degrees/minutes/seconds.
 *
 * Only the digits of n that are visible on TI-59 display are considered
 * for the conversion. For example, if format is FLOAT and fix is 2, 3.2549 will
 * be first trimmed down to 3.25 and then converted to 3.15 (3 hours and 15
 * minutes)..
 *
 * Note: fix must be in 0..9.
 */
n_t n_idms(n_t n, int fix, n_format_t format, n_err_t *err_out);

/** Converts polar coordinates to rectangular coordinates. */
void n_p_r(n_t n_rho, n_t n_theta, n_trig_t mode,
           n_t *n_x_out, n_t *n_y_out, n_err_t *err_out);

/**
 * Converts rectangular coordinates to polar coordinates.
 *
 * Angle theta_out is in range -90..270 (in DEG mode).
 */
void n_r_p(n_t n_x, n_t n_y, n_trig_t mode,
           n_t *n_rho_out, n_t *n_theta_out, n_err_t *err_out);


/******************************************************************************
 *
 *  CONVERSION FROM/TO STRINGS.
 *
 ******************************************************************************/

/**
 * Number to string.
 *
 * The return string is the number as it would appear on the display with
 * leading and trailing spaces trimmed:
 * - '-' if negative number,
 * - followed by digits and exactly 1 '.',
 * - optionally followed by exponent: ' ' or '-', and exactly 2 digits.
 *
 * For example, for pi, with fix 2, format SCI: '3.14 00'.
 *
 * String 'str_out' must be of size at least 'N_N2S_MAX_SIZE'.
 * Sets error if overflow.
 */
void n_n2s(n_t n, int fix, n_format_t format, char *str_out, n_err_t *err_out);

/**
 * String to number.
 *
 * String must be composed of:
 * - a float: a, a., .b, a.b, -a, -a., -.b or -a.b  where a and b are sequences
 *   of 1 or more digits,
 * - followed, possibly, by an exponent: e or -e where e is a sequence of 1 or
 *   more digits.
 *
 * There may be spaces at the beginning and end of the string, and between the
 * float and the exponent. If the the exponent is positive, a space is required
 * between the float and the exponent.
 *
 * Sets error N_ERR_DOMAIN if s is incorrectly formatted.
 * Sets error if underflow or overflow.
 */
n_t n_s2n(char *s, n_err_t *err_out);


/******************************************************************************
 *
 *  CONVERSION FROM/TO DOUBLES.
 *
 ******************************************************************************/

/** Number to double. */
double n_n2d(n_t n);

/**
 * Double to number.
 *
 * Sets error if under/overflow.
 */
n_t n_d2n(double d, n_err_t *err_out);

#endif  // N59_H