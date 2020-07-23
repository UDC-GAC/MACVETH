#include <immintrin.h>

#include "redux.h"
//#include "utilities/polybench.h"
#include <algorithm>
#include <immintrin.h>
#include <math.h>
#include <stdio.h>
#include <vector>

// ---------------------------------------------
long redux_flops(red_vec(v0)) {
  int channel = v0.size();
  return channel * 2 * 1;
}

void redux(red_vec(v0)) {
  int width = v0.size();

  DATA_TYPE sum = 0.0;
  DATA_TYPE alpha = 1.1f;
#pragma macveth w 8
  int w;
  __m128 __mv_lo, __mv_hi;
  __m256 __mv_vop2, __mv_vop0, __mv_vop1, __mv_accm0, __mv_vop3;
  __mv_accm0 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum); // latency = 1
  for (w = 0; (w + 8) <= width; w += 8) {
    __mv_vop0 = _mm256_loadu_ps(&v0[(w + 0)]); // latency = 1
    __mv_vop1 = _mm256_broadcast_ss(&alpha);   // latency = 8
    __mv_accm0 =
        _mm256_fmadd_ps(__mv_vop0, __mv_vop1, __mv_accm0); // latency = 5
    // vectorized: sum = sum + v0[w] * alpha;
  }
  __mv_lo = _mm256_castps256_ps128(__mv_accm0);           // latency = 0
  __mv_hi = _mm256_extractf128_ps(__mv_accm0, 0x1);       // latency = 3
  __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
  __mv_hi = _mm_movehl_ps(__mv_lo, __mv_lo);              // latency = 1
  __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
  __mv_hi = _mm_shuffle_ps(__mv_lo, __mv_lo, 0b00001110); // latency = 1
  __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
  sum = _mm_cvtss_f32(__mv_lo);                           // latency = 1

  for (w = w; w < width; w += 1) {
    sum = sum + v0[w] * alpha;
  }

#pragma endmacveth
  v0[0] = sum;
}

// ---------------------------------------------
long redux2_flops(red_vec(v0)) {
  int channel = v0.size();
  return channel * 2 * 2;
}

void redux2(red_vec(v0), red_vec(v1)) {
  int width = v0.size();

  DATA_TYPE sum0 = 0.0;
  DATA_TYPE sum1 = 0.0;
  DATA_TYPE alpha = 1.1f;
  DATA_TYPE beta = 0.9f;
#pragma macveth w 8
  int w;
  __m256 __mv_vop2, __mv_vop0, __mv_vop1, __mv_accm0, __mv_vop6, __mv_vop5,
      __mv_vop3, __mv_vop4, __mv_accm1, __mv_vop7;
  float __mv_arr0[8];
  __mv_accm0 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum0); // latency = 1
  __mv_accm1 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum1); // latency = 1
  for (w = 0; (w + 8) <= width; w += 8) {
    __mv_vop0 = _mm256_loadu_ps(&v0[(w + 0)]); // latency = 1
    __mv_vop1 = _mm256_broadcast_ss(&alpha);   // latency = 8
    __mv_accm0 =
        _mm256_fmadd_ps(__mv_vop0, __mv_vop1, __mv_accm0); // latency = 5
    // vectorized: sum0 = sum0 + v0[w] * alpha;
    __mv_vop3 = _mm256_loadu_ps(&v1[(w + 0)]); // latency = 1
    __mv_vop4 = _mm256_broadcast_ss(&beta);    // latency = 8
    __mv_accm1 =
        _mm256_fmadd_ps(__mv_vop3, __mv_vop4, __mv_accm1); // latency = 5
    // vectorized: sum1 = sum1 + v1[w] * beta;
  }
  __mv_accm0 = _mm256_hadd_ps(__mv_accm0, __mv_accm1); // latency = 6
  __mv_accm0 = _mm256_add_ps(
      _mm256_blend_ps(__mv_accm0, __mv_accm1, 0b11110000),
      _mm256_permute2f128_ps(__mv_accm0, __mv_accm1, 0x21)); // latency = 4
  _mm256_storeu_ps(__mv_arr0, __mv_accm0);                   // latency = 1
  sum0 = __mv_arr0[0];                                       // latency = 1
  sum1 = __mv_arr0[1];                                       // latency = 1

  for (w = w; w < width; w += 1) {
    sum0 = sum0 + v0[w] * alpha;
    sum1 = sum1 + v1[w] * beta;
  }

