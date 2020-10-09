/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* gesummv.c: this file is part of PolyBench/C */

#include <assert.h>
#include <hdf5.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>

/* Include polybench common header. */
#include "polybench.h"

#define TEST_RESULTS

/* Include benchmark-specific header. */
#include "spmv.h"
#define BLOCKSIZE 1

#define ceild(n, d) ceil(((double)(n)) / ((double)(d)))
#define floord(n, d) floor(((double)(n)) / ((double)(d)))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

// Template for the kernel_spmv() function (to be implemented by each
// independent "codelet")
void kernel_spmv(double *restrict A, double *restrict x, double *restrict y,
                 long long n, long long *indptr, long long *indices);

/* Array initialization. */
static void init_array(char *spfpath, long long *nrow, long long **rowptr,
                       long long **colidx, double **A, double **x, double **y) {

  int debug = 0;

  // Open file
  int fd = open(spfpath, O_RDONLY);

  // Read NNZ, incorporated points, shapes in dictionary, matrix shape (4 x
  // integer)
  int nnz, inc_nnz, nshapes, ncols;
  read(fd, &nnz, sizeof(int));
  read(fd, &inc_nnz, sizeof(int));
  read(fd, &nshapes, sizeof(int));
  int intnrow;
  read(fd, &intnrow, sizeof(int));
  *nrow = intnrow;
  read(fd, &ncols, sizeof(int));
  if (debug) {
    printf("Reading matrix:\n");
    printf("\tNNZ    = %d\n", nnz);
    printf("\tINC    = %d\n", inc_nnz);
    printf("\tshapes = %d\n", nshapes);
    printf("\tdims   = %d x %d\n", *nrow, ncols);
  }

  // Read shapes (FIXME:to do, this is just traversing them)
  if (debug)
    printf("\nReading shapes:\n");
  for (int i = 0; i < nshapes; ++i) {
    if (debug)
      printf("\tShape #%d\n", i);
    // Read dimensions of U
    unsigned char Ushape[2];
    read(fd, Ushape, 2 * sizeof(unsigned char));
    if (debug)
      printf("\t\tShape dims: %d x %d\n", Ushape[0], Ushape[1]);

    // Read U data
    int Udata[Ushape[0] * Ushape[1]];
    read(fd, Udata, Ushape[0] * Ushape[1] * sizeof(int));
    if (debug) {
      printf("\t\tU = [ ");
      for (int j = 0; j < Ushape[0]; ++j) {
        printf("[ ");
        for (int k = 0; k < Ushape[1]; ++k) {
          printf("%d ", Udata[j * Ushape[1] + k]);
        }
        printf("], ");
      }
      printf("]\n");
    }

    // Read w data
    int wdata[Ushape[0]];
    read(fd, wdata, Ushape[0] * sizeof(int));
    if (debug) {
      printf("\t\tw = [ ");
      for (int j = 0; j < Ushape[0]; ++j) {
        printf("%d ", wdata[j]);
      }
      printf("]\n");
    }

    // Read c data
    int cdata[2];
    read(fd, cdata, 2 * sizeof(int));
    if (debug) {
      printf("\t\tc = [ ");
      for (int j = 0; j < 2; ++j) {
        printf("%d ", cdata[j]);
      }
      printf("]\n");
    }

    // Read AST location pointer
    int ast_pointer;
    read(fd, &ast_pointer, sizeof(int));
    if (debug)
      printf("\t\tASTs start at offset: %d\n", ast_pointer);
  }

  // Read AST locations for each shape
  for (int i = 0; i < nshapes; ++i) {
    if (debug)
      printf("\nASTs for shape #%d\n", i);

    // Read number of ASTs
    int nASTs;
    read(fd, &nASTs, sizeof(int));
    if (debug)
      printf("\tNumber of ASTs: %d\n", nASTs);

    // Read pointer to data start
    int data_pointer;
    read(fd, &data_pointer, sizeof(int));
    if (debug)
      printf("\tData starts at offset: %d\n", data_pointer);

    if (debug)
      printf("\tLocations:\n");
    for (int j = 0; j < nASTs; ++j) {
      int loc[2];
      read(fd, loc, 2 * sizeof(int));
      if (debug)
        printf("\t\tAST #%d: ( %d, %d )\n", j, loc[0], loc[1]);
    }
  }

  // Unincorporated metadata
  // Read unincorporated format: CSR, COO
  unsigned char uninc_format;
  read(fd, &uninc_format, sizeof(unsigned char));
  if (debug)
    printf("\nUnincorporated metadata:\n");
  if (debug)
    printf("\tUnincorporated format: %d (0:CSR, 1:COO)\n", uninc_format);

  // Read pointer to start of unincorporated data
  int uninc_data_pointer;
  read(fd, &uninc_data_pointer, sizeof(int));
  if (debug)
    printf("\tOffset to unincorporated data: %d\n", uninc_data_pointer);

  // Read CSR/COO data
  int uninc_nnz = nnz - inc_nnz;
  if (uninc_format == 0) {
    int tmprow[(*nrow) + 1], tmpcol[uninc_nnz];
    *rowptr = (long long *)malloc(sizeof(long long) * ((*nrow) + 1));
    *colidx = (long long *)malloc(sizeof(long long) * uninc_nnz);
    read(fd, tmprow, ((*nrow) + 1) * sizeof(int));
    read(fd, tmpcol, uninc_nnz * sizeof(int));
    for (int i = 0; i < (*nrow) + 1; ++i) {
      (*rowptr)[i] = tmprow[i];
    }
    for (int i = 0; i < uninc_nnz; ++i) { // Convert from int to long long
      (*colidx)[i] = tmpcol[i];
    }
    if (debug) {
      printf("\tRowptr: [ ");
      for (int i = 0; i < (*nrow) + 1; ++i) {
        printf("%d, ", tmprow[i]);
      }
      for (int i = 0; i < uninc_nnz; ++i) {
        printf("%d, ", tmpcol[i]);
      }
      printf("]\n");
    }
  } else if (uninc_format == 1) {
    int tmprow[uninc_nnz], tmpcol[uninc_nnz];
    *rowptr = (long long *)malloc(sizeof(long long) * uninc_nnz);
    *colidx = (long long *)malloc(sizeof(long long) * uninc_nnz);
    read(fd, tmprow, uninc_nnz * sizeof(int));
    read(fd, tmpcol, uninc_nnz * sizeof(int));
    for (int i = 0; i < uninc_nnz; ++i) { // Convert from int to long long
      (*rowptr)[i] = tmprow[i];
      (*colidx)[i] = tmpcol[i];
    }

    if (debug) {
      printf("\tLocations of unincorporated points: [ ");
      for (int i = 0; i < uninc_nnz; ++i) {
        printf("( %d, %d ), ", tmprow[i], tmpcol[i]);
      }
      printf("]\n");
    }
  }

  // Allocate data and read
  *A = (double *)malloc(nnz * sizeof(double));
  read(fd, *A, nnz * sizeof(double));

  if (debug) {
    printf("\nData section: ");
    for (int i = 0; i < nnz; ++i) {
      printf("%lf ", (*A)[i]);
    }
    printf("\n");
    printf("\nFile ended at offset: %d\n", lseek(fd, 0, SEEK_CUR));
  }

  // Allocate and initialize x, y
  *x = (double *)malloc(sizeof(double) * ncols);
  *y = (double *)malloc(sizeof(double) * (*nrow));
  for (int i = 0; i < ncols; i++) {
    (*x)[i] = (double)(i % ncols) / ncols;
  }
  for (int i = 0; i < (*nrow); ++i) {
    (*y)[i] = 0;
  }
}

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static void print_array(int n, DATA_TYPE POLYBENCH_1D(y, N, n))

