// begin MACVETH: headers added
#include <immintrin.h>
// end MACVETH
#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))
#include <stdio.h>

#define restrict __restrict

void kernel_spmv_fragment_0(double *restrict A, double *restrict x,
                            double *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;

#pragma macveth unroll i0 full
  __m128d __mv_vop14, __mv_vop12, __mv_vop13, __mv_accm2, __mv_vop61, __mv_vop17, __mv_vop15, __mv_vop16, __mv_accm3, __mv_vop62, __mv_vop20, __mv_vop18, __mv_vop19, __mv_vop23, __mv_vop21, __mv_vop22, __mv_accm4, __mv_vop63, __mv_vop26, __mv_vop24, __mv_vop25, __mv_accm5, __mv_vop64, __mv_vop28, __mv_vop27, __mv_accm6, __mv_vop65, __mv_vop31, __mv_vop29, __mv_vop30, __mv_vop34, __mv_vop32, __mv_vop33, __mv_accm7, __mv_vop66, __mv_vop37, __mv_vop35, __mv_vop36, __mv_vop54, __mv_vop39, __mv_vop38, __mv_vop55, __mv_vop42, __mv_vop40, __mv_vop41, __mv_vop56, __mv_vop45, __mv_vop43, __mv_vop44, __mv_vop57, __mv_vop48, __mv_vop46, __mv_vop47, __mv_vop58, __mv_vop50, __mv_vop49, __mv_vop59, __mv_vop52, __mv_vop51, __mv_vop60;
  __m256d __mv_vop2, __mv_vop0, __mv_vop1, __mv_accm0, __mv_vop68, __mv_vop5, __mv_vop3, __mv_vop4, __mv_accm1, __mv_vop67, __mv_vop8, __mv_vop6, __mv_vop7, __mv_vop11, __mv_vop9, __mv_vop10, __mv_vop53;
  __mv_accm0 = _mm256_setzero_pd();	// latency = 1
__mv_accm1 = _mm256_setzero_pd();	// latency = 1
__mv_accm2 = _mm_setzero_pd();	// latency = 1
__mv_accm3 = _mm_setzero_pd();	// latency = 1
__mv_accm4 = _mm_setzero_pd();	// latency = 1
__mv_accm5 = _mm_setzero_pd();	// latency = 1
__mv_accm6 = _mm_setzero_pd();	// latency = 1
__mv_accm7 = _mm_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 3; i0 += 4) {
    __mv_vop0 = _mm256_loadu_pd(&A[(i0 + 0)]);	// latency = 1
    __mv_vop1 = _mm256_i64gather_pd(&x[((7 * (i0 + 0)) + 4)], _mm256_set_epi64x(21, 14, 7, 0), 8);	// latency = 10
    __mv_accm0 = _mm256_fmadd_pd(__mv_vop0, __mv_vop1, __mv_accm0);	// latency = 5
    // stmt vectorized: y[0 * i0 + 9] += A[i0 + 0] * x[7 * i0 + 4];
  }

  for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop3 = _mm256_maskload_pd(&A[((i0 + 0) + 4)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop4 = _mm256_mask_i64gather_pd(_mm256_setzero_pd(), &x[((2 * (i0 + 0)) + 4)], _mm256_set_epi64x(0,4, 2, 0), _mm256_set_pd(0,-1,-1,-1), 8);	// latency = 10
    __mv_accm1 = _mm256_fmadd_pd(__mv_vop3, __mv_vop4, __mv_accm1);	// latency = 5
    // stmt vectorized: y[0 * i0 + 0] += A[i0 + 4] * x[2 * i0 + 4];
  }

  for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop6 = _mm256_maskload_pd(&A[((i0 + 0) + 7)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop7 = _mm256_mask_i64gather_pd(_mm256_setzero_pd(), &x[((5 * (i0 + 0)) + 12)], _mm256_set_epi64x(0,10, 5, 0), _mm256_set_pd(0,-1,-1,-1), 8);	// latency = 10
    __mv_accm1 = _mm256_fmadd_pd(__mv_vop6, __mv_vop7, __mv_accm1);	// latency = 5
    // stmt vectorized: y[0 * i0 + 0] += A[i0 + 7] * x[5 * i0 + 12];
  }__mv_accm0 = _mm256_hadd_pd(__mv_accm0, __mv_accm1);	// latency = 6
__mv_accm0 = _mm256_add_pd(_mm256_blend_pd(__mv_accm0,__mv_accm1,0b1100), _mm256_permute2f128_pd(__mv_accm0,__mv_accm1,0x21));	// latency = 4
y[9] = y[9] + __mv_accm0[0];	// latency = 1
y[0] = y[0] + __mv_accm0[1];	// latency = 1

  for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop9 = _mm256_maskload_pd(&A[((i0 + 0) + 10)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop10 = _mm256_mask_i64gather_pd(_mm256_setzero_pd(), &x[((2 * (i0 + 0)) + 3)], _mm256_set_epi64x(0,4, 2, 0), _mm256_set_pd(0,-1,-1,-1), 8);	// latency = 10
    __mv_vop53 = _mm256_maskload_pd(&y[(i0 + 0)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop53 = _mm256_fmadd_pd(__mv_vop9, __mv_vop10, __mv_vop53);	// latency = 5
    _mm256_maskstore_pd(&y[(i0 + 0)], _mm256_set_epi64x(0,-1,-1,-1), __mv_vop53);	// latency = 1
    // stmt vectorized: y[1 * i0 + 0] += A[i0 + 10] * x[2 * i0 + 3];
  }

  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop12 = _mm_loadu_pd(&A[((i0 + 0) + 13)]);	// latency = 1
    __mv_vop13 = _mm_i64gather_pd(&x[((2 * (i0 + 0)) + 29)], _mm_set_epi64x(2, 0), 8);	// latency = 10
    __mv_accm2 = _mm_fmadd_pd(__mv_vop12, __mv_vop13, __mv_accm2);	// latency = 5
    // stmt vectorized: y[0 * i0 + 0] += A[i0 + 13] * x[2 * i0 + 29];
  }
  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop15 = _mm_loadu_pd(&A[((i0 + 0) + 15)]);	// latency = 1
    __mv_vop16 = _mm_i64gather_pd(&x[((2 * (i0 + 0)) + 15)], _mm_set_epi64x(2, 0), 8);	// latency = 10
    __mv_accm3 = _mm_fmadd_pd(__mv_vop15, __mv_vop16, __mv_accm3);	// latency = 5
    // stmt vectorized: y[0 * i0 + 9] += A[i0 + 15] * x[2 * i0 + 15];
  }__mv_accm2 = _mm_hadd_pd(__mv_accm2, __mv_accm3);	// latency = 6
y[0] = y[0] + __mv_accm2[0];	// latency = 1
y[9] = y[9] + __mv_accm2[1];	// latency = 1


  __mv_accm3 = _mm_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop18 = _mm_loadu_pd(&A[((i0 + 0) + 17)]);	// latency = 1
    __mv_vop19 = _mm_i64gather_pd(&x[((2 * (i0 + 0)) + 28)], _mm_set_epi64x(2, 0), 8);	// latency = 10
    __mv_accm3 = _mm_fmadd_pd(__mv_vop18, __mv_vop19, __mv_accm3);	// latency = 5
    // stmt vectorized: y[0 * i0 + 9] += A[i0 + 17] * x[2 * i0 + 28];
  }
  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop21 = _mm_loadu_pd(&A[((i0 + 0) + 19)]);	// latency = 1
    __mv_vop22 = _mm_i64gather_pd(&x[((3 * (i0 + 0)) + 24)], _mm_set_epi64x(3, 0), 8);	// latency = 10
    __mv_accm4 = _mm_fmadd_pd(__mv_vop21, __mv_vop22, __mv_accm4);	// latency = 5
    // stmt vectorized: y[0 * i0 + 23] += A[i0 + 19] * x[3 * i0 + 24];
  }__mv_accm3 = _mm_hadd_pd(__mv_accm3, __mv_accm4);	// latency = 6
y[9] = y[9] + __mv_accm3[0];	// latency = 1
y[23] = y[23] + __mv_accm3[1];	// latency = 1

  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop24 = _mm_loadu_pd(&A[((i0 + 0) + 21)]);	// latency = 1
    __mv_vop25 = _mm_i64gather_pd(&x[((4 * (i0 + 0)) + 9)], _mm_set_epi64x(4, 0), 8);	// latency = 10
    __mv_accm5 = _mm_fmadd_pd(__mv_vop24, __mv_vop25, __mv_accm5);	// latency = 5
    // stmt vectorized: y[0 * i0 + 2] += A[i0 + 21] * x[4 * i0 + 9];
  }
  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop27 = _mm_loadu_pd(&A[((i0 + 0) + 23)]);	// latency = 1
    __mv_vop25 = _mm_i64gather_pd(&x[((4 * (i0 + 0)) + 9)], _mm_set_epi64x(4, 0), 8);	// latency = 10
    __mv_accm6 = _mm_fmadd_pd(__mv_vop27, __mv_vop25, __mv_accm6);	// latency = 5
    // stmt vectorized: y[0 * i0 + 22] += A[i0 + 23] * x[4 * i0 + 9];
  }__mv_accm5 = _mm_hadd_pd(__mv_accm5, __mv_accm6);	// latency = 6
