/*
 *  This is the default license template.
 *
 *  File: test_perf_redux.c
 *  Author: markoshorro
 *  Copyright (c) 2020 markoshorro
 *
 *  To edit this license information: Press Ctrl+Shift+P and press 'Create new
 * License Template...'.
 */

/**
 * File              : test_perf_redux.c
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 28 Feb 2020 19:09:34 CET
 * Last Modified Date: Ven 28 Feb 2020 19:09:34 CET
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#include <immintrin.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Include polybench common header. */
#include "polybench.h"

/* Include benchmark-specific header. */
/* Default data type is double, default size is N=1024. */
#include "../definitions.h"

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
static void print_value(double S) {
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
static void kernel_template(int n, double *S, double *F,
                            DATA_TYPE POLYBENCH_2D(A, N, N, n, n),
                            DATA_TYPE POLYBENCH_2D(B, N, N, n, n),
                            DATA_TYPE POLYBENCH_1D(x, N, n),
                            DATA_TYPE POLYBENCH_1D(y, N, n)) {
  double G = (*S);
  double H = (*F);
  double sum0 = 0.0;
  double sum1 = 0.0;
  double sum2 = 0.0;
  double sum3 = 0.0;
  double w[N], z[N];
  int i, j, t;
  for (int t = 0; t < 100; ++t) {
    sum0 = 0.0;
    sum1 = 0.0;
    sum2 = 0.0;
    sum3 = 0.0;
#pragma macveth unroll i 4
    for (i = 0; i < _PB_N; ++i) {
      sum0 = sum0 + x[i];
      sum1 = sum1 + y[i];
      sum2 = sum2 + w[i];
      sum3 = sum3 + z[i];
    }
#pragma endmacveth
  }
  (*S) = sum0;
  (*F) = H;
}

int main(int argc, char **argv) {
  /* Retrieve problem size. */
  int n = N;

  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, N, N, n, n);
  POLYBENCH_2D_ARRAY_DECL(B, DATA_TYPE, N, N, n, n);
  POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(y, DATA_TYPE, N, n);

  /* Initialize array(s). */
  init_2darray(n, POLYBENCH_ARRAY(A));
  init_2darray(n, POLYBENCH_ARRAY(B));
  init_1darray(n, POLYBENCH_ARRAY(x));
  init_1darray(n, POLYBENCH_ARRAY(y));

  double S = 0;
  double F = 0;

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_template(n, &S, &F, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B),
                  POLYBENCH_ARRAY(x), POLYBENCH_ARRAY(y));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_1darray(n, POLYBENCH_ARRAY(x)));
  polybench_prevent_dce(print_value(S));
  polybench_prevent_dce(print_1darray(n, POLYBENCH_ARRAY(y)));
  polybench_prevent_dce(print_value(F));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(x);
  POLYBENCH_FREE_ARRAY(y);

  return 0;
}
