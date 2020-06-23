
#include "layers.h"
//#include "utilities/polybench.h"
//#include <immintrin.h>
#include <algorithm>
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
  int a, b, x, d, c = 0;
  int h = 0;
#pragma macveth c 1 h 1 w 8
  int c;
__m256 __mv_vop0 = {0}, __mv_vop1 = {0};
  for (c = 0; (c + 1) <= channel; c += 1) {
    int h;
for (h = 0; (h + 1) <= height; h += 1) {
      int w;
for (w = 0; (w + 8) <= width; w += 8) {
        __mv_vop0 = _mm256_load_ps(&bottom[c][h][w + 0]);	// cost = 1
        __mv_vop1 = _mm_set1_pd(2.0);	// cost = 1
        __mv_vop0 = _mm256_mul_ps(__mv_vop0, __mv_vop1);	// cost = 5
        _mm256_store_ps(&bottom[c][h][w + 0], __mv_vop0);	// cost = 1
        // statement replaced: bottom[c][h][w] = bottom[c][h][w] * 2.0;
      }
for (w = (width / 8 ) * 8; w < width; w += 1) {
bottom[c][h][w] = bottom[c][h][w] * 2.0;
}

    }
for (h = (height / 1 ) * 1; h < height; h += 1) {
for (int w = 0; w < width; ++w) {
        bottom[c][h][w] = bottom[c][h][w] * 2.0;
      };
}

  }
for (c = (channel / 1 ) * 1; c < channel; c += 1) {
for (int h = 0; h < height; ++h) {
      for (int w = 0; w < width; ++w) {
        bottom[c][h][w] = bottom[c][h][w] * 2.0;
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
  //#pragma macveth
  for (int i = 0; i < count; ++i) {
    bottom[i] = std::max(bottom[i], 0.0f);
  }
  //#pragma endmacveth
}

// // ---------------------------------------------
// long dropout_flops(vector<float> &bottom, float ratio) {
//   int count = bottom.size();
//   return count;
// }
// void dropout(vector<float> &bottom, float ratio) {
//   int count = bottom.size();
//   //#pragma macveth
//   for (int i = 0; i < count; ++i) {
//     bottom[i] = bottom[i] * ratio;
//   }
//   //#pragma endmacveth
// }

// // ---------------------------------------------
// long dropout_flops(quote_vector3D(bottom), float ratio) {
//   // ratio 是一个 0~1的浮点数
//   int channel = bottom.size();
//   int width = bottom[0].size();
//   int height = bottom[0][0].size();
//   return channel * width * height;
// }

// void dropout(quote_vector3D(bottom), float ratio) {
//   // ratio 是一个 0~1的浮点数
//   int channel = bottom.size();
//   int width = bottom[0].size();
//   int height = bottom[0][0].size();
//   //#pragma macveth c 1 w 1 h 8
//   for (int c = 0; c < channel; c++) {
//     for (int w = 0; w < width; ++w) {
//       for (int h = 0; h < height; ++h) {
//         bottom[c][w][h] = bottom[c][w][h] * ratio;
//       }
//     }
//   }
//   //#pragma endmacveth
// }

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
__m256 __mv_vop4 = {0}, __mv_vop2 = {0}, __mv_vop3 = {0}, __mv_accm0 = {0}, __mv_vop5 = {0};
  __m32 __mv_vop5 = {0}, __mv_vop6 = {0};
  float __mv_arr0[4] = {0};
  for (n = 0; (n + 1) <= num_output; n += 1) {
    for (c = 0; (c + 1) <= channel; c += 1) {
      for (h = 0; (h + 1) <= height; h += 1) {
        for (w = 0; (w + 8) <= width; w += 8) {
          // top[n] += weights[n][c][h][w] * bottom[c][h][w];
          __mv_vop2 = _mm256_load_ps(&weights[n][c][h][w + 0]);	// cost = 1
          __mv_vop3 = _mm256_load_ps(&bottom[c][h][w + 0]);	// cost = 1
          __mv_accm0 = _mm256_fmadd_ps(__mv_vop2, __mv_vop3, __mv_accm0);	// cost = 5
          // statement replaced: top[n] = top[n] + weights[n][c][h][w] * bottom[c][h][w];
        }
for (w = (width / 8 ) * 8; w < width; w += 1) {
top[n] = top[n] + weights[n][c][h][w] * bottom[c][h][w];
}
__mv_accm0 = _mm256_hadd_ps(__mv_accm0, __mv_accm0);	// cost = 6
__mv_accm0 = _mm256_hadd_ps(_mm256_permutevar8x32_ps(__mv_accm0,_mm256_set_epi32(7,3,6,2,5,1,4,0)), __mv_accm0);	// cost = 6
_mm256_storeu_ps(__mv_arr0, __mv_accm0);	// cost = 1
top[n + 0] = __mv_arr0[0];	// cost = 0

      }
for (h = (height / 1 ) * 1; h < height; h += 1) {
for (int w = 0; w < width; ++w) {
          // top[n] += weights[n][c][h][w] * bottom[c][h][w];
          top[n] = top[n] + weights[n][c][h][w] * bottom[c][h][w];
        };
}

    }
for (c = (channel / 1 ) * 1; c < channel; c += 1) {
for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
          // top[n] += weights[n][c][h][w] * bottom[c][h][w];
          top[n] = top[n] + weights[n][c][h][w] * bottom[c][h][w];
        }
      };
}

    // top[n] += bias[n];
    __mv_vop5 = _mm_set1_ps(top[n + 0]);	// cost = 1
    __mv_vop6 = _mm256_broadcast_ss(&bias[n + 0]);	// cost = 10
    __mv_vop5 = _mm_add_ps(__mv_vop5, __mv_vop6);	// cost = 4
    _mm_store_ss(&top[n + 0]);	// cost = 1
    // statement replaced: top[n] = top[n] + bias[n];
  }