y[2] = y[2] + __mv_accm5[0];	// latency = 1
y[22] = y[22] + __mv_accm5[1];	// latency = 1

  __mv_accm3 = _mm_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop29 = _mm_loadu_pd(&A[((i0 + 0) + 25)]);	// latency = 1
    __mv_vop30 = _mm_i64gather_pd(&x[((5 * (i0 + 0)) + 1)], _mm_set_epi64x(5, 0), 8);	// latency = 10
    __mv_accm3 = _mm_fmadd_pd(__mv_vop29, __mv_vop30, __mv_accm3);	// latency = 5
    // stmt vectorized: y[0 * i0 + 9] += A[i0 + 25] * x[5 * i0 + 1];
  }
  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop32 = _mm_loadu_pd(&A[((i0 + 0) + 27)]);	// latency = 1
    __mv_vop33 = _mm_i64gather_pd(&x[((7 * (i0 + 0)) + 9)], _mm_set_epi64x(7, 0), 8);	// latency = 10
    __mv_accm7 = _mm_fmadd_pd(__mv_vop32, __mv_vop33, __mv_accm7);	// latency = 5
    // stmt vectorized: y[0 * i0 + 10] += A[i0 + 27] * x[7 * i0 + 9];
  }__mv_accm3 = _mm_hadd_pd(__mv_accm3, __mv_accm7);	// latency = 6
