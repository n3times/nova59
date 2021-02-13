#ifndef N_H
#define N_H
#include <stdbool.h>

#define ABS(x)          ( (x) < 0 ? (-x) : (x) )
#define MIN(x, y)       ( (x) < (y) ? (x) : (y) )
#define MAX(x, y)       ( (x) > (y) ? (x) : (y) )
#define N_ELEMS(array)  ( sizeof(array) / sizeof((array)[0]) )

#define POW10_12  1000000000000LL
#define POW10_13 10000000000000LL

typedef struct n_s {
  long long mant;
  int exp;
} n_t;

typedef enum mode_e {
  FLOAT,
  EE,
  ENG
} mode_t;

typedef enum trig_e {
  RAD,
  DEG,
  GRAD
} trig_t;


/******************************************************************************
 *
 *  CONSTANTS.
 *
 ******************************************************************************/

extern n_t N_0;
extern n_t N_1;
extern n_t N_PI;
extern n_t N_INF;
extern n_t N_EPS;


/******************************************************************************
 *
 *  CONVERSIONS.
 *
 ******************************************************************************/

/** Number to double. */
extern double n2d(n_t n);

/** Double to number. Sets error if under/overflow. */
extern n_t d2n(double d, bool *err);

// Numbers are represented as strings:
// - optional "-"
// - followed by digits and at most 1 "."
// - optionally followed by exponent: " " or "-", and 2 digits
// - optionally followed by overflow: " ?".

/** Number to string. Return string must have at least 16 characters */
extern void n2s(n_t n, int fix, mode_t mode, char *str_out);

/** String to number. Sets error if under/overflow. */
extern n_t s2n(char *s, bool *err);


/******************************************************************************
 *
 *  FUNCTIONS.
 *
 ******************************************************************************/

/** Arithmetic functions. */
n_t n_plus(n_t x,  n_t y, bool *err);
n_t n_minus(n_t x, n_t y, bool *err);
n_t n_times(n_t x, n_t y, bool *err);
n_t n_div(n_t x,   n_t y, bool *err);

/** Math functions. */
n_t n_chs(n_t n);
n_t n_abs(n_t n);
n_t n_sign(n_t n);
n_t n_int(n_t n);
n_t n_frac(n_t n);
n_t n_square(n_t n, bool *err);
n_t n_1_x(n_t n,    bool *err);
n_t n_root(n_t n,   bool *err);
n_t n_ln(n_t n,     bool *err);
n_t n_log(n_t n,    bool *err);
n_t n_exp(n_t n,    bool *err);
n_t n_pow10(n_t n,  bool *err);

/** Trigonometric functions. */
n_t n_sin(n_t n,  trig_t mode, bool *err);
n_t n_cos(n_t n,  trig_t mode, bool *err);
n_t n_tan(n_t n,  trig_t mode, bool *err);
n_t n_asin(n_t n, trig_t mode, bool *err);
n_t n_acos(n_t n, trig_t mode, bool *err);
n_t n_atan(n_t n, trig_t mode, bool *err);

/** Misc. functions. */
n_t n_dms(n_t n,  bool *err);
n_t n_idms(n_t n, bool *err);
void n_p_r(n_t rho, n_t theta, n_t *x, n_t *y, trig_t mode, bool *err);
void n_r_p(n_t x, n_t y, n_t *rho, n_t *theta, trig_t mode, bool *err);
#endif  // N_H
