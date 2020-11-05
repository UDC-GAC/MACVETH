/* begin INFO MACVETH: headers added */
#include <immintrin.h>
/* end INFO MACVETH */

#include "layers.h"
//#include "utilities/polybench.h"
#include <algorithm>
#include <immintrin.h>
#include <math.h>
#include <stdio.h>
#include <vector>

using namespace std;

#define vector4D(name, n, c, h, w)                                             \
  vector<vector<vector<vector<float>>>> name(                                  \
      n, vector<vector<vector<float>>>(                                        \
             c, vector<vector<float>>(h, vector<float>(w))))
#define vector3D(name, c, h, w)                                                \
  vector<vector<vector<float>>> name(                                          \
      c, vector<vector<float>>(h, vector<float>(w)))
#define quote_vector4D(name) vector<vector<vector<vector<float>>>> &name
#define quote_vector3D(name) vector<vector<vector<float>>> &name

// ---------------------------------------------
long relu_flops(quote_vector3D(bottom)) {
  int channel = bottom.size();
  int width = bottom[0].size();
  int height = bottom[0][0].size();
  return channel * height * width * 1;
}

void relu(quote_vector3D(bottom)) {
  int channel = bottom.size();
  int width = bottom[0].size();
  int height = bottom[0][0].size();
  int a, b, x, d;
  int h = 0;
#pragma macveth c 1 h 1 w 8
  int c;
  __m256 __mv_vop0, __mv_vop1;
  for (c = 0; (c + 1) <= channel; c += 1) {
    int h;
    for (h = 0; (h + 1) <= height; h += 1) {
      int w;
      for (w = 0; (w + 8) <= width; w += 8) {
        __mv_vop0 = _mm256_loadu_ps(&bottom[(c + 0)][(h + 0)][(w + 0)]);
        __mv_vop1 = _mm256_set1_ps(0.0f);
        __mv_vop0 = _mm256_max_ps(__mv_vop0, __mv_vop1);
        _mm256_storeu_ps(&bottom[(c + 0)][(h + 0)][(w + 0)], __mv_vop0);
        // bottom[c][h][w] = std::max(bottom[c][h][w], 0.0f);
      }
      for (w = w; w < width; w += 1) {
        bottom[c][h][w] = std::max(bottom[c][h][w], 0.0f);
      }
    }
    for (h = h; h < height; h += 1) {
      for (int w = 0; w < width; ++w) {
        bottom[c][h][w] = std::max(bottom[c][h][w], 0.0f);
      };
    }
  }
  for (c = c; c < channel; c += 1) {
    for (int h = 0; h < height; ++h) {
      for (int w = 0; w < width; ++w) {
        bottom[c][h][w] = std::max(bottom[c][h][w], 0.0f);
      }
    };
  }

#pragma endmacveth
}

// ---------------------------------------------
long relu_flops(vector<float> &bottom) {
  int count = bottom.size();
  return count;
}

void relu(vector<float> &bottom) {
  int count = bottom.size();
#pragma macveth i 8
  int i;
  __m256 __mv_vop0, __mv_vop1;
  for (i = 0; (i + 8) <= count; i += 8) {
    __mv_vop0 = _mm256_loadu_ps(&bottom[(i + 0)]);
    __mv_vop1 = _mm256_set1_ps(0.0f);
    __mv_vop0 = _mm256_max_ps(__mv_vop0, __mv_vop1);
    _mm256_storeu_ps(&bottom[(i + 0)], __mv_vop0);
    // bottom[i] = std::max(bottom[i], 0.0f);
  }
  for (i = i; i < count; i += 1) {
    bottom[i] = std::max(bottom[i], 0.0f);
  }

#pragma endmacveth
}

// ---------------------------------------------
long dropout_flops(vector<float> &bottom, float ratio) {
  int count = bottom.size();
  return count;
}
void dropout(vector<float> &bottom, float ratio) {
  int count = bottom.size();
#pragma macveth i 8
  int i;
  __m256 __mv_vop0, __mv_vop1;
  for (i = 0; (i + 8) <= count; i += 8) {
    __mv_vop0 = _mm256_loadu_ps(&bottom[(i + 0)]);
    __mv_vop1 = _mm256_broadcast_ss(&ratio);
    __mv_vop0 = _mm256_mul_ps(__mv_vop0, __mv_vop1);
    _mm256_storeu_ps(&bottom[(i + 0)], __mv_vop0);
    // bottom[i] = bottom[i] * ratio;
  }
  for (i = i; i < count; i += 1) {
    bottom[i] = bottom[i] * ratio;
  }

#pragma endmacveth
}

// ---------------------------------------------
long dropout_flops(quote_vector3D(bottom), float ratio) {
  int channel = bottom.size();
  int width = bottom[0].size();
  int height = bottom[0][0].size();
  return channel * width * height;
}

