/**
 * File              : main.c
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 08 Dec 2019 11:39:00 MST
 * Last Modified Date: Dom 08 Dec 2019 21:54:34 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Include polybench common header. */
#include "utilities/polybench.h"

/* Include benchmark-specific header. */
/* Default data type is double, default size is N=1024. */
#ifdef CONV_AVX
#include "conv-avx/conv-avx.h"
#else
#include "conv/conv.h"
#endif
#include "definitions.h"

int main(int argc, char **argv) {
  /* Retrieve problem size. */
  int n = N;
  int kernel_size = KERNEL_SIZE;
  int pad = PAD;
  int stride = STRIDE;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(bias, DATA_TYPE, N, n);
  POLYBENCH_3D_ARRAY_DECL(bottom, DATA_TYPE, N, N, N, n, n, n);
  POLYBENCH_3D_ARRAY_DECL(top, DATA_TYPE, N, N, N, n, n, n);
  POLYBENCH_4D_ARRAY_DECL(weights, DATA_TYPE, N, N, N, N, n, n, n, n);

  /* Initialize array(s). */
  init_arrays(n, POLYBENCH_ARRAY(bias), POLYBENCH_ARRAY(bottom),
              POLYBENCH_ARRAY(weights), POLYBENCH_ARRAY(top));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  convolution(n, kernel_size, stride, pad, POLYBENCH_ARRAY(bias),
              POLYBENCH_ARRAY(bottom), POLYBENCH_ARRAY(weights),
              POLYBENCH_ARRAY(top));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(bias)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(bias);

  return 0;
}
