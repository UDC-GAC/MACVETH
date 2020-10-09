#include <immintrin.h>

#define N 1024

float test(float a[256], __m256 x) {
  // float array[256] = {0,1,2,3,4,6,7};
  __m256 init = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, a[0]);
  //__m256 t = _mm256_i32gather_ps(&a[0], _mm256_set_epi32(14,12,10,8,6,4,2, 0),
  // 8); const __m256 t =
  // _mm256_setr_ps(a[0],a[2],a[4],a[6],a[8],a[10],a[12],a[14]);
  __m256 ab = _mm256_hadd_ps(x, init);
  //__m256 s = _mm256_set1_ps(2.0);
  // ab = _mm256_add_ps(ab,s);
  //__m256 bcast = _mm256_broadcast_ss(&a[0]);
  // ab = _mm256_add_ps(ab,bcast);
  __m256 log = _mm256_log_ps(init);
  ab = _mm256_add_ps(ab, log);
  // hiQuad = ( x7, x6, x5, x4 )
  const __m128 hiQuad = _mm256_extractf128_ps(ab, 1);
  return _mm_cvtss_f32(hiQuad);
}