void dropout(quote_vector3D(bottom), float ratio) {
  int channel = bottom.size();
  int width = bottom[0].size();
  int height = bottom[0][0].size();
#pragma macveth c 1 w 1 h 8
  int c;
  __m256 __mv_vop0, __mv_vop1;
  for (c = 0; (c + 1) <= channel; c += 1) {
    int w;
    for (w = 0; (w + 1) <= width; w += 1) {
      int h;
      for (h = 0; (h + 8) <= height; h += 8) {
        __mv_vop0 = _mm256_loadu_ps(&bottom[(c + 0)][(w + 0)][(h + 0)]);
        __mv_vop1 = _mm256_broadcast_ss(&ratio);
        __mv_vop0 = _mm256_mul_ps(__mv_vop0, __mv_vop1);
        _mm256_storeu_ps(&bottom[(c + 0)][(w + 0)][(h + 0)], __mv_vop0);
        // bottom[c][w][h] = bottom[c][w][h] * ratio;
      }
      for (h = h; h < height; h += 1) {
        bottom[c][w][h] = bottom[c][w][h] * ratio;
      }
    }
    for (w = w; w < width; w += 1) {
      for (int h = 0; h < height; ++h) {
        bottom[c][w][h] = bottom[c][w][h] * ratio;
      };
    }
  }
  for (c = c; c < channel; c += 1) {
    for (int w = 0; w < width; ++w) {
      for (int h = 0; h < height; ++h) {
        bottom[c][w][h] = bottom[c][w][h] * ratio;
      }
    };
  }

#pragma endmacveth
}

// ---------------------------------------------
long fullconnection_flops(const quote_vector3D(bottom),
                          const quote_vector4D(weights),
                          const vector<float> &bias, vector<float> &top) {
  int channel = bottom.size();
  int height = bottom[0].size();
  int width = bottom[0][0].size();
  int num_output = top.size();
  return (num_output * channel * height * width) * 2 + (num_output);
}

void fullconnection(const quote_vector3D(bottom), const quote_vector4D(weights),
                    const vector<float> &bias, vector<float> &top) {
  int channel = bottom.size();
  int height = bottom[0].size();
  int width = bottom[0][0].size();
  int num_output = top.size();
  top.assign(num_output, 0);
#pragma macveth n 1 c 1 h 1 w 8
  int n;
  __m128 __mv_lo256, __mv_hi256;
  __m256 __mv_vop2, __mv_vop0, __mv_vop1, __mv_accm0, __mv_vop3;
  for (n = 0; (n + 1) <= num_output; n += 1) {
    int c;
    for (c = 0; (c + 1) <= channel; c += 1) {
      int h;
      for (h = 0; (h + 1) <= height; h += 1) {
        int w;
        __mv_accm0 = _mm256_setzero_ps();
        for (w = 0; (w + 8) <= width; w += 8) {
          __mv_vop0 =
              _mm256_loadu_ps(&weights[(n + 0)][(c + 0)][(h + 0)][(w + 0)]);
          __mv_vop1 = _mm256_loadu_ps(&bottom[(c + 0)][(h + 0)][(w + 0)]);
          __mv_accm0 = _mm256_fmadd_ps(__mv_vop0, __mv_vop1, __mv_accm0);
          // top[n] = top[n] + weights[n][c][h][w] * bottom[c][h][w];
        }
        __mv_lo256 = _mm256_castps256_ps128(__mv_accm0);
        __mv_hi256 = _mm256_extractf128_ps(__mv_accm0, 0x1);
        __mv_lo256 = _mm_add_ps(__mv_lo256, __mv_hi256);
        __mv_hi256 = _mm_movehl_ps(__mv_lo256, __mv_lo256);
        __mv_lo256 = _mm_add_ps(__mv_lo256, __mv_hi256);
        __mv_hi256 = _mm_shuffle_ps(__mv_lo256, __mv_lo256, 0b00001110);
        __mv_lo256 = _mm_add_ps(__mv_lo256, __mv_hi256);
        top[(n + 0)] = top[(n + 0)] + __mv_lo256[0];

        for (w = w; w < width; w += 1) {
          top[n] = top[n] + weights[n][c][h][w] * bottom[c][h][w];
        }
      }
      for (h = h; h < height; h += 1) {
        for (int w = 0; w < width; ++w) {
          top[n] = top[n] + weights[n][c][h][w] * bottom[c][h][w];
        };
      }
    }
    for (c = c; c < channel; c += 1) {
      for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
          top[n] = top[n] + weights[n][c][h][w] * bottom[c][h][w];
        }
      };
    }

    top[(n + 0)] = (top[(n + 0)] + bias[(n + 0)]);
    // top[n] = top[n] + bias[n];
  }
  for (n = n; n < num_output; n += 1) {
    for (int c = 0; c < channel; ++c) {
      for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
          top[n] = top[n] + weights[n][c][h][w] * bottom[c][h][w];
        }
      }
    };
    top[n] = top[n] + bias[n];
  }

#pragma endmacveth
}

