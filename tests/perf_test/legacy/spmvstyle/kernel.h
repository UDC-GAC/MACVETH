#include <polybench.h>

static
void kernel_spmvstyle(int m, int n,
		 int uI, int uIt, int uIs,
		 int uJ, int uJt, int uJs,
		 DATA_TYPE POLYBENCH_2D(A,M,N,m,n),
		 DATA_TYPE POLYBENCH_1D(x,N,n),
		 DATA_TYPE POLYBENCH_1D(y,N,n))
