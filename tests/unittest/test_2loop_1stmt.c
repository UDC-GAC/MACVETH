#include <stdlib.h>

void kernel_spmvstyle(int m, int n, double A[m][n], double x[n], double y[n],
                      int uI, int uIt, int uIs, int uJ, int uJt, int uJs) {
  double *p = (double *)malloc(sizeof(double) * 1024);
  double sum = 0.0;

#pragma macveth
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      x[i] = x[i] * 4 + p[i];
    }
  }
#pragma endmacveth
}
