/**
 * Code re-written by Wei Zuo, UIUC, with Louis-Noel Pouchet.
 * Copyright (c) 2015-2018
 *
 */

#include "layers.h"
#include "polybench.h"
#ifdef POLYBENCH_PAPI
#include <papi.h>
#endif

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

#define _PB_TSTEPS POLYBENCH_LOOP_BOUND(TSTEPS, tsteps)
#define _PB_N POLYBENCH_LOOP_BOUND(N, n)

/* Default data type */
#if !defined(DATA_TYPE_IS_INT) && !defined(DATA_TYPE_IS_FLOAT) &&              \
    !defined(DATA_TYPE_IS_DOUBLE)
#define DATA_TYPE_IS_FLOAT
#endif

#ifdef DATA_TYPE_IS_INT
#define DATA_TYPE int
#define DATA_PRINTF_MODIFIER "%d "
#endif

#ifdef DATA_TYPE_IS_FLOAT
#define DATA_TYPE float
#define DATA_PRINTF_MODIFIER "%0.8f "
#define SCALAR_VAL(x) x##f
#define SQRT_FUN(x) sqrtf(x)
#define EXP_FUN(x) expf(x)
#define POW_FUN(x, y) powf(x, y)
#endif

#ifdef DATA_TYPE_IS_DOUBLE
#define DATA_TYPE double
#define DATA_PRINTF_MODIFIER "%0.2lf "
#define SCALAR_VAL(x) x
#define SQRT_FUN(x) sqrt(x)
#define EXP_FUN(x) exp(x)
#define POW_FUN(x, y) pow(x, y)
#endif

#ifdef POLYBENCH_PAPI
struct T {
  char *name;
  char *feat;
  long flops;
  long_long counters[128];
} values_t;

struct T values[128];

long long n_func = 0;

void update_row(char *name, char *features, long flops, int t, long_long val) {
  if (t == 0) {
    struct T tmp;
    tmp.name = name;
    tmp.feat = features;
    tmp.flops = flops;
    tmp.counters[t] = val;
    values[n_func] = tmp;
    return;
  }
  values[n_func].counters[t] = val;
}
#endif
/**
 * Macro to encapsulate a function call in the main function to enable
 * its profiling with the PolyBench harness.
 *
 *
 */
#ifndef POLYBENCH_PAPI
#define polybench_profile_one_function(X, name, features, flops, i)            \
  {                                                                            \
    polybench_prepare_instruments;                                             \
    polybench_start_instruments;                                               \
    X;                                                                         \
    polybench_stop_instruments;                                                \
    printf("%s,%s,%ld,", name, features, flops);                               \
    polybench_print_instruments_flops(flops);                                  \
  }
#else
#define polybench_profile_one_function(X, name, features, flops, t)            \
  {                                                                            \
    polybench_start_instruments_single(t);                                     \
    X;                                                                         \
    polybench_stop_instruments_single(t);                                      \
    update_row(name, features, flops, t, polybench_get_value(t));              \
    n_func++;                                                                  \
  }
#endif

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

static void print_1darray(int n, std::vector<float> C) {
  int i, j;

  for (i = 0; i < n; i++) {
    fprintf(stderr, DATA_PRINTF_MODIFIER, C[i]);
    if (i % 20 == 0)
      fprintf(stderr, "\n");
  }
  fprintf(stderr, "\n");
}

static void print_3darray(int NI, int NJ, int NK, quote_vector3D(C)) {
  int i, j, k;

  for (i = 0; i < NI; i++)
    for (j = 0; j < NJ; j++)
      for (k = 0; k < NK; k++) {
        fprintf(stderr, DATA_PRINTF_MODIFIER, C[i][j][k]);
        if (i % 20 == 0)
          fprintf(stderr, "\n");
      }
  fprintf(stderr, "\n");
}

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static void print_4darray(int NI, int NJ, int NK, int NL, quote_vector4D(C)) {
  int i, j, k, l;

  for (i = 0; i < NI; i++)
    for (j = 0; j < NJ; j++)
      for (k = 0; k < NK; k++)
        for (l = 0; l < NL; l++) {
          fprintf(stderr, DATA_PRINTF_MODIFIER, C[i][j][k][l]);
          if (i % 20 == 0)
            fprintf(stderr, "\n");
        }
  fprintf(stderr, "\n");
}
#ifdef POLYBENCH_PAPI
char *__polybench_papi_eventlist[] = {
#include "papi_counters.list"
    NULL};
#endif

/**
 * Main AlexNet inference code.
 *
 */
