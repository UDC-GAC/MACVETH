/**
 * File              : kernel-pooling.c
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 19 Nov 2019 15:39:53 MST
 * Last Modified Date: Lun 09 Dec 2019 09:49:40 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#include "pooling.h"

void pooling(int n, DATA_TYPE POLYBENCH_3D(bottom, N, N, N, n, n, n),
             int kernel_size, int stride, int pad,
             DATA_TYPE POLYBENCH_3D(top, N, N, N, n, n, n), string type) {
  // int channel = bottom.size();
  int channel = n;
  // int height = bottom[0].size();
  int height = n;
  // int width = bottom[0][0].size();
  int width = n;
  int hout = (height - kernel_size) / stride + 1;
  int wout = (width - kernel_size) / stride + 1;

  //#pragma omp parallel for
  for (int c = 0; c < channel; ++c)
    for (int h = 0; h < hout; ++h)
      for (int w = 0; w < wout; ++w) {
        int h_start = h * stride;
        int h_end = h_start + kernel_size;
        int w_start = w * stride;
        int w_end = w_start + kernel_size;
        if (!strcmp(type.c_str(), "AVE")) {
          float sum = 0;
          for (int i = h_start; i < h_end; ++i)
            for (int j = w_start; j < w_end; ++j) {
              sum = sum + bottom[c][i][j];
            }
          top[c][h][w] = sum / (kernel_size * kernel_size);
        }
        if (!strcmp(type.c_str(), "MAX")) {
          float max_value = -10000000000.0;
          for (int i = h_start; i < h_end; ++i)
            for (int j = w_start; j < w_end; ++j) {
              max_value = std::max(max_value, bottom[c][i][j]);
            }
          top[c][h][w] = max_value;
        }
      }
}
