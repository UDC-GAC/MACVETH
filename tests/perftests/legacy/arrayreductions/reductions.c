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
#define MM_REDUX(addr,a) \
    a = _mm256_hadd_pd(a,_mm256_permute4x64_pd(a,0x4e));\
a = _mm256_hadd_pd(a,a);\
addr = _mm256_cvtsd_f64(a);

#endif

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static void kernel_template(int n, double *S, double *F,
        DATA_TYPE POLYBENCH_2D(A, N, N, n, n),
        DATA_TYPE POLYBENCH_2D(B, N, N, n, n),
        DATA_TYPE POLYBENCH_1D(x, N, n),
        DATA_TYPE POLYBENCH_1D(y, N, n)) {
    //double G = (*S);
    //double H = (*F);
    int i, j, t;
#pragma macveth
    MM_REG vop1,vop2,vop3,vop4,vop5,vop6,vop7,vop8;
    for (t = 0; t < TSTEPS; ++t) {
        for (i = 0; i < _PB_N; i+=1) {
            MM_REG accm = {0};
            //MM_REG accm1 = {0};
            //MM_REG accm2 = {0};
            //MM_REG accm3 = {0};
            //MM_REG accm4 = {0};
            //MM_REG accm5 = {0};
            //MM_REG accm6 = {0};
            //MM_REG accm7 = {0};
            for (j = 0; j < _PB_N; j+=STRIDE) {
                vop1 = MM_LD(&A[i][j]);
                accm = MM_ADD(accm, vop1);
                //vop2 = MM_LD(&A[i+1][j]);
                //accm1 = MM_ADD(accm1, vop2);
                //vop3 = MM_LD(&A[i+2][j]);
                //accm2 = MM_ADD(accm2, vop3);
                //vop4 = MM_LD(&A[i+3][j]);
                //accm3 = MM_ADD(accm3, vop4);
                //vop5 = MM_LD(&A[i+4][j]);
                //accm4 = MM_ADD(accm4, vop5);
                //vop6 = MM_LD(&A[i+5][j]);
                //accm5 = MM_ADD(accm5, vop6);
                //vop7 = MM_LD(&A[i+6][j]);
                //accm6 = MM_ADD(accm6, vop7);
                //vop8 = MM_LD(&A[i+7][j]);
                //accm7 = MM_ADD(accm7, vop8);
            }
            MM_REDUX(x[i], accm);
            //MM_REDUX(x[i+1], accm1);
            //MM_REDUX(x[i+2], accm2);
            //MM_REDUX(x[i+3], accm3);
            //MM_REDUX(x[i+4], accm4);
            //MM_REDUX(x[i+5], accm5);
            //MM_REDUX(x[i+6], accm6);
            //MM_REDUX(x[i+7], accm7);
        }
    }
#pragma endmacveth
    //(*S) = G;
    //(*F) = H;
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
    //init_1darray(n, POLYBENCH_ARRAY(x));
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
    //polybench_prevent_dce(print_value(S));
    //polybench_prevent_dce(print_1darray(n, POLYBENCH_ARRAY(y)));
    //polybench_prevent_dce(print_value(F));

    /* Be clean. */
    POLYBENCH_FREE_ARRAY(A);
    POLYBENCH_FREE_ARRAY(B);
    POLYBENCH_FREE_ARRAY(x);
    POLYBENCH_FREE_ARRAY(y);

    return 0;
}
