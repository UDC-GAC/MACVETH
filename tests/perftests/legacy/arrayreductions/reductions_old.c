#include <immintrin.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Include polybench common header. */
#include "polybench.h"

/* Include benchmark-specific header. */
/* Default data type is double, default size is N=1024. */
#include "../definitions.h"

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

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static void print_2darray(int n, DATA_TYPE POLYBENCH_2D(C, N, N, n, n)) {
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            fprintf(stderr, DATA_PRINTF_MODIFIER, C[i][j]);
    if (i % 20 == 0)
        fprintf(stderr, "\n");
    fprintf(stderr, "\n");
}

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static void kernel_template(int n, double *S, double *F,
        DATA_TYPE POLYBENCH_2D(A, N, N, n, n),
        DATA_TYPE POLYBENCH_2D(B, N, N, n, n),
        DATA_TYPE POLYBENCH_1D(x, N, n),
        DATA_TYPE POLYBENCH_1D(y, N, n)) {
    double G = (*S);
    double H = (*F);
    int i, j, t;
#pragma macveth
    for (t = 0; t < TSTEPS; ++t) {
        __m256d ymm0, ymm1, ymm2, ymm3, ymm4, vop0, vop1, vop2;
        for (i = 0; i < _PB_N; i+= 4) {
            for (j = 0; j < _PB_N; j+=4) {
                vop0 = _mm256_loadu_pd(&x[i + 0]);    // cost = 1
                vop1 = _mm256_loadu_pd(&A[i + 0][j + 0]);         // cost = 1
                ymm0 = _mm256_permute_pd(vop1, 0x05);            // cost = 1
                ymm1 = _mm256_add_pd(vop1, ymm0);                // cost = 1
                ymm2 = _mm256_permute2f128_pd(ymm1, ymm1, 0x01); // cost = 1
                ymm3 = _mm256_add_pd(ymm2, ymm1);                // cost = 1
                ymm4 = _mm256_permute4x64_pd(vop0, 0x00);
                vop2 = _mm256_add_pd(ymm4, ymm3);                // cost = 1


                vop1 = _mm256_loadu_pd(&A[i + 1][j + 0]);         // cost = 1
                ymm0 = _mm256_permute_pd(vop1, 0x05);            // cost = 1
                ymm1 = _mm256_add_pd(vop1, ymm0);                // cost = 1
                ymm2 = _mm256_permute2f128_pd(ymm1, ymm1, 0x01); // cost = 1
                ymm3 = _mm256_add_pd(ymm2, ymm1);                // cost = 1
                ymm4 = _mm256_permute4x64_pd(vop0, 0x55);
                vop2 = _mm256_blendv_pd(vop2,_mm256_add_pd(ymm4, ymm3),_mm256_set_pd(0,0,0xffffffffffffffff,0));                // cost = 2

                vop1 = _mm256_loadu_pd(&A[i + 2][j + 0]);         // cost = 1
                ymm0 = _mm256_permute_pd(vop1, 0x05);            // cost = 1
                ymm1 = _mm256_add_pd(vop1, ymm0);                // cost = 1
                ymm2 = _mm256_permute2f128_pd(ymm1, ymm1, 0x01); // cost = 1
                ymm3 = _mm256_add_pd(ymm2, ymm1);                // cost = 1
                ymm4 = _mm256_permute4x64_pd(vop0, 0xaa);
                vop2 = _mm256_blendv_pd(vop2,_mm256_add_pd(ymm4, ymm3),_mm256_set_pd(0,0xffffffffffffffff,0,0));                // cost = 2

                vop1 = _mm256_loadu_pd(&A[i + 3][j + 0]);         // cost = 1
                ymm0 = _mm256_permute_pd(vop1, 0x05);            // cost = 1
                ymm1 = _mm256_add_pd(vop1, ymm0);                // cost = 1
                ymm2 = _mm256_permute2f128_pd(ymm1, ymm1, 0x01); // cost = 1
                ymm3 = _mm256_add_pd(ymm2, ymm1);                // cost = 1
                ymm4 = _mm256_permute4x64_pd(vop0, 0xff);
                vop2 = _mm256_blendv_pd(vop2,_mm256_add_pd(ymm4, ymm3),_mm256_set_pd(0xffffffffffffffff,0,0,0));                // cost = 2
                _mm256_storeu_pd(&x[i + 0], vop2);                       // cost = 1
                // statement replaced: x[i] = x[i] + A[i][j];
            }
        }
    }
#pragma endmacveth
    (*S) = G;
    (*F) = H;
}

int main(int argc, char **argv) {
    /* Retrieve problem size. */
    int n = N;

    /* Variable declaration/allocation. */
    POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, N, N, n, n);
    POLYBENCH_2D_ARRAY_DECL(B, DATA_TYPE, N, N, n, n);
    POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N, n);
    POLYBENCH_1D_ARRAY_DECL(y, DATA_TYPE, N, n);

    /* Initialize array(s). */
    init_2darray(n, POLYBENCH_ARRAY(A));
    init_2darray(n, POLYBENCH_ARRAY(B));
    init_1darray(n, POLYBENCH_ARRAY(x));
    init_1darray(n, POLYBENCH_ARRAY(y));

    double S = 0;
    double F = 0;

    /* Start timer. */
    polybench_start_instruments;

    /* Run kernel. */
    kernel_template(n, &S, &F, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B), POLYBENCH_ARRAY(x), POLYBENCH_ARRAY(y));

    /* Stop and print timer. */
    polybench_stop_instruments;
    polybench_print_instruments;

    /* Prevent dead-code elimination. All live-out data must be printed
       by the function call in argument. */
    //polybench_prevent_dce(print_2darray(n, POLYBENCH_ARRAY(A)));
    polybench_prevent_dce(print_1darray(n, POLYBENCH_ARRAY(x)));
    //polybench_prevent_dce(print_value(S));
    //polybench_prevent_dce(print_2darray(n, POLYBENCH_ARRAY(B)));
    //polybench_prevent_dce(print_value(F));

    /* Be clean. */
    POLYBENCH_FREE_ARRAY(A);
    POLYBENCH_FREE_ARRAY(B);
    POLYBENCH_FREE_ARRAY(x);
    POLYBENCH_FREE_ARRAY(y);

    return 0;
}
