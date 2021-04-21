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
static void print_1darray(int n, DATA_TYPE POLYBENCH_1D(C, N, n)) {
  int i, j;

  for (i = 0; i < n; i++)
    fprintf(stderr, DATA_PRINTF_MODIFIER, C[i]);
  if (i % 20 == 0)
    fprintf(stderr, "\n");
  fprintf(stderr, "\n");
}

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static void kernel_template(int n, DATA_TYPE POLYBENCH_1D(x, N, n),
                            DATA_TYPE POLYBENCH_1D(y, N, n),
                            DATA_TYPE POLYBENCH_1D(A, N, n)) {
  register int i0, i1, i2, i3, i4, i5, i6;
#pragma macveth i0 full
  for (i0 = 0; i0 < 3; ++i0) {
    y[0 * i0 + 0] = y[0 * i0 + 0] + A[1 * i0 + 0] * x[1 * i0 + 4];
  }
  for (i0 = 0; i0 < 2; ++i0) {
    y[0 * i0 + 1] = y[0 * i0 + 1] + A[1 * i0 + 3] * x[1 * i0 + 0];
  }
  y[2] = y[2] + A[5] * x[0];
  y[2] = y[2] + A[6] * x[0];
  for (i0 = 0; i0 < 4; ++i0) {
    y[1 * i0 + 2] = y[1 * i0 + 2] + A[2 * i0 + 6] * x[1 * i0 + 2];
  }
  for (i0 = 0; i0 < 6; ++i0) {
    y[1 * i0 + 3] = y[1 * i0 + 3] + A[2 * i0 + 7] * x[1 * i0 + 0];
  }

#pragma endmacveth
}

int main(int argc, char **argv) {
  /* Retrieve problem size. */
  int n = N;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(y, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(A, DATA_TYPE, N, n);

  /* Initialize array(s). */
  init_1darray(n, POLYBENCH_ARRAY(x));
  init_1darray(n, POLYBENCH_ARRAY(y));
  init_1darray(n, POLYBENCH_ARRAY(A));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_template(n, POLYBENCH_ARRAY(x), POLYBENCH_ARRAY(y),
                  POLYBENCH_ARRAY(A));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_1darray(n, POLYBENCH_ARRAY(y)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(x);

  return 0;
}
