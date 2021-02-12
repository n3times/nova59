#include <stdbool.h>

#define ABS(x)          ( (x) < 0 ? (-x) : (x) )
#define MIN(x, y)       ( (x) < (y) ? (x) : (y) )
#define MAX(x, y)       ( (x) > (y) ? (x) : (y) )
#define N_ELEMS(array)  ( sizeof(array) / sizeof((array)[0]) )

typedef struct n_s {
  long long mant;
  int exp;
} n_t;

typedef enum mode_e {
  FLOAT,
  EE,
  ENG
} mode_t;

static n_t N_0   = {             0LL,   0 };
static n_t N_INF = { 9999999999999LL,  99 };
static n_t N_EPS = { 1000000000000LL, -99 };

// n number
// fix in 0..9
// mode in { FLOAT, EE, ENG)
// str_out must have at least 16 characters
// 
// Returns representation of number as string:
// - optional "-"
// - followed by float, with exactly 1 "."
// - optionally followed by exponent: " " or "-", and 2 digits
// - optionally followed by overflow: " ?".
void n2s(n_t n, int fix, mode_t mode, char *str_out);

// s is a string:
// - optional "-"
// - followed by float, with at most 1 "."
// - optionally followed by exponent: " " or "-", and 2 digits
// - optionally followed by overflow: " ?".
n_t s2n(char *s, bool *err);
