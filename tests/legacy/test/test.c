#include <immintrin.h>

void reduction(__m256 A0, __m256 A1, __m256 A2, __m256 A3, __m256 A4, __m256 A5,
               __m256 A6, __m256 A7) {
  __m256 A01 = _mm256_hadd_ps(A0, A1);
  __m256 A23 = _mm256_hadd_ps(A2, A3);
  __m256 A45 = _mm256_hadd_ps(A4, A5);
  __m256 A67 = _mm256_hadd_ps(A6, A7);
  __m256 A0123 = _mm256_hadd_ps(A01, A23);
  __m256 A4567 = _mm256_hadd_ps(A01, A23);
}