// ---------------------------------------------
long fullconnection_flops(const vector<float> &bottom,
                          const vector<vector<float>> &weights,
                          const vector<float> &bias, vector<float> &top) {
  int channel = bottom.size();
  int num_output = top.size();
  return num_output * channel * 2 + num_output;
}
void fullconnection(const vector<float> &bottom,
                    const vector<vector<float>> &weights,
                    const vector<float> &bias, vector<float> &top) {
  int channel = bottom.size();
  int num_output = top.size();
  top.assign(num_output, 0);
#pragma macveth n 8 c 8
  int n;
  __m128 __mv_lo256, __mv_hi256;
  __m256 __mv_vop2, __mv_vop0, __mv_vop1, __mv_vop4, __mv_vop3, __mv_vop6,
      __mv_vop5, __mv_vop8, __mv_vop7, __mv_vop10, __mv_vop9, __mv_vop12,
      __mv_vop11, __mv_vop14, __mv_vop13, __mv_vop16, __mv_vop15, __mv_accm0,
      __mv_vop17, __mv_vop18;
  for (n = 0; (n + 8) <= num_output; n += 8) {
    int c;
    __mv_accm0 = _mm256_setzero_ps();
    __mv_accm0 = _mm256_setzero_ps();
    for (c = 0; (c + 8) <= channel; c += 8) {
      __mv_vop0 = _mm256_loadu_ps(&weights[(n + 0)][(c + 0)]);
      __mv_vop1 = _mm256_loadu_ps(&bottom[(c + 0)]);
      __mv_vop3 = _mm256_loadu_ps(&weights[(n + 1)][(c + 0)]);
      __mv_vop5 = _mm256_loadu_ps(&weights[(n + 2)][(c + 0)]);
      __mv_vop7 = _mm256_loadu_ps(&weights[(n + 3)][(c + 0)]);
      __mv_vop9 = _mm256_loadu_ps(&weights[(n + 4)][(c + 0)]);
      __mv_vop11 = _mm256_loadu_ps(&weights[(n + 5)][(c + 0)]);
      __mv_vop13 = _mm256_loadu_ps(&weights[(n + 6)][(c + 0)]);
      __mv_vop15 = _mm256_loadu_ps(&weights[(n + 7)][(c + 0)]);
      __mv_accm0 = _mm256_fmadd_ps(__mv_vop0, __mv_vop1, __mv_accm0);
      __mv_accm0 = _mm256_fmadd_ps(__mv_vop3, __mv_vop1, __mv_accm0);
      __mv_accm0 = _mm256_fmadd_ps(__mv_vop5, __mv_vop1, __mv_accm0);
      __mv_accm0 = _mm256_fmadd_ps(__mv_vop7, __mv_vop1, __mv_accm0);
      __mv_accm0 = _mm256_fmadd_ps(__mv_vop9, __mv_vop1, __mv_accm0);
      __mv_accm0 = _mm256_fmadd_ps(__mv_vop11, __mv_vop1, __mv_accm0);
      __mv_accm0 = _mm256_fmadd_ps(__mv_vop13, __mv_vop1, __mv_accm0);
      __mv_accm0 = _mm256_fmadd_ps(__mv_vop15, __mv_vop1, __mv_accm0);
      // top[n] = top[n] + weights[n][c] * bottom[c];
    }
    __mv_lo256 = _mm256_castps256_ps128(__mv_accm0);
    __mv_hi256 = _mm256_extractf128_ps(__mv_accm0, 0x1);
    __mv_lo256 = _mm_add_ps(__mv_lo256, __mv_hi256);
    __mv_hi256 = _mm_movehl_ps(__mv_lo256, __mv_lo256);
    __mv_lo256 = _mm_add_ps(__mv_lo256, __mv_hi256);
    __mv_hi256 = _mm_shuffle_ps(__mv_lo256, __mv_lo256, 0b00001110);
    __mv_lo256 = _mm_add_ps(__mv_lo256, __mv_hi256);
    top[(n + 0)] = top[(n + 0)] + __mv_lo256[0];

    for (c = c; c < channel; c += 1) {
      top[n] = top[n] + weights[n][c] * bottom[c];
    }

    __mv_vop17 = _mm256_loadu_ps(&top[(n + 0)]);
    __mv_vop18 = _mm256_loadu_ps(&bias[(n + 0)]);
    __mv_vop17 = _mm256_add_ps(__mv_vop17, __mv_vop18);
    _mm256_storeu_ps(&top[(n + 0)], __mv_vop17);
    // top[n] = top[n] + bias[n];
  }
  for (n = n; n < num_output; n += 1) {
    for (int c = 0; c < channel; ++c) {
      top[n] = top[n] + weights[n][c] * bottom[c];
    };
    top[n] = top[n] + bias[n];
  }

#pragma endmacveth
}

