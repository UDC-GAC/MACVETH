/**
 * File              : cfunc_with_if.c
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 08 Nov 2019 12:48:51 MST
 * Last Modified Date: Ven 08 Nov 2019 12:48:51 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
void foo(double* a, double* b, double* c, int N) {
    for (int i = 0; i < N; ++i) {
        // c[i] += a[i] * b[i];
        c[i] = c[i] + a[i] * b[i];
    }
}
