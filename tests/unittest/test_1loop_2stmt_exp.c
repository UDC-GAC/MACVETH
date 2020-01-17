#include <stdlib.h>

void kernel_spmvstyle(int m, int n, double A[m][n], double x[n], double y[n],
                      int uI, int uIt, int uIs, int uJ, int uJt, int uJs) {
  double *p = (double *)malloc(sizeof(double) * 1024);
  double sum = 0.0;

#pragma macveth
  for (int i = 0; i < n; i += 4) {
    __m256d vop2, vop0, vop1, vop4, vop3;
    vop0 = _mm256_load_pd(&p[i + 0]); // cost = 1
    vop1 = _mm256_set_pd(4, 4, 4, 4); // cost = 1
    vop2 = _mm256_mul_pd(vop0, vop1); // cost = 3
    _mm256_store_pd(&x[i + 0], vop2); // cost = 1
    vop3 = _mm256_load_pd(&y[i + 0]); // cost = 1
    vop4 = _mm256_mul_pd(vop3, vop1); // cost = 3
    _mm256_store_pd(&x[i + 0], vop4); // cost = 1
    // REPLACED x[i] = p[i] * 4;
    // REPLACED x[i] = y[i] * 4;
  }
#pragma endmacveth
}
