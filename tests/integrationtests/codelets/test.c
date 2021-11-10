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


void kernel_spmv_fragment_0(float * restrict A, float * restrict x, float * restrict y ) {
  register int i0,i1,i2,i3,i4,i5,i6;
#pragma macveth unroll i0 full
//   for( i0 = 0; i0 <= 5; ++i0 ) {
//     y[0*i0+0] += A[1*i0+0] * x[1*i0+0];
//   }
//   y[1] += A[5] * x[85];
//   for( i0 = 0; i0 < 1; ++i0 ) {
//     y[0*i0+0] += A[1*i0+6] * x[1*i0+88];
//   }
  for( i0 = 0; i0 <= 2; ++i0 ) {
    y[0*i0+0] += A[1*i0+0] * x[1*i0+0];
  }
  y[3] += A[5] * x[85];
  y[4] += A[6] * x[850];
  y[5] += A[7] * x[851];
  y[7] += A[8] * x[853];
  y[7] += A[9] * x[853];
//   for( i0 = 0; i0 < 1; ++i0 ) {
//     y[0*i0+0] += A[1*i0+6] * x[1*i0+88];
//   }
//   for( i0 = 0; i0 <= 1; ++i0 ) {
//     y[0*i0+0] += A[1*i0+8] * x[1*i0+92];
//   }
#pragma endmacveth
}