// begin MACVETH: headers added
#include <immintrin.h>
// end MACVETH
#include <stdio.h>
#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))
#define restrict __restrict

void kernel_spmv_fragment_0(double *restrict A, double *restrict x,
                            double *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;

#pragma macveth unroll i0 full
  __m128d __mv_vop2, __mv_vop0, __mv_vop1, __mv_accm0, __mv_vop9, __mv_vop5,
      __mv_vop3, __mv_vop4, __mv_accm1, __mv_vop8, __mv_vop6, __mv_vop7,
      __mv_accm2, __mv_lo, __mv_hi;
  __mv_accm0 = _mm_setzero_pd(); // latency = 1
  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop0 = _mm_loadu_pd(&A[(i0 + 0)]); // latency = 1
    __mv_vop1 = _mm_i64gather_pd(&x[((2 * (i0 + 0)) + 42)],
                                 _mm_set_epi64x(2, 0), 8);       // latency = 10
    __mv_accm0 = _mm_fmadd_pd(__mv_vop0, __mv_vop1, __mv_accm0); // latency = 5
    // stmt vectorized: y[0 * i0 + 0] += A[i0 + 0] * x[2 * i0 + 42];
  }

  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop3 = _mm_loadu_pd(&A[((i0 + 0) + 2)]); // latency = 1
    __mv_vop4 = _mm_i64gather_pd(&x[((2 * (i0 + 0)) + 57)],
                                 _mm_set_epi64x(2, 0), 8);       // latency = 10
    __mv_accm0 = _mm_fmadd_pd(__mv_vop3, __mv_vop4, __mv_accm0); // latency = 5
    // stmt vectorized: y[0 * i0 + 0] += A[i0 + 2] * x[2 * i0 + 57];
  }

  for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop6 = _mm_loadu_pd(&A[((i0 + 0) + 4)]); // latency = 1
    __mv_vop7 = _mm_i64gather_pd(&x[((4 * (i0 + 0)) + 47)],
                                 _mm_set_epi64x(4, 0), 8);       // latency = 10
    __mv_accm0 = _mm_fmadd_pd(__mv_vop6, __mv_vop7, __mv_accm0); // latency = 5
    // stmt vectorized: y[0 * i0 + 0] += A[i0 + 4] * x[4 * i0 + 47];
  }
  __mv_hi = _mm_shuffle_pd(__mv_accm0, __mv_accm0, 0x1); // latency = 1
  __mv_accm0 = _mm_add_pd(__mv_accm0, __mv_hi);          // latency = 4
  y[0] = y[0] + __mv_accm0[0];                           // latency = 1

#pragma endmacveth
}

void kernel_spmv(double *restrict A, double *restrict x, double *restrict y,
                 long long n, long long *indptr, long long *indices) {
  register int i, j;

  kernel_spmv_fragment_0(A, x, y);
  for (i = 0; i < 1; ++i) {
    for (j = indptr[i]; j < indptr[i + 1]; ++j) {
      y[i] = A[j + 6] * x[indices[j]] + y[i];
    }
  }
}
