/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Include polybench common header. */
#include <polybench.h>

/* Include benchmark-specific header. */
/* Default data type is double, default size is N=1024. */
#include "definitions.h"

/* Array initialization. */
static void init_1darray(int n, DATA_TYPE POLYBENCH_1D(x, N, n)) {
  int i, j;

  for (i = 0; i < n; i++)
    x[i] = 42;
}

static void init_2darray(int n, DATA_TYPE POLYBENCH_2D(C, N, N, n, n)) {
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      C[i][j] = 42;
}

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static void print_array(int n, DATA_TYPE POLYBENCH_2D(C, N, N, n, n)) {
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      fprintf(stderr, DATA_PRINTF_MODIFIER, C[i][j]);
      if (i % 20 == 0)
        fprintf(stderr, "\n");
    }
  fprintf(stderr, "\n");
}

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static void kernel_template(int n, DATA_TYPE POLYBENCH_2D(X, N, N, n, n),
                            DATA_TYPE POLYBENCH_2D(A, N, N, n, n),
                            DATA_TYPE POLYBENCH_2D(C, N, N, n, n)) {
  int t = 0;
  int j, tmax;
#pragma macveth i 1 j 4
  for (int i = 0; i < _PB_N; i++) {
    t = i * 2;
    tmax = N / 2;
    for (j = (t + 1); j < tmax; j++) {
      C[i][j] = C[i][j] + X[i][j] * A[i][j];
    }
  }
#pragma endmacveth
}

int main(int argc, char **argv) {
  /* Retrieve problem size. */
  int n = N;

  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(X, DATA_TYPE, N, N, n, n);
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, N, N, n, n);
  POLYBENCH_2D_ARRAY_DECL(C, DATA_TYPE, N, N, n, n);

  /* Initialize array(s). */
  init_2darray(n, POLYBENCH_ARRAY(A));
  init_2darray(n, POLYBENCH_ARRAY(X));
  init_2darray(n, POLYBENCH_ARRAY(C));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_template(n, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(X),
                  POLYBENCH_ARRAY(C));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(C)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(C);

  return 0;
}
