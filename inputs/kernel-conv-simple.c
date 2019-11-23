//#include <immintrin.h>

/**
 * File              : kernel-conv-simple.c
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 19 Nov 2019 15:38:07 MST
 * Last Modified Date: Sáb 23 Nov 2019 12:01:12 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
inline void convolution4(int N, const double bottom[N][N][N], int stride,
                         int pad, const double weights[N][N][N][N],
                         const double bias[N], double top[N][N][N]) {
    // if (pad) {  //如果要加边框的话
    //    add_pad(bottom, pad);
    //}
    int channel = N;
    int height = N;
    int width = N;
    // int channel = bottom.size();
    // int height = bottom[0].size();
    // int width = bottom[0][0].size();

    //    struct timeval tstart, tend, tdiff;
    //    double timediff = 0;
    //    gettimeofday(&tstart, NULL);
    // int cout = weights.size();
    int cout = N;
    // int kernel_size = weights[0][0].size();
    int kernel_size = N;
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
                for (int i = h_start, m = 0; i < h_end; ++i, ++m)
                    for (int j = w_start, n = 0; j < w_end; ++j, ++n)
                        for (int ci = 0; ci < cout; ++ci) {
                            //__m256d loadReg0 =
                            //    _mm256_load_pd(&weights[co][ci][m][n]);
                            //__m256d loadReg1 =
                            //    _mm256_load_pd(&bottom[ci][i][j]);
                            //__m256d loadReg2 = _mm256_load_pd(&sum);
                            //__m256d t0 =
                            //    _mm256_fmadd_pd(loadReg0, loadReg1, loadReg2);
                            sum =
                                sum + weights[co][ci][m][n] * bottom[ci][i][j];
                        }
                // top[co][h][w] = sum + bias[co];
            }
    }
    // gettimeofday(&tend, NULL);
    // timediff = ((tend.tv_sec - tstart.tv_sec) * 1000000.00) +
    //           (tend.tv_usec - tstart.tv_usec);
    //// timediff = (tdiff.tv_sec*1000000.00) + tdiff.tv_usec;
    // printf("Total Time (Enc + Dec): %f ms \n", timediff);
}
