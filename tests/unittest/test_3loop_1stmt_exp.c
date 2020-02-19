#include <immintrin.h>
#include <stdlib.h>

void kernel_spmvstyle(int m, int n, double X[m][n][n], double A[m][n],
                      double x[n], double y[n], int uI, int uIt, int uIs,
                      int uJ, int uJt, int uJs) {
  double *p = (double *)malloc(sizeof(double) * 1024);
  double sum = 0.0;

#pragma macveth
  int k, j, i;
  for (i = 0; (i + 4) <= n; i += 4) {
    for (j = 0; (j + 4) <= n; j += 4) {
      for (k = 0; (k + 4) <= n; k += 4) {
        __m256d vop2, vop0, vop1, vop3, vop4, vop5, vop7, vop6, vop8, vop9,
            vop10, vop12, vop11, vop13, vop14, vop15, vop17, vop16, vop18,
            vop19, vop20, vop22, vop21, vop23, vop24, vop25, vop26, vop27,
            vop28, vop29, vop30, vop31, vop32, vop33, vop34, vop35, vop36,
            vop37;
        vop0 = _mm256_load_pd(&A[i + 0][k + 0]);         // cost = 1
        vop1 = _mm256_set_pd(4, 4, 4, 4);                // cost = 1
        vop6 = _mm256_load_pd(&A[i + 1][k + 0]);         // cost = 1
        vop11 = _mm256_load_pd(&A[i + 2][k + 0]);        // cost = 1
        vop16 = _mm256_load_pd(&A[i + 3][k + 0]);        // cost = 1
        vop21 = _mm256_load_pd(&x[k + 0]);               // cost = 1
        vop22 = _mm256_fmadd_pd(vop0, vop1, vop21);      // cost = 5
        _mm256_store_pd(&X[i + 0][j + 0][k + 0], vop22); // cost = 1
        vop23 = _mm256_fmadd_pd(vop0, vop1, vop21);      // cost = 5
        _mm256_store_pd(&X[i + 0][j + 1][k + 0], vop23); // cost = 1
        vop24 = _mm256_fmadd_pd(vop0, vop1, vop21);      // cost = 5
        _mm256_store_pd(&X[i + 0][j + 2][k + 0], vop24); // cost = 1
        vop25 = _mm256_fmadd_pd(vop0, vop1, vop21);      // cost = 5
        _mm256_store_pd(&X[i + 0][j + 3][k + 0], vop25); // cost = 1
        vop26 = _mm256_fmadd_pd(vop6, vop1, vop21);      // cost = 5
        _mm256_store_pd(&X[i + 1][j + 0][k + 0], vop26); // cost = 1
        vop27 = _mm256_fmadd_pd(vop6, vop1, vop21);      // cost = 5
        _mm256_store_pd(&X[i + 1][j + 1][k + 0], vop27); // cost = 1
        vop28 = _mm256_fmadd_pd(vop6, vop1, vop21);      // cost = 5
        _mm256_store_pd(&X[i + 1][j + 2][k + 0], vop28); // cost = 1
        vop29 = _mm256_fmadd_pd(vop6, vop1, vop21);      // cost = 5
        _mm256_store_pd(&X[i + 1][j + 3][k + 0], vop29); // cost = 1
        vop30 = _mm256_fmadd_pd(vop11, vop1, vop21);     // cost = 5
        _mm256_store_pd(&X[i + 2][j + 0][k + 0], vop30); // cost = 1
        vop31 = _mm256_fmadd_pd(vop11, vop1, vop21);     // cost = 5
        _mm256_store_pd(&X[i + 2][j + 1][k + 0], vop31); // cost = 1
        vop32 = _mm256_fmadd_pd(vop11, vop1, vop21);     // cost = 5
        _mm256_store_pd(&X[i + 2][j + 2][k + 0], vop32); // cost = 1
        vop33 = _mm256_fmadd_pd(vop11, vop1, vop21);     // cost = 5
        _mm256_store_pd(&X[i + 2][j + 3][k + 0], vop33); // cost = 1
        vop34 = _mm256_fmadd_pd(vop16, vop1, vop21);     // cost = 5
        _mm256_store_pd(&X[i + 3][j + 0][k + 0], vop34); // cost = 1
        vop35 = _mm256_fmadd_pd(vop16, vop1, vop21);     // cost = 5
        _mm256_store_pd(&X[i + 3][j + 1][k + 0], vop35); // cost = 1
        vop36 = _mm256_fmadd_pd(vop16, vop1, vop21);     // cost = 5
        _mm256_store_pd(&X[i + 3][j + 2][k + 0], vop36); // cost = 1
        vop37 = _mm256_fmadd_pd(vop16, vop1, vop21);     // cost = 5
        _mm256_store_pd(&X[i + 3][j + 3][k + 0], vop37); // cost = 1
        // statement replaced: X[i][j][k] = A[i][k] * 4 + x[k];
      }
      for (k = (n / 4) * 4; k < n; ++k) {
        X[i][j][k] = A[i][k] * 4 + x[k];
      }
    }
    for (j = (n / 4) * 4; j < n; ++j) {
      for (k = 0; k < n; ++k) {
        X[i][j][k] = A[i][k] * 4 + x[k];
      }
    }
  }
  for (i = (n / 4) * 4; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      for (k = 0; k < n; ++k) {
        X[i][j][k] = A[i][k] * 4 + x[k];
      }
    }
  }

#pragma endmacveth
}
