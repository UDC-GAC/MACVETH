#include <immintrin.h>
#include <stdlib.h>
/// NOT VERY CORRECT
void kernel_spmvstyle(int m, int n, double A[m][n], double x[n], double y[n],
                      int uI, int uIt, int uIs, int uJ, int uJt, int uJs) {
  double *p = (double *)malloc(sizeof(double) * 1024);
  double sum = 0.0;

#pragma macveth
  for (int i = 0; i < n; i += 4) {
    for (int j = 0; j < n; j += 4) {
      __m256d vop2, vop0, vop1, vop4, vop3, vop6, vop5, vop8, vop7, vop9, vop10,
          vop11, vop12;
      vop0 = _mm256_broadcast_pd(&x[i + 0]);     // cost = 1
      vop1 = _mm256_set_pd(4, 4, 4, 4);          // cost = 1
      vop3 = _mm256_broadcast_pd(&x[i + 1]);     // cost = 1
      vop5 = _mm256_broadcast_pd(&x[i + 2]);     // cost = 1
      vop7 = _mm256_broadcast_pd(&x[i + 3]);     // cost = 1
      vop9 = _mm256_broadcast_pd(&p[i + 0]);     // cost = 1
      vop0 = _mm256_fmadd_pd(vop9, vop0, vop1);  // cost = 5
      _mm256_store_pd(&x[i + 0], vop0);          // cost = 1
      vop10 = _mm256_broadcast_pd(&p[i + 1]);    // cost = 1
      vop3 = _mm256_fmadd_pd(vop10, vop3, vop1); // cost = 5
      _mm256_store_pd(&x[i + 1], vop3);          // cost = 1
      vop11 = _mm256_broadcast_pd(&p[i + 2]);    // cost = 1
      vop5 = _mm256_fmadd_pd(vop11, vop5, vop1); // cost = 5
      _mm256_store_pd(&x[i + 2], vop5);          // cost = 1
      vop12 = _mm256_broadcast_pd(&p[i + 3]);    // cost = 1
      vop7 = _mm256_fmadd_pd(vop12, vop7, vop1); // cost = 5
      _mm256_store_pd(&x[i + 3], vop7);          // cost = 1
      // REPLACED x[i] = x[i] * 4 + p[i];
    }
  }
#pragma endmacveth
}
