/* 
 *  This is the default license template.
 *  
 *  File: kern.c
 *  Author: markoshorro
 *  Copyright (c) 2020 markoshorro
 *  
 *  To edit this license information: Press Ctrl+Shift+P and press 'Create new License Template...'.
 */

/**
 * File              : kern.c
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 07 Nov 2019 15:30:00 MST
 * Last Modified Date: Xov 19 Mar 2020 12:06:09 CET
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
// for max
#include <stdlib.h>
#define N 10

#define LOOPBOUND(x, y) x
#define _N LOOPBOUND(N, n)
void test() {}
void kernel_spmvstyle2(int m, int n, double A[m][n], double x[n], double y[n],
                       int uI, int uIt, int uIs, int uJ, int uJt, int uJs) {
  // double *p = (double *)malloc(sizeof(double) * 1024);
  // double *c = (double *)malloc(sizeof(double) * 1024);
  double sum = 0.0;

  // docs on what is handled
  //#pragma unroll=12,1,12
  //#pragma aligned x[0] 64
  //#pragma tc > 10

  //#pragma macveth
  //  for (int i = 0; i < n; ++i) {
  //    x[i] = p[i] * 4;
  //    y[i] = p[i] * 4;
  //    c[i] = p[i] * 4;
  //  }
  //#pragma endmv
  //#pragma macveth
  //  for (int i = 0; i < n; ++i) {
  //    x[i] = p[i] * 4.0;
  //    sum = sum + (p[i] + (p[i] * x[i]));
  //  }
  //#pragma endmv
  // int i = 0;
  double S, G;
  // int i;
  int j = 10;
#pragma macveth
  A[1][10] += x[0] * x[1];
  A[2][10] += x[0] * x[1];
  A[3][10] += x[0] * x[1];
  for (int i = 0; i < 4; i++) {
    //  A[44][10] += x[0] * x[1];
    for (j = 0; j < 4; j += 1) {
      A[i][j] = x[i] * 42.3;
      // G = G + y[i];
    }
    A[3][10] += x[0] * x[2];
  }
#pragma endmacveth
  //#pragma macveth
  //  for (int i = 0; i < n; ++i) {
  //    for (int j = 0; j < n; ++j) {
  //      sum = sum + A[i][j];
  //    }
  //  }
  //#pragma endmacveth
}
void kernel_spmvstyle(int m, int n, double A[m][n], double x[n], double y[n],
                      int uI, int uIt, int uIs, int uJ, int uJt, int uJs) {
  // double *p = (double *)malloc(sizeof(double) * 1024);
  // double *c = (double *)malloc(sizeof(double) * 1024);
  double sum = 0.0;

  // docs on what is handled
  //#pragma unroll=12,1,12
  //#pragma aligned x[0] 64
  //#pragma tc > 10

  //#pragma macveth
  //  for (int i = 0; i < n; ++i) {
  //    x[i] = p[i] * 4;
  //    y[i] = p[i] * 4;
  //    c[i] = p[i] * 4;
  //  }
  //#pragma endmv
  //#pragma macveth
  //  for (int i = 0; i < n; ++i) {
  //    x[i] = p[i] * 4.0;
  //    sum = sum + (p[i] + (p[i] * x[i]));
  //  }
  //#pragma endmv
  // int i = 0;
  double S, G;
  // int i;
  int j = 10;
#pragma macveth
  A[1][10] += x[0] * x[1];
  A[2][10] += x[0] * x[1];
  A[3][10] += x[0] * x[1];
  for (int i = 0; i < 4; i++) {
    //  A[44][10] += x[0] * x[1];
    for (j = 0; j < 4; j += 1) {
      A[i][j] = x[i] * 42.3;
      // G = G + y[i];
    }
    A[3][10] += x[0] * x[2];
  }
#pragma endmacveth
  //#pragma macveth
  //  for (int i = 0; i < n; ++i) {
  //    for (int j = 0; j < n; ++j) {
  //      sum = sum + A[i][j];
  //    }
  //  }
  //#pragma endmacveth
}

int main() { return 0; }