for (n = (num_output / 1 ) * 1; n < num_output; n += 1) {
for (int c = 0; c < channel; ++c) {
      for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
          // top[n] += weights[n][c][h][w] * bottom[c][h][w];
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
#pragma macveth
  __m128 __mv_vop11 = {0}, __mv_vop12 = {0};
  __m256 __mv_vop9 = {0}, __mv_vop7 = {0}, __mv_vop8 = {0}, __mv_vop10 = {0}, __mv_accm0 = {0}, __mv_vop11 = {0};
  float __mv_arr1[4] = {0};
  for (n = 0; (n + 4) <= num_output; n += 4) {
    for (c = 0; (c + 4) <= channel; c += 4) {
      // top[n] += weights[n][c] * bottom[c];
      __mv_vop7 = _mm256_load_ps(&weights[n][c + 0]);	// cost = 1
      __mv_vop8 = _mm256_load_ps(&bottom[c + 0]);	// cost = 1
      __mv_accm0 = _mm256_fmadd_ps(__mv_vop7, __mv_vop8, __mv_accm0);	// cost = 5
      __mv_accm0 = _mm256_fmadd_ps(__mv_vop7, __mv_vop8, __mv_accm0);	// cost = 5
      // statement replaced: top[n] = top[n] + weights[n][c] * bottom[c];
    }
for (c = (channel / 4 ) * 4; c < channel; c += 1) {
top[n] = top[n] + weights[n][c] * bottom[c];
}
__mv_accm0 = _mm256_hadd_ps(__mv_accm0, __mv_accm0);	// cost = 6
__mv_accm0 = _mm256_hadd_ps(_mm256_permutevar8x32_ps(__mv_accm0,_mm256_set_epi32(7,3,6,2,5,1,4,0)), __mv_accm0);	// cost = 6
_mm256_storeu_ps(__mv_arr1, __mv_accm0);	// cost = 1
top[n + 0] = __mv_arr1[0];	// cost = 0
top[n + 2] = __mv_arr1[1];	// cost = 0

    // top[n] += bias[n];
    __mv_vop11 = _mm_set_ps(+, +, +, +);	// cost = 1
    __mv_vop12 = _mm_load_ps(&bias[n + 0]);	// cost = 1
    __mv_vop11 = _mm_add_ps(__mv_vop11, __mv_vop12);	// cost = 4
    _mm_store_ps(&top[n + 0], __mv_vop11);	// cost = 1
    // statement replaced: top[n] = top[n] + bias[n];
  }
for (n = (num_output / 4 ) * 4; n < num_output; n += 1) {
for (int c = 0; c < channel; ++c) {
      // top[n] += weights[n][c] * bottom[c];
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
#pragma macveth
  __m256 __mv_vop15 = {0}, __mv_vop13 = {0}, __mv_vop14 = {0}, __mv_vop16 = {0}, __mv_accm0 = {0}, __mv_vop17 = {0}, __mv_accm1 = {0}, __mv_vop18 = {0};
  float __mv_arr2[4] = {0};
  for (n = 0; (n + 4) <= num_output; n += 4) {
    for (c = 0; (c + 4) <= channel; c += 4) {
      // top[n] += weights[n][c] * bottom[c];
      __mv_vop13 = _mm256_load_ps(&weights[n][c + 0]);	// cost = 1
      __mv_vop14 = _mm256_load_ps(&bottom[c + 0]);	// cost = 1
      __mv_accm0 = _mm256_fmadd_ps(__mv_vop13, __mv_vop14, __mv_accm0);	// cost = 5
      __mv_accm1 = _mm256_fmadd_ps(__mv_vop13, __mv_vop14, __mv_accm1);	// cost = 5
      // statement replaced: top[n] = top[n] + weights[n][c] * bottom[c];
    }
for (c = (channel / 4 ) * 4; c < channel; c += 1) {
top[n] = top[n] + weights[n][c] * bottom[c];
}
__mv_accm0 = _mm256_hadd_ps(__mv_accm0, __mv_accm1);	// cost = 6
__mv_accm0 = _mm256_hadd_ps(_mm256_permutevar8x32_ps(__mv_accm0,_mm256_set_epi32(7,3,6,2,5,1,4,0)), __mv_accm0);	// cost = 6
_mm256_storeu_ps(__mv_arr2, __mv_accm0);	// cost = 1
top[n + 0] = __mv_arr2[0];	// cost = 0
top[n + 2] = __mv_arr2[1];	// cost = 0

  }
for (n = (num_output / 4 ) * 4; n < num_output; n += 1) {
for (int c = 0; c < channel; ++c) {
      // top[n] += weights[n][c] * bottom[c];
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
  long tmp = 0;
  int c_start, c_end;
  float sum = 0, temp;
  for (int h = 0; h < height; ++h)
    for (int w = 0; w < width; ++w) {
      for (int c = 0; c < channel; ++c) {
        tmp += 2;
        tmp += 3;
        c_start = std::max(0, c - local_size / 2);
        c_end = std::min(channel - 1, c + local_size / 2);
        for (int i = c_start; i <= c_end; ++i) {
          tmp += 2;
        }
        tmp += 3;
        tmp += 4;
      }
    }
  return tmp;
}

void lrn(quote_vector3D(bottom), float k, float alpha, float beta,
         int local_size) {
  int channel = bottom.size();
  int height = bottom[0].size();
  int width = bottom[0][0].size();
  vector<float> temp_vec(channel);
  int c_start, c_end;
  float sum = 0, temp;

  //#pragma macveth
  for (int h = 0; h < height; ++h)
    for (int w = 0; w < width; ++w) {
      for (int c = 0; c < channel; ++c) {
        c_start = std::max(0, c - local_size / 2);
        c_end = std::min(channel - 1, c + local_size / 2);
        sum = 0;
        for (int i = c_start; i <= c_end; ++i) {
          sum = sum + bottom[i][h][w] * bottom[i][h][w];
        }
        temp = sum * alpha / local_size + k;
        temp_vec[c] = bottom[c][h][w] / powf(temp, beta);
      }
      for (int c = 0; c < channel; ++c) {
        bottom[c][h][w] = temp_vec[c];
      }
    }
  //#pragma endmacveth
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
  for (int i = 0; i < count; i++) {
    exp_rst[i] = exp(bottom[i]); //对于每一个输入求其exp(x)；
    sum += exp_rst[i];           //求exp(x)的和；
  }
  for (int i = 0; i < count; i++) {
    bottom[i] = exp_rst[i] / sum; //输出为一个归一化值
  }
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

  return channel * hout * wout;
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
        //#pragma macveth
        for (int i = h_start; i < h_end; ++i) {
          for (int j = w_start; j < w_end; ++j) {
            max_value = std::max(max_value, bottom[c][i][j]);
          }
        }
        //#pragma endmacveth

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
  long tmp = 0;
  for (int co = 0; co < cout; ++co) {
    for (int h = 0; h < hout; ++h)
      for (int w = 0; w < wout; ++w) {
        int h_start = h * stride;
        int h_end = h_start + kernel_size;
        int w_start = w * stride;
        int w_end = w_start + kernel_size;
        tmp += 1 + 2 * (h_end - h_start) * (w_end - w_start) * channel;
      }
  }
  return tmp + add_pad_flops(bottom, pad);
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
  for (int co = 0; co < cout; ++co) {
    for (int h = 0; h < hout; ++h) {
      for (int w = 0; w < wout; ++w) {
        h_start = h * stride;
        h_end = h_start + kernel_size;
        w_start = w * stride;
        w_end = w_start + kernel_size;
        sum = 0;
        int m = 0;
        int n = 0;
        //#pragma macveth
        for (int i = h_start; i < h_end; ++i) {
          for (int j = w_start; j < w_end; ++j) {
            for (int ci = 0; ci < channel; ++ci) {
              sum = sum + weights[co][ci][m++][n++] * bottom[ci][i][j];
            }
          }
        }
        //#pragma endmacveth

        top[co][h][w] = sum + bias[co];
      }
    }
  }
}

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
    top[n] = top[n] + bias[n];
  }
}

long Update_flops(int cont_input, vector<float> &hc_rst) {
  int length = hc_rst.size();
  return length;
}

void Update(int cont_input, vector<float> &hc_rst) {
  int length = hc_rst.size();
  for (int i = 0; i < length; i++) {
    hc_rst[i] = cont_input * hc_rst[i];
  }
}

long LSTMFullconnectionLayer_flops(const vector<float> &bottom,
                                   const vector<vector<float>> &weights,
                                   vector<float> &top) {
  int channel = bottom.size();
  int num_output = top.size();
  return num_output * channel * 2;
}

void LSTMFullconnectionLayer(const vector<float> &bottom,
                             const vector<vector<float>> &weights,
                             vector<float> &top) {

  int channel = bottom.size();
  int num_output = top.size();
  top.assign(num_output, 0);
  for (int n = 0; n < num_output; ++n) {
    for (int c = 0; c < channel; ++c) {
      top[n] = top[n] + weights[n][c] * bottom[c];
    }
  }
}

long Add_Result_flops(vector<float> &gate_input_t,
                      const vector<float> &xcstatic_rst,
                      const vector<float> &Wxc_tm1,
                      const vector<float> &Whc_tm1,
                      const vector<float> &xc_bias) {
  int length = gate_input_t.size();
  return length;
}

void Add_Result(vector<float> &gate_input_t, const vector<float> &xcstatic_rst,
                const vector<float> &Wxc_tm1, const vector<float> &Whc_tm1,
                const vector<float> &xc_bias) {
  int length = gate_input_t.size();
  for (int i = 0; i < length; i++) {
    gate_input_t[i] = xcstatic_rst[i] + Wxc_tm1[i] + Whc_tm1[i] + xc_bias[i];
  }
}

long sigmoid_flops(float x) { return 3; }

float sigmoid(float x) { return 1. / (1. + exp(-x)); }

// float tanh(float x) {
//  return 2. * sigmoid(2. * x) - 1.;
//}

long LSTMlayer_flops(int cont_input, const vector<float> &gate_input_t,
                     vector<float> &c_tm1, vector<float> &hc_tm1) {
  int length = gate_input_t.size();
  return length / 4 * ((3 * sigmoid_flops(1.0)) + 7);
}

void LSTMlayer(int cont_input, const vector<float> &gate_input_t,
               vector<float> &c_tm1, vector<float> &hc_tm1) {
  int length = gate_input_t.size();
  float it, ft, ot, gt;
  for (int i = 0; i < length / 4; i++) {
    it = sigmoid(gate_input_t[i]);
    ft = sigmoid(gate_input_t[i + 1000]);
    ot = sigmoid(gate_input_t[i + 2000]);
    gt = tanh(gate_input_t[i + 3000]);
    c_tm1[i] = cont_input * (ft * c_tm1[i]) + it * gt;
    hc_tm1[i] = ot * tanh(c_tm1[i]);
  }
}

long argmax_flops(const vector<float> &probs) {
  int rst = -1;
  float max = -1000000;
  int length = probs.size();
  return length;
}

int argmax(const vector<float> &probs) {
  int rst = -1;
  float max = -1000000;
  int length = probs.size();
  for (int i = 0; i < length; i++) {
    if (max < probs[i]) {
      max = probs[i];
      rst = i;
    }
  }
  return rst;
}
