#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))
#include <stdio.h>

#define restrict __restrict

void kernel_spmv_fragment_0(double *restrict A, double *restrict x,
                            double *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;

  for (i0 = 0; i0 <= 3; ++i0) {
    y[0 * i0 + 9] += A[i0 + 0] * x[7 * i0 + 4];
  }

  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 0] += A[i0 + 4] * x[2 * i0 + 4];
  }

  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 0] += A[i0 + 7] * x[5 * i0 + 12];
  }
  for (i0 = 0; i0 <= 2; ++i0) {
    y[1 * i0 + 0] += A[i0 + 10] * x[2 * i0 + 3];
  }

  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 0] += A[i0 + 13] * x[2 * i0 + 29];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 9] += A[i0 + 15] * x[2 * i0 + 15];
  }

  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 9] += A[i0 + 17] * x[2 * i0 + 28];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 23] += A[i0 + 19] * x[3 * i0 + 24];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 2] += A[i0 + 21] * x[4 * i0 + 9];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 22] += A[i0 + 23] * x[4 * i0 + 9];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 9] += A[i0 + 25] * x[5 * i0 + 1];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 10] += A[i0 + 27] * x[7 * i0 + 9];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 1] += A[i0 + 29] * x[0 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 21] += A[i0 + 31] * x[0 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 9] += A[i0 + 33] * x[4 * i0 + 22];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[2 * i0 + 32] += A[i0 + 35] * x[2 * i0 + 33];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[3 * i0 + 23] += A[i0 + 37] * x[-7 * i0 + 16];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[4 * i0 + 10] += A[i0 + 39] * x[0 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[4 * i0 + 1] += A[i0 + 41] * x[7 * i0 + 9];
  }
}

void kernel_spmv_fragment_0(double *restrict A, double *restrict x,
                            double *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;

  for (i0 = 0; i0 <= 3; ++i0) {
    y[0 * i0 + 9] += A[i0 + 0] * x[7 * i0 + 4];
  }

  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 0] += A[i0 + 4] * x[2 * i0 + 4];
  }

  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 0] += A[i0 + 7] * x[5 * i0 + 12];
  }
  for (i0 = 0; i0 <= 2; ++i0) {
    y[1 * i0 + 0] += A[i0 + 10] * x[2 * i0 + 3];
  }

  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 0] += A[i0 + 13] * x[2 * i0 + 29];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 9] += A[i0 + 15] * x[2 * i0 + 15];
  }

  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 9] += A[i0 + 17] * x[2 * i0 + 28];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 23] += A[i0 + 19] * x[3 * i0 + 24];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 2] += A[i0 + 21] * x[4 * i0 + 9];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 22] += A[i0 + 23] * x[4 * i0 + 9];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 9] += A[i0 + 25] * x[5 * i0 + 1];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 10] += A[i0 + 27] * x[7 * i0 + 9];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 1] += A[i0 + 29] * x[0 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 21] += A[i0 + 31] * x[0 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 9] += A[i0 + 33] * x[4 * i0 + 22];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[2 * i0 + 32] += A[i0 + 35] * x[2 * i0 + 33];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[3 * i0 + 23] += A[i0 + 37] * x[-7 * i0 + 16];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[4 * i0 + 10] += A[i0 + 39] * x[0 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[4 * i0 + 1] += A[i0 + 41] * x[7 * i0 + 9];
  }
}

void kernel_spmv(double *restrict A, double *restrict x, double *restrict y,
                 long long n, long long *indptr, long long *indices) {
  register int i, j;

  kernel_spmv_fragment_0(A, x, y);

  for (i = 0; i < 7; ++i) {
    y[indptr[i]] += A[43 + i] * x[indices[i]];
  }
}
