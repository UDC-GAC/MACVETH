/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#include <immintrin.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Include polybench common header. */
#include <polybench.h>

/* Include benchmark-specific header. */
/* Default data type is double, default size is N=1024. */
#include "definitions.h"

//#ifdef REDUX
//#ifdef FUSED
//#include "fused_kernels.h"
//#else
//#include "redux_kernels.h"
//#endif
//#else
//#include "original_kernel.h"
//#endif

/* Array initialization. */
static void init_1darray(int n, DATA_TYPE POLYBENCH_1D(x, N, n)) {
    int i, j;

    for (i = 0; i < n; i++)
        x[i] = 42;
}

static void init_2darray(int n, DATA_TYPE POLYBENCH_2D(C, N, N, n, n)) {
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            C[i][j] = 42;
}

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static void print_value(double S) {
    fprintf(stderr, DATA_PRINTF_MODIFIER, S);
    fprintf(stderr, "\n");
}

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static void print_1darray(int n, DATA_TYPE POLYBENCH_1D(C, N, n)) {
    int i, j;

    for (i = 0; i < n; i++)
        fprintf(stderr, DATA_PRINTF_MODIFIER, C[i]);
    if (i % 20 == 0)
        fprintf(stderr, "\n");
    fprintf(stderr, "\n");
}

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static void kernel_template(int n, double *S, double *F,
        DATA_TYPE POLYBENCH_1D(x, N, n),
        DATA_TYPE POLYBENCH_1D(y, N, n)) {
    double G = (*S);
    double H = (*F);
    double tmp[4] = {G, H, G, H};
    int i, t;
#pragma macveth
    for (t = 0; t < TSTEPS; ++t) {
        __m256d vt;
        vt = _mm256_set_pd(G,H,G,H);
        for (i = 0; i < _PB_N; i += 4) {
            __m256d vop0, vop1;
            vop0 = _mm256_load_pd(&x[i]);
            vop1 = _mm256_load_pd(&y[i]);
            vop0 = _mm256_hadd_pd(vop0, vop1); // cost = 3
            vop0 = _mm256_hadd_pd(vop0, _mm256_permute_pd(vop0,0x11)); // 3
            vt = _mm256_add_pd(vop0, vt); // 3
        }
        _mm256_store_pd(tmp, vt); // 1
    }
#pragma endmacveth
    (*S) = tmp[0];
    (*F) = tmp[1];
}

int main(int argc, char **argv) {
    /* Retrieve problem size. */
    int n = N;

    /* Variable declaration/allocation. */
    POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N, n);
    POLYBENCH_1D_ARRAY_DECL(y, DATA_TYPE, N, n);

    /* Initialize array(s). */
    init_1darray(n, POLYBENCH_ARRAY(x));
    init_1darray(n, POLYBENCH_ARRAY(y));

    double S = 0;
    double F = 0;

    /* Start timer. */

    /* Run kernel. */
    polybench_start_instruments;
    kernel_template(n, &S, &F, POLYBENCH_ARRAY(x), POLYBENCH_ARRAY(y));
    polybench_stop_instruments;
    polybench_print_instruments;
    /* Stop and print timer. */

    /* Prevent dead-code elimination. All live-out data must be printed
       by the function call in argument. */
    // polybench_prevent_dce(print_1darray(n, POLYBENCH_ARRAY(x)));
    polybench_prevent_dce(print_value(S));
    // polybench_prevent_dce(print_1darray(n, POLYBENCH_ARRAY(y)));
    polybench_prevent_dce(print_value(F));

    /* Be clean. */
    POLYBENCH_FREE_ARRAY(x);
    POLYBENCH_FREE_ARRAY(y);

    return 0;
}
