/**
 * File              : alexnet.cc
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 11 Nov 2019 21:24:25 MST
 * Last Modified Date: Mar 19 Nov 2019 15:35:11 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#include <omp.h>
#include <sys/time.h>

#include "PLA.h"
#include "layers_hls.h"

/* Default to LARGE_DATASET. */
#if !defined(MINI_DATASET) && !defined(SMALL_DATASET) && \
    !defined(MEDIUM_DATASET) && !defined(LARGE_DATASET)
#define LARGE_DATASET
#endif

/* Define sample dataset sizes. */
#ifdef MINI_DATASET
#define CROP_FACTOR 8
#endif

#ifdef SMALL_DATASET
#define CROP_FACTOR 4
#endif

#ifdef MEDIUM_DATASET
#define CROP_FACTOR 2
#endif

#ifdef LARGE_DATASET
#define CROP_FACTOR 1
#endif

static double rtclock() {
    struct timeval Tp;
    int stat;
    stat = gettimeofday(&Tp, NULL);
    if (stat != 0) printf("Error return from gettimeofday: %d", stat);
    return (Tp.tv_sec + Tp.tv_usec * 1.0e-6);
}

#define release4D(name) vector<vector<vector<vector<float>>>>(name).swap(name)
#define release3D(name) vector<vector<vector<float>>>(name).swap(name)
#define release2D(name) vector<vector<float>>(name).swap(name)
#define release(name) vector<float>(name).swap(name)
// image image.bin
// caffe_model
//     conv1.bin
//     conv2.bin
//     conv3.bin
//     conv4.bin
//     conv5.bin
//     fc6.bin
//     fc7.bin
//     fc8.bin

inline void relu(quote_vector3D(bottom)) {
    //		double lat_sw = 1062000;
    //		double power_sw = 1.32;
    int channel = bottom.size();
    int width = bottom[0].size();
    int height = bottom[0][0].size();
#pragma omp parallel for
    for (int c = 0; c < channel; ++c)
        for (int h = 0; h < height; ++h)
            for (int w = 0; w < width; ++w) {
                bottom[c][h][w] = std::max(bottom[c][h][w], 0.0f);
            }
}
inline void relu(vector<float>& bottom) {
    //		double lat_sw = 1062000;
    //		double power_sw = 1.32;
    int count = bottom.size();
#pragma omp parallel for
    for (int i = 0; i < count; ++i) bottom[i] = std::max(bottom[i], 0.0f);
}

inline void dropout(vector<float>& bottom, float ratio) {
    // ratio 是一个 0~1的浮点数

    int count = bottom.size();
#pragma omp parallel for
    for (int i = 0; i < count; i++) {
        bottom[i] = bottom[i] * ratio;
    }
}

inline void dropout(quote_vector3D(bottom), float ratio) {
    // ratio 是一个 0~1的浮点数
    int channel = bottom.size();
    int width = bottom[0].size();
    int height = bottom[0][0].size();
    for (int c = 0; c < channel; c++)
#pragma omp parallel for
        for (int w = 0; w < width; ++w)
            for (int h = 0; h < height; ++h) {
                bottom[c][w][h] *= ratio;
            }
}

inline void fullconnection1(const quote_vector3D(bottom),
                            const quote_vector4D(weights),
                            const vector<float>& bias, vector<float>& top) {
    int channel = bottom.size();
    int height = bottom[0].size();
    int width = bottom[0][0].size();
    int num_output = top.size();
// top.assign(num_output, 0);
// #pragma clang loop critical(disable)
#pragma omp parallel for
    for (int n = 0; n < num_output; ++n) {  //对于每一个输出的输出神经元
        double lat_sw = 1009860000;
        double power_sw = 1.32;
        int InLine = 6;
        double lat_hw = LAT_HW_6;
        double power_hw = POW_HW_6;
        int BRAM = BRAM_HW_6;
        int DSP = DSP_HW_6;
        int LUT = LUT_HW_6;
        int FF = FF_HW_6;
        for (int c = 0; c < channel; ++c)
            for (int h = 0; h < height; ++h)
                for (int w = 0; w < width; ++w) {
                    top[n] += weights[n][c][h][w] * bottom[c][h][w];
                }

        top[n] += bias[n];  //神经元的输出最后加上bias
    }
}

inline void fullconnection2(const vector<float>& bottom,
                            const vector<vector<float>>& weights,
                            const vector<float>& bias, vector<float>& top) {
    int channel = bottom.size();
    int num_output = top.size();
// top.assign(num_output, 0);
//	#pragma clang loop critical(disable)
#pragma omp parallel for
    for (int n = 0; n < num_output; ++n) {  //对于每一个输出的输出神经元
        double lat_sw = 417910000;
        double power_sw = 1.32;
        int InLine = 7;
        double lat_hw = LAT_HW_7;
        double power_hw = POW_HW_7;
        int BRAM = BRAM_HW_7;
        int DSP = DSP_HW_7;
        int LUT = LUT_HW_7;
        int FF = FF_HW_7;
        for (int c = 0; c < channel; ++c) {
            top[n] += weights[n][c] * bottom[c];
        }
        top[n] += bias[n];  //神经元的输出最后加上bias
    }
}

