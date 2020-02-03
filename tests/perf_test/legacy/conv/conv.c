//#include <immintrin.h>

/**
 * File              : conv.c
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 19 nov 2019 15:38:07 MST
 * Last Modified Date: Dom 08 Dec 2019 22:09:21 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#include "conv.h"

void convolution(int n, int kernel_size, int stride, int pad,
                 DATA_TYPE POLYBENCH_1D(bias, N, n),
                 DATA_TYPE POLYBENCH_3D(bottom, N, N, N, n, n, n),
                 DATA_TYPE POLYBENCH_4D(weights, N, N, N, N, n, n, n, n),
                 DATA_TYPE POLYBENCH_3D(top, N, N, N, n, n, n)) {
  // if (pad) {  //如果要加边框的话
  //    add_pad(bottom, pad);
  //}
  int channel = n;
  int height = n;
  int width = n;
  // int channel = bottom.size();
  // int height = bottom[0].size();
  // int width = bottom[0][0].size();

  //    struct timeval tstart, tend, tdiff;
  //    double timediff = 0;
  //    gettimeofday(&tstart, nULL);
  // int cout = weights.size();
  int cout = n;
  // int kernel_size = weights[0][0].size();
  // int kernel_size = n;
  int hout = (height - kernel_size) / stride + 1;
  int wout = (width - kernel_size) / stride + 1;
  for (int co = 0; co < cout; ++co) {
    int InLine = 4;
    // double lat_hw = LAT_HW_4;
    double lat_sw = 2529486000;
    // double power_hw = POW_HW_4;
    double power_sw = 1.32;
    // int BRAM = BRAM_HW_4;
    // int DSP = DSP_HW_4;
    // int LUT = LUT_HW_4;
    // int FF = FF_HW_4;
    for (int h = 0; h < hout; ++h)
      for (int w = 0; w < wout; ++w) {
        int h_start = h * stride;
        int h_end = h_start + kernel_size;
        int w_start = w * stride;
        int w_end = w_start + kernel_size;
        double sum;
        for (int i = h_start, mm = 0; i < h_end, mm < h_end; ++i, ++mm)
          for (int j = w_start, nn = 0; j < w_end; ++j, ++nn)
            // for (int ci = 0; ci < cout; ++ci) {
            for (int ci = 0; ci < 32; ++ci) {
              sum += weights[co][ci][mm][nn] * bottom[ci][i][j];
            }
        top[co][h][w] = sum + bias[co];
      }
  }
  // gettimeofday(&tend, nULL);
  // timediff = ((tend.tv_sec - tstart.tv_sec) * 1000000.00) +
  //           (tend.tv_usec - tstart.tv_usec);
  //// timediff = (tdiff.tv_sec*1000000.00) + tdiff.tv_usec;
  // printf("Total Time (Enc + Dec): %f ms \n", timediff);
};
