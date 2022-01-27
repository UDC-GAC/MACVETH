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

void kernel_spmv_fragment_0(float *restrict A, float *restrict x,
                            float *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;
#pragma macveth unroll i0 full
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 0] += A[1 * i0 + 0] * x[1 * i0 + 256];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 8] += A[1 * i0 + 8] * x[1 * i0 + 264];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 16] += A[1 * i0 + 16] * x[1 * i0 + 272];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 24] += A[1 * i0 + 24] * x[1 * i0 + 280];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 32] += A[1 * i0 + 32] * x[1 * i0 + 288];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 40] += A[1 * i0 + 40] * x[1 * i0 + 296];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 48] += A[1 * i0 + 48] * x[1 * i0 + 304];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 56] += A[1 * i0 + 56] * x[1 * i0 + 312];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 64] += A[1 * i0 + 64] * x[1 * i0 + 320];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 72] += A[1 * i0 + 72] * x[1 * i0 + 328];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 80] += A[1 * i0 + 80] * x[1 * i0 + 336];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 88] += A[1 * i0 + 88] * x[1 * i0 + 360];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 96] += A[1 * i0 + 96] * x[1 * i0 + 368];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 104] += A[1 * i0 + 104] * x[1 * i0 + 376];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 112] += A[1 * i0 + 112] * x[1 * i0 + 384];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 120] += A[1 * i0 + 120] * x[1 * i0 + 392];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 128] += A[1 * i0 + 128] * x[0 * i0 + 16];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 129] += A[1 * i0 + 130] * x[0 * i0 + 17];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 130] += A[1 * i0 + 132] * x[0 * i0 + 18];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 131] += A[1 * i0 + 134] * x[0 * i0 + 19];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 132] += A[1 * i0 + 136] * x[0 * i0 + 20];
  }
#pragma endmacveth
}