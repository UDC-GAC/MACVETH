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

#ifdef DATA_TYPE
#undef DATA_TYPE
#define DATA_TYPE double
#endif

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

static void init_3darray(int n, DATA_TYPE POLYBENCH_3D(C, N, N, N, n, n, n)) {
  int i, j, k;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      for (k = 0; k < n; k++)
        C[i][j][k] = 42;
}

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static void print_2darray(int n, DATA_TYPE POLYBENCH_2D(C, N, N, n, n)) {
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      fprintf(stderr, DATA_PRINTF_MODIFIER, C[i][j]);
      if (i % 20 == 0)
        fprintf(stderr, "\n");
    }
  fprintf(stderr, "\n");
}

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static void print_3darray(int n, DATA_TYPE POLYBENCH_3D(C, N, N, N, n, n, n)) {
  int i, j, k;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      for (k = 0; k < n; k++) {
        fprintf(stderr, DATA_PRINTF_MODIFIER, C[i][j][k]);
        if (i % 20 == 0)
          fprintf(stderr, "\n");
      }
  fprintf(stderr, "\n");
}

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static void kernel_template(int n, DATA_TYPE POLYBENCH_1D(x, N, n),
                            DATA_TYPE POLYBENCH_2D(C, N, N, n, n),
                            DATA_TYPE POLYBENCH_3D(A, N, N, N, n, n, n)) {
  int j, k;
  double alpha = 0.0;
#pragma macveth i 1 j 1 k 4
  for (int i = 0; i < _PB_N; i++) {
    for (j = 0; j < _PB_N; j++) {
      for (k = 0; k < _PB_N; k++) {
        A[i][j][k] = 42.3f;
      }
    }
  }
  // for (int i = 0; i < _PB_N; i++) {
  //   for (j = 0; j < _PB_N; j++) {
  //     for (k = 0; k < _PB_N; k++) {
  //       A[i][j][k] = 42.3;
  //     }
  //   }
  // }
#pragma endmacveth
}

int main(int argc, char **argv) {
  /* Retrieve problem size. */
  int n = 32;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N, n);
  POLYBENCH_2D_ARRAY_DECL(C, DATA_TYPE, N, N, n, n);
  POLYBENCH_3D_ARRAY_DECL(A, DATA_TYPE, N, N, N, n, n, n);

  /* Initialize array(s). */
  init_1darray(n, POLYBENCH_ARRAY(x));
  init_2darray(n, POLYBENCH_ARRAY(C));
  init_3darray(n, POLYBENCH_ARRAY(A));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_template(n, POLYBENCH_ARRAY(x), POLYBENCH_ARRAY(C),
                  POLYBENCH_ARRAY(A));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_2darray(n, POLYBENCH_ARRAY(C)));
  polybench_prevent_dce(print_3darray(n, POLYBENCH_ARRAY(A)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(C);

  return 0;
}