// ---------------------------------------------
long fullconnection_flops(const vector<float> &bottom,
                          const vector<vector<float>> &weights,
                          vector<float> &top) {
  int channel = bottom.size();
  int num_output = top.size();
  return num_output * channel * 2;
}
void fullconnection(const vector<float> &bottom,
                    const vector<vector<float>> &weights, vector<float> &top) {
  int channel = bottom.size();
  int num_output = top.size();
  top.assign(num_output, 0);
#pragma macveth unroll n 8 c 8
  int n;
  __m256 __mv_vop2, __mv_vop0, __mv_vop1, __mv_vop4, __mv_vop3, __mv_vop6,
      __mv_vop5, __mv_vop8, __mv_vop7, __mv_vop10, __mv_vop9, __mv_vop12,
      __mv_vop11, __mv_vop14, __mv_vop13, __mv_vop16, __mv_vop15, __mv_accm0,
      __mv_vop17, __mv_accm1, __mv_vop18, __mv_accm2, __mv_vop19, __mv_accm3,
      __mv_vop20, __mv_accm4, __mv_vop21, __mv_accm5, __mv_vop22, __mv_accm6,
      __mv_vop23, __mv_accm7, __mv_vop24;
  for (n = 0; (n + 8) <= num_output; n += 8) {
    int c;
    __mv_accm0 = _mm256_setzero_ps();
    __mv_accm1 = _mm256_setzero_ps();
    __mv_accm2 = _mm256_setzero_ps();
    __mv_accm3 = _mm256_setzero_ps();
    __mv_accm4 = _mm256_setzero_ps();
    __mv_accm5 = _mm256_setzero_ps();
    __mv_accm6 = _mm256_setzero_ps();
    __mv_accm7 = _mm256_setzero_ps();
    __mv_accm0 = _mm256_setzero_ps();
    for (c = 0; (c + 8) <= channel; c += 8) {
      __mv_vop0 = _mm256_loadu_ps(&weights[(n + 0)][(c + 0)]);
      __mv_vop1 = _mm256_loadu_ps(&bottom[(c + 0)]);
      __mv_vop3 = _mm256_loadu_ps(&weights[(n + 1)][(c + 0)]);
      __mv_vop5 = _mm256_loadu_ps(&weights[(n + 2)][(c + 0)]);
      __mv_vop7 = _mm256_loadu_ps(&weights[(n + 3)][(c + 0)]);
      __mv_vop9 = _mm256_loadu_ps(&weights[(n + 4)][(c + 0)]);
      __mv_vop11 = _mm256_loadu_ps(&weights[(n + 5)][(c + 0)]);
      __mv_vop13 = _mm256_loadu_ps(&weights[(n + 6)][(c + 0)]);
      __mv_vop15 = _mm256_loadu_ps(&weights[(n + 7)][(c + 0)]);
      __mv_accm0 = _mm256_fmadd_ps(__mv_vop0, __mv_vop1, __mv_accm0);
      __mv_accm1 = _mm256_fmadd_ps(__mv_vop3, __mv_vop1, __mv_accm1);
      __mv_accm2 = _mm256_fmadd_ps(__mv_vop5, __mv_vop1, __mv_accm2);
      __mv_accm3 = _mm256_fmadd_ps(__mv_vop7, __mv_vop1, __mv_accm3);
      __mv_accm4 = _mm256_fmadd_ps(__mv_vop9, __mv_vop1, __mv_accm4);
      __mv_accm5 = _mm256_fmadd_ps(__mv_vop11, __mv_vop1, __mv_accm5);
      __mv_accm6 = _mm256_fmadd_ps(__mv_vop13, __mv_vop1, __mv_accm6);
      __mv_accm7 = _mm256_fmadd_ps(__mv_vop15, __mv_vop1, __mv_accm7);
      // top[n] = top[n] + weights[n][c] * bottom[c];
    }
    __mv_accm0 = _mm256_hadd_ps(__mv_accm0, __mv_accm1);
    __mv_accm2 = _mm256_hadd_ps(__mv_accm2, __mv_accm3);
    __mv_accm4 = _mm256_hadd_ps(__mv_accm4, __mv_accm5);
    __mv_accm6 = _mm256_hadd_ps(__mv_accm6, __mv_accm7);
    __mv_accm0 = _mm256_hadd_ps(__mv_accm0, __mv_accm2);
    __mv_accm4 = _mm256_hadd_ps(__mv_accm4, __mv_accm6);
    __mv_accm0 =
        _mm256_add_ps(_mm256_blend_ps(__mv_accm0, __mv_accm4, 0b11110000),
                      _mm256_permute2f128_ps(__mv_accm0, __mv_accm4, 0x21));
    top[(n + 0)] = top[(n + 0)] + __mv_accm0[0];
    top[(n + 1)] = top[(n + 1)] + __mv_accm0[1];
    top[(n + 2)] = top[(n + 2)] + __mv_accm0[2];
    top[(n + 3)] = top[(n + 3)] + __mv_accm0[3];
    top[(n + 4)] = top[(n + 4)] + __mv_accm0[4];
    top[(n + 5)] = top[(n + 5)] + __mv_accm0[5];
    top[(n + 6)] = top[(n + 6)] + __mv_accm0[6];
    top[(n + 7)] = top[(n + 7)] + __mv_accm0[7];

    for (c = c; c < channel; c += 1) {
      top[n] = top[n] + weights[n][c] * bottom[c];
    }
  }
  for (n = n; n < num_output; n += 1) {
    for (int c = 0; c < channel; ++c) {
      top[n] = top[n] + weights[n][c] * bottom[c];
    };
  }

#pragma endmacveth
}

// ---------------------------------------------
long lrn_flops(quote_vector3D(bottom), float k, float alpha, float beta,
               int local_size) {
  int channel = bottom.size();
  int height = bottom[0].size();
  int width = bottom[0][0].size();
  vector<float> temp_vec(channel);
  long flops = 0;
  int c_start, c_end;
  float sum = 0, temp;
  for (int h = 0; h < height; ++h)
    for (int w = 0; w < width; ++w) {
      for (int c = 0; c < channel; ++c) {
        flops += 2;
        flops += 3;
        c_start = std::max(0, c - local_size / 2);
        c_end = std::min(channel - 1, c + local_size / 2);
        for (int i = c_start; i <= c_end; ++i) {
          flops += 2;
        }
        flops += 3;
        flops += 4;
      }
    }
  return flops;
}

