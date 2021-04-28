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

#define DATA_TYPE_IS_FLOAT

/* Include benchmark-specific header. */
/* Default data type is float, default size is N=1024. */
#include "definitions.h"

/* Array initialization. */
static void init_1darray(int n, DATA_TYPE POLYBENCH_1D(x, N, n)) {
  int i, j;

  for (i = 0; i < n; i++)
    x[i] = 1.05;
}

static void init_2darray(int n, DATA_TYPE POLYBENCH_2D(C, N, N, n, n)) {
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      C[i][j] = 42;
}

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static void print_value(float S) {
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
static void kernel_template(
    int n, float *S, float *F, float *G, float *H, float *SS, float *FF,
    float *GG, float *HH, DATA_TYPE POLYBENCH_1D(x, N, n),
    DATA_TYPE POLYBENCH_1D(y, N, n), DATA_TYPE POLYBENCH_1D(w, N, n),
    DATA_TYPE POLYBENCH_1D(z, N, n), DATA_TYPE POLYBENCH_1D(xx, N, n),
    DATA_TYPE POLYBENCH_1D(yy, N, n), DATA_TYPE POLYBENCH_1D(ww, N, n),
    DATA_TYPE POLYBENCH_1D(zz, N, n)) {
  float s = (*S);
  float f = (*F);
  float g = (*G);
  float h = (*H);
  float ss = (*SS);
  float ff = (*FF);
  float gg = (*GG);
  float hh = (*HH);
#pragma macveth i 8
  for (int i = 0; i < _PB_N; i++) {
    s = s * x[i];
    f = f * y[i];
    g = g * w[i];
    h = h * z[i];
    ss = ss * xx[i];
    ff = ff * yy[i];
    gg = gg * ww[i];
    hh = hh * zz[i];
  }
#pragma endmacveth
  (*S) = s;
  (*F) = f;
  (*G) = g;
  (*H) = h;
  (*SS) = ss;
  (*FF) = ff;
  (*GG) = gg;
  (*HH) = hh;
}

int main(int argc, char **argv) {
  /* Retrieve problem size. */
  int n = N;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(y, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(w, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(z, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(xx, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(yy, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(ww, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(zz, DATA_TYPE, N, n);

  /* Initialize array(s). */
  init_1darray(n, POLYBENCH_ARRAY(x));
  init_1darray(n, POLYBENCH_ARRAY(y));
  init_1darray(n, POLYBENCH_ARRAY(w));
  init_1darray(n, POLYBENCH_ARRAY(z));
  init_1darray(n, POLYBENCH_ARRAY(xx));
  init_1darray(n, POLYBENCH_ARRAY(yy));
  init_1darray(n, POLYBENCH_ARRAY(ww));
  init_1darray(n, POLYBENCH_ARRAY(zz));

  float S = 2.0;
  float F = 1.0;
  float G = 2.0;
  float H = 1.1;
  float SS = 2.1;
  float FF = 1.3;
  float GG = 1.2;
  float HH = 1.1;

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_template(n, &S, &F, &G, &H, &SS, &FF, &GG, &HH, POLYBENCH_ARRAY(x),
                  POLYBENCH_ARRAY(y), POLYBENCH_ARRAY(w), POLYBENCH_ARRAY(z),
                  POLYBENCH_ARRAY(xx), POLYBENCH_ARRAY(yy), POLYBENCH_ARRAY(ww),
                  POLYBENCH_ARRAY(zz));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_value(S));
  polybench_prevent_dce(print_value(F));
  polybench_prevent_dce(print_value(G));
  polybench_prevent_dce(print_value(H));
  polybench_prevent_dce(print_value(SS));
  polybench_prevent_dce(print_value(FF));
  polybench_prevent_dce(print_value(GG));
  polybench_prevent_dce(print_value(HH));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(x);
  POLYBENCH_FREE_ARRAY(y);
  POLYBENCH_FREE_ARRAY(w);
  POLYBENCH_FREE_ARRAY(z);
  POLYBENCH_FREE_ARRAY(xx);
  POLYBENCH_FREE_ARRAY(yy);
  POLYBENCH_FREE_ARRAY(ww);
  POLYBENCH_FREE_ARRAY(zz);

  return 0;
}
