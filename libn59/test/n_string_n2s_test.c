#include "n_test.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static int fixes[] = { 0, 5, 8, 9 };
static n_format_t formats[] = { N_FLOAT, N_SCI, N_ENG };
static char *format_strs[] = { "FLOAT", "SCI", "ENG" };

/* Exhaustive test over thousands of combinations of numbers and formats. */
static void test_exhaustive() {
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

  for (int i = 0; i < N_ELEMS(mants); i++) {
    printf("==========================================================\n\n");
    printf("mantissa = %lld\n\n", mants[i]);
    for (int j = 0; j < N_ELEMS(formats); j++) {
      for (int k = 0; k < N_ELEMS(fixes); k++) {
        printf("=== mode %s | fix %d ===\n\n", format_strs[j], fixes[k]);
        for (int l = 0; l < N_ELEMS(exps); l++) {
          if (mants[i] == 0 && exps[l]) continue;

          n_t n = n_make(mants[i], exps[l]);

          char str[N_N2S_MAX_SIZE];
          n_err_t err;
          n_n2s(n, fixes[k], formats[j], (char *)&str, &err);
          printf("%s%s\n", str, err ? " ?" : "");
        }
        printf("\n");
      }
    }
  }
}

/**
 * Verifies n_n2s(n_s2n(s)) = s, for numbers display numbers n.
 * For example '3.1415-12' => (3141500000000, -12) => '3.1415-12'.
 */
static void test_n2s_s2n() {
  n_t numbers[] = { TEST_NUMBERS, N_E, N_10 };
  for (int i = 0; i < N_ELEMS(numbers); i++) {
    printf("\n\n\n");
    for (int j = 0; j < N_ELEMS(formats); j++) {
      printf("\n%s (fix 0, 5, 8, 9)\n", format_strs[j]);
      for (int k = 0; k < N_ELEMS(fixes); k++) {
        // n1 -> str_n1 -> n2 -> str_n2 -> n3.
        n_t n1, n2, n3;
        char str_n1[N_N2S_MAX_SIZE], str_n2[N_N2S_MAX_SIZE];

        n1 = numbers[i];
        n_n2s(n1, fixes[k], formats[j], str_n1, NULL);
        n2 = n_s2n(str_n1, NULL);
        n_n2s(n2, fixes[k], formats[j], str_n2, NULL);
        n3 = n_s2n(str_n2, NULL);

        assert(n_equals(n2, n3));
        assert(strcmp(str_n1, str_n2) == 0);

        char str1[N_PRINT_MAX_SIZE];
        char str2[N_PRINT_MAX_SIZE];
        printf("%s => %14s => %s => %14s\n",
               n_print(n1,str1), str_n1, n_print(n2, str2), str_n2);
      }
    }
  }
}

int main(void) {
  test_exhaustive();
  test_n2s_s2n();
}