inline void fullconnection3(const vector<float>& bottom,
                            const vector<vector<float>>& weights,
                            const vector<float>& bias, vector<float>& top) {
    int channel = bottom.size();
    int num_output = top.size();
// top.assign(num_output, 0);
//	#pragma clang loop critical(disable)
#pragma omp parallel for
    for (int n = 0; n < num_output; ++n) {  //对于每一个输出的输出神经元
        double lat_sw = 417910000;
        double power_sw = 1.32;
        int InLine = 8;
        double lat_hw = LAT_HW_8;
        double power_hw = POW_HW_8;
        int BRAM = BRAM_HW_8;
        int DSP = DSP_HW_8;
        int LUT = LUT_HW_8;
        int FF = FF_HW_8;
        for (int c = 0; c < channel; ++c) {
            top[n] += weights[n][c] * bottom[c];
        }
        top[n] += bias[n];  //神经元的输出最后加上bias
    }
}
// inline void fullconnection(const vector <float>& bottom , const vector <
// vector <float> >& weights, vector <float> & top){
//  printf("fc3\n");
//	int channel = bottom.size();
//	int num_output = top.size();
//	top.assign(num_output, 0);
//	for( int n = 0; n < num_output; ++n){     //对于每一个输出的输出神经元
//		for(int c = 0; c < channel; ++c){
//					top[n] += weights[n][c] * bottom[c];
//				}
//		}
//}

inline void lrn(quote_vector3D(bottom), float k, float alpha, float beta,
                int local_size) {
    // bottom: n x c x n x m
    // top: n x c x n x m
    //输入输出维数不变
    // k, alpha, beta, n 计算lrn的参数，详情见公式
    int channel = bottom.size();
    int height = bottom[0].size();
    int width = bottom[0][0].size();
    vector<float> temp_vec(channel);

    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            for (int c = 0; c < channel; ++c) {
                int c_start = std::max(
                    0, c - local_size / 2);  //确定 lrn计算时取的相邻channel范围
                int c_end = std::min(channel - 1, c + local_size / 2);
                float sum = 0;
                for (int i = c_start; i <= c_end; ++i) {
                    sum += bottom[i][h][w] * bottom[i][h][w];
                }
                float temp;
                temp = sum * alpha / local_size + k;
                temp_vec[c] = bottom[c][h][w] / powf(temp, beta);  // pow
                                                                   // 为幂运算
            }
            for (int c = 0; c < channel; ++c) {
                bottom[c][h][w] = temp_vec[c];
            }
        }
    }
}

inline void softmax(vector<float>& bottom) {
    double lat_sw = 561000;
    double power_sw = 1.32;

    //一般softmax作为最后一层输出，此时输入是一个vector，输出是一个vector
    //我们可以认为：

    int count = bottom.size();
    vector<float> exp_rst(count);
    float sum = 0;
    for (int i = 0; i < count; i++) {
        exp_rst[i] = exp(bottom[i]);  //对于每一个输入求其exp(x)；
        sum += exp_rst[i];            //求exp(x)的和；
    }
    for (int i = 0; i < count; i++) {
        bottom[i] = exp_rst[i] / sum;  //输出为一个归一化值
    }
}

