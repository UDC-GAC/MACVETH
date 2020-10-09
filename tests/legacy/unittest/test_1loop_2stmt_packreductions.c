#include <stdlib.h>

void kernel_spmvstyle(int m, int n, double A[m][n], double x[n], double y[n],
                      int uI, int uIt, int uIs, int uJ, int uJt, int uJs) {
  double *p = (double *)malloc(sizeof(double) * 1024);
  double sum = 0.0;
  double S, G;

#pragma macveth
  for (int i = 0; i < n; ++i) {
    S = S + x[i];
    G = G + y[i];
  }
#pragma endmacveth
}
