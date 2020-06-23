/**
 * Code re-written by Wei Zuo, UIUC, with Louis-Noel Pouchet.
 * Copyright (c) 2015-2018
 *
 */

#include "layers.h"
#include "polybench.h"

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

/**
 * Macro to encapsulate a function call in the main function to enable
 * its profiling with the PolyBench harness.
 *
 *
 */
#define polybench_profile_one_function(X, name, features, flops)               \
  {                                                                            \
    polybench_prepare_instruments;                                             \
    polybench_start_instruments;                                               \
    X;                                                                         \
    polybench_stop_instruments;                                                \
    printf("%s,%s,%ld,", name, features, flops);                               \
    polybench_print_instruments_flops(flops);                                  \
  }

// #define polybench_profile_one_function(X, name, features, flops)               \
//   {                                                                            \
//     polybench_prepare_instruments;                                             \
//     polybench_start_instruments;                                               \
//     for (int t = 0; t < TSTEPS; ++t) {                                         \
//       X;                                                                       \
//     };                                                                         \
//     polybench_stop_instruments;                                                \
//     printf("%s,%s,%d,", name, features, flops);                                \
//     polybench_print_instruments;                                               \
//   }

/**
 * Main AlexNet inference code.
 *
 */
int main() {
  // read image
  vector3D(image, 3, 227, 227);
  long flops = readimage_flops("image.bin", image);
  polybench_profile_one_function(readimage("image.bin", image), "readimage",
                                 "image.bin", flops);
  // printf("readimage done\n");

  /////////////conv1//////////////
  // prepare weight
  int conv_kernel_size = 11, channels = 3, num_output = 96;
  vector4D(conv1_weights, num_output, channels, conv_kernel_size,
           conv_kernel_size);
  vector<float> conv1_bias(num_output);
  flops = readweights_flops("conv1.bin", conv1_weights, conv1_bias);
  polybench_profile_one_function(
      readweights("conv1.bin", conv1_weights, conv1_bias), "readweights",
      "conv1.bin", flops);
  int stride = 4, pad = 0;
  vector3D(conv1_rst, num_output, 55, 55);
  // compute
  flops = convolution_flops(image, stride, pad, conv1_weights, conv1_bias,
                            conv1_rst, "conv1");
  polybench_profile_one_function(convolution(image, stride, pad, conv1_weights,
                                             conv1_bias, conv1_rst, "conv1"),
                                 "convolution", "conv1", flops);
  // free space
  // printf("conv1 done\n");

  /////////////relu1///////////////
  flops = relu_flops(conv1_rst);
  polybench_profile_one_function(relu(conv1_rst), "relu", "conv1_rst", flops);
  // printf("relu1 done\n");

  ////////////pool1////////////////
  int pooling_kernel_size = 3;
  stride = 2, pad = 0;
  vector3D(pool1_rst, 96, 27, 27);
  /// EXAMPLE: default run (no profiling)
  // pooling(conv1_rst, pooling_kernel_size, stride, pad, pool1_rst,"MAX");
  /// EXAMPLE: with profiling (comment out the above, uncomment this one):
  flops = pooling_flops(conv1_rst, pooling_kernel_size, stride, pad, pool1_rst,
                        "MAX");
  polybench_profile_one_function(
      pooling(conv1_rst, pooling_kernel_size, stride, pad, pool1_rst, "MAX"),
      "pooling", "conv1_rst_max", flops);

  // printf("pool1 done\n");

  ////////////conv2/////////////
  vector3D(pool1_rst_1, num_output / 2, 27, 27);
  vector3D(pool1_rst_2, num_output / 2, 27, 27);
  conv_kernel_size = 5, channels = num_output / 2, num_output = 256;
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
  stride = 1, pad = 2;
  vector3D(conv2_rst, num_output, 27, 27);
  vector3D(conv2_rst_1, num_output / 2, 27, 27);
  vector3D(conv2_rst_2, num_output / 2, 27, 27);
  flops = convolution_flops(pool1_rst_1, stride, pad, conv2_weights_1,
                            conv2_bias_1, conv2_rst_1, "conv2_1");
  polybench_profile_one_function(convolution(pool1_rst_1, stride, pad,
                                             conv2_weights_1, conv2_bias_1,
                                             conv2_rst_1, "conv2_1"),
                                 "convolution", "conv2_1", flops);
  flops = convolution_flops(pool1_rst_2, stride, pad, conv2_weights_2,
                            conv2_bias_2, conv2_rst_2, "conv2_2");
  polybench_profile_one_function(convolution(pool1_rst_2, stride, pad,
                                             conv2_weights_2, conv2_bias_2,
                                             conv2_rst_2, "conv2_2"),
                                 "convolution", "conv2_2", flops);
  // printf("conv2 done\n");

  ///////////////relu2////////////////
  flops = relu_flops(conv2_rst);
  polybench_profile_one_function(relu(conv2_rst), "relu", "conv2_rst", flops);
  // printf("relu2 done\n");

  ////////////////lrn2/////////////////
  float alpha = 1.0;
  float beta = 1.0;
  float k = 1.0;
  int local_size = 1;
  flops = lrn_flops(conv2_rst, k, alpha, beta, local_size);
  polybench_profile_one_function(lrn(conv2_rst, k, alpha, beta, local_size),
                                 "lrn", "conv2_rst", flops);
  // printf("lrn2 done\n");

  ////////////////pool2////////////////
  pooling_kernel_size = 3;
  stride = 2, pad = 0;
  vector3D(pool2_rst, num_output, 13, 13);
  flops = pooling_flops(conv2_rst, pooling_kernel_size, stride, pad, pool2_rst,
                        "MAX");
  polybench_profile_one_function(
      pooling(conv2_rst, pooling_kernel_size, stride, pad, pool2_rst, "MAX"),
      "pooling", "conv2_rst_max", flops);
  // printf("pool2 done\n");

  ////////////////conv3////////////////
  conv_kernel_size = 3, channels = num_output, num_output = 384;
  vector4D(conv3_weights, num_output, channels, conv_kernel_size,
           conv_kernel_size);
  vector<float> conv3_bias(num_output);
  readweights("conv3.bin", conv3_weights, conv3_bias);
  stride = 1, pad = 1;
  vector3D(conv3_rst, num_output, 13, 13);
  flops = convolution_flops(pool2_rst, stride, pad, conv3_weights, conv3_bias,
                            conv3_rst, "conv3");
  polybench_profile_one_function(convolution(pool2_rst, stride, pad,
                                             conv3_weights, conv3_bias,
                                             conv3_rst, "conv3"),
                                 "convolution", "conv3", flops);
  // printf("conv3 done\n");

  ////////////////relu3////////////////

  flops = relu_flops(conv3_rst);
  polybench_profile_one_function(relu(conv3_rst), "relu", "conv3_rst", flops);
  // printf("relu3 done\n");

  ////////////////conv4////////////////
  vector3D(conv3_rst_1, num_output / 2, 13, 13);
  vector3D(conv3_rst_2, num_output / 2, 13, 13);
  conv_kernel_size = 3, channels = num_output / 2, num_output = 384;
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
  vector3D(conv4_rst_1, num_output / 2, 13, 13);
  vector3D(conv4_rst_2, num_output / 2, 13, 13);
  flops = convolution_flops(conv3_rst_1, stride, pad, conv4_weights_1,
                            conv4_bias_1, conv4_rst_1, "conv4_1");
  polybench_profile_one_function(convolution(conv3_rst_1, stride, pad,
                                             conv4_weights_1, conv4_bias_1,
                                             conv4_rst_1, "conv4_1"),
                                 "convolution", "conv4_1", flops);
  flops = convolution_flops(conv3_rst_2, stride, pad, conv4_weights_2,
                            conv4_bias_2, conv4_rst_2, "conv4_2");
  polybench_profile_one_function(convolution(conv3_rst_2, stride, pad,
                                             conv4_weights_2, conv4_bias_2,
                                             conv4_rst_2, "conv4_2"),
                                 "convolution", "conv4_2", flops);
  // printf("conv4 done \n");

  ////////////////relu4////////////////
  flops = relu_flops(conv4_rst_1);
  polybench_profile_one_function(relu(conv4_rst_1), "relu", "conv4_rst_1",
                                 flops);
  flops = relu_flops(conv4_rst_2);
  polybench_profile_one_function(relu(conv4_rst_2), "relu", "conv4_rst_2",
                                 flops);
  // printf("relu4 done\n");

  ////////////////conv5////////////////
  conv_kernel_size = 3, channels = num_output / 2, num_output = 256;
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
  stride = 1, pad = 1;
  vector3D(conv5_rst, num_output, 13, 13);
  vector3D(conv5_rst_1, num_output / 2, 13, 13);
  vector3D(conv5_rst_2, num_output / 2, 13, 13);
  flops = convolution_flops(conv4_rst_1, stride, pad, conv5_weights_1,
                            conv5_bias_1, conv5_rst_1, "conv5_1");
  polybench_profile_one_function(convolution(conv4_rst_1, stride, pad,
                                             conv5_weights_1, conv5_bias_1,
                                             conv5_rst_1, "conv5_1"),
                                 "convolution", "conv5_1", flops);
  flops = convolution_flops(conv4_rst_2, stride, pad, conv5_weights_2,
                            conv5_bias_2, conv5_rst_2, "conv5_2");
  polybench_profile_one_function(convolution(conv4_rst_2, stride, pad,
                                             conv5_weights_2, conv5_bias_2,
                                             conv5_rst_2, "conv5_2"),
                                 "convolution", "conv5_2", flops);
  // printf("conv5 done\n");

  ////////////////relu5////////////////
  flops = relu_flops(conv5_rst);
  polybench_profile_one_function(relu(conv5_rst), "relu", "conv5_rst",
                                 flops); // printf("relu5 done\n");

  ////////////////pool5////////////////
  pooling_kernel_size = 3;
  stride = 2, pad = 0;
  vector3D(pool5_rst, num_output, 6, 6);
  flops = pooling_flops(conv5_rst, pooling_kernel_size, stride, pad, pool5_rst,
                        "MAX");

  polybench_profile_one_function(
      pooling(conv5_rst, pooling_kernel_size, stride, pad, pool5_rst, "MAX"),
      "pooling", "conv5_rst_max", flops);
  // printf("pool5 done\n");

  ////////////////fc6////////////////
  channels = num_output, num_output = 4096;
  vector4D(fc6_weights, num_output, channels, 6, 6);
  vector<float> fc6_bias(num_output);
  vector<float> fc6_rst(num_output);
  readweights("fc6.bin", fc6_weights, fc6_bias);
  flops = fullconnection_flops(pool5_rst, fc6_weights, fc6_bias, fc6_rst);
  polybench_profile_one_function(
      fullconnection(pool5_rst, fc6_weights, fc6_bias, fc6_rst),
      "fullconnection", "pool5_rst", flops);
  // printf("fc6 done\n");

  ////////////////relu6////////////////
  flops = relu_flops(fc6_rst);
  polybench_profile_one_function(relu(fc6_rst), "relu", "fc6_rst", flops);
  // printf("relu6 done");

  ////////////////fc7////////////////
  channels = num_output, num_output = 4096;
  vector<vector<float>> fc7_weights(num_output, vector<float>(channels));
  vector<float> fc7_bias(num_output);
  vector<float> fc7_rst(num_output);
  readweights("fc7.bin", fc7_weights, fc7_bias);
  flops = fullconnection_flops(fc6_rst, fc7_weights, fc7_bias, fc7_rst);
  polybench_profile_one_function(
      fullconnection(fc6_rst, fc7_weights, fc7_bias, fc7_rst), "fullconnection",
      "fc6_rst", flops);
  // printf("fc7 done\n");

  ////////////////relu7////////////////
  flops = relu_flops(fc7_rst);
  polybench_profile_one_function(relu(fc7_rst), "relu", "fc7_rst", flops);
  // printf("relu7 done\n");

  ////////////////fc8////////////////
  channels = num_output, num_output = 1000;
  vector<vector<float>> fc8_weights(num_output, vector<float>(channels));
  vector<float> fc8_bias(num_output);
  vector<float> fc8_rst(num_output);
  readweights("fc8.bin", fc8_weights, fc8_bias);
  flops = fullconnection_flops(fc7_rst, fc8_weights, fc8_bias, fc8_rst);
  polybench_profile_one_function(
      fullconnection(fc7_rst, fc8_weights, fc8_bias, fc8_rst), "fullconnection",
      "fc7_rst", flops);
  // printf("fc8 done\n");

  ////////////////softmax////////////////
  flops = softmax_flops(fc8_rst);
  polybench_profile_one_function(softmax(fc8_rst), "softmax", "fc8_rst", flops);
  // printf("softmax done\n");
  // print_vec("fc8_softmax_rst.txt", fc8_rst);
  // fc8_rst是1000个概率数字，对应1000组分类
  return 0;
}