#pragma endmacveth
  v0[0] = sum0;
  v0[1] = sum1;
}

// ---------------------------------------------
long redux4_flops(red_vec(v0)) {
  int channel = v0.size();
  return channel * 2 * 4;
}

void redux4(red_vec(v0), red_vec(v1), red_vec(v2), red_vec(v3)) {
  int width = v0.size();

  DATA_TYPE sum0 = 0.0;
  DATA_TYPE sum1 = 0.0;
  DATA_TYPE sum2 = 0.0;
  DATA_TYPE sum3 = 0.0;
  DATA_TYPE alpha = 1.1f;
  DATA_TYPE beta = 0.9f;
#pragma macveth w 8
  int w;
  __m256 __mv_vop2, __mv_vop0, __mv_vop1, __mv_accm0, __mv_vop10, __mv_vop5,
      __mv_vop3, __mv_vop4, __mv_accm1, __mv_vop11, __mv_vop7, __mv_vop6,
      __mv_accm2, __mv_vop12, __mv_vop9, __mv_vop8, __mv_accm3, __mv_vop13;
  float __mv_arr0[8];

  __mv_accm0 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum0); // latency = 1
  __mv_accm1 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum1); // latency = 1
  __mv_accm2 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum2); // latency = 1
  __mv_accm3 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum3); // latency = 1
  for (w = 0; (w + 8) <= width; w += 8) {
    __mv_vop0 = _mm256_loadu_ps(&v0[(w + 0)]); // latency = 1
    __mv_vop1 = _mm256_broadcast_ss(&alpha);   // latency = 8
    __mv_accm0 =
        _mm256_fmadd_ps(__mv_vop0, __mv_vop1, __mv_accm0); // latency = 5
    // vectorized: sum0 = sum0 + v0[w] * alpha;
    __mv_vop3 = _mm256_loadu_ps(&v1[(w + 0)]); // latency = 1
    __mv_vop4 = _mm256_broadcast_ss(&beta);    // latency = 8
    __mv_accm1 =
        _mm256_fmadd_ps(__mv_vop3, __mv_vop4, __mv_accm1); // latency = 5
    // vectorized: sum1 = sum1 + v1[w] * beta;
    __mv_vop6 = _mm256_loadu_ps(&v2[(w + 0)]); // latency = 1
    __mv_accm2 =
        _mm256_fmadd_ps(__mv_vop6, __mv_vop4, __mv_accm2); // latency = 5
    // vectorized: sum2 = sum2 + v2[w] * beta;
    __mv_vop8 = _mm256_loadu_ps(&v3[(w + 0)]); // latency = 1
    __mv_accm3 =
        _mm256_fmadd_ps(__mv_vop8, __mv_vop4, __mv_accm3); // latency = 5
    // vectorized: sum3 = sum3 + v3[w] * beta;
  }
  __mv_accm0 = _mm256_hadd_ps(__mv_accm0, __mv_accm1); // latency = 6
  __mv_accm2 = _mm256_hadd_ps(__mv_accm2, __mv_accm3); // latency = 6
  __mv_accm0 = _mm256_add_ps(
      _mm256_blend_ps(__mv_accm0, __mv_accm2, 0b11110000),
      _mm256_permute2f128_ps(__mv_accm0, __mv_accm2, 0x21)); // latency = 4
  _mm256_storeu_ps(__mv_arr0, __mv_accm0);                   // latency = 1
  sum0 = __mv_arr0[0];                                       // latency = 1
  sum1 = __mv_arr0[1];                                       // latency = 1
  sum2 = __mv_arr0[2];                                       // latency = 1
  sum3 = __mv_arr0[3];                                       // latency = 1

  for (w = w; w < width; w += 1) {
    sum0 = sum0 + v0[w] * alpha;
    sum1 = sum1 + v1[w] * beta;
    sum2 = sum2 + v2[w] * beta;
    sum3 = sum3 + v3[w] * beta;
  }