int main(int argc, char **argv) {
  int i = 0;
#ifndef POLYBENCH_PAPI
  printf("name,features,flops,time,gflops\n");
#else
  printf("name,features,flops,");
  while (__polybench_papi_eventlist[i] != NULL) {
    if (__polybench_papi_eventlist[i + 1] == NULL) {
      printf("%s", __polybench_papi_eventlist[i++]);
    } else {
      printf("%s,", __polybench_papi_eventlist[i++]);
    }
  }
  printf("\n");
  i = 0;
  polybench_papi_init();
  while (__polybench_papi_eventlist[i] != NULL) {
    n_func = 0;
    polybench_prepare_instruments();
#endif
  // read image
  vector3D(image, 3, 227, 227);
  long flops = readimage_flops("image.bin", image);
  readimage("image.bin", image);
  // printf("readimage done\n");

  /////////////conv1//////////////
  // prepare weight
  int conv_kernel_size = 11, channels = 3, num_output = 96;
  vector4D(conv1_weights, num_output, channels, conv_kernel_size,
           conv_kernel_size);
  vector<float> conv1_bias(num_output);
  flops = readweights_flops("conv1.bin", conv1_weights, conv1_bias);
  readweights("conv1.bin", conv1_weights, conv1_bias);

  int stride = 4, pad = 0;
  vector3D(conv1_rst, num_output, 55, 55);
  // compute
  flops = convolution_flops(image, stride, pad, conv1_weights, conv1_bias,
                            conv1_rst, "conv1");
  polybench_profile_one_function(convolution(image, stride, pad, conv1_weights,
                                             conv1_bias, conv1_rst, "conv1"),
                                 "convolution", "conv1", flops, i);
  // free space
  // printf("conv1 done\n");

  /////////////relu1///////////////
  flops = relu_flops(conv1_rst);
  polybench_profile_one_function(relu(conv1_rst), "relu", "conv1_rst", flops,
                                 i);
  // polybench_prevent_dce(print_3darray(conv1_rst.size(), conv1_rst[0].size(),
  //                                     conv1_rst[0][0].size(), conv1_rst));

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
      "pooling", "conv1_rst_max", flops, i);

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
                                 "convolution", "conv2_1", flops, i);
  flops = convolution_flops(pool1_rst_2, stride, pad, conv2_weights_2,
                            conv2_bias_2, conv2_rst_2, "conv2_2");
  polybench_profile_one_function(convolution(pool1_rst_2, stride, pad,
                                             conv2_weights_2, conv2_bias_2,
                                             conv2_rst_2, "conv2_2"),
                                 "convolution", "conv2_2", flops, i);
  // printf("conv2 done\n");

  ///////////////relu2////////////////
  flops = relu_flops(conv2_rst);
  polybench_profile_one_function(relu(conv2_rst), "relu", "conv2_rst", flops,
                                 i);
  // polybench_prevent_dce(print_3darray(conv2_rst.size(), conv2_rst[0].size(),
  //                                    conv2_rst[0][0].size(), conv2_rst));
  // printf("relu2 done\n");

  ////////////////lrn2/////////////////
  float alpha = 1.0;
  float beta = 1.0;
  float k = 1.0;
  int local_size = 1;
  flops = lrn_flops(conv2_rst, k, alpha, beta, local_size);
  polybench_profile_one_function(lrn(conv2_rst, k, alpha, beta, local_size),
                                 "lrn", "conv2_rst", flops, i);
  // printf("lrn2 done\n");

  ////////////////pool2////////////////
  pooling_kernel_size = 3;
  stride = 2, pad = 0;
  vector3D(pool2_rst, num_output, 13, 13);
  flops = pooling_flops(conv2_rst, pooling_kernel_size, stride, pad, pool2_rst,
                        "MAX");
  polybench_profile_one_function(
      pooling(conv2_rst, pooling_kernel_size, stride, pad, pool2_rst, "MAX"),
      "pooling", "conv2_rst_max", flops, i);
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
                                 "convolution", "conv3", flops, i);
  // printf("conv3 done\n");

  ////////////////relu3////////////////

  flops = relu_flops(conv3_rst);
  polybench_profile_one_function(relu(conv3_rst), "relu", "conv3_rst", flops,
                                 i);
  // polybench_prevent_dce(print_3darray(conv3_rst.size(), conv3_rst[0].size(),
  //                                     conv3_rst[0][0].size(), conv3_rst));
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
                                 "convolution", "conv4_1", flops, i);
  flops = convolution_flops(conv3_rst_2, stride, pad, conv4_weights_2,
                            conv4_bias_2, conv4_rst_2, "conv4_2");
  polybench_profile_one_function(convolution(conv3_rst_2, stride, pad,
                                             conv4_weights_2, conv4_bias_2,
                                             conv4_rst_2, "conv4_2"),
                                 "convolution", "conv4_2", flops, i);
  // printf("conv4 done \n");

  ////////////////relu4////////////////
  flops = relu_flops(conv4_rst_1);
  polybench_profile_one_function(relu(conv4_rst_1), "relu", "conv4_rst_1",
                                 flops, i);
  // polybench_prevent_dce(print_3darray(conv4_rst_1.size(),
  // conv4_rst_1[0].size(),
  //                                     conv4_rst_1[0][0].size(),
  //                                     conv4_rst_1));
  flops = relu_flops(conv4_rst_2);
  polybench_profile_one_function(relu(conv4_rst_2), "relu", "conv4_rst_2",
                                 flops, i);
  // polybench_prevent_dce(print_3darray(conv4_rst_2.size(),
  // conv4_rst_2[0].size(),
  //                                     conv4_rst_2[0][0].size(),
  //                                     conv4_rst_2));
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
                                 "convolution", "conv5_1", flops, i);
  flops = convolution_flops(conv4_rst_2, stride, pad, conv5_weights_2,
                            conv5_bias_2, conv5_rst_2, "conv5_2");
  polybench_profile_one_function(convolution(conv4_rst_2, stride, pad,
                                             conv5_weights_2, conv5_bias_2,
                                             conv5_rst_2, "conv5_2"),
                                 "convolution", "conv5_2", flops, i);
  // printf("conv5 done\n");

  ////////////////relu5////////////////
  flops = relu_flops(conv5_rst);
  polybench_profile_one_function(relu(conv5_rst), "relu", "conv5_rst", flops,
                                 i); // printf("relu5 done\n");

  ////////////////pool5////////////////
  pooling_kernel_size = 3;
  stride = 2, pad = 0;
  vector3D(pool5_rst, num_output, 6, 6);
  flops = pooling_flops(conv5_rst, pooling_kernel_size, stride, pad, pool5_rst,
                        "MAX");

  polybench_profile_one_function(
      pooling(conv5_rst, pooling_kernel_size, stride, pad, pool5_rst, "MAX"),
      "pooling", "conv5_rst_max", flops, i);
  // printf("pool5 done\n");

  ////////////////fc6//////////////// 3dimensional
  channels = num_output, num_output = 4096;
  vector4D(fc6_weights, num_output, channels, 6, 6);
  vector<float> fc6_bias(num_output);
  vector<float> fc6_rst(num_output);
  readweights("fc6.bin", fc6_weights, fc6_bias);
  flops = fullconnection_flops(pool5_rst, fc6_weights, fc6_bias, fc6_rst);
  polybench_profile_one_function(
      fullconnection(pool5_rst, fc6_weights, fc6_bias, fc6_rst),
      "fullconnection", "pool5_rst", flops, i);
  // printf("fc6 done\n");

  ////////////////fc7//////////////// 2dimensional + bias
  channels = num_output, num_output = 4096;
  vector<vector<float>> fc7_weights(num_output, vector<float>(channels));
  vector<float> fc7_bias(num_output);
  vector<float> fc7_rst(num_output);
  readweights("fc7.bin", fc7_weights, fc7_bias);
  flops = fullconnection_flops(fc6_rst, fc7_weights, fc7_bias, fc7_rst);
  polybench_profile_one_function(
      fullconnection(fc6_rst, fc7_weights, fc7_bias, fc7_rst), "fullconnection",
      "fc6_rst", flops, i);
  // printf("fc7 done\n");

  ////////////////relu7////////////////
  flops = relu_flops(fc7_rst);
  polybench_profile_one_function(relu(fc7_rst), "relu", "fc7_rst", flops, i);
  // printf("relu7 done\n");

  ////////////////fc8//////////////// 2dimensional
  channels = num_output, num_output = 1000;
  vector<vector<float>> fc8_weights(num_output, vector<float>(channels));
  vector<float> fc8_bias(num_output);
  vector<float> fc8_rst(num_output);
  readweights("fc8.bin", fc8_weights, fc8_bias);
  flops = fullconnection_flops(fc7_rst, fc8_weights, fc8_bias, fc8_rst);
  polybench_profile_one_function(
      fullconnection(fc7_rst, fc8_weights, fc8_bias, fc8_rst), "fullconnection",
      "fc7_rst", flops, i);
  // printf("fc8 done\n");

  ////////////////softmax////////////////
  flops = softmax_flops(fc8_rst);
  polybench_profile_one_function(softmax(fc8_rst), "softmax", "fc8_rst", flops,
                                 i);
  polybench_prevent_dce(print_1darray(fc8_rst.size(), fc8_rst));
  // printf("softmax done\n");z
  // print_vec("fc8_softmax_rst.txt", fc8_rst);

#ifdef POLYBENCH_PAPI
  i++;
}
int n_count = i;
for (int n = 0; n < n_func; ++n) {
  struct T val = values[n];
  printf("%s,%s,%ld,", val.name, val.feat, val.flops);
  for (int c = 0; c < n_count; ++c) {
    printf("%llu", val.counters[c]);
    if (c + 1 < n_count) {
      printf(",");
    }
  }
  printf("\n");
}
polybench_papi_close();
#endif

return 0;
}
