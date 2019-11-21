/**
 * File              : kernel-pooling.c
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 19 Nov 2019 15:39:53 MST
 * Last Modified Date: Mar 19 Nov 2019 15:40:30 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
inline void pooling(quote_vector3D(bottom), int kernel_size, int stride,
                    int pad, quote_vector3D(top), string type) {
    //    // double lat_sw = 2390000;
    //    // double power_sw = 1.32;
    //    // bottom : n x c x h x w
    //    // kernel_size: pooling 的模板大小
    //    // stride： 步长
    //    // pad：若为0，则不在边上补充边框，若不为0，则补充
    //    // top: n x c x hout x wout
    //    //所以pooling只改变了图像的大小，其他维度没有变化
    //    if (pad) {  //如果要加边框的话
    //        add_pad(
    //            bottom,
    //            pad);  //我们这里定义add_pad(blob bottom, int
    //                   //
    //                   pad)函数，给每一个图像加上一个厚度为pad，像素值为0的边框
    //                   //则此时 bottom：n x c x (h+2*pad) x (w+2*pad)
    //    }
    int channel = bottom.size();
    int height = bottom[0].size();
    int width = bottom[0][0].size();
    int hout = (height - kernel_size) / stride + 1;
    int wout = (width - kernel_size) / stride + 1;

#pragma omp parallel for
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
                            sum += bottom[c][i][j];
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
