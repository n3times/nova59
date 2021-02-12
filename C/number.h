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

static n_t N_0 = { 0LL, 0 };
static n_t N_INF = { 9999999999999LL,  99 };
static n_t N_EPS = { 1000000000000LL, -99 };

void n2s(n_t n, int fix, mode_t mode, char *str_out);
n_t s2n(char *s, bool *err);
