#include <stdio.h>
#include <immintrin.h>

#define restrict __restrict

#define ORIG_y 0
#define ORIG_A 0
#define ORIG_x 0

#define DATA_TYPE float

/*
void codelet(DATA_TYPE *restrict A, DATA_TYPE *restrict x, DATA_TYPE *restrict y)
{
  register int i, j, i2, i3, i4, i5, i6;
    for (i = 0; i <= 2; ++i) {
        for (j = 0; j <= 15; j+=2) {
            y[8*i + 1*j + ORIG_y] += A[8*i + ORIG_A] * x[8*i + j + ORIG_x];
        }
    }
}
*/
void codelet_case_0(DATA_TYPE *restrict A, DATA_TYPE *restrict x, DATA_TYPE *restrict y)
{
    {
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
            y[Ya * i + Yb * j + ORIG_y] += A[Aa * i + Ab * j + ORIG_A] *
                                    x[Xa * i + Xb * j + ORIG_x];
        }
    }
    }
    {
    int I = 42;
    int J = 42;
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
            y[Ya * i + Yb * j + ORIG_y] += A[Aa * i + Ab * j + ORIG_A] *
                                    x[Xa * i + Xb * j + ORIG_x];
        }
    }
    }
}


void codelet_case_0_fused(DATA_TYPE *restrict A, DATA_TYPE *restrict x,
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
      y[Ya * (i + 42) + Yb * (j + 42) + ORIG_y] +=
          A[Aa * (i + 42) + Ab * (j + 42) + ORIG_A] *
          x[Xa * (i + 42) + Xb * (j + 42) + ORIG_x];
    }
  }
}

void codelet_case_0_mv(DATA_TYPE *restrict A, DATA_TYPE *restrict x,
                    DATA_TYPE *restrict y) {
  int I = 0;
  int J = 0;
  int I_sz = 2;
  int J_sz = 2;
  int I_Str = 1;
  int J_Str = 1;
  int Ya = 1;
  int Aa = 1;
  int Xa = 1;
  int Yb = 0;
  int Ab = 0;
  int Xb = I;
  int i, j;

#pragma macveth unroll i full j full
  __m256 __mv_vop2, __mv_vop0, __mv_vop1, __mv_vop3;
  //for (i = 0; i < I + I_sz; i += 2) {
    //for (j = 0; j < J + J_sz; j += 2) {
      __mv_vop0 =
          _mm256_set_ps(A[((Aa * ((i + 1) + 42)) + (Ab * ((j + 1) + 42)))],
                        A[((Aa * ((i + 1) + 42)) + (Ab * ((j + 0) + 42)))],
                        A[((Aa * ((i + 0) + 42)) + (Ab * ((j + 1) + 42)))],
                        A[((Aa * ((i + 0) + 42)) + (Ab * ((j + 0) + 42)))],
                        A[((Aa * (i + 1)) + (Ab * (j + 1)))],
                        A[((Aa * (i + 1)) + (Ab * (j + 0)))],
                        A[((Aa * (i + 0)) + (Ab * (j + 1)))],
                        A[((Aa * (i + 0)) + (Ab * (j + 0)))]);
      __mv_vop1 =
          _mm256_set_ps(x[((Xa * ((i + 1) + 42)) + (Xb * ((j + 1) + 42)))],
                        x[((Xa * ((i + 1) + 42)) + (Xb * ((j + 0) + 42)))],
                        x[((Xa * ((i + 0) + 42)) + (Xb * ((j + 1) + 42)))],
                        x[((Xa * ((i + 0) + 42)) + (Xb * ((j + 0) + 42)))],
                        x[((Xa * (i + 1)) + (Xb * (j + 1)))],
                        x[((Xa * (i + 1)) + (Xb * (j + 0)))],
                        x[((Xa * (i + 0)) + (Xb * (j + 1)))],
                        x[((Xa * (i + 0)) + (Xb * (j + 0)))]);
      __mv_vop3 =
          _mm256_set_ps(y[((Ya * ((i + 1) + 42)) + (Yb * ((j + 1) + 42)))],
                        y[((Ya * ((i + 1) + 42)) + (Yb * ((j + 0) + 42)))],
                        y[((Ya * ((i + 0) + 42)) + (Yb * ((j + 1) + 42)))],
                        y[((Ya * ((i + 0) + 42)) + (Yb * ((j + 0) + 42)))],
                        y[((Ya * (i + 1)) + (Yb * (j + 1)))],
                        y[((Ya * (i + 1)) + (Yb * (j + 0)))],
                        y[((Ya * (i + 0)) + (Yb * (j + 1)))],
                        y[((Ya * (i + 0)) + (Yb * (j + 0)))]);
      __mv_vop3 = _mm256_fmadd_ps(__mv_vop0, __mv_vop1, __mv_vop3);
      y[((Ya * (i + 0)) + (Yb * (j + 0)))] = __mv_vop3[0];
      y[((Ya * (i + 0)) + (Yb * (j + 1)))] = __mv_vop3[1];
      y[((Ya * (i + 1)) + (Yb * (j + 0)))] = __mv_vop3[2];
      y[((Ya * (i + 1)) + (Yb * (j + 1)))] = __mv_vop3[3];
      y[((Ya * ((i + 0) + 42)) + (Yb * ((j + 0) + 42)))] = __mv_vop3[4];
      y[((Ya * ((i + 0) + 42)) + (Yb * ((j + 1) + 42)))] = __mv_vop3[5];
      y[((Ya * ((i + 1) + 42)) + (Yb * ((j + 0) + 42)))] = __mv_vop3[6];
      y[((Ya * ((i + 1) + 42)) + (Yb * ((j + 1) + 42)))] = __mv_vop3[7];
    //}
  //}
#pragma endmacveth
}
