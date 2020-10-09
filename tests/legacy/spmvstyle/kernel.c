#include <polybench.h>

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_spmvstyle(int m, int n,
		 int _uI, int _uIt, int _uIs,
		 int _uJ, int _uJt, int _uJs,
		 DATA_TYPE POLYBENCH_2D(A,M,N,m,n),
		 DATA_TYPE POLYBENCH_1D(x,N,n),
		 DATA_TYPE POLYBENCH_1D(y,N,n))
{
  int i, j;

  for (i = _uI; i < _uI + _uIt; i += _uIs)
      for (j = _uJ; j < _uJ + _uJt; j += _uJs)
        {
          y[i] = y[i] + A[i][j] * x[j];
	}
}

