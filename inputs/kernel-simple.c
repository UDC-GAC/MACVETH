/**
 * File              : kernel-2simpleloop.c
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 07 Nov 2019 15:30:00 MST
 * Last Modified Date: Lun 18 Nov 2019 09:59:11 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
void kernel_spmvstyle(int m, int n, double A[m][n], double x[n], double y[n],
                      int uI, int uIt, int uIs, int uJ, int uJt, int uJs) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; j++) {
            y[i] = A[i][j] * 5.;
        }
}
