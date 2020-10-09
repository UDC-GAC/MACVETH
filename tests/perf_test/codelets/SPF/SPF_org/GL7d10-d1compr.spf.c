#define max(x,y)    ((x) > (y) ? (x) : (y))
#define min(x,y)    ((x) < (y) ? (x) : (y))

void kernel_spmv_fragment_0(double * restrict A, double * restrict x, double * restrict y ) {
  register int i0,i1,i2,i3,i4,i5,i6;

  for( i0 = 0; i0 <= 1; ++i0 ) {
    y[0*i0+0] += A[i0+0] * x[2*i0+42];
  }
  for( i0 = 0; i0 <= 1; ++i0 ) {
    y[0*i0+0] += A[i0+2] * x[2*i0+57];
  }
  for( i0 = 0; i0 <= 1; ++i0 ) {
    y[0*i0+0] += A[i0+4] * x[4*i0+47];
  }
}

void kernel_spmv( double * restrict A, double * restrict x, double * restrict y, long long n, long long * indptr, long long * indices ) {
  register int i, j;

  kernel_spmv_fragment_0( A, x, y );

  for( i = 0; i < 1; ++i ) {
    for( j = indptr[i]; j < indptr[i+1]; ++j ) {
      y[i] = A[j+6] * x[indices[j]] + y[i];
    }
  }
}
