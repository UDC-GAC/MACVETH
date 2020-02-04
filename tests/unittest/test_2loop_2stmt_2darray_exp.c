#include <immintrin.h>
#include <stdlib.h>

void kernel_spmvstyle(int m, int n, double A[m][n], double x[n], double y[n],
                      int uI, int uIt, int uIs, int uJ, int uJt, int uJs) {
  double *p = (double *)malloc(sizeof(double) * 1024);
  double sum = 0.0;

#pragma macveth
  for (int i = 0; i < n; i += 4) {
    for (int j = 0; j < n; j += 4) {
      __m256d vop2, vop0, vop1, vop3, vop4, vop5, vop7, vop6;
      vop0 = _mm256_load_pd(&p[j + 0]);        // cost = 1
      vop1 = _mm256_set_pd(4, 4, 4, 4);        // cost = 1
      vop2 = _mm256_mul_pd(vop0, vop1);        // cost = 3
      _mm256_store_pd(&A[i + 0][j + 0], vop2); // cost = 1
      _mm256_store_pd(&A[i + 1][j + 0], vop2); // cost = 1
      _mm256_store_pd(&A[i + 2][j + 0], vop2); // cost = 1
      _mm256_store_pd(&A[i + 3][j + 0], vop2); // cost = 1
      vop3 = _mm256_load_pd(&y[j + 0]);        // cost = 1
      vop4 = _mm256_mul_pd(vop3, vop1);        // cost = 3
      _mm256_store_pd(&x[j + 0], vop4);        // cost = 1
      // REPLACED A[i][j] = p[j] * 4;
      // REPLACED x[j] = y[j] * 4;
    }
  }
#pragma endmacveth
}