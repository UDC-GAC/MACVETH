#include <stdio.h>
#include <immintrin.h>

#define restrict __restrict

#define ORIG_y 0
#define ORIG_A 0
#define ORIG_x 0

#define DATA_TYPE float

void codelet_case_1(DATA_TYPE *restrict A, DATA_TYPE *restrict x,
                    DATA_TYPE *restrict y, int I, int J, int I_sz, int J_sz,
                    int I_Str, int J_Str, int Ya, int Aa, int Xa, int Yb,
                    int Ab, int Xb) {
  int i, j;
  for (i = I; i < I + I_sz; i += I_Str) {
    for (j = J; j < J + J_sz; j += J_Str) {
      y[Ya * i + ORIG_y] += A[Aa * i + ORIG_A] * x[Xa * i + ORIG_x];
      y[Ya * (i + 42) + ORIG_y] +=
          A[Aa * (i + 42) + ORIG_A] * x[Xa * (i + 42) + ORIG_x];
    }
  }
}

void codelet_case_1_mv(DATA_TYPE *restrict A, DATA_TYPE *restrict x,
                    DATA_TYPE *restrict y, int I, int J, int I_sz, int J_sz,
                    int I_Str, int J_Str, int Ya, int Aa, int Xa, int Yb,
                    int Ab, int Xb) {
  int i, j;
#pragma macveth
  for (i = I; i < I + I_sz; i += I_Str) {
    for (j = J; j < J + J_sz; j += J_Str) {
      y[Ya * i + ORIG_y] += A[Aa * i + ORIG_A] * x[Xa * i + ORIG_x];
      y[Ya * (i + 42) + ORIG_y] +=
          A[Aa * (i + 42) + ORIG_A] * x[Xa * (i + 42) + ORIG_x];
    }
  }
#pragma endmacveth
}