#pragma endmacveth
  v0[0] = sum0;
  v0[1] = sum1;
  v0[2] = sum2;
  v0[3] = sum3;
}

// ---------------------------------------------
long redux8_flops(red_vec(v0)) {
  int channel = v0.size();
  return channel * 2 * 8;
}

void redux8(red_vec(v0), red_vec(v1), red_vec(v2), red_vec(v3), red_vec(v4),
            red_vec(v5), red_vec(v6), red_vec(v7)) {
  int width = v0.size();

  // DATA_TYPE v1[width];
  // init_array(v1, width);
  // DATA_TYPE v2[width];
  // init_array(v2, width);
  // DATA_TYPE v3[width];
  // init_array(v3, width);
  // DATA_TYPE v4[width];
  // init_array(v4, width);
  // DATA_TYPE v5[width];
  // init_array(v5, width);
  // DATA_TYPE v6[width];
  // init_array(v6, width);
  // DATA_TYPE v7[width];
  // init_array(v7, width);

  DATA_TYPE sum0 = 0.0;
  DATA_TYPE sum1 = 0.0;
  DATA_TYPE sum2 = 0.0;
  DATA_TYPE sum3 = 0.0;
  DATA_TYPE sum4 = 0.0;
  DATA_TYPE sum5 = 0.0;
  DATA_TYPE sum6 = 0.0;
  DATA_TYPE sum7 = 0.0;
  DATA_TYPE alpha = 1.1f;
  DATA_TYPE beta = 0.9f;
#pragma macveth w 8
  int w;
  __m256 __mv_vop2, __mv_vop0, __mv_vop1, __mv_accm0, __mv_vop18, __mv_vop5,
      __mv_vop3, __mv_vop4, __mv_accm1, __mv_vop19, __mv_vop7, __mv_vop6,
      __mv_accm2, __mv_vop20, __mv_vop9, __mv_vop8, __mv_accm3, __mv_vop21,
      __mv_vop11, __mv_vop10, __mv_accm4, __mv_vop22, __mv_vop13, __mv_vop12,
      __mv_accm5, __mv_vop23, __mv_vop15, __mv_vop14, __mv_accm6, __mv_vop24,
      __mv_vop17, __mv_vop16, __mv_accm7, __mv_vop25;
  float __mv_arr0[8];
  __mv_accm0 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum0); // latency = 1
  __mv_accm1 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum1); // latency = 1
  __mv_accm2 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum2); // latency = 1
  __mv_accm3 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum3); // latency = 1
  __mv_accm4 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum4); // latency = 1
  __mv_accm5 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum5); // latency = 1
  __mv_accm6 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum6); // latency = 1
  __mv_accm7 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, sum7); // latency = 1
  for (w = 0; (w + 8) <= width; w += 8) {
    __mv_vop0 = _mm256_loadu_ps(&v0[(w + 0)]); // latency = 1
    __mv_vop1 = _mm256_broadcast_ss(&alpha);   // latency = 8
    __mv_accm0 =
        _mm256_fmadd_ps(__mv_vop0, __mv_vop1, __mv_accm0); // latency = 5
    // vectorized: sum0 = sum0 + v0[w] * alpha;
    __mv_vop3 = _mm256_loadu_ps(&v1[(w + 0)]); // latency = 1
    __mv_vop4 = _mm256_broadcast_ss(&beta);    // latency = 8
    __mv_accm1 =
        _mm256_fmadd_ps(__mv_vop3, __mv_vop4, __mv_accm1); // latency = 5
    // vectorized: sum1 = sum1 + v1[w] * beta;
    __mv_vop6 = _mm256_loadu_ps(&v2[(w + 0)]); // latency = 1
    __mv_accm2 =
        _mm256_fmadd_ps(__mv_vop6, __mv_vop1, __mv_accm2); // latency = 5
    // vectorized: sum2 = sum2 + v2[w] * alpha;
    __mv_vop8 = _mm256_loadu_ps(&v3[(w + 0)]); // latency = 1
    __mv_accm3 =
        _mm256_fmadd_ps(__mv_vop8, __mv_vop4, __mv_accm3); // latency = 5
    // vectorized: sum3 = sum3 + v3[w] * beta;
    __mv_vop10 = _mm256_loadu_ps(&v4[(w + 0)]); // latency = 1
    __mv_accm4 =
        _mm256_fmadd_ps(__mv_vop10, __mv_vop1, __mv_accm4); // latency = 5
    // vectorized: sum4 = sum4 + v4[w] * alpha;
    __mv_vop12 = _mm256_loadu_ps(&v5[(w + 0)]); // latency = 1
    __mv_accm5 =
        _mm256_fmadd_ps(__mv_vop12, __mv_vop4, __mv_accm5); // latency = 5
    // vectorized: sum5 = sum5 + v5[w] * beta;
    __mv_vop14 = _mm256_loadu_ps(&v6[(w + 0)]); // latency = 1
    __mv_accm6 =
        _mm256_fmadd_ps(__mv_vop14, __mv_vop1, __mv_accm6); // latency = 5
    // vectorized: sum6 = sum6 + v6[w] * alpha;
    __mv_vop16 = _mm256_loadu_ps(&v7[(w + 0)]); // latency = 1
    __mv_accm7 =
        _mm256_fmadd_ps(__mv_vop16, __mv_vop4, __mv_accm7); // latency = 5
    // vectorized: sum7 = sum7 + v7[w] * beta;
  }
  /*
    __m128 __mv_lo, __mv_hi;

    __mv_lo = _mm256_castps256_ps128(__mv_accm0);           // latency = 0
    __mv_hi = _mm256_extractf128_ps(__mv_accm0, 0x1);       // latency = 3
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_movehl_ps(__mv_lo, __mv_lo);              // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_shuffle_ps(__mv_lo, __mv_lo, 0b00001110); // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    sum0 = _mm_cvtss_f32(__mv_lo);                          // latency = 1
    __mv_lo = _mm256_castps256_ps128(__mv_accm1);           // latency = 0
    __mv_hi = _mm256_extractf128_ps(__mv_accm1, 0x1);       // latency = 3
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_movehl_ps(__mv_lo, __mv_lo);              // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_shuffle_ps(__mv_lo, __mv_lo, 0b00001110); // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    sum1 = _mm_cvtss_f32(__mv_lo);                          // latency = 1
    __mv_lo = _mm256_castps256_ps128(__mv_accm2);           // latency = 0
    __mv_hi = _mm256_extractf128_ps(__mv_accm2, 0x1);       // latency = 3
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_movehl_ps(__mv_lo, __mv_lo);              // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_shuffle_ps(__mv_lo, __mv_lo, 0b00001110); // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    sum2 = _mm_cvtss_f32(__mv_lo);                          // latency = 1
    __mv_lo = _mm256_castps256_ps128(__mv_accm3);           // latency = 0
    __mv_hi = _mm256_extractf128_ps(__mv_accm3, 0x1);       // latency = 3
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_movehl_ps(__mv_lo, __mv_lo);              // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_shuffle_ps(__mv_lo, __mv_lo, 0b00001110); // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    sum3 = _mm_cvtss_f32(__mv_lo);                          // latency = 1
    __mv_lo = _mm256_castps256_ps128(__mv_accm4);           // latency = 0
    __mv_hi = _mm256_extractf128_ps(__mv_accm4, 0x1);       // latency = 3
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_movehl_ps(__mv_lo, __mv_lo);              // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_shuffle_ps(__mv_lo, __mv_lo, 0b00001110); // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    sum4 = _mm_cvtss_f32(__mv_lo);                          // latency = 1
    __mv_lo = _mm256_castps256_ps128(__mv_accm5);           // latency = 0
    __mv_hi = _mm256_extractf128_ps(__mv_accm5, 0x1);       // latency = 3
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_movehl_ps(__mv_lo, __mv_lo);              // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_shuffle_ps(__mv_lo, __mv_lo, 0b00001110); // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    sum5 = _mm_cvtss_f32(__mv_lo);                          // latency = 1
    __mv_lo = _mm256_castps256_ps128(__mv_accm6);           // latency = 0
    __mv_hi = _mm256_extractf128_ps(__mv_accm6, 0x1);       // latency = 3
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_movehl_ps(__mv_lo, __mv_lo);              // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_shuffle_ps(__mv_lo, __mv_lo, 0b00001110); // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    sum6 = _mm_cvtss_f32(__mv_lo);                          // latency = 1
    __mv_lo = _mm256_castps256_ps128(__mv_accm7);           // latency = 0
    __mv_hi = _mm256_extractf128_ps(__mv_accm7, 0x1);       // latency = 3
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_movehl_ps(__mv_lo, __mv_lo);              // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    __mv_hi = _mm_shuffle_ps(__mv_lo, __mv_lo, 0b00001110); // latency = 1
    __mv_lo = _mm_add_ps(__mv_lo, __mv_hi);                 // latency = 4
    sum7 = _mm_cvtss_f32(__mv_lo);                          // latency = 1
  */
  __mv_accm0 = _mm256_hadd_ps(__mv_accm0, __mv_accm1); // latency = 6
  __mv_accm2 = _mm256_hadd_ps(__mv_accm2, __mv_accm3); // latency = 6
  __mv_accm4 = _mm256_hadd_ps(__mv_accm4, __mv_accm5); // latency = 6
  __mv_accm6 = _mm256_hadd_ps(__mv_accm6, __mv_accm7); // latency = 6
  __mv_accm0 = _mm256_hadd_ps(__mv_accm0, __mv_accm2); // latency = 6
  __mv_accm4 = _mm256_hadd_ps(__mv_accm4, __mv_accm6); // latency = 6
  __mv_accm0 = _mm256_add_ps(
      _mm256_blend_ps(__mv_accm0, __mv_accm4, 0b11110000),
      _mm256_permute2f128_ps(__mv_accm0, __mv_accm4, 0x21)); // latency = 4
  _mm256_storeu_ps(__mv_arr0, __mv_accm0);                   // latency = 1
  sum0 = __mv_arr0[0];                                       // latency = 1
  sum1 = __mv_arr0[1];                                       // latency = 1
  sum2 = __mv_arr0[2];                                       // latency = 1
  sum3 = __mv_arr0[3];                                       // latency = 1
  sum4 = __mv_arr0[4];                                       // latency = 1
  sum5 = __mv_arr0[5];                                       // latency = 1
  sum6 = __mv_arr0[6];                                       // latency = 1
  sum7 = __mv_arr0[7];                                       // latency = 1

  for (w = w; w < width; w += 1) {
    sum0 = sum0 + v0[w] * alpha;
    sum1 = sum1 + v1[w] * beta;
    sum2 = sum2 + v2[w] * alpha;
    sum3 = sum3 + v3[w] * beta;
    sum4 = sum4 + v4[w] * alpha;
    sum5 = sum5 + v5[w] * beta;
    sum6 = sum6 + v6[w] * alpha;
    sum7 = sum7 + v7[w] * beta;
  }

#pragma endmacveth
  v0[0] = sum0;
  v0[1] = sum1;
  v0[2] = sum2;
  v0[3] = sum3;
  v0[4] = sum4;
  v0[5] = sum5;
  v0[6] = sum6;
  v0[7] = sum7;
}