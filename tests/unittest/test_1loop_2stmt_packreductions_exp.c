#include <stdlib.h>

void kernel_spmvstyle(int m, int n, double A[m][n], double x[n], double y[n],
                      int uI, int uIt, int uIs, int uJ, int uJt, int uJs) {
  double *p = (double *)malloc(sizeof(double) * 1024);
  double sum = 0.0;
  double S, G;

#pragma macveth
  int i;
  for (i = 0; (i + 4) <= n; i += 4) {
    __m256d vop0, vop1, vop2;
    vop0 = toImplement(vop0, vop1); // cost = 1
    vop0 = toImplement(vop0, vop2); // cost = 1
    // statement replaced: S = S + x[i];
    // statement replaced: G = G + y[i];
  }
  for (i = (n / 4) * 4; i < n; ++i) {
    S = S + x[i];
    G = G + y[i];
  }

#pragma endmacveth
}
