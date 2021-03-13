#include "n59.h"

#define N_ELEMS(array)  (sizeof(array) / sizeof((array)[0]))

#define ABS(x)     ((x) <  0  ? (-x) : (x))
#define MAX(x, y)  ((x) > (y) ?  (x) : (y))

#define TEST_NUMBERS N_0, N_1, N_PI, N_EPS, N_INF, \
                     n_chs(N_1), n_chs(N_PI), n_chs(N_EPS), n_chs(N_INF)

#define TEST_NUMBERS_2 N_0, N_1, N_PI, N_EPS, N_INF, \
                       n_chs(N_1), n_chs(N_PI), n_chs(N_EPS), n_chs(N_INF), \
                       N_10, N_E, n_chs(N_10), n_chs(N_E)