void lrn(quote_vector3D(bottom), float k, float alpha, float beta,
         int local_size) {
  int channel = bottom.size();
  int height = bottom[0].size();
  int width = bottom[0][0].size();
  vector<float> temp_vec(channel);
  int c_start, c_end;
  float sum = 0, temp;

  for (int h = 0; h < height; ++h) {
    for (int w = 0; w < width; ++w) {
      for (int c = 0; c < channel; ++c) {
        c_start = std::max(0, c - local_size / 2);
        c_end = std::min(channel - 1, c + local_size / 2);
        sum = 0;
#pragma macveth unroll i 8
        int i;
        __m128 __mv_lo256, __mv_hi256;
        __m256 __mv_vop1, __mv_vop0, __mv_accm0, __mv_vop2;
        __mv_accm0 = _mm256_setzero_ps();
        __mv_accm0 = _mm256_setzero_ps();
        for (i = c_start; (i + 8) <= c_end; i += 8) {
          __mv_vop0 =
              _mm256_set_ps(bottom[(i + 7)][h][w], bottom[(i + 6)][h][w],
                            bottom[(i + 5)][h][w], bottom[(i + 4)][h][w],
                            bottom[(i + 3)][h][w], bottom[(i + 2)][h][w],
                            bottom[(i + 1)][h][w], bottom[(i + 0)][h][w]);
          __mv_accm0 = _mm256_fmadd_ps(__mv_vop0, __mv_vop0, __mv_accm0);
          // sum = sum + bottom[i][h][w] * bottom[i][h][w];
        }
        __mv_lo256 = _mm256_castps256_ps128(__mv_accm0);
        __mv_hi256 = _mm256_extractf128_ps(__mv_accm0, 0x1);
        __mv_lo256 = _mm_add_ps(__mv_lo256, __mv_hi256);
        __mv_hi256 = _mm_movehl_ps(__mv_lo256, __mv_lo256);
        __mv_lo256 = _mm_add_ps(__mv_lo256, __mv_hi256);
        __mv_hi256 = _mm_shuffle_ps(__mv_lo256, __mv_lo256, 0b00001110);
        __mv_lo256 = _mm_add_ps(__mv_lo256, __mv_hi256);
        sum = sum + __mv_lo256[0];

        for (i = i; i < c_end; i += 1) {
          sum = sum + bottom[i][h][w] * bottom[i][h][w];
        }

#pragma endmacveth
        temp = sum * alpha / local_size + k;
        temp_vec[c] = bottom[c][h][w] / powf(temp, beta);
        // bottom[c][h][w] = bottom[c][h][w] / powf(temp, beta);
      }
      for (int c = 0; c < channel; ++c) {
        bottom[c][h][w] = temp_vec[c];
      }
    }
  }
}

// ---------------------------------------------
long softmax_flops(vector<float> &bottom) {
  int count = bottom.size();
  return count * 2 + count;
}

void softmax(vector<float> &bottom) {
  int count = bottom.size();
  vector<float> exp_rst(count);
  float sum = 0;
#pragma macveth i 8
  for (int i = 0; i < count; i++) {
    exp_rst[i] = exp(bottom[i]);
    sum = sum + exp_rst[i];
  }
  for (int i = 0; i < count; i++) {
    bottom[i] = exp_rst[i] / sum;
  }
#pragma endmacveth
}

// ---------------------------------------------
long add_pad_flops(quote_vector3D(bottom), int pad) {
  int channel = bottom.size();
  int height = bottom[0].size();
  int width = bottom[0][0].size();
  return channel * pad * 2 + channel * (height + 2 * pad) * pad * 2;
}
void add_pad(quote_vector3D(bottom), int pad) {
  int channel = bottom.size();
  int height = bottom[0].size();
  int width = bottom[0][0].size();
  for (int c = 0; c < channel; ++c) {
    for (int p = 0; p < pad; ++p) {
      bottom[c].insert(bottom[c].begin(), vector<float>(width, 0.0f));
      bottom[c].push_back(vector<float>(width, 0.0f));
    }
    for (int h = 0; h < height + 2 * pad; ++h) {
      for (int p = 0; p < pad; ++p) {
        bottom[c][h].insert(bottom[c][h].begin(), 0);
        bottom[c][h].push_back(0);
      }
    }
  }
}

// ---------------------------------------------
long pooling_flops(quote_vector3D(bottom), int kernel_size, int stride, int pad,
                   quote_vector3D(top), string type) {

  if (pad) {
    add_pad(bottom, pad);
  }
  int channel = bottom.size();
  int height = bottom[0].size();
  int width = bottom[0][0].size();
  int hout = (height - kernel_size) / stride + 1;
  int wout = (width - kernel_size) / stride + 1;

  long flops = add_pad_flops(bottom, pad);

  int h_start, h_end, w_start, w_end;
  float max_value;
  for (int c = 0; c < channel; ++c) {
    for (int h = 0; h < hout; ++h) {
      for (int w = 0; w < wout; ++w) {
        h_start = h * stride;
        h_end = h_start + kernel_size;
        w_start = w * stride;
        w_end = w_start + kernel_size;
        flops += 4;
        for (int i = h_start; i < h_end; ++i) {
          for (int j = w_start; j < w_end; ++j) {
            flops += 1;
          }
        }
      }
    }
  }

  return flops;
}

