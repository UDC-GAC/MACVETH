
#include "redux.h"
//#include "utilities/polybench.h"
#include <algorithm>
#include <immintrin.h>
#include <math.h>
#include <stdio.h>
#include <vector>

void init_array(DATA_TYPE *A, int n) {
  for (int i = 0; i < n; ++i) {
    A[i] = i + 0.1;
  }
}

// ---------------------------------------------
long redux_flops(red_vec(v0)) {
  int channel = v0.size();
  return channel * 2 * 1;
}

void redux(red_vec(v0)) {
  int width = v0.size();
  int a, b, x, d;
  int h = 0;
  DATA_TYPE sum = 0.0;
  DATA_TYPE alpha = 1.1f;
#pragma macveth w 8
  for (int w = 0; w < width; ++w) {
    sum = sum + v0[w] * alpha;
  }
#pragma endmacveth
  v0[0] = sum;
}

// ---------------------------------------------
long redux2_flops(red_vec(v0)) {
  int channel = v0.size();
  return channel * 2 * 2;
}

void redux2(red_vec(v0)) {
  int width = v0.size();
  int a, b, x, d;
  int h = 0;

  DATA_TYPE v1[width];
  init_array(v1, width);

  DATA_TYPE sum0 = 0.0;
  DATA_TYPE sum1 = 0.0;
  DATA_TYPE alpha = 1.1f;
  DATA_TYPE beta = 0.9f;
#pragma macveth w 8
  for (int w = 0; w < width; ++w) {
    sum0 = sum0 + v0[w] * alpha;
    sum1 = sum1 + v1[w] * beta;
  }
#pragma endmacveth
  v0[0] = sum0;
  v0[1] = sum1;
}

// ---------------------------------------------
long redux4_flops(red_vec(v0)) {
  int channel = v0.size();
  return channel * 2 * 4;
}

void redux4(red_vec(v0)) {
  int width = v0.size();

  DATA_TYPE v1[width];
  init_array(v1, width);
  DATA_TYPE v2[width];
  init_array(v2, width);
  DATA_TYPE v3[width];
  init_array(v3, width);

  DATA_TYPE sum0 = 0.0;
  DATA_TYPE sum1 = 0.0;
  DATA_TYPE sum2 = 0.0;
  DATA_TYPE sum3 = 0.0;
  DATA_TYPE alpha = 1.1f;
  DATA_TYPE beta = 0.9f;
#pragma macveth w 8
  for (int w = 0; w < width; ++w) {
    sum0 = sum0 + v0[w] * alpha;
    sum1 = sum1 + v1[w] * beta;
    sum2 = sum2 + v2[w] * beta;
    sum3 = sum3 + v3[w] * beta;
  }
#pragma endmacveth
  v0[0] = sum0;
  v0[1] = sum1;
  v0[2] = sum2;
  v0[3] = sum3;
}

// ---------------------------------------------
long redux8_flops(red_vec(v0)) {
  int channel = v0.size();
  return channel * 2 * 8;
}

void redux8(red_vec(v0)) {
  int width = v0.size();

  DATA_TYPE v1[width];
  init_array(v1, width);
  DATA_TYPE v2[width];
  init_array(v2, width);
  DATA_TYPE v3[width];
  init_array(v3, width);
  DATA_TYPE v4[width];
  init_array(v4, width);
  DATA_TYPE v5[width];
  init_array(v5, width);
  DATA_TYPE v6[width];
  init_array(v6, width);
  DATA_TYPE v7[width];
  init_array(v7, width);

  DATA_TYPE sum0 = 0.0;
  DATA_TYPE sum1 = 0.0;
  DATA_TYPE sum2 = 0.0;
  DATA_TYPE sum3 = 0.0;
  DATA_TYPE sum4 = 0.0;
  DATA_TYPE sum5 = 0.0;
  DATA_TYPE sum6 = 0.0;
  DATA_TYPE sum7 = 0.0;
  DATA_TYPE alpha = 1.1f;
  DATA_TYPE beta = 0.9f;
#pragma macveth w 8
  for (int w = 0; w < width; ++w) {
    sum0 = sum0 + v0[w] * alpha;
    sum1 = sum1 + v1[w] * beta;
    sum2 = sum2 + v2[w] * alpha;
    sum3 = sum3 + v3[w] * beta;
    sum4 = sum4 + v4[w] * alpha;
    sum5 = sum5 + v5[w] * beta;
    sum6 = sum6 + v6[w] * alpha;
    sum7 = sum7 + v7[w] * beta;
  }
#pragma endmacveth
  v0[0] = sum0;
  v0[1] = sum1;
  v0[2] = sum2;
  v0[3] = sum3;
  v0[4] = sum4;
  v0[5] = sum5;
  v0[6] = sum6;
  v0[7] = sum7;
}