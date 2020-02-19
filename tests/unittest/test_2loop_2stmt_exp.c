#include <immintrin.h>
#include <stdlib.h>

void kernel_spmvstyle(int m, int n, double A[m][n], double x[n], double y[n],
                      int uI, int uIt, int uIs, int uJ, int uJt, int uJs) {
  double *p = (double *)malloc(sizeof(double) * 1024);
  double sum = 0.0;

#pragma macveth
  int j, i;
  for (i = 0; (i + 4) <= n; i += 4) {
    for (j = 0; (j + 4) <= n; j += 4) {
      __m256d vop2, vop0, vop1, vop3;
      vop0 = _mm256_load_pd(&p[j + 0]); // cost = 1
      vop1 = _mm256_set_pd(4, 4, 4, 4); // cost = 1
      vop2 = _mm256_mul_pd(vop0, vop1); // cost = 5
      _mm256_store_pd(&x[j + 0], vop2); // cost = 1
      vop2 = _mm256_mul_pd(vop0, vop1); // cost = 5
      _mm256_store_pd(&x[j + 0], vop2); // cost = 1
      vop2 = _mm256_mul_pd(vop0, vop1); // cost = 5
      _mm256_store_pd(&x[j + 0], vop2); // cost = 1
      vop2 = _mm256_mul_pd(vop0, vop1); // cost = 5
      _mm256_store_pd(&x[j + 0], vop2); // cost = 1
      // statement replaced: x[j] = p[j] * 4;
      vop3 = _mm256_load_pd(&y[j + 0]); // cost = 1
      vop2 = _mm256_mul_pd(vop3, vop1); // cost = 5
      _mm256_store_pd(&x[j + 0], vop2); // cost = 1
      vop2 = _mm256_mul_pd(vop3, vop1); // cost = 5
      _mm256_store_pd(&x[j + 0], vop2); // cost = 1
      vop2 = _mm256_mul_pd(vop3, vop1); // cost = 5
      _mm256_store_pd(&x[j + 0], vop2); // cost = 1
      vop2 = _mm256_mul_pd(vop3, vop1); // cost = 5
      _mm256_store_pd(&x[j + 0], vop2); // cost = 1
      // statement replaced: x[j] = y[j] * 4;
    }
    for (j = (n / 4) * 4; j < n; ++j) {
      x[j] = p[j] * 4;
      x[j] = y[j] * 4;
    }
  }
  for (i = (n / 4) * 4; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      x[j] = p[j] * 4;
      x[j] = y[j] * 4;
    }
  }

#pragma endmacveth
}