inline void add_pad(quote_vector3D(bottom), int pad) {
    // double lat_sw = 4267000;
    // double power_sw = 1.32;
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

inline void pooling(quote_vector3D(bottom), int kernel_size, int stride,
                    int pad, quote_vector3D(top), string type) {
    // double lat_sw = 2390000;
    // double power_sw = 1.32;
    // bottom : n x c x h x w
    // kernel_size: pooling 的模板大小
    // stride： 步长
    // pad：若为0，则不在边上补充边框，若不为0，则补充
    // top: n x c x hout x wout
    //所以pooling只改变了图像的大小，其他维度没有变化
    if (pad) {  //如果要加边框的话
        add_pad(
            bottom,
            pad);  //我们这里定义add_pad(blob bottom, int
                   // pad)函数，给每一个图像加上一个厚度为pad，像素值为0的边框
                   //则此时 bottom：n x c x (h+2*pad) x (w+2*pad)
    }
    int channel = bottom.size();
    int height = bottom[0].size();
    int width = bottom[0][0].size();
    int hout =
        (height - kernel_size) / stride +
        1;  //根据输入的大小，以及pooling模板大小和步长可以计算出pooling输出的大小
    int wout = (width - kernel_size) / stride + 1;

#pragma omp parallel for
    for (int c = 0; c < channel; ++c)  //对于一张图片产生的每一个channel
        for (int h = 0; h < hout; ++h)  //计算输出的一个点 (h,w)
            for (int w = 0; w < wout; ++w) {
                int h_start = h * stride;
                int h_end =
                    h_start + kernel_size;  //这里h_end和w_end应该是不会出界的
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

inline void convolution1(quote_vector3D(bottom), int stride, int pad,
                         const quote_vector4D(weights),
                         const vector<float>& bias, quote_vector3D(top),
                         string layer_name) {
    if (pad) {  //如果要加边框的话
        add_pad(bottom, pad);
    }
    int channel = bottom.size();
    int height = bottom[0].size();
    int width = bottom[0][0].size();

    struct timeval tstart, tend, tdiff;
    double timediff = 0;
    gettimeofday(&tstart, NULL);
    int cout = weights.size();
    int kernel_size = weights[0][0].size();
    int hout =
        (height - kernel_size) / stride +
        1;  //根据输入的大小，以及卷积模板大小和步长可以计算出卷积输出的大小
    int wout = (width - kernel_size) / stride + 1;
// #pragma clang loop critical(disable)
#pragma omp parallel for
    for (int co = 0; co < cout; ++co) {  //一个图片产生的每一个输出的channel
        int InLine = 1;
        double lat_hw = LAT_HW_1;
        double lat_sw = 893386000;
        double power_hw = POW_HW_1;
        double power_sw = 1.32;
        int BRAM = BRAM_HW_1;
        int DSP = DSP_HW_1;
        int LUT = LUT_HW_1;
        int FF = FF_HW_1;
// printf("calculating the %s of channel %d\n", layer_name.c_str(), co+1);
#pragma omp parallel for
        for (int h = 0; h < hout; ++h)  //计算输出的一个点 (h,w)
            for (int w = 0; w < wout; ++w) {
                // printf("calculating the convoluton of point (%d,%d)\n", h,
                // w);
                int h_start = h * stride;
                int h_end = h_start + kernel_size;
                int w_start = w * stride;
                int w_end =
                    w_start + kernel_size;  //求cin x kernel_size x
                                            // kernel_size个数据对应相乘的和；
                float sum = 0;
                for (int i = h_start, m = 0; i < h_end; ++i, ++m)
                    for (int j = w_start, n = 0; j < w_end; ++j, ++n)
                        for (int ci = 0; ci < channel; ++ci) {
                            sum += weights[co][ci][m][n] * bottom[ci][i][j];
                        }
                top[co][h][w] = sum + bias[co];  //最后加上偏置
            }
    }
    gettimeofday(&tend, NULL);
    timediff = (tdiff.tv_sec * 1000000.00) + tdiff.tv_usec;
    printf("Total Time (Enc + Dec): %f ms \n", timediff);
}

inline void convolution2(quote_vector3D(bottom), int stride, int pad,
                         const quote_vector4D(weights),
                         const vector<float>& bias, quote_vector3D(top),
                         string layer_name) {
    if (pad) {  //如果要加边框的话
        add_pad(bottom, pad);
    }
    int channel = bottom.size();
    int height = bottom[0].size();
    int width = bottom[0][0].size();

    struct timeval tstart, tend, tdiff;
    double timediff = 0;
    gettimeofday(&tstart, NULL);
    int cout = weights.size();
    int kernel_size = weights[0][0].size();
    int hout =
        (height - kernel_size) / stride +
        1;  //根据输入的大小，以及卷积模板大小和步长可以计算出卷积输出的大小
    int wout = (width - kernel_size) / stride + 1;
    //	#pragma clang loop critical(disable)
#pragma omp parallel for
    for (int co = 0; co < cout; ++co) {  //一个图片产生的每一个输出的channel
        int InLine = 2;
        double lat_hw = LAT_HW_2;
        double lat_sw = 3168746000;
        double power_hw = POW_HW_2;
        double power_sw = 1.32;
        int BRAM = BRAM_HW_2;
        int DSP = DSP_HW_2;
        int LUT = LUT_HW_2;
        int FF = FF_HW_2;
        // printf("calculating the %s of channel %d\n", layer_name.c_str(),
        // co+1);
        for (int h = 0; h < hout; ++h)  //计算输出的一个点 (h,w)
            for (int w = 0; w < wout; ++w) {
                // printf("calculating the convoluton of point (%d,%d)\n", h,
                // w);
                int h_start = h * stride;
                int h_end = h_start + kernel_size;
                int w_start = w * stride;
                int w_end =
                    w_start + kernel_size;  //求cin x kernel_size x
                                            // kernel_size个数据对应相乘的和；
                float sum = 0;
                for (int i = h_start, m = 0; i < h_end; ++i, ++m)
                    for (int j = w_start, n = 0; j < w_end; ++j, ++n)
                        for (int ci = 0; ci < channel; ++ci) {
                            sum += weights[co][ci][m][n] * bottom[ci][i][j];
                        }
                top[co][h][w] = sum + bias[co];  //最后加上偏置
            }
    }
    gettimeofday(&tend, NULL);
    timediff = ((tend.tv_sec - tstart.tv_sec) * 1000000.00) +
               (tend.tv_usec - tstart.tv_usec);
    // timediff = (tdiff.tv_sec*1000000.00) + tdiff.tv_usec;
    printf("Total Time (Enc + Dec): %f ms \n", timediff);
}

inline void convolution3(quote_vector3D(bottom), int stride, int pad,
                         const quote_vector4D(weights),
                         const vector<float>& bias, quote_vector3D(top),
                         string layer_name) {
    if (pad) {  //如果要加边框的话
        add_pad(bottom, pad);
    }
    int channel = bottom.size();
    int height = bottom[0].size();
    int width = bottom[0][0].size();

    struct timeval tstart, tend, tdiff;
    double timediff = 0;
    gettimeofday(&tstart, NULL);
    int cout = weights.size();
    int kernel_size = weights[0][0].size();
    int hout =
        (height - kernel_size) / stride +
        1;  //根据输入的大小，以及卷积模板大小和步长可以计算出卷积输出的大小
    int wout = (width - kernel_size) / stride + 1;
//	#pragma clang loop critical(disable)
#pragma omp parallel for
    for (int co = 0; co < cout; ++co) {  //一个图片产生的每一个输出的channel
        int InLine = 3;
        double lat_hw = LAT_HW_3;
        double lat_sw = 8534933000;
        double power_hw = POW_HW_3;
        double power_sw = 1.32;
        int BRAM = BRAM_HW_3;
        int DSP = DSP_HW_3;
        int LUT = LUT_HW_3;
        int FF = FF_HW_3;
        // printf("calculating the %s of channel %d\n", layer_name.c_str(),
        // co+1);
        for (int h = 0; h < hout; ++h)  //计算输出的一个点 (h,w)
            for (int w = 0; w < wout; ++w) {
                // printf("calculating the convoluton of point (%d,%d)\n", h,
                // w);
                int h_start = h * stride;
                int h_end = h_start + kernel_size;
                int w_start = w * stride;
                int w_end =
                    w_start + kernel_size;  //求cin x kernel_size x
                                            // kernel_size个数据对应相乘的和；
                float sum = 0;
                for (int i = h_start, m = 0; i < h_end; ++i, ++m)
                    for (int j = w_start, n = 0; j < w_end; ++j, ++n)
                        for (int ci = 0; ci < channel; ++ci) {
                            sum += weights[co][ci][m][n] * bottom[ci][i][j];
                        }
                top[co][h][w] = sum + bias[co];  //最后加上偏置
            }
    }
    gettimeofday(&tend, NULL);
    timediff = ((tend.tv_sec - tstart.tv_sec) * 1000000.00) +
               (tend.tv_usec - tstart.tv_usec);
    // timediff = (tdiff.tv_sec*1000000.00) + tdiff.tv_usec;
    printf("Total Time (Enc + Dec): %f ms \n", timediff);
}

inline void convolution4(quote_vector3D(bottom), int stride, int pad,
                         const quote_vector4D(weights),
                         const vector<float>& bias, quote_vector3D(top),
                         string layer_name) {
    if (pad) {  //如果要加边框的话
        add_pad(bottom, pad);
    }
    int channel = bottom.size();
    int height = bottom[0].size();
    int width = bottom[0][0].size();

    struct timeval tstart, tend, tdiff;
    double timediff = 0;
    gettimeofday(&tstart, NULL);
    int cout = weights.size();
    int kernel_size = weights[0][0].size();
    int hout =
        (height - kernel_size) / stride +
        1;  //根据输入的大小，以及卷积模板大小和步长可以计算出卷积输出的大小
    int wout = (width - kernel_size) / stride + 1;
//	#pragma clang loop critical(disable)
#pragma omp parallel for
    for (int co = 0; co < cout; ++co) {  //一个图片产生的每一个输出的channel
        int InLine = 4;
        double lat_hw = LAT_HW_4;
        double lat_sw = 2529486000;
        double power_hw = POW_HW_4;
        double power_sw = 1.32;
        int BRAM = BRAM_HW_4;
        int DSP = DSP_HW_4;
        int LUT = LUT_HW_4;
        int FF = FF_HW_4;
        // printf("calculating the %s of channel %d\n", layer_name.c_str(),
        // co+1);
        for (int h = 0; h < hout; ++h)  //计算输出的一个点 (h,w)
            for (int w = 0; w < wout; ++w) {
                // printf("calculating the convoluton of point (%d,%d)\n", h,
                // w);
                int h_start = h * stride;
                int h_end = h_start + kernel_size;
                int w_start = w * stride;
                int w_end =
                    w_start + kernel_size;  //求cin x kernel_size x
                                            // kernel_size个数据对应相乘的和；
                float sum = 0;
                for (int i = h_start, m = 0; i < h_end; ++i, ++m)
                    for (int j = w_start, n = 0; j < w_end; ++j, ++n)
                        for (int ci = 0; ci < channel; ++ci) {
                            sum += weights[co][ci][m][n] * bottom[ci][i][j];
                        }
                top[co][h][w] = sum + bias[co];  //最后加上偏置
            }
    }
    gettimeofday(&tend, NULL);
    timediff = ((tend.tv_sec - tstart.tv_sec) * 1000000.00) +
               (tend.tv_usec - tstart.tv_usec);
    // timediff = (tdiff.tv_sec*1000000.00) + tdiff.tv_usec;
    printf("Total Time (Enc + Dec): %f ms \n", timediff);
}

inline void convolution5(quote_vector3D(bottom), int stride, int pad,
                         const quote_vector4D(weights),
                         const vector<float>& bias, quote_vector3D(top),
                         string layer_name) {
    if (pad) {  //如果要加边框的话
        add_pad(bottom, pad);
    }
    int channel = bottom.size();
    int height = bottom[0].size();
    int width = bottom[0][0].size();

    struct timeval tstart, tend, tdiff;
    double timediff = 0;
    gettimeofday(&tstart, NULL);
    int cout = weights.size();
    int kernel_size = weights[0][0].size();
    int hout =
        (height - kernel_size) / stride +
        1;  //根据输入的大小，以及卷积模板大小和步长可以计算出卷积输出的大小
    int wout = (width - kernel_size) / stride + 1;
    //	#pragma clang loop critical(disable)
#pragma omp parallel for
    for (int co = 0; co < cout; ++co) {  //一个图片产生的每一个输出的channel
        int InLine = 5;
        double lat_hw = LAT_HW_5;
        double lat_sw = 1637551000;
        double power_hw = POW_HW_5;
        double power_sw = 1.32;
        int BRAM = BRAM_HW_5;
        int DSP = DSP_HW_5;
        int LUT = LUT_HW_5;
        int FF = FF_HW_5;
        // printf("calculating the %s of channel %d\n", layer_name.c_str(),
        // co+1);
        for (int h = 0; h < hout; ++h)  //计算输出的一个点 (h,w)
            for (int w = 0; w < wout; ++w) {
                // printf("calculating the convoluton of point (%d,%d)\n", h,
                // w);
                int h_start = h * stride;
                int h_end = h_start + kernel_size;
                int w_start = w * stride;
                int w_end =
                    w_start + kernel_size;  //求cin x kernel_size x
                                            // kernel_size个数据对应相乘的和；
                float sum = 0;
                for (int i = h_start, m = 0; i < h_end; ++i, ++m)
                    for (int j = w_start, n = 0; j < w_end; ++j, ++n)
                        for (int ci = 0; ci < channel; ++ci) {
                            sum += weights[co][ci][m][n] * bottom[ci][i][j];
                        }
                top[co][h][w] = sum + bias[co];  //最后加上偏置
            }
    }
    gettimeofday(&tend, NULL);
    timediff = ((tend.tv_sec - tstart.tv_sec) * 1000000.00) +
               (tend.tv_usec - tstart.tv_usec);
    // timediff = (tdiff.tv_sec*1000000.00) + tdiff.tv_usec;
    printf("Total Time (Enc + Dec): %f ms \n", timediff);
}

inline void readimage(string ss, quote_vector3D(image)) {
    // double lat_sw = 33408000;
    // double power_sw = 1.32;
    FILE* fin;
    fin = fopen(ss.c_str(), "rb");
    int channel = image.size();
    int height = image[0].size();
    int width = image[0][0].size();
    float* temp = (float*)malloc(sizeof(float) * width);
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

inline void readdescriptor(char* s, vector<float>& descriptor) {
    FILE* fin;
    fin = fopen(s, "rb");
    int length = descriptor.size();
    float* temp = (float*)malloc(sizeof(float) * length);
    fread(temp, sizeof(float), length, fin);
    vector<float> temp_vec(temp, temp + length);
    descriptor = temp_vec;
    fclose(fin);
    free(temp);
    return;
}

inline void readweights(string ss, quote_vector4D(weights),
                        vector<float>& bias) {
    FILE* fin;
    fin = fopen(ss.c_str(), "rb");
    int num_output = weights.size();
    int channel = weights[0].size();
    int kernel_size = weights[0][0].size();
    float* temp = (float*)malloc(sizeof(float) * kernel_size);
    for (int n = 0; n < num_output; ++n) {
        for (int c = 0; c < channel; ++c)
            for (int k = 0; k < kernel_size; ++k) {
                fread(temp, sizeof(float), kernel_size, fin);
                vector<float> temp_vec(temp, temp + kernel_size);
                weights[n][c][k] = temp_vec;
            }
    }
    float* temp_bias = (float*)malloc(sizeof(float) * num_output);
    fread(temp_bias, sizeof(float), num_output, fin);
    vector<float> bias_vec(temp_bias, temp_bias + num_output);
    bias = bias_vec;
    fclose(fin);
    free(temp);
    free(temp_bias);
    return;
}

inline void readweights(string ss, vector<vector<float>>& weights) {
    // double lat_sw = 248570000;
    // double power_sw = 1.32;
    FILE* fin;
    fin = fopen(ss.c_str(), "rb");
    int num_output = weights.size();
    int channel = weights[0].size();
    float* temp = (float*)malloc(sizeof(float) * channel);
    //	#pragma clang loop critical(disable)
    for (int n = 0; n < num_output; ++n) {
        //		int InLine = 4;
        //		double lat_hw = LAT_HW_4;
        //		double lat_sw = 643600001;
        //		double power_hw = POW_HW_4;
        //		double power_sw = 1.32;
        //		int BRAM = BRAM_HW_4;
        //		int DSP = DSP_HW_4;
        //		int LUT = LUT_HW_4;
        //		int FF = FF_HW_4;
        fread(temp, sizeof(float), channel, fin);
        vector<float> temp_vec(temp, temp + channel);
        weights[n] = temp_vec;
    }
    fclose(fin);
    free(temp);
}

inline void readweights(string ss, vector<vector<float>>& weights,
                        vector<float>& bias) {
    //  double lat_sw = 766107000;
    // double power_sw = 1.32;
    FILE* fin;
    fin = fopen(ss.c_str(), "rb");
    int num_output = weights.size();
    int channel = weights[0].size();
    float* temp = (float*)malloc(sizeof(float) * channel);
    for (int n = 0; n < num_output; ++n) {
        fread(temp, sizeof(float), channel, fin);
        vector<float> temp_vec(temp, temp + channel);
        weights[n] = temp_vec;
    }
    if (bias.size()) {
        float* temp_bias = (float*)malloc(sizeof(float) * num_output);
        fread(temp_bias, sizeof(float), num_output, fin);
        vector<float> bias_vec(temp_bias, temp_bias + num_output);
        bias = bias_vec;
        free(temp_bias);
    }
    fclose(fin);
    free(temp);
}

/*void print_vec(char * s, const vector <float>& vec ){
        FILE *fin;
        fin = fopen(s, "wt");
        int count = vec.size();
        for (int i = 0; i < count; ++i){
                fprintf(fin, "%0.8f ", vec[i]);
        }
        fclose(fin);
}*/
/*
void print_vec(char * s, const vector <int>& vec){
        FILE *fin;
        fin = fopen(s, "wt");
        int count = vec.size();
        for (int i = 0; i < count; ++i){
                fprintf(fin, "%d ", vec[i]);
        }
        fclose(fin);
}
*/
/*
void print_vec(char * s, const vector < vector <float> >& vec){
        FILE *fin;
        fin = fopen(s, "wt");
        int height = vec.size();
        int width = vec[0].size();
        for (int i = 0; i < height; ++i){
                for (int j = 0; j < width; ++j){
                        fprintf(fin, "%14.8f ", vec[i][j]);
                        //fwrite((void *)&(vec[i][j]), sizeof(float), 1, fin);
                }
                fprintf(fin, "\n");
        }
        fclose(fin);
}
*/
inline void EmbedLayer(int word_input,
                       const vector<vector<float>>& embed_weights,
                       vector<float>& input_sentence) {
    //把一个输入的单词转换成向量1->1000维
    int length = embed_weights.size();
    if (word_input < 0 || word_input >= length) { /*printf("illegal input\n");*/
        return;
    }
    input_sentence = embed_weights[word_input];
}

inline void PredictLayer(const vector<float>& bottom,
                         const vector<vector<float>>& weights,
                         const vector<float>& bias, vector<float>& top) {
    //一个1000维的向量转成对应的词典长度的向量 8801维
    int channel = bottom.size();
    int num_output = top.size();
    top.assign(num_output, 0);
    for (int n = 0; n < num_output; ++n) {  //对于每一个输出的输出神经元
        for (int c = 0; c < channel; ++c) {
            top[n] += weights[n][c] * bottom[c];
        }
        top[n] += bias[n];  //神经元的输出最后加上bias
    }
}
inline void Update(int cont_input, vector<float>& hc_rst) {
    int length = hc_rst.size();
    for (int i = 0; i < length; i++) {
        hc_rst[i] = cont_input * hc_rst[i];
    }
}
inline void LSTMFullconnectionLayer(const vector<float>& bottom,
                                    const vector<vector<float>>& weights,
                                    vector<float>& top) {
    // lstm中的全连接计算
    int channel = bottom.size();
    int num_output = top.size();
    top.assign(num_output, 0);
    for (int n = 0; n < num_output; ++n) {  //对于每一个输出的输出神经元
        for (int c = 0; c < channel; ++c) {
            top[n] += weights[n][c] * bottom[c];
        }
    }
}
inline void Add_Result(vector<float>& gate_input_t,
                       const vector<float>& xcstatic_rst,
                       const vector<float>& Wxc_tm1,
                       const vector<float>& Whc_tm1,
                       const vector<float>& xc_bias) {
    //全连接计算结果求和
    int length = gate_input_t.size();
    for (int i = 0; i < length; i++) {
        gate_input_t[i] =
            xcstatic_rst[i] + Wxc_tm1[i] + Whc_tm1[i] + xc_bias[i];
    }
}
inline float sigmoid(float x) { return 1. / (1. + exp(-x)); }

// float tanh(float x) {
//  return 2. * sigmoid(2. * x) - 1.;
//}
inline void LSTMlayer(int cont_input, const vector<float>& gate_input_t,
                      vector<float>& c_tm1, vector<float>& hc_tm1) {
    //非线性操作更新ht和ct
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
inline int argmax(const vector<float>& probs) {
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

__attribute__((flatten)) int main() {
    // read image
    vector3D(image, 3, 227, 227);
    readimage("image.bin", image);
    printf("readimage done\n");

    double polybench_t_start = rtclock();

    /////////////conv1//////////////
    // prepare weight
    int conv_kernel_size = 11, channels = 3, num_output = 96 / CROP_FACTOR;
    vector4D(conv1_weights, num_output, channels, conv_kernel_size,
             conv_kernel_size);
    vector<float> conv1_bias(num_output);
    readweights("conv1.bin", conv1_weights, conv1_bias);
    int stride = 4, pad = 0;
    vector3D(conv1_rst, num_output, 55, 55);
    // compute
    convolution1(image, stride, pad, conv1_weights, conv1_bias, conv1_rst,
                 "conv1");
    // free space
    release3D(image);
    release(conv1_bias);
    release4D(conv1_weights);
    printf("conv1 done\n");

    /////////////relu1///////////////
    relu(conv1_rst);
    printf("relu1 done\n");

    ////////////lrn1/////////////////
    int local_size = 5;
    float k = 1.0, alpha = 0.0001, beta = 0.75;
    //	lrn(conv1_rst, k, alpha, beta, local_size);
    // printf("lrn1 done\n");

    ////////////pool1////////////////
    int pooling_kernel_size = 3;
    stride = 2, pad = 0;
    vector3D(pool1_rst, num_output, 27, 27);
    pooling(conv1_rst, pooling_kernel_size, stride, pad, pool1_rst, "MAX");
    release3D(conv1_rst);
    printf("pool1 done\n");

    ////////////conv2/////////////
    vector3D(pool1_rst_1, num_output / 2, 27, 27);
    copy(pool1_rst.begin(), pool1_rst.begin() + (num_output / 2),
         pool1_rst_1.begin());
    vector3D(pool1_rst_2, num_output / 2, 27, 27);
    copy(pool1_rst.begin() + (num_output / 2), pool1_rst.end(),
         pool1_rst_2.begin());
    conv_kernel_size = 5, channels = num_output / 2,
    num_output = 256 / CROP_FACTOR;  // channels 是上一层的num_output
    vector4D(conv2_weights, num_output, channels, conv_kernel_size,
             conv_kernel_size);
    vector4D(conv2_weights_1, num_output / 2, channels, conv_kernel_size,
             conv_kernel_size);
    vector4D(conv2_weights_2, num_output / 2, channels, conv_kernel_size,
             conv_kernel_size);
    vector<float> conv2_bias(num_output);
    vector<float> conv2_bias_1(num_output / 2);
    vector<float> conv2_bias_2(num_output / 2);
    readweights("conv2.bin", conv2_weights, conv2_bias);
    copy(conv2_weights.begin(), conv2_weights.begin() + (num_output / 2),
         conv2_weights_1.begin());
    copy(conv2_weights.begin() + (num_output / 2), conv2_weights.end(),
         conv2_weights_2.begin());
    copy(conv2_bias.begin(), conv2_bias.begin() + (num_output / 2),
         conv2_bias_1.begin());
    copy(conv2_bias.begin() + (num_output / 2), conv2_bias.end(),
         conv2_bias_2.begin());
    release3D(pool1_rst);
    release(conv2_bias);
    release4D(conv2_weights);
    stride = 1, pad = 2;
    vector3D(conv2_rst, num_output, 27, 27);
    vector3D(conv2_rst_1, num_output / 2, 27, 27);
    vector3D(conv2_rst_2, num_output / 2, 27, 27);
    convolution2(pool1_rst_1, stride, pad, conv2_weights_1, conv2_bias_1,
                 conv2_rst_1, "conv2_1");
    convolution2(pool1_rst_2, stride, pad, conv2_weights_2, conv2_bias_2,
                 conv2_rst_2, "conv2_2");
    copy(conv2_rst_1.begin(), conv2_rst_1.end(), conv2_rst.begin());
    copy(conv2_rst_2.begin(), conv2_rst_2.end(),
         conv2_rst.begin() + num_output / 2);
    release3D(conv2_rst_1);
    release3D(conv2_rst_2);
    release3D(pool1_rst_1);
    release3D(pool1_rst_2);
    release(conv2_bias_1);
    release(conv2_bias_2);
    release4D(conv2_weights_1);
    release4D(conv2_weights_2);
    printf("conv2 done\n");

    ///////////////relu2////////////////
    relu(conv2_rst);
    printf("relu2 done\n");

    ////////////////lrn2/////////////////
    // lrn(conv2_rst, k, alpha, beta, local_size);
    // printf("lrn2 done\n");

    ////////////////pool2////////////////
    pooling_kernel_size = 3;
    stride = 2, pad = 0;
    vector3D(pool2_rst, num_output, 13, 13);
    pooling(conv2_rst, pooling_kernel_size, stride, pad, pool2_rst, "MAX");
    release3D(conv2_rst);
    printf("pool2 done\n");

#ifdef DATA_SIZE_SMALL
    return 0;
#endif

    ////////////////conv3////////////////
    conv_kernel_size = 3, channels = num_output, num_output = 384 / CROP_FACTOR;
    vector4D(conv3_weights, num_output, channels, conv_kernel_size,
             conv_kernel_size);
    vector<float> conv3_bias(num_output);
    readweights("conv3.bin", conv3_weights, conv3_bias);
    stride = 1, pad = 1;
    vector3D(conv3_rst, num_output, 13, 13);
    convolution3(pool2_rst, stride, pad, conv3_weights, conv3_bias, conv3_rst,
                 "conv3");
    release3D(pool2_rst);
    release(conv3_bias);
    release4D(conv3_weights);
    printf("conv3 done\n");

    ////////////////relu3////////////////
    relu(conv3_rst);
    printf("relu3 done\n");

    ////////////////conv4////////////////
    vector3D(conv3_rst_1, num_output / 2, 13, 13);
    copy(conv3_rst.begin(), conv3_rst.begin() + (num_output / 2),
         conv3_rst_1.begin());
    vector3D(conv3_rst_2, num_output / 2, 13, 13);
    copy(conv3_rst.begin() + (num_output / 2), conv3_rst.end(),
         conv3_rst_2.begin());
    conv_kernel_size = 3, channels = num_output / 2,
    num_output = 384 / CROP_FACTOR;
    vector4D(conv4_weights, num_output, channels, conv_kernel_size,
             conv_kernel_size);
    vector4D(conv4_weights_1, num_output / 2, channels, conv_kernel_size,
             conv_kernel_size);
    vector4D(conv4_weights_2, num_output / 2, channels, conv_kernel_size,
             conv_kernel_size);
    vector<float> conv4_bias(num_output);
    vector<float> conv4_bias_1(num_output / 2);
    vector<float> conv4_bias_2(num_output / 2);
    readweights("conv4.bin", conv4_weights, conv4_bias);
    copy(conv4_weights.begin(), conv4_weights.begin() + (num_output / 2),
         conv4_weights_1.begin());
    copy(conv4_weights.begin() + (num_output / 2), conv4_weights.end(),
         conv4_weights_2.begin());
    copy(conv4_bias.begin(), conv4_bias.begin() + (num_output / 2),
         conv4_bias_1.begin());
    copy(conv4_bias.begin() + (num_output / 2), conv4_bias.end(),
         conv4_bias_2.begin());
    release3D(conv3_rst);
    release(conv4_bias);
    release4D(conv4_weights);
    stride = 1, pad = 1;
    vector3D(conv4_rst_1, num_output / 2, 13, 13);
    vector3D(conv4_rst_2, num_output / 2, 13, 13);
    convolution4(conv3_rst_1, stride, pad, conv4_weights_1, conv4_bias_1,
                 conv4_rst_1, "conv4_1");
    convolution4(conv3_rst_2, stride, pad, conv4_weights_2, conv4_bias_2,
                 conv4_rst_2, "conv4_2");
    release3D(conv3_rst_1);
    release3D(conv3_rst_2);
    release(conv4_bias_1);
    release(conv4_bias_2);
    release4D(conv4_weights_1);
    release4D(conv4_weights_2);
    printf("conv4 done \n");

    ////////////////relu4////////////////
    relu(conv4_rst_1);
    relu(conv4_rst_2);
    printf("relu4 done\n");

#ifdef DATA_SIZE_MEDIUM
    return 0;
#endif

    ////////////////conv5////////////////
    conv_kernel_size = 3, channels = num_output / 2,
    num_output = 256 / CROP_FACTOR;
    vector4D(conv5_weights, num_output, channels, conv_kernel_size,
             conv_kernel_size);
    vector4D(conv5_weights_1, num_output / 2, channels, conv_kernel_size,
             conv_kernel_size);
    vector4D(conv5_weights_2, num_output / 2, channels, conv_kernel_size,
             conv_kernel_size);
    vector<float> conv5_bias(num_output);
    vector<float> conv5_bias_1(num_output / 2);
    vector<float> conv5_bias_2(num_output / 2);
    readweights("conv5.bin", conv5_weights, conv5_bias);
    copy(conv5_weights.begin(), conv5_weights.begin() + (num_output / 2),
         conv5_weights_1.begin());
    copy(conv5_weights.begin() + (num_output / 2), conv5_weights.end(),
         conv5_weights_2.begin());
    copy(conv5_bias.begin(), conv5_bias.begin() + (num_output / 2),
         conv5_bias_1.begin());
    copy(conv5_bias.begin() + (num_output / 2), conv5_bias.end(),
         conv5_bias_2.begin());
    release(conv5_bias);
    release4D(conv5_weights);
    stride = 1, pad = 1;
    vector3D(conv5_rst, num_output, 13, 13);
    vector3D(conv5_rst_1, num_output / 2, 13, 13);
    vector3D(conv5_rst_2, num_output / 2, 13, 13);
    convolution5(conv4_rst_1, stride, pad, conv5_weights_1, conv5_bias_1,
                 conv5_rst_1, "conv5_1");
    convolution5(conv4_rst_2, stride, pad, conv5_weights_2, conv5_bias_2,
                 conv5_rst_2, "conv5_2");
    copy(conv5_rst_1.begin(), conv5_rst_1.end(), conv5_rst.begin());
    copy(conv5_rst_2.begin(), conv5_rst_2.end(),
         conv5_rst.begin() + num_output / 2);
    release3D(conv4_rst_1);
    release3D(conv4_rst_2);
    release(conv5_bias_1);
    release(conv5_bias_2);
    release4D(conv5_weights_1);
    release4D(conv5_weights_2);
    release3D(conv5_rst_1);
    release3D(conv5_rst_2);
    printf("conv5 done\n");

    ////////////////relu5////////////////
    relu(conv5_rst);
    printf("relu5 done\n");

    ////////////////pool5////////////////
    pooling_kernel_size = 3;
    stride = 2, pad = 0;
    vector3D(pool5_rst, num_output, 6, 6);
    pooling(conv5_rst, pooling_kernel_size, stride, pad, pool5_rst, "MAX");
    release3D(conv5_rst);
    printf("pool5 done\n");

    ////////////////fc6////////////////
    channels = num_output, num_output = 4096 / CROP_FACTOR;
    vector4D(fc6_weights, num_output, channels, 6, 6);
    vector<float> fc6_bias(num_output);
    vector<float> fc6_rst(num_output);
    readweights("fc6.bin", fc6_weights, fc6_bias);
    fullconnection1(pool5_rst, fc6_weights, fc6_bias, fc6_rst);
    release3D(pool5_rst);
    release(fc6_bias);
    // release4D (fc6_weights);
    printf("fc6 done\n");

    ////////////////relu6////////////////
    relu(fc6_rst);
    printf("relu6 done");

    ////////////////fc7////////////////
    channels = num_output, num_output = 4096 / CROP_FACTOR;
    vector<vector<float>> fc7_weights(num_output, vector<float>(channels));
    vector<float> fc7_bias(num_output);
    vector<float> fc7_rst(num_output);
    readweights("fc7.bin", fc7_weights, fc7_bias);
    fullconnection2(fc6_rst, fc7_weights, fc7_bias, fc7_rst);
    release(fc6_rst);
    release(fc7_bias);
    release2D(fc7_weights);
    printf("fc7 done\n");

    ////////////////relu7////////////////
    relu(fc7_rst);
    printf("relu7 done\n");

    ////////////////fc8////////////////
    channels = num_output, num_output = 1000;
    vector<vector<float>> fc8_weights(num_output, vector<float>(channels));
    vector<float> fc8_bias(num_output);
    vector<float> fc8_rst(num_output);
    readweights("fc8.bin", fc8_weights, fc8_bias);
    fullconnection3(fc7_rst, fc8_weights, fc8_bias, fc8_rst);
    release(fc7_rst);
    release(fc8_bias);
    release2D(fc8_weights);
    printf("fc8 done\n");

    ////////////////softmax////////////////
    softmax(fc8_rst);
    // printf("softmax done\n");
    // print_vec("fc8_softmax_rst.txt",
    // fc8_rst);//fc8_rst是1000个概率数字，对应1000组分类

    double polybench_t_end = rtclock();
    printf("%0.6f\n", 0);

    return 0;
}
