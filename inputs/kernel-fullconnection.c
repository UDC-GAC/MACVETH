/**
 * File              : kernel-fullconnection.c
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 21 Nov 2019 11:35:29 MST
 * Last Modified Date: Xov 21 Nov 2019 11:37:30 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
inline void fullconnection1(int N, double bottom[N][N][N],
                            double weights[N][N][N][N], double bias[N],
                            double top[N]) {
    int channel = N;
    int height = N;
    int width = N;
    int num_output = N;
    // int channel = bottom.size();
    // int height = bottom[0].size();
    // int width = bottom[0][0].size();
    // int num_output = top.size();
// top.assign(num_output, 0);
// #pragma clang loop critical(disable)
#pragma omp parallel for
    for (int n = 0; n < num_output; ++n) {  //对于每一个输出的输出神经元
        double lat_sw = 1009860000;
        double power_sw = 1.32;
        int InLine = 6;
        // double lat_hw = LAT_HW_6;
        // double power_hw = POW_HW_6;
        // int BRAM = BRAM_HW_6;
        // int DSP = DSP_HW_6;
        // int LUT = LUT_HW_6;
        // int FF = FF_HW_6;
        for (int c = 0; c < channel; ++c)
            for (int h = 0; h < height; ++h)
                for (int w = 0; w < width; ++w) {
                    top[n] += weights[n][c][h][w] * bottom[c][h][w];
                }

        top[n] += bias[n];  //神经元的输出最后加上bias
    }
}
