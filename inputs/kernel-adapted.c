/**
 * File              : kernel-adapted.c
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 07 Nov 2019 15:30:00 MST
 * Last Modified Date: Xov 07 Nov 2019 15:45:03 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
void kernel_spmvstyle(int m, int n, double A[m][n], double x[n], double y[n],
                      int uI, int uIt, int uIs, int uJ, int uJt, int uJs) {
    for (int i = uI; i < uI + uIt; i += uIs)
        for (int j = uJ; j < uJ + uJt; j += uJs) {
            y[i] = y[i] + A[i][j] * x[j];
        }
}
