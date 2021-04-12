#include <stdio.h>
#include <immintrin.h>

#define restrict __restrict

#define ORIG_y 0
#define ORIG_A 0
#define ORIG_x 0

#define DATA_TYPE float

void codelet_case_2_fused(DATA_TYPE *restrict A, DATA_TYPE *restrict x,
                    DATA_TYPE *restrict y) {
    int I = 0;
    int J = 0;
    int I_sz=2;
    int J_sz=2;
    int I_Str=1;
    int J_Str=1;
    int Ya=1;
    int Aa=1;
    int Xa=1;
    int Yb=0;
    int Ab=0;
    int Xb=0;
    int i,j;

  for (i = I; i < I + I_sz; i += I_Str) {
    for (j = J; j < J + J_sz; j += J_Str) {
      y[Ya * i + Yb * j + ORIG_y] +=
          A[Aa * i + Ab * j + ORIG_A] * x[Xa * i + Xb * j + ORIG_x];
      y[42 * (i + 42) + Yb * (j + 42) + ORIG_y] +=
          A[42 * (i + 42) + Ab * (j + 42) + ORIG_A] *
          x[42 * (i + 42) + Xb * (j + 42) + ORIG_x];
    }
  }
}

void codelet_case_2_mv(DATA_TYPE *restrict A, DATA_TYPE *restrict x,
                    DATA_TYPE *restrict y) {
    int I = 0;
    int J = 0;
    int I_sz=2;
    int J_sz=2;
    int I_Str=1;
    int J_Str=1;
    int Ya=1;
    int Aa=1;
    int Xa=1;
    int Yb=0;
    int Ab=0;
    int Xb=0;
    int i,j;

#pragma macveth
  for (i = I; i < I + I_sz; i += I_Str) {
    for (j = J; j < J + J_sz; j += J_Str) {
      y[Ya * i + Yb * j + ORIG_y] +=
          A[Aa * i + Ab * j + ORIG_A] * x[Xa * i + Xb * j + ORIG_x];
      y[42 * (i + 42) + Yb * (j + 42) + ORIG_y] +=
          A[42 * (i + 42) + Ab * (j + 42) + ORIG_A] *
          x[42 * (i + 42) + Xb * (j + 42) + ORIG_x];
    }
  }
#pragma endmacveth
}