y[9] = y[9] + __mv_accm3[0];	// latency = 1
y[10] = y[10] + __mv_accm3[1];	// latency = 1

  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop35 = _mm_loadu_pd(&A[((i0 + 0) + 29)]);	// latency = 1
    __mv_vop36 = _mm_set1_pd(x[0]);	// latency = 1
    __mv_vop54 = _mm_loadu_pd(&y[((i0 + 0) + 1)]);	// latency = 1
    __mv_vop54 = _mm_fmadd_pd(__mv_vop35, __mv_vop36, __mv_vop54);	// latency = 5
    _mm_storeu_pd(&y[((i0 + 0) + 1)], __mv_vop54);	// latency = 1
    // stmt vectorized: y[1 * i0 + 1] += A[i0 + 29] * x[0 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop38 = _mm_loadu_pd(&A[((i0 + 0) + 31)]);	// latency = 1
    __mv_vop36 = _mm_set1_pd(x[0]);	// latency = 1
    __mv_vop55 = _mm_loadu_pd(&y[((i0 + 0) + 21)]);	// latency = 1
    __mv_vop55 = _mm_fmadd_pd(__mv_vop38, __mv_vop36, __mv_vop55);	// latency = 5
    _mm_storeu_pd(&y[((i0 + 0) + 21)], __mv_vop55);	// latency = 1
    // stmt vectorized: y[1 * i0 + 21] += A[i0 + 31] * x[0 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop40 = _mm_loadu_pd(&A[((i0 + 0) + 33)]);	// latency = 1
    __mv_vop41 = _mm_i64gather_pd(&x[((4 * (i0 + 0)) + 22)], _mm_set_epi64x(4, 0), 8);	// latency = 10
    __mv_vop56 = _mm_loadu_pd(&y[((i0 + 0) + 9)]);	// latency = 1
    __mv_vop56 = _mm_fmadd_pd(__mv_vop40, __mv_vop41, __mv_vop56);	// latency = 5
    _mm_storeu_pd(&y[((i0 + 0) + 9)], __mv_vop56);	// latency = 1
    // stmt vectorized: y[1 * i0 + 9] += A[i0 + 33] * x[4 * i0 + 22];
  }
  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop43 = _mm_loadu_pd(&A[((i0 + 0) + 35)]);	// latency = 1
    __mv_vop44 = _mm_i64gather_pd(&x[((2 * (i0 + 0)) + 33)], _mm_set_epi64x(2, 0), 8);	// latency = 10
    __mv_vop57 = _mm_i64gather_pd(&y[((2 * (i0 + 0)) + 32)], _mm_set_epi64x(2, 0), 8);	// latency = 10
    __mv_vop57 = _mm_fmadd_pd(__mv_vop43, __mv_vop44, __mv_vop57);	// latency = 5
    y[((2 * (i0 + 0)) + 32)] = __mv_vop57[0];	// latency = 1
    y[((2 * (i0 + 1)) + 32)] = __mv_vop57[1];	// latency = 1
    // stmt vectorized: y[2 * i0 + 32] += A[i0 + 35] * x[2 * i0 + 33];
  }
  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop46 = _mm_loadu_pd(&A[((i0 + 0) + 37)]);	// latency = 1
    __mv_vop47 = _mm_i64gather_pd(&x[((-7 * (i0 + 1)) + 16)], _mm_set_epi64x(0, 7), 8);	// latency = 10
    __mv_vop58 = _mm_i64gather_pd(&y[((3 * (i0 + 0)) + 23)], _mm_set_epi64x(3, 0), 8);	// latency = 10
    __mv_vop58 = _mm_fmadd_pd(__mv_vop46, __mv_vop47, __mv_vop58);	// latency = 5
    y[((3 * (i0 + 0)) + 23)] = __mv_vop58[0];	// latency = 1
    y[((3 * (i0 + 1)) + 23)] = __mv_vop58[1];	// latency = 1
    // stmt vectorized: y[3 * i0 + 23] += A[i0 + 37] * x[-7 * i0 + 16];
  }
  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop49 = _mm_loadu_pd(&A[((i0 + 0) + 39)]);	// latency = 1
    __mv_vop36 = _mm_set1_pd(x[0]);	// latency = 1
    __mv_vop59 = _mm_i64gather_pd(&y[((4 * (i0 + 0)) + 10)], _mm_set_epi64x(4, 0), 8);	// latency = 10
    __mv_vop59 = _mm_fmadd_pd(__mv_vop49, __mv_vop36, __mv_vop59);	// latency = 5
    y[((4 * (i0 + 0)) + 10)] = __mv_vop59[0];	// latency = 1
    y[((4 * (i0 + 1)) + 10)] = __mv_vop59[1];	// latency = 1
    // stmt vectorized: y[4 * i0 + 10] += A[i0 + 39] * x[0 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop51 = _mm_loadu_pd(&A[((i0 + 0) + 41)]);	// latency = 1
    __mv_vop33 = _mm_i64gather_pd(&x[((7 * (i0 + 0)) + 9)], _mm_set_epi64x(7, 0), 8);	// latency = 10
    __mv_vop60 = _mm_i64gather_pd(&y[((4 * (i0 + 0)) + 1)], _mm_set_epi64x(4, 0), 8);	// latency = 10
    __mv_vop60 = _mm_fmadd_pd(__mv_vop51, __mv_vop33, __mv_vop60);	// latency = 5
    y[((4 * (i0 + 0)) + 1)] = __mv_vop60[0];	// latency = 1
    y[((4 * (i0 + 1)) + 1)] = __mv_vop60[1];	// latency = 1
    // stmt vectorized: y[4 * i0 + 1] += A[i0 + 41] * x[7 * i0 + 9];
  }

#pragma endmacveth
}

void kernel_spmv(double *restrict A, double *restrict x, double *restrict y,
                 long long n, long long *indptr, long long *indices) {
  register int i, j;

  kernel_spmv_fragment_0(A, x, y);

  for (i = 0; i < 7; ++i) {
    y[indptr[i]] += A[43 + i] * x[indices[i]];
  }
}
