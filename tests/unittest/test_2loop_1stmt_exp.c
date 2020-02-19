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
      __m256d vop2, vop0, vop1, vop3, vop4, vop5, vop6;
      vop0 = _mm256_load_pd(&x[i + 0]);         // cost = 1
      vop1 = _mm256_set_pd(4, 4, 4, 4);         // cost = 1
      vop3 = _mm256_load_pd(&p[i + 0]);         // cost = 1
      vop0 = _mm256_fmadd_pd(vop0, vop1, vop3); // cost = 5
      _mm256_store_pd(&x[i + 0], vop0);         // cost = 1
      vop0 = _mm256_fmadd_pd(vop0, vop1, vop3); // cost = 5
      _mm256_store_pd(&x[i + 0], vop0);         // cost = 1
      vop0 = _mm256_fmadd_pd(vop0, vop1, vop3); // cost = 5
      _mm256_store_pd(&x[i + 0], vop0);         // cost = 1
      vop0 = _mm256_fmadd_pd(vop0, vop1, vop3); // cost = 5
      _mm256_store_pd(&x[i + 0], vop0);         // cost = 1
      // statement replaced: x[i] = x[i] * 4 + p[i];
    }
    for (j = (n / 4) * 4; j < n; ++j) {
      x[i] = x[i] * 4 + p[i];
    }
  }
  for (i = (n / 4) * 4; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      x[i] = x[i] * 4 + p[i];
    }
  }

#pragma endmacveth
}
