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
#define DATA_TYPE float
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

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static void print_value(DATA_TYPE S) {
  fprintf(stderr, DATA_PRINTF_MODIFIER, S);
  fprintf(stderr, "\n");
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
static void kernel_template(int n, DATA_TYPE *S,
                            DATA_TYPE POLYBENCH_1D(x, N, n)) {
  DATA_TYPE tmp = (*S);
#pragma macveth i 8
  for (int i = 0; i < _PB_N; i++) {
    tmp = tmp + x[i];
  }
#pragma endmacveth
  (*S) = tmp;
}

int main(int argc, char **argv) {
  /* Retrieve problem size. */
  int n = N;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N, n);

  /* Initialize array(s). */
  init_1darray(n, POLYBENCH_ARRAY(x));

  DATA_TYPE S = 0;

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_template(n, &S, POLYBENCH_ARRAY(x));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_1darray(n, POLYBENCH_ARRAY(x)));
  polybench_prevent_dce(print_value(S));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(x);

  return 0;
}
