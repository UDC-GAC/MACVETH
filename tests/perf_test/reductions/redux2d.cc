
#include "redux.h"
#include <algorithm>
#include <immintrin.h>
#include <math.h>
#include <stdio.h>
#include <vector>

using namespace std;

// ---------------------------------------------
long redux_flops(quote_vector2D(bottom)) {
  int channel = bottom.size();
  int width = bottom[0].size();
  return channel * width * 1;
}

void redux(quote_vector2D(bottom)) {
  int channel = bottom.size();
  int width = bottom[0].size();
  int a, b, x, d;
  int h = 0;
  DATA_TYPE sum = 0.0;
#pragma macveth c 1 w 8
  for (int c = 0; c < channel; ++c) {
    for (int w = 0; w < width; ++w) {
      sum = sum + bottom[c][w];
    }
  }
#pragma endmacveth
  bottom[0][0] = sum;
}