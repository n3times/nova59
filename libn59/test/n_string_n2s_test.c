#include "n_test.h"

#include <stdio.h>

int main(void) {
  long long mants[] = { 
                         1000000000000LL,
                         9999999999999LL,
                         9999999999499LL,
                         9999999999500LL,
                         9999999949999LL,
                         9999999950000LL,
                         1234123412341LL,
                        -1234123412341LL,
                         5678956789567LL,
                        -5678956789567LL,
                         5000000000000LL,
                         4999999999999LL,
                                     0LL,
                   };
  int exps[] = {
      -99, -98, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1,
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 98, 99
  };
  int fixes[] = { 0, 5, 8, 9 };
  n_format_t formats[] = { N_FLOAT, N_SCI, N_ENG };

  for (int i = 0; i < N_ELEMS(mants); i++) {
    printf("==========================================================\n\n");
    printf("mantissa = %lld\n\n", mants[i]);
    for (int j = 0; j < N_ELEMS(formats); j++) {
      for (int k = 0; k < N_ELEMS(fixes); k++) {
        printf("=== mode %s | fix %d ===\n\n",
               j == N_FLOAT ? "FLOAT" : j == N_SCI ? "EE" : "ENG", fixes[k]);
        for (int l = 0; l < N_ELEMS(exps); l++) {
          if (mants[i] == 0 && exps[l]) continue;
          n_t n = n_make(mants[i], exps[l]);
          char str[N_STR_MAX_SIZE];
          n_err_t err;
          n_n2s(n, fixes[k], formats[j], (char *)&str, &err);
          printf("%s%s\n", str, err ? " ?" : "");
        }
        printf("\n");
      }
    }
  }
}
