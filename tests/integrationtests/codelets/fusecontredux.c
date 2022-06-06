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
  // #pragma macveth unroll i0 full
  //   for (i0 = 0; i0 <= 1; ++i0) {
  //     y[0 * i0 + 7685] += A[1 * i0 + 122985] * x[1 * i0 + 7590];
  //   }
  //   y[7685] += A[122987] * x[7593];
  //   y[7685] += A[122988] * x[7604];
  //   y[7685] += A[122989] * x[7628];
  //   y[7685] += A[122990] * x[7642];
  //   for (i0 = 0; i0 <= 1; ++i0) {
  //     y[0 * i0 + 7685] += A[1 * i0 + 122991] * x[1 * i0 + 7684];
  //   }
  //   for (i0 = 0; i0 <= 1; ++i0) {
  //     y[0 * i0 + 7685] += A[1 * i0 + 122982] * x[1 * i0 + 759565];
  //   }
  //   y[7685] += A[1222987] * x[7593];
  //   y[7685] += A[1229488] * x[76044];
  //   y[7685] += A[122989] * x[7628];
  //   y[7685] += A[1229910] * x[76424];
  //   for (i0 = 0; i0 <= 1; ++i0) {
  //     y[0 * i0 + 7685] += A[1 * i0 + 1322991] * x[1 * i0 + 76844];
  //   }
  //   for (i0 = 0; i0 <= 1; ++i0) {
  //     y[0 * i0 + 7685] += A[1 * i0 + 1229822] * x[1 * i0 + 7595652];
  //   }
  //   y[7685] += A[12287] * x[75931];
  //   y[7685] += A[12288] * x[76044];
  //   y[7685] += A[1229] * x[7628];
  //   y[7685] += A[1229910] * x[76424];
  //   for (i0 = 0; i0 <= 1; ++i0) {
  //     y[0 * i0 + 7685] += A[1 * i0 + 11322991] * x[1 * i0 + 76844];
  //   }
  // #pragma endmacveth

  // #pragma macveth unroll i0 full
  //   for (i0 = 0; i0 <= 1; ++i0) {
  //     y[0 * i0 + 7685] += A[1 * i0 + 122985] * x[1 * i0 + 7590];
  //   }
  //   y[7685] += A[122987] * x[7593];
  //   y[7685] += A[122988] * x[7604];
  //   y[7685] += A[122989] * x[7628];
  //   y[7685] += A[122990] * x[7642];
  //   for (i0 = 0; i0 <= 1; ++i0) {
  //     y[0 * i0 + 7685] += A[1 * i0 + 122991] * x[1 * i0 + 7684];
  //   }
  //   for (i0 = 0; i0 <= 3; ++i0) {
  //     y[0 * i0 + 7686] += A[1 * i0 + 122993] * x[1 * i0 + 7217];
  //   }
  //   y[7686] += A[122997] * x[7222];
  //   y[7686] += A[122998] * x[7224];
  //   y[7686] += A[122999] * x[7229];
  //   for (i0 = 0; i0 <= 1; ++i0) {
  //     y[0 * i0 + 7686] += A[1 * i0 + 123000] * x[1 * i0 + 7231];
  //   }
  //   y[7686] += A[123002] * x[7235];
  //   y[7686] += A[123003] * x[7249];
  //   y[7686] += A[123004] * x[7268];
  //   for (i0 = 0; i0 <= 2; ++i0) {
  //     y[0 * i0 + 7686] += A[1 * i0 + 123005] * x[2 * i0 + 7302];
  //   }
  //   for (i0 = 0; i0 <= 1; ++i0) {
  //     y[0 * i0 + 7686] += A[1 * i0 + 123008] * x[1 * i0 + 7343];
  //   }
  //   y[7686] += A[123010] * x[7346];
  //   y[7686] += A[123011] * x[7349];
  //   for (i0 = 0; i0 <= 1; ++i0) {
  //     y[0 * i0 + 7686] += A[1 * i0 + 123012] * x[1 * i0 + 7355];
  //   }
  //   y[7686] += A[123014] * x[7381];
  // #pragma endmacveth

#pragma macveth unroll i0 full
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 167] += A[i0 + 0] * x[1 * i0 + 576];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 168] += A[i0 + 8] * x[1 * i0 + 577];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 169] += A[i0 + 16] * x[1 * i0 + 578];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 170] += A[i0 + 24] * x[1 * i0 + 579];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 171] += A[i0 + 32] * x[1 * i0 + 580];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 172] += A[i0 + 40] * x[1 * i0 + 581];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 173] += A[i0 + 48] * x[1 * i0 + 582];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 174] += A[i0 + 56] * x[1 * i0 + 583];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 175] += A[i0 + 64] * x[1 * i0 + 584];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 336] += A[i0 + 72] * x[1 * i0 + 672];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 336] += A[i0 + 80] * x[1 * i0 + 680];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 337] += A[i0 + 88] * x[1 * i0 + 688];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 337] += A[i0 + 96] * x[1 * i0 + 696];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 338] += A[i0 + 104] * x[1 * i0 + 704];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 338] += A[i0 + 112] * x[1 * i0 + 712];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 339] += A[i0 + 120] * x[1 * i0 + 720];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 339] += A[i0 + 128] * x[1 * i0 + 728];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 340] += A[i0 + 136] * x[1 * i0 + 736];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 340] += A[i0 + 144] * x[1 * i0 + 744];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 341] += A[i0 + 152] * x[1 * i0 + 752];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 341] += A[i0 + 160] * x[1 * i0 + 760];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 342] += A[i0 + 168] * x[1 * i0 + 768];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 342] += A[i0 + 176] * x[1 * i0 + 776];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 343] += A[i0 + 184] * x[1 * i0 + 784];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[0 * i0 + 343] += A[i0 + 192] * x[1 * i0 + 792];
  }
#pragma endmacveth
}