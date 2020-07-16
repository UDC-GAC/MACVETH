#include <immintrin.h>

/**
 * File              : conv-avx.c
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 19 nov 2019 15:38:07 MST
 * Last Modified Date: Lun 09 Dec 2019 17:12:48 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#include "conv-avx.h"

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
            for (int ci = 0; ci < 32; ci += 32) {
              int offset = h_end * sizeof(double) + w_end * sizeof(double);
              __m256d loadReg0 = _mm256_loadu_pd(&weights[co][ci][mm][nn]);
              __m256d loadReg1 = _mm256_loadu_pd(&bottom[ci][i][j]);
              __m256d loadReg2 =
                  _mm256_loadu_pd(&weights[co][ci][mm][nn] + 32 + offset);
              __m256d loadReg3 =
                  _mm256_loadu_pd(&bottom[ci][i][j] + 32 + offset);
              __m256d loadReg4 =
                  _mm256_loadu_pd(&weights[co][ci][mm][nn] + 64 + offset);
              __m256d loadReg5 =
                  _mm256_loadu_pd(&bottom[ci][i][j] + 64 + offset);
              __m256d loadReg6 =
                  _mm256_loadu_pd(&weights[co][ci][mm][nn] + 96 + offset);
              __m256d loadReg7 =
                  _mm256_loadu_pd(&bottom[ci][i][j] + 96 + offset);
              __m256d loadReg8 =
                  _mm256_loadu_pd(&weights[co][ci][mm][nn] + 128 + offset);
              __m256d loadReg9 =
                  _mm256_loadu_pd(&bottom[ci][i][j] + 128 + offset);
              __m256d loadReg10 =
                  _mm256_loadu_pd(&weights[co][ci][mm][nn] + 160 + offset);
              __m256d loadReg11 =
                  _mm256_loadu_pd(&bottom[ci][i][j] + 160 + offset);
              __m256d loadReg12 =
                  _mm256_loadu_pd(&weights[co][ci][mm][nn] + 192 + offset);
              __m256d loadReg13 =
                  _mm256_loadu_pd(&bottom[ci][i][j] + 192 + offset);
              __m256d loadReg14 =
                  _mm256_loadu_pd(&weights[co][ci][mm][nn] + 224 + offset);
              __m256d loadReg15 =
                  _mm256_loadu_pd(&bottom[ci][i][j] + 224 + offset);
              __m256d unroll0 = _mm256_mul_pd(loadReg0, loadReg1);
              __m256d unroll4 = _mm256_fmadd_pd(loadReg2, loadReg3, unroll0);
              __m256d unroll8 = _mm256_fmadd_pd(loadReg4, loadReg5, unroll4);
              __m256d unroll12 = _mm256_fmadd_pd(loadReg6, loadReg7, unroll8);
              __m256d unroll16 = _mm256_fmadd_pd(loadReg8, loadReg9, unroll12);
              __m256d unroll20 =
                  _mm256_fmadd_pd(loadReg10, loadReg11, unroll16);
              __m256d unroll24 =
                  _mm256_fmadd_pd(loadReg12, loadReg13, unroll20);
              __m256d unroll28 =
                  _mm256_fmadd_pd(loadReg14, loadReg15, unroll24);
              __m256d ymm0 = _mm256_permute_pd(unroll28, 0x05);
              __m256d ymm1 = _mm256_add_pd(unroll28, ymm0);
              __m256d ymm2 = _mm256_permute2f128_pd(ymm1, ymm1, 0x01);
              __m256d ymm3 = _mm256_add_pd(ymm1, ymm2);
              double output[4] = {0.0, 0.0, 0.0, 0.0};
              _mm256_storeu_pd(output, ymm3);

              sum = output[0];
            }
        //  for (int ci = 0; ci < cout; ++ci) {
        //  sum = sum + weights[co][ci][mm][nn] * bottom[ci][i][j];
        //}
        top[co][h][w] = sum + bias[co];
      }
  }
  // gettimeofday(&tend, nULL);
  // timediff = ((tend.tv_sec - tstart.tv_sec) * 1000000.00) +
  //           (tend.tv_usec - tstart.tv_usec);
  //// timediff = (tdiff.tv_sec*1000000.00) + tdiff.tv_usec;
  // printf("Total Time (Enc + Dec): %f ms \n", timediff);
};