void pooling(quote_vector3D(bottom), int kernel_size, int stride, int pad,
             quote_vector3D(top), string type) {

  if (pad) {
    add_pad(bottom, pad);
  }
  int channel = bottom.size();
  int height = bottom[0].size();
  int width = bottom[0][0].size();
  int hout = (height - kernel_size) / stride + 1;
  int wout = (width - kernel_size) / stride + 1;

  int h_start, h_end, w_start, w_end;
  float max_value;
  for (int c = 0; c < channel; ++c) {
    for (int h = 0; h < hout; ++h) {
      for (int w = 0; w < wout; ++w) {
        h_start = h * stride;
        h_end = h_start + kernel_size;
        w_start = w * stride;
        w_end = w_start + kernel_size;
        // if (!strcmp(type.c_str(), "AVE")) {
        //   float sum = 0;
        //   for (int i = h_start; i < h_end; ++i)
        //     for (int j = w_start; j < w_end; ++j) {
        //       sum += bottom[c][i][j];
        //     }
        //   top[c][h][w] = sum / (kernel_size * kernel_size);
        // }
        // if (!strcmp(type.c_str(), "MAX")) {
        max_value = -10000000000.0;
        // float max_value2 = -10000000000.0;

#pragma macveth i 1 j 8
        int i;
        __m128 __mv_lo256, __mv_hi256;
        __m256 __mv_accm0, __mv_vop0, __mv_vop1;
        for (i = h_start; (i + 1) <= h_end; i += 1) {
          int j;
          __mv_accm0 = _mm256_setzero_ps();
          __mv_accm0 = _mm256_setzero_ps();
          for (j = w_start; (j + 8) <= w_end; j += 8) {
            __mv_vop1 = _mm256_loadu_ps(&bottom[c][(i + 0)][(j + 0)]);
            __mv_accm0 = _mm256_max_ps(__mv_vop1, __mv_accm0);
            // max_value = std::max(max_value, bottom[c][i][j]);
            // max_value2 = std::max(max_value2, bottom[c][i][j]);
          }
          __mv_lo256 = _mm256_castps256_ps128(__mv_accm0);
          __mv_hi256 = _mm256_extractf128_ps(__mv_accm0, 0x1);
          __mv_lo256 = _mm_max_ps(__mv_lo256, __mv_hi256);
          __mv_hi256 = _mm_movehl_ps(__mv_lo256, __mv_lo256);
          __mv_lo256 = _mm_max_ps(__mv_lo256, __mv_hi256);
          __mv_hi256 = _mm_shuffle_ps(__mv_lo256, __mv_lo256, 0b00001110);
          __mv_lo256 = _mm_max_ps(__mv_lo256, __mv_hi256);
          max_value = std::max(max_value, __mv_lo256[0]);

          for (j = j; j < w_end; j += 1) {
            max_value = std::max(max_value, bottom[c][i][j]);
          }
        }
        for (i = i; i < h_end; i += 1) {
          for (int j = w_start; j < w_end; ++j) {
            max_value = std::max(max_value, bottom[c][i][j]);
            // max_value2 = std::max(max_value2, bottom[c][i][j]);
          };
        }

#pragma endmacveth
        top[c][h][w] = max_value;
        //}
      }
    }
  }
}

// ---------------------------------------------
long convolution_flops(quote_vector3D(bottom), int stride, int pad,
                       const quote_vector4D(weights), const vector<float> &bias,
                       quote_vector3D(top), string layer_name) {
  long channel = bottom.size();
  long height = bottom[0].size();
  long width = bottom[0][0].size();

  long cout = weights.size();
  long kernel_size = weights[0][0].size();
  long hout = (height - kernel_size) / stride + 1;
  long wout = (width - kernel_size) / stride + 1;
  long flops = 0;
  for (int co = 0; co < cout; ++co) {
    for (int h = 0; h < hout; ++h)
      for (int w = 0; w < wout; ++w) {
        int h_start = h * stride;
        int h_end = h_start + kernel_size;
        int w_start = w * stride;
        int w_end = w_start + kernel_size;
        flops += 1 + 2 * (h_end - h_start) * (w_end - w_start) * channel;
      }
  }
  return flops + add_pad_flops(bottom, pad);
}

