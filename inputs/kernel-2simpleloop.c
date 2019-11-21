/**
 * File              : kernel-2simpleloop.c
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 07 Nov 2019 15:30:00 MST
 * Last Modified Date: Mar 19 Nov 2019 13:02:18 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
void kernel_spmvstyle(int m, int n, double B[m][n][n], double A[m][n],
                      double x[n], double y[n], int uI, int uIt, int uIs,
                      int uJ, int uJt, int uJs) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; j++) {
            y[i] = y[i] + B[i][j][i] * x[j];
            //            y[i] = y[i] + A[i][j] * x[j];
            //            y[i] = y[i] + A[i][j] * x[j];
            //            y[i] = y[i] + A[i][j] * x[j];
            //            y[i] = y[i] + A[i][j] * x[j];
            //            y[i] *= 2.0;
            //            y[i] *= (x[j] + 2.0);
        }
}
