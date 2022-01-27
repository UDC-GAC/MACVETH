/**
 * Copyright 2021 Marcos Horro
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * Copyright 2021 Marcos Horro
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Include polybench common header. */
#include <polybench.h>

#define DATA_TYPE_IS_FLOAT

#ifdef N
#undef N
#define N 128
#endif

/* Include benchmark-specific header. */
/* Default data type is float, default size is N=1024. */
#include "definitions.h"

/* Array initialization. */
static void init_1darray(int n, DATA_TYPE POLYBENCH_1D(x, N, n)) {
  int i, j;

  for (i = 0; i < n; i++)
    x[i] = i + 1;
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
static void kernel(DATA_TYPE POLYBENCH_1D(A, N, n),
                   DATA_TYPE POLYBENCH_1D(x, N, n),
                   DATA_TYPE POLYBENCH_1D(y, N, n)) {
  register int i0, i1, i2, i3, i4, i5, i6;
#pragma macveth unroll i0 full
  y[0] += A[0] * x[192];
  y[0] += A[1] * x[208];
  y[0] += A[2] * x[224];
  y[0] += A[3] * x[240];
  y[0] += A[4] * x[256];
  y[0] += A[5] * x[272];
  y[0] += A[6] * x[288];
  y[0] += A[7] * x[304];
  y[0] += A[8] * x[320];
  y[0] += A[9] * x[336];
  y[0] += A[10] * x[368];
  y[0] += A[11] * x[384];
  y[0] += A[12] * x[400];
  y[0] += A[13] * x[416];
  y[0] += A[14] * x[432];
  y[0] += A[15] * x[672];
  y[0] += A[16] * x[688];
  y[0] += A[17] * x[704];
  y[0] += A[18] * x[720];
  y[0] += A[19] * x[736];
  y[0] += A[20] * x[752];
  y[0] += A[21] * x[768];
  y[0] += A[22] * x[784];
  y[1] += A[23] * x[193];
  y[1] += A[24] * x[209];
  y[1] += A[25] * x[225];
  y[1] += A[26] * x[241];
  y[1] += A[27] * x[257];
  y[1] += A[28] * x[273];
  y[1] += A[29] * x[289];
  y[1] += A[30] * x[305];
  y[1] += A[31] * x[321];
  y[1] += A[32] * x[337];
  y[1] += A[33] * x[369];
  y[1] += A[34] * x[385];
  y[1] += A[35] * x[401];
  y[1] += A[36] * x[417];
  y[1] += A[37] * x[433];
  y[1] += A[38] * x[673];
  y[1] += A[39] * x[689];
  y[1] += A[40] * x[705];
  y[1] += A[41] * x[721];
  y[1] += A[42] * x[737];
  y[1] += A[43] * x[753];
  y[1] += A[44] * x[769];
  y[1] += A[45] * x[785];
  y[2] += A[46] * x[194];
  y[2] += A[47] * x[210];
  y[2] += A[48] * x[226];
  y[2] += A[49] * x[242];
  y[2] += A[50] * x[258];
  y[2] += A[51] * x[274];
  y[2] += A[52] * x[290];
  y[2] += A[53] * x[306];
  y[2] += A[54] * x[322];
  y[2] += A[55] * x[338];
  y[2] += A[56] * x[370];
  y[2] += A[57] * x[386];
  y[2] += A[58] * x[402];
  y[2] += A[59] * x[418];
  y[2] += A[60] * x[434];
  y[2] += A[61] * x[674];
  y[2] += A[62] * x[690];
  y[2] += A[63] * x[706];
  y[2] += A[64] * x[722];
  y[2] += A[65] * x[738];
  y[2] += A[66] * x[754];
  y[2] += A[67] * x[770];
  y[2] += A[68] * x[786];
  y[3] += A[69] * x[195];
  y[3] += A[70] * x[211];
  y[3] += A[71] * x[227];
  y[3] += A[72] * x[243];
  y[3] += A[73] * x[259];
  y[3] += A[74] * x[275];
  y[3] += A[75] * x[291];
  y[3] += A[76] * x[307];
  y[3] += A[77] * x[323];
  y[3] += A[78] * x[339];
  y[3] += A[79] * x[371];
  y[3] += A[80] * x[387];
  y[3] += A[81] * x[403];
  y[3] += A[82] * x[419];
  y[3] += A[83] * x[435];
  y[3] += A[84] * x[675];
  y[3] += A[85] * x[691];
  y[3] += A[86] * x[707];
  y[3] += A[87] * x[723];
  y[3] += A[88] * x[739];
  y[3] += A[89] * x[755];
  y[3] += A[90] * x[771];
  y[3] += A[91] * x[787];
#pragma endmacveth
}

int main(int argc, char **argv) {
  /* Retrieve problem size. */
  int n = N;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(A, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(y, DATA_TYPE, N, n);

  /* Initialize array(s). */
  init_1darray(n, POLYBENCH_ARRAY(A));
  init_1darray(n, POLYBENCH_ARRAY(x));
  init_1darray(n, POLYBENCH_ARRAY(y));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel(POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(x), POLYBENCH_ARRAY(y));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_1darray(n, POLYBENCH_ARRAY(y)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(x);
  POLYBENCH_FREE_ARRAY(y);

  return 0;
}