void convolution(quote_vector3D(bottom), int stride, int pad,
                 const quote_vector4D(weights), const vector<float> &bias,
                 quote_vector3D(top), string layer_name) {
  if (pad) {
    add_pad(bottom, pad);
  }
  int channel = bottom.size();
  int height = bottom[0].size();
  int width = bottom[0][0].size();

  int cout = weights.size();
  int kernel_size = weights[0][0].size();
  int hout = (height - kernel_size) / stride + 1;
  int wout = (width - kernel_size) / stride + 1;
  int h_start, h_end, w_start, w_end;
  float sum;
  //#pragma macveth co 1 h 1 w 1 i 1 j 1 ci 8
  for (int co = 0; co < cout; ++co) {
    for (int h = 0; h < hout; ++h) {
      for (int w = 0; w < wout; ++w) {
        h_start = h * stride;
        h_end = h_start + kernel_size;
        w_start = w * stride;
        w_end = w_start + kernel_size;
        sum = 0;
#pragma macveth i 1 j 8 ci 1
        int ci;
        __m128 __mv_lo256, __mv_hi256;
        __m256 __mv_vop2, __mv_vop0, __mv_vop1, __mv_accm0, __mv_vop3;
        for (ci = 0; (ci + 1) <= channel; ci += 1) {
          int i;
          for (i = h_start; (i + 1) <= h_end; i += 1) {
            int j;
            __mv_accm0 = _mm256_setzero_ps();
            __mv_accm0 = _mm256_setzero_ps();
            for (j = w_start; (j + 8) <= w_end; j += 8) {
              __mv_vop0 =
                  _mm256_loadu_ps(&weights[co][(ci + 0)][((i + 0) - h_start)]
                                          [((j + 0) - w_start)]);
              __mv_vop1 = _mm256_loadu_ps(&bottom[(ci + 0)][(i + 0)][(j + 0)]);
              __mv_accm0 = _mm256_fmadd_ps(__mv_vop0, __mv_vop1, __mv_accm0);
              // sum = sum + weights[co][ci][i - h_start][j - w_start] *
              bottom[ci][i][j];
            }
            __mv_lo256 = _mm256_castps256_ps128(__mv_accm0);
            __mv_hi256 = _mm256_extractf128_ps(__mv_accm0, 0x1);
            __mv_lo256 = _mm_add_ps(__mv_lo256, __mv_hi256);
            __mv_hi256 = _mm_movehl_ps(__mv_lo256, __mv_lo256);
            __mv_lo256 = _mm_add_ps(__mv_lo256, __mv_hi256);
            __mv_hi256 = _mm_shuffle_ps(__mv_lo256, __mv_lo256, 0b00001110);
            __mv_lo256 = _mm_add_ps(__mv_lo256, __mv_hi256);
            sum = sum + __mv_lo256[0];

            for (j = j; j < w_end; j += 1) {
              sum = sum + weights[co][ci][i - h_start][j - w_start] *
                              bottom[ci][i][j];
            }
          }
          for (i = i; i < h_end; i += 1) {
            for (int j = w_start; j < w_end; ++j) {
              sum = sum + weights[co][ci][i - h_start][j - w_start] *
                              bottom[ci][i][j];
            };
          }
        }
        for (ci = ci; ci < channel; ci += 1) {
          for (int i = h_start; i < h_end; ++i) {
            for (int j = w_start; j < w_end; ++j) {
              sum = sum + weights[co][ci][i - h_start][j - w_start] *
                              bottom[ci][i][j];
            }
          };
        }

#pragma endmacveth

        top[co][h][w] = sum + bias[co];
      }
    }
  }
  //#pragma endmacveth
}

// ---------------------------------------------

long readimage_flops(string ss, quote_vector3D(image)) {
  FILE *fin;
  fin = fopen(ss.c_str(), "rb");
  int channel = image.size();
  int height = image[0].size();
  int width = image[0][0].size();

  return channel * height;
}

void readimage(string ss, quote_vector3D(image)) {
  FILE *fin;
  fin = fopen(ss.c_str(), "rb");
  int channel = image.size();
  int height = image[0].size();
  int width = image[0][0].size();
  float *temp = (float *)malloc(sizeof(float) * width);
  for (int c = 0; c < channel; ++c)
    for (int h = 0; h < height; ++h) {
      fread(temp, sizeof(float), width, fin);
      vector<float> temp_vec(temp, temp + width);
      image[c][h] = temp_vec;
    }
  fclose(fin);
  free(temp);
  return;
}

// ---------------------------------------------

long readdescriptor_flops(char *s, vector<float> &descriptor) { return 1; }

void readdescriptor(char *s, vector<float> &descriptor) {
  FILE *fin;
  fin = fopen(s, "rb");
  int length = descriptor.size();
  float *temp = (float *)malloc(sizeof(float) * length);
  fread(temp, sizeof(float), length, fin);
  vector<float> temp_vec(temp, temp + length);
  descriptor = temp_vec;
  fclose(fin);
  free(temp);
  return;
}

// ---------------------------------------------

long readweights_flops(string ss, quote_vector4D(weights),
                       vector<float> &bias) {
  FILE *fin;
  fin = fopen(ss.c_str(), "rb");
  int num_output = weights.size();
  int channel = weights[0].size();
  int kernel_size = weights[0][0].size();
  return num_output * channel * kernel_size;
}

void readweights(string ss, quote_vector4D(weights), vector<float> &bias) {
  FILE *fin;
  fin = fopen(ss.c_str(), "rb");
  int num_output = weights.size();
  int channel = weights[0].size();
  int kernel_size = weights[0][0].size();
  float *temp = (float *)malloc(sizeof(float) * kernel_size);
  for (int n = 0; n < num_output; ++n)
    for (int c = 0; c < channel; ++c)
      for (int k = 0; k < kernel_size; ++k) {
        fread(temp, sizeof(float), kernel_size, fin);
        vector<float> temp_vec(temp, temp + kernel_size);
        weights[n][c][k] = temp_vec;
      }
  float *temp_bias = (float *)malloc(sizeof(float) * num_output);
  fread(temp_bias, sizeof(float), num_output, fin);
  vector<float> bias_vec(temp_bias, temp_bias + num_output);
  bias = bias_vec;
  fclose(fin);
  free(temp);
  free(temp_bias);
  return;
}

