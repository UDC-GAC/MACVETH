//#include <immintrin.h>

/**
 * File              : conv.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 19 nov 2019 15:38:07 MST
 * Last Modified Date: Dom 08 Dec 2019 14:58:57 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef CONV_H
#define CONV_H

#include <stdio.h>

#include "../definitions.h"
#include "../utilities/polybench.h"

// static inline void init_arrays(int n, DATA_TYPE bias[n], DATA_TYPE
// bottom[n][n][n],
//                               DATA_TYPE weights[n][n][n][n],
//                               DATA_TYPE top[n][n][n]) {
static inline void init_arrays(int n, DATA_TYPE POLYBENCH_1D(bias, N, n),
                               DATA_TYPE POLYBENCH_3D(bottom, N, N, N, n, n, n),
                               DATA_TYPE POLYBENCH_4D(weights, N, N, N, N, n, n,
                                                      n, n),
                               DATA_TYPE POLYBENCH_3D(top, N, N, N, n, n, n)) {
  int i, j, k, l;
  for (i = 0; i < n; ++i) {
    bias[i] = 42;
    for (j = 0; j < n; ++j) {
      for (k = 0; k < n; ++k) {
        bottom[i][j][k] = 42;
        top[i][j][k] = 42;
        for (l = 0; l < n; ++l) {
          weights[i][j][k][l] = 42;
        }
      }
    }
  }
}

static inline void print_array(int n, DATA_TYPE POLYBENCH_1D(bias, N, n)) {
  for (int i = 0; i < n; i++) {
    fprintf(stderr, "%f", bias[i]);
    if (i % 20 == 0)
      fprintf(stderr, "\n");
  }
  fprintf(stderr, "\n");
}

inline void convolution(int n, int kernel_size, int stride, int pad,
                        DATA_TYPE POLYBENCH_1D(bias, N, n),
                        DATA_TYPE POLYBENCH_3D(bottom, N, N, N, n, n, n),
                        DATA_TYPE POLYBENCH_4D(weights, N, N, N, N, n, n, n, n),
                        DATA_TYPE POLYBENCH_3D(top, N, N, N, n, n, n));
#endif
