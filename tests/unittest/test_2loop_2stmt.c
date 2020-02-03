#include <stdlib.h>

void kernel_spmvstyle(int m, int n, double A[m][n], double x[n], double y[n],
                      int uI, int uIt, int uIs, int uJ, int uJt, int uJs) {
  double *p = (double *)malloc(sizeof(double) * 1024);
  double sum = 0.0;

#pragma macveth
  // for (int i = 0; i < n; ++i) {
  for (int j = 0; j < n; ++j) {
    x[j] = p[j] * 4;
    x[j] = y[j] * 4;
  }
  //}
#pragma endmacveth
}
