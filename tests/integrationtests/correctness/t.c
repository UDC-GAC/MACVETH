/* BEGIN MACVETH headers*/
#include "macveth_api.h"
#include "macveth_api.h"
#include <immintrin.h>
/* END MACVETH */
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

  __m256 __vop448, __vop381, __vop449, __vop384, __vop450, __vop387, __vop451,
      __vop390, __vop452, __vop393, __vop453, __vop396, __vop454, __vop399,
      __vop455, __vop402, __vop456, __vop405, __vop457, __vop408, __vop411,
      __vop409, __vop410, __vop458, __vop459, __vop414, __vop460, __vop417,
      __vop420, __vop418, __vop419, __vop461, __vop423, __vop421, __vop422,
      __vop462, __vop426, __vop424, __vop425, __vop463, __vop246, __vop244,
      __vop245, __vop249, __vop247, __vop248, __vop252, __vop250, __vop251,
      __vop464, __vop429, __vop465, __vop432, __vop466, __vop435, __vop467,
      __vop438, __vop255, __vop253, __vop254, __vop258, __vop256, __vop257,
      __vop261, __vop259, __vop260, __vop264, __vop262, __vop263, __vop468,
      __vop441, __vop469, __vop444, __vop2, __vop0, __vop1, __vop5, __vop3,
      __vop4, __vop8, __vop6, __vop7, __vop11, __vop9, __vop10, __vop14,
      __vop12, __vop13, __vop17, __vop15, __vop16, __vop20, __vop18, __vop19,
      __vop23, __vop21, __vop22, __vop27, __vop25, __vop26, __vop30, __vop28,
      __vop29, __vop33, __vop31, __vop32, __vop24, __vop368, __vop337, __vop36,
      __vop34, __vop35, __vop40, __vop38, __vop39, __vop43, __vop41, __vop42,
      __vop46, __vop44, __vop45, __vop391, __vop392, __vop37, __vop369,
      __vop340, __vop49, __vop47, __vop48, __vop394, __vop395, __vop50, __vop53,
      __vop51, __vop52, __vop56, __vop54, __vop55, __vop59, __vop57, __vop58,
      __vop267, __vop265, __vop266, __vop270, __vop268, __vop269, __vop271,
      __vop62, __vop60, __vop61, __vop63, __vop66, __vop64, __vop65, __vop69,
      __vop67, __vop68, __vop72, __vop70, __vop71, __vop274, __vop272, __vop273,
      __vop275, __vop278, __vop276, __vop277, __vop279, __vop75, __vop73,
      __vop74, __vop76, __vop78, __vop77, __vop80, __vop79, __vop82, __vop81,
      __vop282, __vop280, __vop281, __vop283, __vop286, __vop284, __vop285,
      __vop287, __vop85, __vop83, __vop84, __vop86, __vop88, __vop87, __vop90,
      __vop89, __vop92, __vop91, __vop290, __vop288, __vop289, __vop291,
      __vop294, __vop292, __vop293, __vop295, __vop95, __vop93, __vop94,
      __vop96, __vop99, __vop97, __vop98, __vop102, __vop100, __vop101,
      __vop343, __vop341, __vop342, __vop370, __vop105, __vop103, __vop104,
      __vop108, __vop106, __vop107, __vop109, __vop112, __vop110, __vop111,
      __vop115, __vop113, __vop114, __vop371, __vop346, __vop118, __vop116,
      __vop117, __vop121, __vop119, __vop120, __vop122, __vop125, __vop123,
      __vop124, __vop128, __vop126, __vop127, __vop131, __vop129, __vop130,
      __vop298, __vop296, __vop297, __vop299, __vop302, __vop300, __vop301,
      __vop303, __vop134, __vop132, __vop133, __vop135, __vop138, __vop136,
      __vop137, __vop141, __vop139, __vop140, __vop144, __vop142, __vop143,
      __vop306, __vop304, __vop305, __vop344, __vop345, __vop307, __vop310,
      __vop308, __vop309, __vop311, __vop147, __vop145, __vop146, __vop148,
      __vop151, __vop149, __vop150, __vop154, __vop152, __vop153, __vop372,
      __vop349, __vop157, __vop155, __vop156, __vop347, __vop348, __vop160,
      __vop158, __vop159, __vop403, __vop404, __vop161, __vop164, __vop162,
      __vop163, __vop167, __vop165, __vop166, __vop373, __vop352, __vop170,
      __vop168, __vop169, __vop350, __vop351, __vop173, __vop171, __vop172,
      __vop406, __vop407, __vop174, __vop177, __vop175, __vop176, __vop180,
      __vop178, __vop179, __vop183, __vop181, __vop182, __vop374, __vop355,
      __vop186, __vop184, __vop185, __vop353, __vop354, __vop187, __vop190,
      __vop188, __vop189, __vop193, __vop191, __vop192, __vop196, __vop194,
      __vop195, __vop375, __vop358, __vop199, __vop197, __vop198, __vop202,
      __vop200, __vop201, __vop205, __vop203, __vop204, __vop208, __vop206,
      __vop207, __vop379, __vop380, __vop427, __vop428, __vop211, __vop209,
      __vop210, __vop214, __vop212, __vop213, __vop217, __vop215, __vop216,
      __vop314, __vop312, __vop313, __vop317, __vop315, __vop316, __vop356,
      __vop357, __vop361, __vop359, __vop360, __vop376, __vop320, __vop318,
      __vop319, __vop323, __vop321, __vop322, __vop385, __vop386, __vop433,
      __vop434, __vop364, __vop362, __vop363, __vop377, __vop220, __vop218,
      __vop219, __vop382, __vop383, __vop430, __vop431, __vop221, __vop224,
      __vop222, __vop223, __vop227, __vop225, __vop226, __vop230, __vop228,
      __vop229, __vop326, __vop324, __vop325, __vop335, __vop336, __vop388,
      __vop389, __vop436, __vop437, __vop439, __vop440, __vop329, __vop327,
      __vop328, __vop330, __vop233, __vop231, __vop232, __vop397, __vop398,
      __vop412, __vop413, __vop234, __vop237, __vop235, __vop236, __vop240,
      __vop238, __vop239, __vop243, __vop241, __vop242, __vop442, __vop443,
      __vop333, __vop331, __vop332, __vop338, __vop339, __vop400, __vop401,
      __vop415, __vop416, __vop334, __mv_lo256, __mv_hi256, __tmp0_256,
      __tmp1_256, __tmp2_256;
  __m128 __mv_aux0, __mv_aux1, __mv_aux2, __mv_aux3, __mv_aux4, __mv_aux5,
      __mv_aux6, __tmp_extract_128, __mv_aux7, __mv_aux8, __vop367, __vop365,
      __vop366, __vop378, __vop472, __vop470, __vop471, __vop447, __vop445,
      __vop446, __mv_lo128, __mv_hi128;

  __vop448 = _mm256_loadu_ps(&y[0]);
  __vop448 = _mm256_add_ps(__vop448, __vop381);
  _mm256_storeu_ps(&y[0], __vop448);

  __vop449 = _mm256_loadu_ps(&y[8]);
  __vop449 = _mm256_add_ps(__vop449, __vop384);
  _mm256_storeu_ps(&y[8], __vop449);

  __vop450 = _mm256_loadu_ps(&y[16]);
  __vop450 = _mm256_add_ps(__vop450, __vop387);
  _mm256_storeu_ps(&y[16], __vop450);

  __vop451 = _mm256_loadu_ps(&y[24]);
  __vop451 = _mm256_add_ps(__vop451, __vop390);
  _mm256_storeu_ps(&y[24], __vop451);

  __vop452 = _mm256_loadu_ps(&y[32]);
  __vop452 = _mm256_add_ps(__vop452, __vop393);
  _mm256_storeu_ps(&y[32], __vop452);

  __vop453 = _mm256_loadu_ps(&y[40]);
  __vop453 = _mm256_add_ps(__vop453, __vop396);
  _mm256_storeu_ps(&y[40], __vop453);

  __vop454 = _mm256_loadu_ps(&y[48]);
  __vop454 = _mm256_add_ps(__vop454, __vop399);
  _mm256_storeu_ps(&y[48], __vop454);

  __vop455 = _mm256_loadu_ps(&y[56]);
  __vop455 = _mm256_add_ps(__vop455, __vop402);
  _mm256_storeu_ps(&y[56], __vop455);

  __vop456 = _mm256_loadu_ps(&y[64]);
  __vop456 = _mm256_add_ps(__vop456, __vop405);
  _mm256_storeu_ps(&y[64], __vop456);

  __vop457 = _mm256_loadu_ps(&y[72]);
  __vop457 = _mm256_add_ps(__vop457, __vop408);
  _mm256_storeu_ps(&y[72], __vop457);

  __vop409 = _mm256_loadu_ps(&A[80]);
  __vop410 = _mm256_loadu_ps(&x[336]);
  __vop411 = _mm256_mul_ps(__vop409, __vop410);
  __vop458 = _mm256_loadu_ps(&y[80]);
  __vop458 = _mm256_add_ps(__vop458, __vop411);
  _mm256_storeu_ps(&y[80], __vop458);

  __vop459 = _mm256_loadu_ps(&y[88]);
  __vop459 = _mm256_add_ps(__vop459, __vop414);
  _mm256_storeu_ps(&y[88], __vop459);

  __vop460 = _mm256_loadu_ps(&y[96]);
  __vop460 = _mm256_add_ps(__vop460, __vop417);
  _mm256_storeu_ps(&y[96], __vop460);

  __vop418 = _mm256_loadu_ps(&A[104]);
  __vop419 = _mm256_loadu_ps(&x[376]);
  __vop420 = _mm256_mul_ps(__vop418, __vop419);
  __vop461 = _mm256_loadu_ps(&y[104]);
  __vop461 = _mm256_add_ps(__vop461, __vop420);
  _mm256_storeu_ps(&y[104], __vop461);

  __vop421 = _mm256_loadu_ps(&A[112]);
  __vop422 = _mm256_loadu_ps(&x[384]);
  __vop423 = _mm256_mul_ps(__vop421, __vop422);
  __vop462 = _mm256_loadu_ps(&y[112]);
  __vop462 = _mm256_add_ps(__vop462, __vop423);
  _mm256_storeu_ps(&y[112], __vop462);

  __vop424 = _mm256_loadu_ps(&A[120]);
  __vop425 = _mm256_loadu_ps(&x[392]);
  __vop426 = _mm256_mul_ps(__vop424, __vop425);
  __vop463 = _mm256_loadu_ps(&y[120]);
  __vop463 = _mm256_add_ps(__vop463, __vop426);
  _mm256_storeu_ps(&y[120], __vop463);

  __vop244 = _mm256_loadu_ps(&A[129]);
  __mv_aux0 = _mm_loadu_ps(&x[16]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[17], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[18], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[18]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[19], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[20], 0x00000030);
  __vop245 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __vop246 = _mm256_mul_ps(__vop244, __vop245);
  y[133] = (y[133] + (A[137] * x[20]));

  __vop247 = _mm256_loadu_ps(&A[137]);
  __mv_aux0 = _mm_loadu_ps(&x[20]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[21], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[22], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[22]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[23], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[24], 0x00000030);
  __vop248 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __vop249 = _mm256_mul_ps(__vop247, __vop248);
  y[137] = (y[137] + (A[145] * x[24]));

  __vop250 = _mm256_loadu_ps(&A[145]);
  __mv_aux0 = _mm_loadu_ps(&x[24]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[25], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[26], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[26]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[27], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[28], 0x00000030);
  __vop251 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __vop252 = _mm256_mul_ps(__vop250, __vop251);
  y[141] = (y[141] + (A[153] * x[28]));

  __mv_aux0 = _mm_load_ss(&y[128]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, y[144 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_loadu_ps(&y[147]);
  __vop464 = _mm256_set_m128(__mv_aux2, __mv_aux1);
  __vop464 = _mm256_add_ps(__vop464, __vop429);
  __tmp_extract_128 = _mm256_extractf128_ps(__vop464, 0x1);
  _mm_storeu_ps(&y[147], __tmp_extract_128);
  y[128] = __vop464[0];
  y[144] = __vop464[1];
  y[145] = __vop464[2];
  y[146] = __vop464[3];

  __vop465 = _mm256_loadu_ps(&y[151]);
  __vop465 = _mm256_add_ps(__vop465, __vop432);
  _mm256_storeu_ps(&y[151], __vop465);

  __vop466 = _mm256_loadu_ps(&y[159]);
  __vop466 = _mm256_add_ps(__vop466, __vop435);
  _mm256_storeu_ps(&y[159], __vop466);

  __vop467 = _mm256_loadu_ps(&y[167]);
  __vop467 = _mm256_add_ps(__vop467, __vop438);
  _mm256_storeu_ps(&y[167], __vop467);

  __mv_aux0 = _mm_loadu_ps(&A[153]);
  __mv_aux1 = _mm_loadu_ps(&A[157]);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[192], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[193], 0x00000030);
  __vop253 = _mm256_set_m128(__mv_aux3, __mv_aux0);
  __mv_aux0 = _mm_loadu_ps(&x[28]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[29], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[30], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[30]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[16], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[17], 0x00000030);
  __vop254 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __vop255 = _mm256_mul_ps(__vop253, __vop254);
  y[178] = (y[178] + (A[194] * x[17]));

  __vop256 = _mm256_loadu_ps(&A[194]);
  __vop257 = _mm256_set_ps(__vop248[2], __vop248[0], __vop248[0], __vop245[6],
                           __vop245[6], __vop245[4], __vop245[4], __vop254[7]);
  __vop258 = _mm256_mul_ps(__vop256, __vop257);
  y[182] = (y[182] + (A[202] * x[21]));

  __vop259 = _mm256_loadu_ps(&A[202]);
  __vop260 = _mm256_set_ps(__vop251[2], __vop251[0], __vop251[0], __vop248[6],
                           __vop248[6], __vop248[4], __vop248[4], __vop248[2]);
  __vop261 = _mm256_mul_ps(__vop259, __vop260);
  y[186] = (y[186] + (A[210] * x[25]));

  __vop262 = _mm256_loadu_ps(&A[210]);
  __vop263 = _mm256_set_ps(__vop254[2], __vop254[0], __vop254[0], __vop251[6],
                           __vop251[6], __vop251[4], __vop251[4], __vop251[2]);
  __vop264 = _mm256_mul_ps(__vop262, __vop263);
  y[190] = (y[190] + (A[218] * x[29]));

  __mv_aux0 = _mm_loadu_ps(&y[175]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, y[192], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, y[193], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&y[194]);
  __vop468 = _mm256_set_m128(__mv_aux3, __mv_aux2);
  __vop468 = _mm256_add_ps(__vop468, __vop441);
  _mm_storel_pi(&y[175], _mm256_castps256_ps128(__vop468));
  _mm_storeh_pi(&y[192], _mm256_castps256_ps128(__vop468));
  __tmp_extract_128 = _mm256_extractf128_ps(__vop468, 0x1);
  _mm_storeu_ps(&y[194], __tmp_extract_128);

  __vop469 = _mm256_loadu_ps(&y[198]);
  __vop469 = _mm256_add_ps(__vop469, __vop444);
  _mm256_storeu_ps(&y[198], __vop469);

  __mv_aux0 = _mm_loadu_ps(&A[238]);
  __mv_aux1 = _mm_loadu_ps(&A[242]);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[245], 0x00000010);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[243], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[244], 0x00000030);
  __vop0 = _mm256_set_m128(__mv_aux4, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[0]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[32], 0x00000014);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[33], 0x00000068);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[256], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&x[1]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[257], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[33], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[34], 0x00000030);
  __vop1 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop2 = _mm256_mul_ps(__vop0, __vop1);
  y[209] = (y[209] + (A[244] * x[34]));

  __mv_aux0 = _mm_load_ss(&A[246]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[249], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[247], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[248], 0x00000030);
  __mv_aux4 = _mm_load_ss(&A[250]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[253], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[251], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[252], 0x000000b0);
  __vop3 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[2]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[258], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[34], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[35], 0x00000030);
  __mv_aux4 = _mm_load_ss(&x[3]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[259], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[35], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[36], 0x000000b0);
  __vop4 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop5 = _mm256_mul_ps(__vop3, __vop4);

  __mv_aux0 = _mm_load_ss(&A[254]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[257], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[255], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[256], 0x00000030);
  __mv_aux4 = _mm_load_ss(&A[258]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[261], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[259], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[260], 0x000000b0);
  __vop6 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[4]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[260], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[36], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[37], 0x00000030);
  __mv_aux4 = _mm_load_ss(&x[5]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[261], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[37], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[38], 0x000000b0);
  __vop7 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop8 = _mm256_mul_ps(__vop6, __vop7);

  __mv_aux0 = _mm_load_ss(&A[262]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[265], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[263], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[264], 0x00000030);
  __mv_aux4 = _mm_load_ss(&A[266]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[269], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[267], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[268], 0x000000b0);
  __vop9 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[6]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[262], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[38], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[39], 0x00000030);
  __mv_aux4 = _mm_load_ss(&x[7]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[263], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[39], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[40], 0x000000b0);
  __vop10 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop11 = _mm256_mul_ps(__vop9, __vop10);

  __mv_aux0 = _mm_loadu_ps(&A[270]);
  __mv_aux1 = _mm_loadu_ps(&A[274]);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[277], 0x00000010);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[275], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[276], 0x00000030);
  __vop12 = _mm256_set_m128(__mv_aux4, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[8]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[40], 0x00000014);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[41], 0x00000068);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[264], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&x[9]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[265], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[41], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[42], 0x00000030);
  __vop13 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop14 = _mm256_mul_ps(__vop12, __vop13);
  y[217] = (y[217] + (A[276] * x[42]));

  __mv_aux0 = _mm_load_ss(&A[278]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[281], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[279], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[280], 0x00000030);
  __mv_aux4 = _mm_load_ss(&A[282]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[285], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[283], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[284], 0x000000b0);
  __vop15 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[10]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[266], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[42], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[43], 0x00000030);
  __mv_aux4 = _mm_load_ss(&x[11]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[267], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[43], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[44], 0x000000b0);
  __vop16 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop17 = _mm256_mul_ps(__vop15, __vop16);

  __mv_aux0 = _mm_load_ss(&A[286]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[289], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[287], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[288], 0x00000030);
  __mv_aux4 = _mm_load_ss(&A[290]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[293], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[291], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[292], 0x000000b0);
  __vop18 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[12]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[268], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[44], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[45], 0x00000030);
  __mv_aux4 = _mm_load_ss(&x[13]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[269], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[45], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[46], 0x000000b0);
  __vop19 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop20 = _mm256_mul_ps(__vop18, __vop19);

  __mv_aux0 = _mm_load_ss(&A[294]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[297], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[295], 0x00000068);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[296], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[301]);
  __vop21 = _mm256_set_m128(__mv_aux4, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[14]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[270], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[46], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[47], 0x00000030);
  __mv_aux4 = _mm_load_ss(&x[0]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[256], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[288], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[304], 0x00000030);
  __vop22 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_loadu_ps(&A[306]);
  __mv_aux1 = _mm_loadu_ps(&A[311]);
  __vop25 = _mm256_set_m128(__mv_aux1, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[1]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[257], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[289], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[305], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[2]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[258], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[290], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[306], 0x00000030);
  __vop26 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop27 = _mm256_mul_ps(__vop25, __vop26);
  __mv_aux0 = _mm_loadu_ps(&A[316]);
  __mv_aux1 = _mm_loadu_ps(&A[321]);
  __vop28 = _mm256_set_m128(__mv_aux1, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[3]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[259], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[291], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[307], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[4]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[260], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[292], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[308], 0x00000030);
  __vop29 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop30 = _mm256_mul_ps(__vop28, __vop29);
  __mv_aux0 = _mm_loadu_ps(&A[326]);
  __mv_aux1 = _mm_loadu_ps(&A[331]);
  __vop31 = _mm256_set_m128(__mv_aux1, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[5]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[261], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[293], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[309], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[6]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[262], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[294], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[310], 0x00000030);
  __vop32 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop33 = _mm256_mul_ps(__vop31, __vop32);

  __tmp0_256 = _mm256_permute_ps(__vop23, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop21, __vop22, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[222] = y[222] + __mv_lo256[0];
  y[224] = y[224] + __mv_lo256[4];

  __vop368 = _mm256_loadu_ps(&y[223]);
  __vop368 = _mm256_add_ps(__vop368, __vop337);
  _mm256_storeu_ps(&y[223], __vop368);

  __mv_aux0 = _mm_loadu_ps(&A[336]);
  __mv_aux1 = _mm_loadu_ps(&A[341]);
  __vop34 = _mm256_set_m128(__mv_aux1, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[7]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[263], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[295], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[311], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[8]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[264], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[296], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[312], 0x00000030);
  __vop35 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_loadu_ps(&A[346]);
  __mv_aux1 = _mm_loadu_ps(&A[351]);
  __vop38 = _mm256_set_m128(__mv_aux1, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[9]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[265], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[297], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[313], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[10]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[266], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[298], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[314], 0x00000030);
  __vop39 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop40 = _mm256_mul_ps(__vop38, __vop39);
  __mv_aux0 = _mm_loadu_ps(&A[356]);
  __mv_aux1 = _mm_loadu_ps(&A[361]);
  __vop41 = _mm256_set_m128(__mv_aux1, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[11]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[267], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[299], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[315], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[12]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[268], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[300], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[316], 0x00000030);
  __vop42 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop43 = _mm256_mul_ps(__vop41, __vop42);
  __mv_aux0 = _mm_loadu_ps(&A[366]);
  __mv_aux1 = _mm_loadu_ps(&A[371]);
  __vop44 = _mm256_set_m128(__mv_aux1, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[13]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[269], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[301], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[317], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[14]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[270], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[302], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[318], 0x00000030);
  __vop45 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop46 = _mm256_mul_ps(__vop44, __vop45);
  __vop391 = _mm256_loadu_ps(&A[32]);
  __vop392 = _mm256_set_ps(__vop35[2], __vop32[6], __vop32[2], __vop29[6],
                           __vop29[2], __vop26[6], __vop26[2], __vop22[6]);
  __vop393 = _mm256_mul_ps(__vop391, __vop392);

  __tmp0_256 = _mm256_permute_ps(__vop36, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop34, __vop35, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[231] = y[231] + __mv_lo256[0];
  y[232] = y[232] + __mv_lo256[4];

  __vop369 = _mm256_loadu_ps(&y[231]);
  __vop369 = _mm256_add_ps(__vop369, __vop340);
  _mm256_storeu_ps(&y[231], __vop369);

  __mv_aux0 = _mm_loadu_ps(&A[376]);
  __mv_aux1 = _mm_loadu_ps(&A[381]);
  __vop47 = _mm256_set_m128(__mv_aux1, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[15]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[271], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[303], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[319], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[0]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[48], 0x00000018);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[49], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[256], 0x00000030);
  __vop48 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop394 = _mm256_loadu_ps(&A[40]);
  __vop395 = _mm256_set_ps(__vop48[2], __vop45[6], __vop45[2], __vop42[6],
                           __vop42[2], __vop39[6], __vop39[2], __vop35[6]);
  __vop396 = _mm256_mul_ps(__vop394, __vop395);

  __tmp0_256 = _mm256_permute_ps(__vop49, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop47, __vop48, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[239] = y[239] + __mv_lo256[0];
  y[240] = y[240] + __mv_lo256[4];

  __mv_aux0 = _mm_load_ss(&A[387]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[390 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[393]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[390 + (12)], 0x0000000e);
  __vop51 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[1]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[257], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[305], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[361], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[2]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[258], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[306], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[362], 0x00000030);
  __vop52 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop53 = _mm256_mul_ps(__vop51, __vop52);
  __mv_aux0 = _mm_load_ss(&A[399]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[402 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[405]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[402 + (12)], 0x0000000e);
  __vop54 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[3]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[259], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[307], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[363], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[4]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[260], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[308], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[364], 0x00000030);
  __vop55 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop56 = _mm256_mul_ps(__vop54, __vop55);
  __mv_aux0 = _mm_load_ss(&A[411]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[414 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[417]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[414 + (12)], 0x0000000e);
  __vop57 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[5]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[261], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[309], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[365], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[6]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[262], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[310], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[366], 0x00000030);
  __vop58 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop59 = _mm256_mul_ps(__vop57, __vop58);
  __mv_aux0 = _mm_loadu_ps(&A[218]);
  __mv_aux1 = _mm_loadu_ps(&A[298]);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[385], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[386], 0x00000030);
  __vop265 = _mm256_set_m128(__mv_aux3, __mv_aux0);
  __mv_aux0 = _mm_loadu_ps(&x[29]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[30], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[31], 0x00000030);
  __mv_aux3 = _mm_load_ss(&x[15]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[47], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[304], 0x00000028);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[360], 0x00000030);
  __vop266 = _mm256_set_m128(__mv_aux7, __mv_aux2);
  __vop267 = _mm256_mul_ps(__vop265, __vop266);
  y[247] = (y[247] + (A[428] * x[367]));

  __mv_aux0 = _mm_loadu_ps(&A[388]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[394], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[395], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[400]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[406], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[407], 0x00000030);
  __vop268 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_loadu_ps(&x[49]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[50], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[51], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[51]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[52], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[53], 0x00000030);
  __vop269 = _mm256_set_m128(__mv_aux5, __mv_aux2);

  __tmp0_256 = _mm256_permute_ps(__vop270, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop268, __vop269, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[241] = y[241] + __mv_lo256[0];
  y[242] = y[242] + __mv_lo256[2];
  y[243] = y[243] + __mv_lo256[4];
  y[244] = y[244] + __mv_lo256[6];

  __mv_aux0 = _mm_load_ss(&A[423]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[426 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_loadu_ps(&A[429]);
  __vop60 = _mm256_set_m128(__mv_aux2, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[7]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[263], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[311], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[367], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[8]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[56], 0x00000018);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[57], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[264], 0x00000030);
  __vop61 = _mm256_set_m128(__mv_aux8, __mv_aux3);

  __tmp0_256 = _mm256_permute_ps(__vop62, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop60, __vop61, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[247] = y[247] + __mv_lo256[0];
  y[248] = y[248] + __mv_lo256[4];

  __mv_aux0 = _mm_load_ss(&A[435]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[438 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[441]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[438 + (12)], 0x0000000e);
  __vop64 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[9]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[265], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[313], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[369], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[10]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[266], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[314], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[370], 0x00000030);
  __vop65 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop66 = _mm256_mul_ps(__vop64, __vop65);
  __mv_aux0 = _mm_load_ss(&A[447]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[450 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[453]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[450 + (12)], 0x0000000e);
  __vop67 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[11]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[267], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[315], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[371], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[12]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[268], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[316], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[372], 0x00000030);
  __vop68 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop69 = _mm256_mul_ps(__vop67, __vop68);
  __mv_aux0 = _mm_load_ss(&A[459]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[462 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[465]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[462 + (12)], 0x0000000e);
  __vop70 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[13]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[269], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[317], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[373], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[14]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[270], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[318], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[374], 0x00000030);
  __vop71 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop72 = _mm256_mul_ps(__vop70, __vop71);
  __mv_aux0 = _mm_loadu_ps(&A[412]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[418], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[419], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[424]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[433], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[434], 0x00000030);
  __vop272 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_loadl_pi(_mm_set_ps(0, 0, 0, 0), &x[53]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[54], 0x00000020);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[55], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[55]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[312], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[368], 0x00000030);
  __vop273 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  y[254] = (y[254] + (A[470] * x[374]));

  __tmp0_256 = _mm256_permute_ps(__vop274, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop272, __vop273, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[245] = y[245] + __mv_lo256[0];
  y[246] = y[246] + __mv_lo256[2];
  y[247] = y[247] + __mv_lo256[4];
  y[248] = y[248] + __mv_lo256[6];

  __mv_aux0 = _mm_loadu_ps(&A[436]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[442], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[443], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[448]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[454], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[455], 0x00000030);
  __vop276 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_loadu_ps(&x[57]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[58], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[59], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[59]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[60], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[61], 0x00000030);
  __vop277 = _mm256_set_m128(__mv_aux5, __mv_aux2);

  __tmp0_256 = _mm256_permute_ps(__vop278, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop276, __vop277, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[249] = y[249] + __mv_lo256[0];
  y[250] = y[250] + __mv_lo256[2];
  y[251] = y[251] + __mv_lo256[4];
  y[252] = y[252] + __mv_lo256[6];

  __mv_aux0 = _mm_loadu_ps(&A[471]);
  __mv_aux1 = _mm_loadu_ps(&A[476]);
  __vop73 = _mm256_set_m128(__mv_aux1, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[15]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[63], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[271], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[319], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[0]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[64], 0x00000018);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[65], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[256], 0x00000030);
  __vop74 = _mm256_set_m128(__mv_aux8, __mv_aux3);

  __tmp0_256 = _mm256_permute_ps(__vop75, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop73, __vop74, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[255] = y[255] + __mv_lo256[0];
  y[256] = y[256] + __mv_lo256[4];

  __mv_aux0 = _mm_load_ss(&A[482]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[485 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[488]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[485 + (12)], 0x0000000e);
  __vop77 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[1]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[257], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[305], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[361], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[2]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[258], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[306], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[362], 0x00000030);
  __vop52 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop78 = _mm256_mul_ps(__vop77, __vop52);
  __mv_aux0 = _mm_load_ss(&A[494]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[497 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[500]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[497 + (12)], 0x0000000e);
  __vop79 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[3]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[259], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[307], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[363], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[4]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[260], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[308], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[364], 0x00000030);
  __vop55 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop80 = _mm256_mul_ps(__vop79, __vop55);
  __mv_aux0 = _mm_load_ss(&A[506]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[509 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[512]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[509 + (12)], 0x0000000e);
  __vop81 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[5]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[261], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[309], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[365], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[6]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[262], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[310], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[366], 0x00000030);
  __vop58 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop82 = _mm256_mul_ps(__vop81, __vop58);

  __mv_aux0 = _mm_loadu_ps(&A[460]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[466], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[467], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[480]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[483], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[484], 0x00000030);
  __vop280 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_loadu_ps(&x[61]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[62], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[63], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[304]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[360], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[65], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[66], 0x000000f0);
  __vop281 = _mm256_set_m128(__mv_aux7, __mv_aux2);

  __tmp0_256 = _mm256_permute_ps(__vop282, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop280, __vop281, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[253] = y[253] + __mv_lo256[0];
  y[254] = y[254] + __mv_lo256[2];
  y[256] = y[256] + __mv_lo256[4];
  y[257] = y[257] + __mv_lo256[6];

  __mv_aux0 = _mm_loadu_ps(&A[489]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[495], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[496], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[501]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[507], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[508], 0x00000030);
  __vop284 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_loadu_ps(&x[66]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[67], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[68], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[68]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[69], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[70], 0x00000030);
  __vop285 = _mm256_set_m128(__mv_aux5, __mv_aux2);

  __tmp0_256 = _mm256_permute_ps(__vop286, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop284, __vop285, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[258] = y[258] + __mv_lo256[0];
  y[259] = y[259] + __mv_lo256[2];
  y[260] = y[260] + __mv_lo256[4];
  y[261] = y[261] + __mv_lo256[6];

  __mv_aux0 = _mm_load_ss(&A[518]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[521 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_loadu_ps(&A[524]);
  __vop83 = _mm256_set_m128(__mv_aux2, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[7]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[263], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[311], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[367], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[8]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[72], 0x00000018);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[73], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[264], 0x00000030);
  __vop84 = _mm256_set_m128(__mv_aux8, __mv_aux3);

  __tmp0_256 = _mm256_permute_ps(__vop85, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop83, __vop84, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[263] = y[263] + __mv_lo256[0];
  y[264] = y[264] + __mv_lo256[4];

  __mv_aux0 = _mm_load_ss(&A[530]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[533 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[536]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[533 + (12)], 0x0000000e);
  __vop87 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[9]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[265], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[313], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[369], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[10]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[266], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[314], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[370], 0x00000030);
  __vop65 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop88 = _mm256_mul_ps(__vop87, __vop65);
  __mv_aux0 = _mm_load_ss(&A[542]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[545 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[548]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[545 + (12)], 0x0000000e);
  __vop89 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[11]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[267], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[315], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[371], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[12]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[268], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[316], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[372], 0x00000030);
  __vop68 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop90 = _mm256_mul_ps(__vop89, __vop68);
  __mv_aux0 = _mm_load_ss(&A[554]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[557 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[560]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[557 + (12)], 0x0000000e);
  __vop91 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[13]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[269], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[317], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[373], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[14]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[270], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[318], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[374], 0x00000030);
  __vop71 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop92 = _mm256_mul_ps(__vop91, __vop71);
  y[270] = (y[270] + (A[565] * x[374]));

  __mv_aux0 = _mm_loadu_ps(&A[513]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[519], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[520], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[528]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[531], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[532], 0x00000030);
  __vop288 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_loadu_ps(&x[70]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[71], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[72], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[312]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[368], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[73], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[74], 0x000000f0);
  __vop289 = _mm256_set_m128(__mv_aux7, __mv_aux2);

  __tmp0_256 = _mm256_permute_ps(__vop290, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop288, __vop289, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[262] = y[262] + __mv_lo256[0];
  y[263] = y[263] + __mv_lo256[2];
  y[264] = y[264] + __mv_lo256[4];
  y[265] = y[265] + __mv_lo256[6];

  __mv_aux0 = _mm_loadu_ps(&A[537]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[543], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[544], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[549]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[555], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[556], 0x00000030);
  __vop292 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_loadu_ps(&x[74]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[75], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[76], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[76]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[77], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[78], 0x00000030);
  __vop293 = _mm256_set_m128(__mv_aux5, __mv_aux2);

  __tmp0_256 = _mm256_permute_ps(__vop294, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop292, __vop293, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[266] = y[266] + __mv_lo256[0];
  y[267] = y[267] + __mv_lo256[2];
  y[268] = y[268] + __mv_lo256[4];
  y[269] = y[269] + __mv_lo256[6];

  __mv_aux0 = _mm_loadu_ps(&A[566]);
  __mv_aux1 = _mm_loadu_ps(&A[571]);
  __vop93 = _mm256_set_m128(__mv_aux1, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[15]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[79], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[271], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[319], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[0]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[80], 0x00000018);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[81], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[256], 0x00000030);
  __vop94 = _mm256_set_m128(__mv_aux8, __mv_aux3);

  __tmp0_256 = _mm256_permute_ps(__vop95, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop93, __vop94, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[271] = y[271] + __mv_lo256[0];
  y[272] = y[272] + __mv_lo256[4];

  __mv_aux0 = _mm_loadu_ps(&A[576]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[579], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[580], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[577], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[581]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[584], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[585], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[582], 0x00000030);
  __vop97 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[1]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[257], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[321], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[81], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[2]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[258], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[322], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[82], 0x00000030);
  __vop98 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop99 = _mm256_mul_ps(__vop97, __vop98);

  __mv_aux0 = _mm_loadu_ps(&A[586]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[589], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[590], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[587], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[591]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[594], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[595], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[592], 0x00000030);
  __vop100 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[3]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[259], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[323], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[83], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[4]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[260], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[324], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[84], 0x00000030);
  __vop101 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop102 = _mm256_mul_ps(__vop100, __vop101);
  __mv_aux0 = _mm_load_ss(&A[380]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[475], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[570], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[575], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&A[578]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[583], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[588], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[593], 0x00000030);
  __vop341 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[375]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[375], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[375], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[320], 0x00000070);
  __mv_aux4 = _mm_loadu_ps(&x[82]);
  __vop342 = _mm256_set_m128(__mv_aux4, __mv_aux3);
  __vop343 = _mm256_mul_ps(__vop341, __vop342);
  __mv_aux0 = _mm_load_ss(&y[239]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, y[255], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, y[271], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, y[272], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&y[273]);
  __vop370 = _mm256_set_m128(__mv_aux4, __mv_aux3);
  __vop370 = _mm256_add_ps(__vop370, __vop343);
  _mm_storeh_pi(&y[271], _mm256_castps256_ps128(__vop370));
  y[239] = _mm256_castps256_ps128(__vop370)[0];
  y[255] = _mm256_castps256_ps128(__vop370)[1];
  __tmp_extract_128 = _mm256_extractf128_ps(__vop370, 0x1);
  _mm_storeu_ps(&y[273], __tmp_extract_128);

  __mv_aux0 = _mm_loadu_ps(&A[596]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[599], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[600], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[597], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[601]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[604], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[605], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[602], 0x00000030);
  __vop103 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[5]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[261], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[325], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[85], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[6]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[262], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[326], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[86], 0x00000030);
  __vop104 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop105 = _mm256_mul_ps(__vop103, __vop104);

  __mv_aux0 = _mm_load_ss(&A[606]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[609], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[610], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[607], 0x000000f0);
  __mv_aux4 = _mm_loadu_ps(&A[611]);
  __vop106 = _mm256_set_m128(__mv_aux4, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[7]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[263], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[327], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[87], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[8]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[88], 0x00000018);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[89], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[264], 0x00000030);
  __vop107 = _mm256_set_m128(__mv_aux8, __mv_aux3);

  __tmp0_256 = _mm256_permute_ps(__vop108, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop106, __vop107, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[279] = y[279] + __mv_lo256[0];
  y[280] = y[280] + __mv_lo256[4];

  __mv_aux0 = _mm_loadu_ps(&A[616]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[619], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[620], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[617], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[621]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[624], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[625], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[622], 0x00000030);
  __vop110 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[9]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[265], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[329], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[89], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[10]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[266], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[330], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[90], 0x00000030);
  __vop111 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop112 = _mm256_mul_ps(__vop110, __vop111);

  __mv_aux0 = _mm_loadu_ps(&A[626]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[629], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[630], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[627], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[631]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[634], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[635], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[632], 0x00000030);
  __vop113 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[11]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[267], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[331], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[91], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[12]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[268], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[332], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[92], 0x00000030);
  __vop114 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop115 = _mm256_mul_ps(__vop113, __vop114);
  __vop371 = _mm256_loadu_ps(&y[277]);
  __vop371 = _mm256_add_ps(__vop371, __vop346);
  _mm256_storeu_ps(&y[277], __vop371);

  __mv_aux0 = _mm_loadu_ps(&A[636]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[639], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[640], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[637], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[641]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[644], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[645], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[642], 0x00000030);
  __vop116 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[13]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[269], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[333], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[93], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[14]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[270], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[334], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[94], 0x00000030);
  __vop117 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop118 = _mm256_mul_ps(__vop116, __vop117);

  __vop119 = _mm256_loadu_ps(&A[646]);
  __mv_aux0 = _mm_load_ss(&x[15]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[95], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[271], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[335], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[0]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[96], 0x00000018);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[97], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[256], 0x00000030);
  __vop120 = _mm256_set_m128(__mv_aux8, __mv_aux3);

  __tmp0_256 = _mm256_permute_ps(__vop121, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop119, __vop120, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[287] = y[287] + __mv_lo256[0];
  y[288] = y[288] + __mv_lo256[4];

  __mv_aux0 = _mm_load_ss(&A[656]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[659 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[662]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[659 + (12)], 0x0000000e);
  __vop123 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[1]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[257], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[321], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[346], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[2]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[258], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[322], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[347], 0x00000030);
  __vop124 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop125 = _mm256_mul_ps(__vop123, __vop124);
  __mv_aux0 = _mm_load_ss(&A[668]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[671 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[674]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[671 + (12)], 0x0000000e);
  __vop126 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[3]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[259], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[323], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[348], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[4]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[260], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[324], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[349], 0x00000030);
  __vop127 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop128 = _mm256_mul_ps(__vop126, __vop127);
  __mv_aux0 = _mm_load_ss(&A[680]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[683 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[686]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[683 + (12)], 0x0000000e);
  __vop129 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[5]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[261], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[325], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[350], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[6]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[262], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[326], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[351], 0x00000030);
  __vop130 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop131 = _mm256_mul_ps(__vop129, __vop130);

  __mv_aux0 = _mm_loadu_ps(&A[561]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[654], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[655], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[657]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[663], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[664], 0x00000030);
  __vop296 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_loadu_ps(&x[78]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[320], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[345], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[97]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[98], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[99], 0x00000070);
  __vop297 = _mm256_set_m128(__mv_aux5, __mv_aux2);

  __tmp0_256 = _mm256_permute_ps(__vop298, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop296, __vop297, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[270] = y[270] + __mv_lo256[0];
  y[288] = y[288] + __mv_lo256[2];
  y[289] = y[289] + __mv_lo256[4];
  y[290] = y[290] + __mv_lo256[6];

  __mv_aux0 = _mm_loadu_ps(&A[669]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[675], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[676], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[681]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[687], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[688], 0x00000030);
  __vop300 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_loadu_ps(&x[99]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[100], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[101], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[101]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[102], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[103], 0x00000030);
  __vop301 = _mm256_set_m128(__mv_aux5, __mv_aux2);

  __tmp0_256 = _mm256_permute_ps(__vop302, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop300, __vop301, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[291] = y[291] + __mv_lo256[0];
  y[292] = y[292] + __mv_lo256[2];
  y[293] = y[293] + __mv_lo256[4];
  y[294] = y[294] + __mv_lo256[6];

  __mv_aux0 = _mm_load_ss(&A[692]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[695 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_loadu_ps(&A[698]);
  __vop132 = _mm256_set_m128(__mv_aux2, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[7]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[263], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[327], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[352], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[8]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[104], 0x00000018);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[105], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[264], 0x00000030);
  __vop133 = _mm256_set_m128(__mv_aux8, __mv_aux3);

  __tmp0_256 = _mm256_permute_ps(__vop134, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop132, __vop133, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[295] = y[295] + __mv_lo256[0];
  y[296] = y[296] + __mv_lo256[4];

  __mv_aux0 = _mm_load_ss(&A[704]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[707 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[710]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[707 + (12)], 0x0000000e);
  __vop136 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[9]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[265], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[329], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[354], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[10]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[266], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[330], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[355], 0x00000030);
  __vop137 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop138 = _mm256_mul_ps(__vop136, __vop137);
  __mv_aux0 = _mm_load_ss(&A[716]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[719 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[722]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[719 + (12)], 0x0000000e);
  __vop139 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[11]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[267], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[331], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[356], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[12]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[268], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[332], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[357], 0x00000030);
  __vop140 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop141 = _mm256_mul_ps(__vop139, __vop140);
  __mv_aux0 = _mm_load_ss(&A[728]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[731 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[734]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[731 + (12)], 0x0000000e);
  __vop142 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[13]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[269], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[333], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[358], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[14]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[270], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[334], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[359], 0x00000030);
  __vop143 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop144 = _mm256_mul_ps(__vop142, __vop143);
  y[302] = (y[302] + (A[739] * x[359]));

  __mv_aux0 = _mm_loadu_ps(&A[693]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[702], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[703], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[705]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[711], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[712], 0x00000030);
  __vop304 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_loadu_ps(&x[103]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[328], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[353], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[105]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[106], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[107], 0x00000070);
  __vop305 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_load_ss(&A[598]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[603], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[608], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[615], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&A[618]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[623], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[628], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[633], 0x00000030);
  __vop344 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop345 = _mm256_set_ps(__vop117[3], __vop114[7], __vop114[3], __vop111[7],
                           __vop305[2], __vop107[5], __vop107[3], __vop104[7]);
  __vop346 = _mm256_mul_ps(__vop344, __vop345);

  __tmp0_256 = _mm256_permute_ps(__vop306, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop304, __vop305, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[295] = y[295] + __mv_lo256[0];
  y[296] = y[296] + __mv_lo256[2];
  y[297] = y[297] + __mv_lo256[4];
  y[298] = y[298] + __mv_lo256[6];

  __mv_aux0 = _mm_loadu_ps(&A[717]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[723], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[724], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[729]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[735], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[736], 0x00000030);
  __vop308 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_loadu_ps(&x[107]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[108], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[109], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[109]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[110], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[111], 0x00000030);
  __vop309 = _mm256_set_m128(__mv_aux5, __mv_aux2);

  __tmp0_256 = _mm256_permute_ps(__vop310, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop308, __vop309, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[299] = y[299] + __mv_lo256[0];
  y[300] = y[300] + __mv_lo256[2];
  y[301] = y[301] + __mv_lo256[4];
  y[302] = y[302] + __mv_lo256[6];

  __vop145 = _mm256_loadu_ps(&A[740]);
  __mv_aux0 = _mm_load_ss(&x[15]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[111], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[271], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[335], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[0]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[112], 0x00000018);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[113], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[256], 0x00000030);
  __vop146 = _mm256_set_m128(__mv_aux8, __mv_aux3);

  __tmp0_256 = _mm256_permute_ps(__vop147, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop145, __vop146, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[303] = y[303] + __mv_lo256[0];
  y[304] = y[304] + __mv_lo256[4];

  __mv_aux0 = _mm_loadu_ps(&A[749]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[752], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[753], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[750], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[754]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[757], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[758], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[755], 0x00000030);
  __vop149 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[1]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[257], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[321], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[113], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[2]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[258], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[322], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[114], 0x00000030);
  __vop150 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop151 = _mm256_mul_ps(__vop149, __vop150);

  __mv_aux0 = _mm_loadu_ps(&A[759]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[762], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[763], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[760], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[764]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[767], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[768], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[765], 0x00000030);
  __vop152 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[3]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[259], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[323], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[115], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[4]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[260], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[324], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[116], 0x00000030);
  __vop153 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop154 = _mm256_mul_ps(__vop152, __vop153);

  __mv_aux0 = _mm_loadu_ps(&y[285]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, y[304], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, y[305], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&y[306]);
  __vop372 = _mm256_set_m128(__mv_aux3, __mv_aux2);
  __vop372 = _mm256_add_ps(__vop372, __vop349);
  _mm_storel_pi(&y[285], _mm256_castps256_ps128(__vop372));
  _mm_storeh_pi(&y[304], _mm256_castps256_ps128(__vop372));
  __tmp_extract_128 = _mm256_extractf128_ps(__vop372, 0x1);
  _mm_storeu_ps(&y[306], __tmp_extract_128);

  __mv_aux0 = _mm_loadu_ps(&A[769]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[772], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[773], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[770], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[774]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[777], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[778], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[775], 0x00000030);
  __vop155 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[5]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[261], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[325], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[117], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[6]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[262], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[326], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[118], 0x00000030);
  __vop156 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop157 = _mm256_mul_ps(__vop155, __vop156);
  __mv_aux0 = _mm_load_ss(&A[638]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[643], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[748], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[751], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&A[756]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[761], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[766], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[771], 0x00000030);
  __vop347 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop348 = _mm256_set_ps(__vop156[7], __vop156[3], __vop153[7], __vop153[3],
                           __vop150[7], __vop342[3], __vop120[1], __vop117[7]);
  __vop349 = _mm256_mul_ps(__vop347, __vop348);

  __mv_aux0 = _mm_load_ss(&A[779]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[782], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[783], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[780], 0x000000f0);
  __mv_aux4 = _mm_loadu_ps(&A[784]);
  __vop158 = _mm256_set_m128(__mv_aux4, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[7]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[263], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[327], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[119], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[8]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[120], 0x00000018);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[121], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[264], 0x00000030);
  __vop159 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop403 = _mm256_loadu_ps(&A[64]);
  __vop404 = _mm256_set_ps(__vop159[2], __vop156[6], __vop156[2], __vop153[6],
                           __vop153[2], __vop150[6], __vop150[2], __vop342[3]);
  __vop405 = _mm256_mul_ps(__vop403, __vop404);

  __tmp0_256 = _mm256_permute_ps(__vop160, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop158, __vop159, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[311] = y[311] + __mv_lo256[0];
  y[312] = y[312] + __mv_lo256[4];

  __mv_aux0 = _mm_loadu_ps(&A[789]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[792], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[793], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[790], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[794]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[797], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[798], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[795], 0x00000030);
  __vop162 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[9]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[265], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[329], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[121], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[10]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[266], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[330], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[122], 0x00000030);
  __vop163 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop164 = _mm256_mul_ps(__vop162, __vop163);

  __mv_aux0 = _mm_loadu_ps(&A[799]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[802], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[803], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[800], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[804]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[807], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[808], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[805], 0x00000030);
  __vop165 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[11]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[267], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[331], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[123], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[12]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[268], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[332], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[124], 0x00000030);
  __vop166 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop167 = _mm256_mul_ps(__vop165, __vop166);

  __vop373 = _mm256_loadu_ps(&y[310]);
  __vop373 = _mm256_add_ps(__vop373, __vop352);
  _mm256_storeu_ps(&y[310], __vop373);

  __mv_aux0 = _mm_loadu_ps(&A[809]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[812], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[813], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[810], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[814]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[817], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[818], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[815], 0x00000030);
  __vop168 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[13]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[269], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[333], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[125], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[14]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[270], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[334], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[126], 0x00000030);
  __vop169 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop170 = _mm256_mul_ps(__vop168, __vop169);
  __mv_aux0 = _mm_load_ss(&A[776]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[781], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[788], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[791], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&A[796]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[801], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[806], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[811], 0x00000030);
  __vop350 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop351 = _mm256_set_ps(__vop169[7], __vop169[3], __vop166[7], __vop166[3],
                           __vop163[7], __vop305[2], __vop159[5], __vop159[3]);
  __vop352 = _mm256_mul_ps(__vop350, __vop351);

  __vop171 = _mm256_loadu_ps(&A[819]);
  __mv_aux0 = _mm_load_ss(&x[15]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[127], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[271], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[335], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[0]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[128], 0x00000018);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[129], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[256], 0x00000030);
  __vop172 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop406 = _mm256_loadu_ps(&A[72]);
  __vop407 = _mm256_set_ps(__vop172[3], __vop169[6], __vop169[2], __vop166[6],
                           __vop166[2], __vop163[6], __vop163[2], __vop305[2]);
  __vop408 = _mm256_mul_ps(__vop406, __vop407);

  __tmp0_256 = _mm256_permute_ps(__vop173, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop171, __vop172, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[319] = y[319] + __mv_lo256[0];
  y[320] = y[320] + __mv_lo256[4];

  __mv_aux0 = _mm_loadu_ps(&A[828]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[831], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[832], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[829], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[833]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[836], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[837], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[834], 0x00000030);
  __vop175 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[1]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[257], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[346], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[129], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[2]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[258], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[347], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[130], 0x00000030);
  __vop176 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop177 = _mm256_mul_ps(__vop175, __vop176);

  __mv_aux0 = _mm_loadu_ps(&A[838]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[841], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[842], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[839], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[843]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[846], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[847], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[844], 0x00000030);
  __vop178 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[3]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[259], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[348], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[131], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[4]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[260], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[349], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[132], 0x00000030);
  __vop179 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop180 = _mm256_mul_ps(__vop178, __vop179);

  __mv_aux0 = _mm_loadu_ps(&A[848]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[851], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[852], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[849], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[853]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[856], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[857], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[854], 0x00000030);
  __vop181 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[5]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[261], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[350], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[133], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[6]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[262], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[351], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[134], 0x00000030);
  __vop182 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop183 = _mm256_mul_ps(__vop181, __vop182);
  __mv_aux0 = _mm_load_ss(&y[318]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, y[320 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_loadu_ps(&y[323]);
  __vop374 = _mm256_set_m128(__mv_aux2, __mv_aux1);
  __vop374 = _mm256_add_ps(__vop374, __vop355);
  __tmp_extract_128 = _mm256_extractf128_ps(__vop374, 0x1);
  _mm_storeu_ps(&y[323], __tmp_extract_128);
  y[318] = __vop374[0];
  y[320] = __vop374[1];
  y[321] = __vop374[2];
  y[322] = __vop374[3];

  __mv_aux0 = _mm_load_ss(&A[858]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[861], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[862], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[859], 0x000000f0);
  __mv_aux4 = _mm_loadu_ps(&A[863]);
  __vop184 = _mm256_set_m128(__mv_aux4, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[7]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[263], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[352], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[135], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[8]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[136], 0x00000018);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[137], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[264], 0x00000030);
  __vop185 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&A[816]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[827], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[830], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[835], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&A[840]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[845], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[850], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[855], 0x00000030);
  __vop353 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop354 = _mm256_set_ps(__vop185[3], __vop182[7], __vop182[3], __vop179[7],
                           __vop179[3], __vop176[7], __vop297[3], __vop172[1]);
  __vop355 = _mm256_mul_ps(__vop353, __vop354);

  __tmp0_256 = _mm256_permute_ps(__vop186, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop184, __vop185, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[327] = y[327] + __mv_lo256[0];
  y[328] = y[328] + __mv_lo256[4];

  __mv_aux0 = _mm_loadu_ps(&A[868]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[871], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[872], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[869], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[873]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[876], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[877], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[874], 0x00000030);
  __vop188 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[9]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[265], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[354], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[137], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[10]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[266], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[355], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[138], 0x00000030);
  __vop189 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop190 = _mm256_mul_ps(__vop188, __vop189);

  __mv_aux0 = _mm_loadu_ps(&A[878]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[881], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[882], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[879], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[883]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[886], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[887], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[884], 0x00000030);
  __vop191 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[11]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[267], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[356], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[139], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[12]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[268], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[357], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[140], 0x00000030);
  __vop192 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop193 = _mm256_mul_ps(__vop191, __vop192);

  __mv_aux0 = _mm_loadu_ps(&A[888]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[891], 0x00000094);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[892], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[889], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[893]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[896], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[897], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[894], 0x00000030);
  __vop194 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[13]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[269], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[358], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[141], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[14]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[270], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[359], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[142], 0x00000030);
  __vop195 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop196 = _mm256_mul_ps(__vop194, __vop195);
  __vop375 = _mm256_loadu_ps(&y[327]);
  __vop375 = _mm256_add_ps(__vop375, __vop358);
  _mm256_storeu_ps(&y[327], __vop375);

  __mv_aux0 = _mm_loadu_ps(&A[901]);
  __mv_aux1 = _mm_loadu_ps(&A[905]);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[908], 0x00000010);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[906], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[907], 0x00000030);
  __vop197 = _mm256_set_m128(__mv_aux4, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[0]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[144], 0x00000014);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[145], 0x00000068);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[256], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&x[1]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[257], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[145], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[146], 0x00000030);
  __vop198 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop199 = _mm256_mul_ps(__vop197, __vop198);
  y[337] = (y[337] + (A[907] * x[146]));

  __mv_aux0 = _mm_load_ss(&A[909]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[912], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[910], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[911], 0x00000030);
  __mv_aux4 = _mm_load_ss(&A[913]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[916], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[914], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[915], 0x000000b0);
  __vop200 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[2]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[258], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[146], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[147], 0x00000030);
  __mv_aux4 = _mm_load_ss(&x[3]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[259], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[147], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[148], 0x000000b0);
  __vop201 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop202 = _mm256_mul_ps(__vop200, __vop201);

  __mv_aux0 = _mm_load_ss(&A[917]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[920], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[918], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[919], 0x00000030);
  __mv_aux4 = _mm_load_ss(&A[921]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[924], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[922], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[923], 0x000000b0);
  __vop203 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[4]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[260], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[148], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[149], 0x00000030);
  __mv_aux4 = _mm_load_ss(&x[5]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[261], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[149], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[150], 0x000000b0);
  __vop204 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop205 = _mm256_mul_ps(__vop203, __vop204);

  __mv_aux0 = _mm_load_ss(&A[925]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[928], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[926], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[927], 0x00000030);
  __mv_aux4 = _mm_load_ss(&A[929]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[932], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[930], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[931], 0x000000b0);
  __vop206 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[6]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[262], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[150], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[151], 0x00000030);
  __mv_aux4 = _mm_load_ss(&x[7]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[263], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[151], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[152], 0x000000b0);
  __vop207 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop208 = _mm256_mul_ps(__vop206, __vop207);
  __vop379 = _mm256_loadu_ps(&A[0]);
  __vop380 = _mm256_set_ps(__vop207[5], __vop207[1], __vop204[5], __vop204[1],
                           __vop201[5], __vop201[1], __vop198[5], __vop198[3]);
  __vop381 = _mm256_mul_ps(__vop379, __vop380);
  __mv_aux0 = _mm_load_ss(&A[128]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[159 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_loadu_ps(&A[162]);
  __vop427 = _mm256_set_m128(__mv_aux2, __mv_aux1);
  __vop428 = _mm256_set_ps(__vop207[1], __vop204[5], __vop204[1], __vop201[5],
                           __vop201[1], __vop198[5], __vop198[3], __vop254[6]);
  __vop429 = _mm256_mul_ps(__vop427, __vop428);

  __mv_aux0 = _mm_loadu_ps(&A[933]);
  __mv_aux1 = _mm_loadu_ps(&A[937]);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[940], 0x00000010);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[938], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[939], 0x00000030);
  __vop209 = _mm256_set_m128(__mv_aux4, __mv_aux0);
  __mv_aux0 = _mm_load_ss(&x[8]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[152], 0x00000014);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[153], 0x00000068);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[264], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&x[9]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[265], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[153], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[154], 0x00000030);
  __vop210 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop211 = _mm256_mul_ps(__vop209, __vop210);
  y[345] = (y[345] + (A[939] * x[154]));

  __mv_aux0 = _mm_load_ss(&A[941]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[944], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[942], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[943], 0x00000030);
  __mv_aux4 = _mm_load_ss(&A[945]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[948], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[946], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[947], 0x000000b0);
  __vop212 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[10]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[266], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[154], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[155], 0x00000030);
  __mv_aux4 = _mm_load_ss(&x[11]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[267], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[155], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[156], 0x000000b0);
  __vop213 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop214 = _mm256_mul_ps(__vop212, __vop213);

  __mv_aux0 = _mm_load_ss(&A[949]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[952], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[950], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[951], 0x00000030);
  __mv_aux4 = _mm_load_ss(&A[953]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[956], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[954], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[955], 0x000000b0);
  __vop215 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[12]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[268], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[156], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[157], 0x00000030);
  __mv_aux4 = _mm_load_ss(&x[13]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[269], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[157], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[158], 0x000000b0);
  __vop216 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop217 = _mm256_mul_ps(__vop215, __vop216);

  __mv_aux0 = _mm_loadu_ps(&A[898]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[961], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[962], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[964]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[967], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[968], 0x00000030);
  __vop312 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_load_ss(&x[15]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[143], 0x00000018);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[15], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[159], 0x000000f0);
  __mv_aux4 = _mm_load_ss(&x[160]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[272], 0x0000009c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[161], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[273], 0x00000070);
  __vop313 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop314 = _mm256_mul_ps(__vop312, __vop313);
  __mv_aux0 = _mm_loadu_ps(&A[970]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[973], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[974], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[976]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[979], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[980], 0x00000030);
  __vop315 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_load_ss(&x[162]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[274], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[163], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[275], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[164]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[276], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[165], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[277], 0x00000030);
  __vop316 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop317 = _mm256_mul_ps(__vop315, __vop316);
  __mv_aux0 = _mm_load_ss(&A[860]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[867], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[870], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[875], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&A[880]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[885], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[890], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[895], 0x00000030);
  __vop356 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop357 = _mm256_set_ps(__vop313[1], __vop195[7], __vop195[3], __vop192[7],
                           __vop192[3], __vop189[7], __vop305[3], __vop185[5]);
  __vop358 = _mm256_mul_ps(__vop356, __vop357);
  y[359] = (y[359] + (A[986] * x[279]));

  __mv_aux0 = _mm_load_ss(&A[900]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[963], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[966], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[969], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&A[972]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[975], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[978], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[981], 0x00000030);
  __vop359 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop360 = _mm256_set_ps(__vop32[2], __vop29[6], __vop29[2], __vop26[6],
                           __vop26[2], __vop22[6], __vop172[2], __vop172[2]);
  __vop361 = _mm256_mul_ps(__vop359, __vop360);
  __vop376 = _mm256_set_ps(y[335], y[351], y[352], y[353], y[354], y[355],
                           y[356], y[357]);
  __vop376 = _mm256_add_ps(__vop376, __vop361);
  __tmp_extract_128 = _mm256_extractf128_ps(__vop376, 0x1);
  _mm_storeu_ps(&y[354], __tmp_extract_128);
  y[335] = __vop376[0];
  y[351] = __vop376[1];
  y[352] = __vop376[2];
  y[353] = __vop376[3];

  __mv_aux0 = _mm_loadu_ps(&A[982]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[985], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[986], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[988]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[991], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[992], 0x00000030);
  __vop318 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_load_ss(&x[166]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[278], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[167], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[279], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[168]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[280], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[169], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[281], 0x00000030);
  __vop319 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop320 = _mm256_mul_ps(__vop318, __vop319);
  __mv_aux0 = _mm_loadu_ps(&A[994]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[997], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[998], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[1000]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[1003], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[1004], 0x00000030);
  __vop321 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_load_ss(&x[170]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[282], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[171], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[283], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[172]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[284], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[173], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[285], 0x00000030);
  __vop322 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop323 = _mm256_mul_ps(__vop321, __vop322);
  __vop385 = _mm256_loadu_ps(&A[16]);
  __vop386 = _mm256_set_ps(__vop319[3], __vop319[1], __vop316[7], __vop316[5],
                           __vop316[3], __vop316[1], __vop313[7], __vop313[5]);
  __vop387 = _mm256_mul_ps(__vop385, __vop386);
  __vop433 = _mm256_loadu_ps(&A[174]);
  __vop434 = _mm256_set_ps(__vop319[1], __vop316[7], __vop316[5], __vop316[3],
                           __vop316[1], __vop313[7], __vop313[5], __vop172[2]);
  __vop435 = _mm256_mul_ps(__vop433, __vop434);
  y[367] = (y[367] + (A[1010] * x[287]));

  __mv_aux0 = _mm_load_ss(&A[984]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[987], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[990], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[993], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&A[996]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[999], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[1002], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[1005], 0x00000030);
  __vop362 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop363 = _mm256_set_ps(__vop45[2], __vop42[6], __vop42[2], __vop39[6],
                           __vop39[2], __vop35[6], __vop35[2], __vop32[6]);
  __vop364 = _mm256_mul_ps(__vop362, __vop363);
  _mv_blend_mem_ps(__mv_aux0, _mm_set_epi32(0, 0, 0, 0), A[1008], 0x0000000f);
  _mv_insert_mem_ps(__vop365, __mv_aux0, A[1011], 0x0000001c);
  __vop366 = _mm_set_ps(0, 0, __vop48[2], __vop45[6]);
  __vop367 = _mm_mul_ps(__vop365, __vop366);
  __vop377 = _mm256_set_ps(y[358], y[359], y[360], y[361], y[362], y[363],
                           y[364], y[365]);
  __vop377 = _mm256_add_ps(__vop377, __vop364);
  _mm256_storeu_ps(&y[358], __vop377);
  __vop378 = _mm_set_ps(y[366], y[367], 0, 0);
  __vop378 = _mm_add_ps(__vop378, __vop367);
  _mm_storel_pi(&y[366], __vop378);

  __mv_aux0 = _mm_load_ss(&A[957]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[960], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[958], 0x00000068);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[959], 0x00000030);
  __mv_aux4 = _mm_loadu_ps(&A[1012]);
  __vop218 = _mm256_set_m128(__mv_aux4, __mv_aux3);
  __mv_aux0 = _mm_load_ss(&x[14]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[270], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[158], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[159], 0x00000030);
  __mv_aux4 = _mm_load_ss(&x[0]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[176], 0x000000d4);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[177], 0x00000020);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[272], 0x00000030);
  __vop219 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop382 = _mm256_loadu_ps(&A[8]);
  __vop383 = _mm256_set_ps(__vop172[2], __vop219[1], __vop216[5], __vop216[1],
                           __vop213[5], __vop213[1], __vop210[5], __vop210[3]);
  __vop384 = _mm256_mul_ps(__vop382, __vop383);
  __vop430 = _mm256_loadu_ps(&A[166]);
  __vop431 = _mm256_set_ps(__vop219[1], __vop216[5], __vop216[1], __vop213[5],
                           __vop213[1], __vop210[5], __vop210[3], __vop207[5]);
  __vop432 = _mm256_mul_ps(__vop430, __vop431);

  __tmp0_256 = _mm256_permute_ps(__vop220, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop218, __vop219, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[350] = y[350] + __mv_lo256[0];
  y[368] = y[368] + __mv_lo256[4];

  __mv_aux0 = _mm_load_ss(&A[1018]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[1021 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[1024]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[1021 + (12)], 0x0000000e);
  __vop222 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[1]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[273], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[305], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[361], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[2]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[274], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[306], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[362], 0x00000030);
  __vop223 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop224 = _mm256_mul_ps(__vop222, __vop223);
  __mv_aux0 = _mm_load_ss(&A[1030]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[1033 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[1036]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[1033 + (12)], 0x0000000e);
  __vop225 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[3]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[275], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[307], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[363], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[4]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[276], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[308], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[364], 0x00000030);
  __vop226 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop227 = _mm256_mul_ps(__vop225, __vop226);
  __mv_aux0 = _mm_load_ss(&A[1042]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[1045 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[1048]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[1045 + (12)], 0x0000000e);
  __vop228 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[5]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[277], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[309], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[365], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[6]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[278], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[310], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[366], 0x00000030);
  __vop229 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop230 = _mm256_mul_ps(__vop228, __vop229);

  __mv_aux0 = _mm_loadu_ps(&A[1006]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[1009], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[1010], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[1016]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[1019], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[1020], 0x00000030);
  __vop324 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_load_ss(&x[174]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[286], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[175], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[287], 0x00000070);
  __mv_aux4 = _mm_load_ss(&x[304]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[360], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[177], 0x00000028);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[178], 0x00000030);
  __vop325 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop326 = _mm256_mul_ps(__vop324, __vop325);
  __mv_aux0 = _mm_load_ss(&A[300]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[305], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[310], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[315], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&A[320]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[325], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[330], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[335], 0x00000030);
  __vop335 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop336 = _mm256_set_ps(__vop229[7], __vop229[3], __vop226[7], __vop226[3],
                           __vop223[7], __vop223[3], __vop325[5], __vop172[2]);
  __vop337 = _mm256_mul_ps(__vop335, __vop336);
  __vop388 = _mm256_loadu_ps(&A[24]);
  __vop389 = _mm256_set_ps(__vop325[3], __vop325[1], __vop322[7], __vop322[5],
                           __vop322[3], __vop322[1], __vop319[7], __vop319[5]);
  __vop390 = _mm256_mul_ps(__vop388, __vop389);
  __vop436 = _mm256_loadu_ps(&A[182]);
  __vop437 = _mm256_set_ps(__vop325[1], __vop322[7], __vop322[5], __vop322[3],
                           __vop322[1], __vop319[7], __vop319[5], __vop319[3]);
  __vop438 = _mm256_mul_ps(__vop436, __vop437);
  __mv_aux0 = _mm_loadu_ps(&A[190]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[222], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[223], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[224]);
  __vop439 = _mm256_set_m128(__mv_aux3, __mv_aux2);
  __vop440 = _mm256_set_ps(__vop229[0], __vop226[4], __vop226[0], __vop223[4],
                           __vop223[0], __vop219[4], __vop254[6], __vop325[3]);
  __vop441 = _mm256_mul_ps(__vop439, __vop440);

  __mv_aux0 = _mm_loadu_ps(&A[1025]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[1031], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[1032], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[1037]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[1043], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[1044], 0x00000030);
  __vop327 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_loadu_ps(&x[178]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[179], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[180], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[180]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[181], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[182], 0x00000030);
  __vop328 = _mm256_set_m128(__mv_aux5, __mv_aux2);

  __tmp0_256 = _mm256_permute_ps(__vop329, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop327, __vop328, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[370] = y[370] + __mv_lo256[0];
  y[371] = y[371] + __mv_lo256[2];
  y[372] = y[372] + __mv_lo256[4];
  y[373] = y[373] + __mv_lo256[6];

  __mv_aux0 = _mm_loadu_ps(&y[206]);
  _mv_insert_mem_ps(__vop470, __mv_aux0, y[383], 0x000000a8);
  __vop471 = _mm_set_ps(r107i06, r107i07, r551i07, 0);
  __vop472 = _mm_add_ps(__vop470, __vop471);

  y[206] = y[206] + __vop471[0];
  y[207] = y[207] + __vop471[1];
  y[383] = y[383] + __vop471[2];

  __mv_aux0 = _mm_load_ss(&A[1054]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[1057 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_loadu_ps(&A[1060]);
  __vop231 = _mm256_set_m128(__mv_aux2, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[7]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[279], 0x0000005c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[311], 0x00000028);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[367], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[8]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[184], 0x00000018);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[185], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[280], 0x00000030);
  __vop232 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop397 = _mm256_loadu_ps(&A[48]);
  __vop398 = _mm256_set_ps(__vop232[2], __vop229[6], __vop229[2], __vop226[6],
                           __vop226[2], __vop223[6], __vop223[2], __vop325[4]);
  __vop399 = _mm256_mul_ps(__vop397, __vop398);
  __vop412 = _mm256_loadu_ps(&A[88]);
  __vop413 = _mm256_set_ps(__vop232[3], __vop229[7], __vop229[3], __vop226[7],
                           __vop226[3], __vop223[7], __vop223[3], __vop325[5]);
  __vop414 = _mm256_mul_ps(__vop412, __vop413);

  __tmp0_256 = _mm256_permute_ps(__vop233, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop231, __vop232, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[375] = y[375] + __mv_lo256[0];
  y[376] = y[376] + __mv_lo256[4];

  __mv_aux0 = _mm_load_ss(&A[1066]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[1069 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[1072]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[1069 + (12)], 0x0000000e);
  __vop235 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[9]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[281], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[313], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[369], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[10]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[282], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[314], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[370], 0x00000030);
  __vop236 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop237 = _mm256_mul_ps(__vop235, __vop236);
  __mv_aux0 = _mm_load_ss(&A[1078]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[1081 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[1084]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[1081 + (12)], 0x0000000e);
  __vop238 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[11]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[283], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[315], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[371], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[12]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[284], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[316], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[372], 0x00000030);
  __vop239 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop240 = _mm256_mul_ps(__vop238, __vop239);
  __mv_aux0 = _mm_load_ss(&A[1090]);
  _mv_blend_mem_ps(__mv_aux1, __mv_aux0, A[1093 + (-1)], 0x0000000e);
  __mv_aux2 = _mm_load_ss(&A[1096]);
  _mv_blend_mem_ps(__mv_aux3, __mv_aux2, A[1093 + (12)], 0x0000000e);
  __vop241 = _mm256_set_m128(__mv_aux3, __mv_aux1);
  __mv_aux0 = _mm_load_ss(&x[13]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[285], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[317], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, x[373], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&x[14]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[286], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[318], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, x[374], 0x00000030);
  __vop242 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop243 = _mm256_mul_ps(__vop241, __vop242);
  __vop442 = _mm256_loadu_ps(&A[228]);
  __vop443 = _mm256_set_ps(__vop242[0], __vop239[4], __vop239[0], __vop236[4],
                           __vop236[0], __vop232[4], __vop232[0], __vop229[4]);
  __vop444 = _mm256_mul_ps(__vop442, __vop443);
  __mv_aux0 = _mm_loadu_ps(&A[236]);
  _mv_insert_mem_ps(__vop445, __mv_aux0, A[1102], 0x000000a8);
  __vop446 = _mm_set_ps(0, __vop313[2], __vop313[2], __vop242[4]);
  __vop447 = _mm_mul_ps(__vop445, __vop446);
  y[382] = (y[382] + (A[1101] * x[374]));

  __mv_aux0 = _mm_loadu_ps(&A[1049]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[1055], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[1056], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&A[1064]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, A[1067], 0x00000028);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[1068], 0x00000030);
  __vop331 = _mm256_set_m128(__mv_aux5, __mv_aux2);
  __mv_aux0 = _mm_loadu_ps(&x[182]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, x[183], 0x00000028);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, x[184], 0x00000030);
  __mv_aux3 = _mm_loadu_ps(&x[312]);
  _mv_insert_mem_ps(__mv_aux4, __mv_aux3, x[368], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, x[185], 0x000000e8);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, x[186], 0x000000f0);
  __vop332 = _mm256_set_m128(__mv_aux7, __mv_aux2);
  __mv_aux0 = _mm_load_ss(&A[340]);
  _mv_insert_mem_ps(__mv_aux1, __mv_aux0, A[345], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux2, __mv_aux1, A[350], 0x000000a8);
  _mv_insert_mem_ps(__mv_aux3, __mv_aux2, A[355], 0x000000b0);
  __mv_aux4 = _mm_load_ss(&A[360]);
  _mv_insert_mem_ps(__mv_aux5, __mv_aux4, A[365], 0x0000001c);
  _mv_insert_mem_ps(__mv_aux7, __mv_aux5, A[370], 0x00000068);
  _mv_insert_mem_ps(__mv_aux8, __mv_aux7, A[375], 0x00000030);
  __vop338 = _mm256_set_m128(__mv_aux8, __mv_aux3);
  __vop339 = _mm256_set_ps(__vop242[7], __vop242[3], __vop239[7], __vop239[3],
                           __vop236[7], __vop236[3], __vop332[5], __vop232[3]);
  __vop340 = _mm256_mul_ps(__vop338, __vop339);
  __vop400 = _mm256_loadu_ps(&A[56]);
  __vop401 = _mm256_set_ps(__vop94[3], __vop242[6], __vop242[2], __vop239[6],
                           __vop239[2], __vop236[6], __vop236[2], __vop332[4]);
  __vop402 = _mm256_mul_ps(__vop400, __vop401);
  __vop415 = _mm256_loadu_ps(&A[96]);
  __vop416 = _mm256_set_ps(__vop342[2], __vop242[7], __vop242[3], __vop239[7],
                           __vop239[3], __vop236[7], __vop236[3], __vop332[5]);
  __vop417 = _mm256_mul_ps(__vop415, __vop416);

  __tmp0_256 = _mm256_permute_ps(__vop333, 0b00001110);
  __tmp1_256 = _mm256_fmadd_ps(__vop331, __vop332, __tmp0_256);
  __tmp2_256 = _mm256_shuffle_ps(__tmp1_256, __tmp1_256, 0x01);
  __mv_lo256 = _mm256_add_ps(__tmp1_256, __tmp2_256);
  y[374] = y[374] + __mv_lo256[0];
  y[375] = y[375] + __mv_lo256[2];
  y[376] = y[376] + __mv_lo256[4];
  y[377] = y[377] + __mv_lo256[6];

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
