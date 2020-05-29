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
#if defined(DATA_TYPE_IS_FLOAT)
#define STRIDE 8
#define MM_REG __m256
#define MM_LD(x) _mm256_loadu_ps(x)
#define MM_ST(addr,x) _mm256_storeu_ps(addr,x)
#define MM_ADD(a,b) _mm256_add_ps(a,b)
#define MM_REDUX(addr,a) \
    a = _mm256_hadd_ps(a,a);\
a = _mm256_hadd_ps(a,_mm256_permutevar8x32_ps(a,_mm256_set_epi32(7,6,3,2,5,4,1,0)));\
a = _mm256_hadd_ps(a,a);\
addr = _mm256_cvtss_f32(a);

#elif defined(DATA_TYPE_IS_DOUBLE)
#define STRIDE 4
#define MM_REG __m256d
#define MM_LD(x) _mm256_loadu_pd(x)
#define MM_ST(x) _mm256_storeu_pd(addr,x)
#define MM_ADD(a,b) _mm256_add_pd(a,b)
#define MM_REDUX2(addr1,addr2,a,b) \
    a = _mm256_hadd_pd(a,b);\
a = _mm256_shuffle();\
a = _mm256_hadd_pd(a,a);\
addr1 = _mm256_cvtsd_f64(a);\
addr1 = _mm256_cvtsd_f64(a);

#endif

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
        for (i = 0; i < _PB_N; ++i) {
            for (j = 0; j < _PB_N; ++j) {
                x[i] = x[i] + A[i][j];
                y[i] = y[i] + B[i][j];
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
    polybench_prevent_dce(print_1darray(n, POLYBENCH_ARRAY(x)));
    polybench_prevent_dce(print_value(S));
    polybench_prevent_dce(print_1darray(n, POLYBENCH_ARRAY(y)));
    polybench_prevent_dce(print_value(F));

    /* Be clean. */
    POLYBENCH_FREE_ARRAY(x);
    POLYBENCH_FREE_ARRAY(y);

    return 0;
}
