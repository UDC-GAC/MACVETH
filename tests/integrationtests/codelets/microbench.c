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

#define restrict __restrict

void kernel_spmv(float *restrict A, float *restrict x, float *restrict y) {
  //#ifdef COLDCACHE
  //   A = A + offset;
  //   y = y + offset;
  //   x = x + offset;
  //#endif
  register int i0;
#pragma macveth unroll i0 full
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 7685] += A[1 * i0 + 122985] * x[1 * i0 + 7590];
  }
  y[7685] += A[122987] * x[7593];
  y[7685] += A[122988] * x[7604];
  y[7685] += A[122989] * x[7628];
  y[7685] += A[122990] * x[7642];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 7685] += A[1 * i0 + 122991] * x[1 * i0 + 7684];
  }
  for (i0 = 0; i0 <= 3; ++i0) {
    y[0 * i0 + 7686] += A[1 * i0 + 122993] * x[1 * i0 + 7217];
  }
  y[7686] += A[122997] * x[7222];
  y[7686] += A[122998] * x[7224];
  y[7686] += A[122999] * x[7229];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 7686] += A[1 * i0 + 123000] * x[1 * i0 + 7231];
  }
  y[7686] += A[123002] * x[7235];
  y[7686] += A[123003] * x[7249];
  y[7686] += A[123004] * x[7268];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 7686] += A[1 * i0 + 123005] * x[2 * i0 + 7302];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 7686] += A[1 * i0 + 123008] * x[1 * i0 + 7343];
  }
  y[7686] += A[123010] * x[7346];
  y[7686] += A[123011] * x[7349];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 7686] += A[1 * i0 + 123012] * x[1 * i0 + 7355];
  }
  y[7686] += A[123014] * x[7381];
  y[7687] += A[123012] * x[73814];
  y[7688] += A[123014] * x[73812];
#pragma endmacveth
}

// void kernel_spmv_fragment_0(float *restrict A, float *restrict x,
//                             float *restrict y) {
//   register int i0, i1, i2, i3, i4, i5, i6;
//   // #pragma macveth unroll i0 full
//   //   for( i0 = 0; i0 <= 4; ++i0 ) {
//   //     y[0*i0+0] += A[1*i0+0] * x[1*i0+0];
//   //   }
//   //   y[0] += A[5] * x[85];

//   //   for( i0 = 0; i0 <= 1; ++i0 ) {
//   //     y[0*i0+0] += A[1*i0+6] * x[1*i0+88];
//   //   }
//   // // #pragma endmacveth
//   // // #pragma macveth unroll i0 full
//   //   for( i0 = 0; i0 <= 1; ++i0 ) {
//   //     y[0*i0+0] += A[1*i0+8] * x[1*i0+92];
//   //   }

//   //   for( i0 = 0; i0 <= 4; ++i0 ) {
//   //     y[0*i0+1] += A[1*i0+10] * x[1*i0+0];
//   //   }
//   // #pragma endmacveth

// #pragma macveth unroll i0 full

//   // for( i0 = 0; i0 <= 4; ++i0 ) {
//   //   y[0*i0+0] += A[1*i0+0] * x[1*i0+0];
//   // }
//   //   for( i0 = 0; i0 <= 4; ++i0 ) {
//   //   y[1*i0] += A[3*i0+0] * x[2*i0+0];
//   // }
//   y[0] += A[21] * x[23];
//   y[0] += A[213] * x[234];
//   y[1] += A[20] * x[2];
//   y[2] += A[21] * x[3];
//   // y[3] += A[23] * x[5];
//   // y[4] += A[245] * x[6];
//   // y[0] += A[2124] * x[2342];
//   // y[0] += A[21355] * x[2341];
//   // y[5] += A[20] * x[2];
//   // y[6] += A[21] * x[3];
//   // y[7] += A[23] * x[5];
//   // y[8] += A[245] * x[6];

//   // y[0] += A[2131] * x[2344];

//   // y[3] += A[23] * x[54];
//   // y[4] += A[24] * x[233];
//   // y[5] += A[25] * x[234];
//   // y[2] += A[30] * x[3];
//   // y[0] += A[0] * x[0];
//   // y[0] += A[10] * x[1];
//   // y[0] += A[20] * x[13];
//   // y[0] += A[30] * x[14];
//   // y[0] += A[40] * x[15];
//   // y[0] += A[50] * x[18];
//   // y[0] += A[60] * x[23];
//   // y[0] += A[70] * x[1423];

//   // for( i0 = 0; i0 <= 4; ++i0 ) {
//   //   y[0*i0+0] += A[1*i0+0] * x[1*i0+0];
//   // }
//   // y[0] += A[5] * x[85];

//   // for( i0 = 0; i0 <= 1; ++i0 ) {
//   //   y[0*i0+0] += A[1*i0+6] * x[1*i0+88];
//   // }
//   //  for( i0 = 0; i0 <= 3; ++i0 ) {
//   //    y[0*i0+1] += A[1*i0+10] * x[1*i0+0];
//   //  }
//   //  y[0] += A[1203]*x[1234];
//   //  for( i0 = 0; i0 <= 3; ++i0 ) {
//   //    y[0*i0+2] += A[1*i0+20] * x[1*i0+10];
//   //  }
//   // // for( i0 = 0; i0 <= 3; ++i0 ) {
//   // //    y[0*i0+3] += A[1*i0+30] * x[1*i0+20];
//   // //  }
//   //  for( i0 = 0; i0 <= 7; ++i0 ) {
//   //    y[0*i0+2] += A[2*i0+10] * x[3*i0+0];
//   //  }
//   //  for( i0 = 0; i0 <= 7; ++i0 ) {
//   //    y[0*i0+3] += A[2*i0+10] * x[3*i0+0];
//   //  }
//   //  for( i0 = 0; i0 <= 7; ++i0 ) {
//   //    y[0*i0+4] += A[2*i0+10] * x[3*i0+0];
//   //  }
//   //  for( i0 = 0; i0 <= 7; ++i0 ) {
//   //    y[0*i0+5] += A[2*i0+10] * x[3*i0+0];
//   //  }
//   //  for( i0 = 0; i0 <= 7; ++i0 ) {
//   //    y[0*i0+56] += A[2*i0+10] * x[3*i0+0];
//   //  }
//   //  y[1] += A[0] * x[0];
//   //  y[1] += A[0] * x[0];
//   //  y[3] += A[0] * x[0];
//   //  y[1] += A[0] * x[0];
// #pragma endmacveth
// }