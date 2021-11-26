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
  y[4] += A[92] * x[196];
  y[4] += A[93] * x[212];
  y[4] += A[94] * x[228];
  y[4] += A[95] * x[244];
  y[4] += A[96] * x[260];
  y[4] += A[97] * x[276];
  y[4] += A[98] * x[292];
  y[4] += A[99] * x[308];
  y[4] += A[100] * x[324];
  y[4] += A[101] * x[340];
  y[4] += A[102] * x[372];
  y[4] += A[103] * x[388];
  y[4] += A[104] * x[404];
  y[4] += A[105] * x[420];
  y[4] += A[106] * x[436];
  y[4] += A[107] * x[676];
  y[4] += A[108] * x[692];
  y[4] += A[109] * x[708];
  y[4] += A[110] * x[724];
  y[4] += A[111] * x[740];
  y[4] += A[112] * x[756];
  y[4] += A[113] * x[772];
  y[4] += A[114] * x[788];
  y[5] += A[115] * x[197];
  y[5] += A[116] * x[213];
  y[5] += A[117] * x[229];
  y[5] += A[118] * x[245];
  y[5] += A[119] * x[261];
  y[5] += A[120] * x[277];
  y[5] += A[121] * x[293];
  y[5] += A[122] * x[309];
  y[5] += A[123] * x[325];
  y[5] += A[124] * x[341];
  y[5] += A[125] * x[373];
  y[5] += A[126] * x[389];
  y[5] += A[127] * x[405];
  y[5] += A[128] * x[421];
  y[5] += A[129] * x[437];
  y[5] += A[130] * x[677];
  y[5] += A[131] * x[693];
  y[5] += A[132] * x[709];
  y[5] += A[133] * x[725];
  y[5] += A[134] * x[741];
  y[5] += A[135] * x[757];
  y[5] += A[136] * x[773];
  y[5] += A[137] * x[789];
  y[6] += A[138] * x[198];
  y[6] += A[139] * x[214];
  y[6] += A[140] * x[230];
  y[6] += A[141] * x[246];
  y[6] += A[142] * x[262];
  y[6] += A[143] * x[278];
  y[6] += A[144] * x[294];
  y[6] += A[145] * x[310];
  y[6] += A[146] * x[326];
  y[6] += A[147] * x[342];
  y[6] += A[148] * x[374];
  y[6] += A[149] * x[390];
  y[6] += A[150] * x[406];
  y[6] += A[151] * x[422];
  y[6] += A[152] * x[438];
  y[6] += A[153] * x[678];
  y[6] += A[154] * x[694];
  y[6] += A[155] * x[710];
  y[6] += A[156] * x[726];
  y[6] += A[157] * x[742];
  y[6] += A[158] * x[758];
  y[6] += A[159] * x[774];
  y[6] += A[160] * x[790];
  y[7] += A[161] * x[199];
  y[7] += A[162] * x[215];
  y[7] += A[163] * x[231];
  y[7] += A[164] * x[247];
  y[7] += A[165] * x[263];
  y[7] += A[166] * x[279];
  y[7] += A[167] * x[295];
  y[7] += A[168] * x[311];
  y[7] += A[169] * x[327];
  y[7] += A[170] * x[343];
  y[7] += A[171] * x[375];
  y[7] += A[172] * x[391];
  y[7] += A[173] * x[407];
  y[7] += A[174] * x[423];
  y[7] += A[175] * x[439];
  y[7] += A[176] * x[679];
  y[7] += A[177] * x[695];
  y[7] += A[178] * x[711];
  y[7] += A[179] * x[727];
  y[7] += A[180] * x[743];
  y[7] += A[181] * x[759];
  y[7] += A[182] * x[775];
  y[7] += A[183] * x[791];
  y[8] += A[184] * x[200];
  y[8] += A[185] * x[216];
  y[8] += A[186] * x[232];
  y[8] += A[187] * x[248];
  y[8] += A[188] * x[264];
  y[8] += A[189] * x[280];
  y[8] += A[190] * x[296];
  y[8] += A[191] * x[312];
  y[8] += A[192] * x[328];
  y[8] += A[193] * x[344];
  y[8] += A[194] * x[376];
  y[8] += A[195] * x[392];
  y[8] += A[196] * x[408];
  y[8] += A[197] * x[424];
  y[8] += A[198] * x[440];
  y[8] += A[199] * x[680];
  y[8] += A[200] * x[696];
  y[8] += A[201] * x[712];
  y[8] += A[202] * x[728];
  y[8] += A[203] * x[744];
  y[8] += A[204] * x[760];
  y[8] += A[205] * x[776];
  y[8] += A[206] * x[792];
  y[9] += A[207] * x[201];
  y[9] += A[208] * x[217];
  y[9] += A[209] * x[233];
  y[9] += A[210] * x[249];
  y[9] += A[211] * x[265];
  y[9] += A[212] * x[281];
  y[9] += A[213] * x[297];
  y[9] += A[214] * x[313];
  y[9] += A[215] * x[329];
  y[9] += A[216] * x[345];
  y[9] += A[217] * x[377];
  y[9] += A[218] * x[393];
  y[9] += A[219] * x[409];
  y[9] += A[220] * x[425];
  y[9] += A[221] * x[441];
  y[9] += A[222] * x[681];
  y[9] += A[223] * x[697];
  y[9] += A[224] * x[713];
  y[9] += A[225] * x[729];
  y[9] += A[226] * x[745];
  y[9] += A[227] * x[761];
  y[9] += A[228] * x[777];
  y[9] += A[229] * x[793];
  y[10] += A[230] * x[202];
  y[10] += A[231] * x[218];
  y[10] += A[232] * x[234];
  y[10] += A[233] * x[250];
  y[10] += A[234] * x[266];
  y[10] += A[235] * x[282];
  y[10] += A[236] * x[298];
  y[10] += A[237] * x[314];
  y[10] += A[238] * x[330];
  y[10] += A[239] * x[346];
  y[10] += A[240] * x[378];
  y[10] += A[241] * x[394];
  y[10] += A[242] * x[410];
  y[10] += A[243] * x[426];
  y[10] += A[244] * x[442];
  y[10] += A[245] * x[682];
  y[10] += A[246] * x[698];
  y[10] += A[247] * x[714];
  y[10] += A[248] * x[730];
  y[10] += A[249] * x[746];
  y[10] += A[250] * x[762];
  y[10] += A[251] * x[778];
  y[10] += A[252] * x[794];
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