{
  int i;

  POLYBENCH_DUMP_START;
  POLYBENCH_DUMP_BEGIN("y");
  for (i = 0; i < n; i++) {
    if (i % 20 == 0)
      fprintf(POLYBENCH_DUMP_TARGET, "\n");
    fprintf(POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, y[i]);
  }
  POLYBENCH_DUMP_END("y");
  POLYBENCH_DUMP_FINISH;
}

int main(int argc, char **argv) {
  /* Retrieve problem size. */
  long long nrow;

  /* Variable declaration/allocation. */
  double *A;
  double *x;
  double *y;
  long long *indices;
  long long *indptr;
  char *rbpath;

  if (argc < 2) {
    printf("Usage: spmv_spf <SPF file>\n");
    exit(0);
  }

  rbpath = argv[1];

  /* Initialize array(s). */
  init_array(rbpath, &nrow, &indptr, &indices, &A, &x, &y);

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */

  for (int t = 0; t < 1; ++t) {
    kernel_spmv(A, x, y, nrow, indptr, indices);
  }
  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

#ifdef TEST_RESULTS
  for (int i = 0; i < nrow; ++i) {
    printf("y[%d] = %lf\n", i, y[i]);
  }
#endif

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(nrow, y));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(x);
  POLYBENCH_FREE_ARRAY(y);

  return 0;
}