// ---------------------------------------------

long readweights_flops(string ss, vector<vector<float>> &weights) {
  FILE *fin;
  fin = fopen(ss.c_str(), "rb");
  int num_output = weights.size();
  int channel = weights[0].size();
  float *temp = (float *)malloc(sizeof(float) * channel);
  return num_output;
}

void readweights(string ss, vector<vector<float>> &weights) {
  FILE *fin;
  fin = fopen(ss.c_str(), "rb");
  int num_output = weights.size();
  int channel = weights[0].size();
  float *temp = (float *)malloc(sizeof(float) * channel);
  for (int n = 0; n < num_output; ++n) {
    fread(temp, sizeof(float), channel, fin);
    vector<float> temp_vec(temp, temp + channel);
    weights[n] = temp_vec;
  }
  fclose(fin);
  free(temp);
}

// ---------------------------------------------

long readweights_flops(string ss, vector<vector<float>> &weights,
                       vector<float> &bias) {
  FILE *fin;
  fin = fopen(ss.c_str(), "rb");
  int num_output = weights.size();
  int channel = weights[0].size();
  return num_output;
}

void readweights(string ss, vector<vector<float>> &weights,
                 vector<float> &bias) {
  FILE *fin;
  fin = fopen(ss.c_str(), "rb");
  int num_output = weights.size();
  int channel = weights[0].size();
  float *temp = (float *)malloc(sizeof(float) * channel);
  for (int n = 0; n < num_output; ++n) {
    fread(temp, sizeof(float), channel, fin);
    vector<float> temp_vec(temp, temp + channel);
    weights[n] = temp_vec;
  }
  if (bias.size()) {
    float *temp_bias = (float *)malloc(sizeof(float) * num_output);
    fread(temp_bias, sizeof(float), num_output, fin);
    vector<float> bias_vec(temp_bias, temp_bias + num_output);
    bias = bias_vec;
    free(temp_bias);
  }
  fclose(fin);
  free(temp);
}

// ---------------------------------------------

long print_vec_flops(char *s, const vector<float> &vec) {
  FILE *fin;
  fin = fopen(s, "wt");
  int count = vec.size();
  return count;
}

void print_vec(char *s, const vector<float> &vec) {
  FILE *fin;
  fin = fopen(s, "wt");
  int count = vec.size();
  for (int i = 0; i < count; ++i) {
    fprintf(fin, "%0.8f ", vec[i]);
  }
  fclose(fin);
}

// ---------------------------------------------

long print_vec_flops(char *s, const vector<int> &vec) {
  FILE *fin;
  fin = fopen(s, "wt");
  int count = vec.size();
  return count;
}

void print_vec(char *s, const vector<int> &vec) {
  FILE *fin;
  fin = fopen(s, "wt");
  int count = vec.size();
  for (int i = 0; i < count; ++i) {
    fprintf(fin, "%d ", vec[i]);
  }
  fclose(fin);
}

// ---------------------------------------------

long print_vec_flops(char *s, const vector<vector<float>> &vec) {
  FILE *fin;
  fin = fopen(s, "wt");
  int height = vec.size();
  int width = vec[0].size();
  return height * width;
}

void print_vec(char *s, const vector<vector<float>> &vec) {
  FILE *fin;
  fin = fopen(s, "wt");
  int height = vec.size();
  int width = vec[0].size();
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      fprintf(fin, "%14.8f ", vec[i][j]);
      // fwrite((void *)&(vec[i][j]), sizeof(float), 1, fin);
    }
    fprintf(fin, "\n");
  }
  fclose(fin);
}

// ---------------------------------------------

long EmbedLayer_flops(int word_input,
                      const vector<vector<float>> &embed_weights,
                      vector<float> &input_sentence) {
  return 1;
}

void EmbedLayer(int word_input, const vector<vector<float>> &embed_weights,
                vector<float> &input_sentence) {
  int length = embed_weights.size();
  if (word_input < 0 || word_input >= length) {
    printf("illegal input\n");
    return;
  }
  input_sentence = embed_weights[word_input];
}

// ---------------------------------------------

long PredictLayer_flops(const vector<float> &bottom,
                        const vector<vector<float>> &weights,
                        const vector<float> &bias, vector<float> &top) {
  int channel = bottom.size();
  int num_output = top.size();
  return num_output * channel * 2 + num_output;
}

void PredictLayer(const vector<float> &bottom,
                  const vector<vector<float>> &weights,
                  const vector<float> &bias, vector<float> &top) {

  int channel = bottom.size();
  int num_output = top.size();
  top.assign(num_output, 0);
  for (int n = 0; n < num_output; ++n) {
    for (int c = 0; c < channel; ++c) {
      top[n] += weights[n][c] * bottom[c];
    }
    top[