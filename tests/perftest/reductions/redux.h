/**
 * File					 :
 * /home/markoshorro/workspace/MACVETH/tmp/cs553-hw5-alexnet/layers.h Author
 * : Marcos Horro Date					 : Wed 03 Jun 2020 11:25
 * +02:00
 *
 * Last Modified : Wed 10 Jun 2020 09:38 +02:00
 * Modified By	 : Marcos Horro (marcos.horro@udc.gal>)
 *
 * MIT License
 *
 * Copyright (c) 2020 Colorado State University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _LAYERS_H
#define _LAYERS_H

#include "../utilities/polybench.h"
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

#define vector1D(name, c) vector<float> name(c)
#define vector2D(name, c, h) vector<vector<float>> name(c, vector<float>(h))
#define vector3D(name, c, h, w)                                                \
  vector<vector<vector<float>>> name(                                          \
      c, vector<vector<float>>(h, vector<float>(w)))

#define vector4D(name, n, c, h, w)                                             \
  vector<vector<vector<vector<float>>>> name(                                  \
      n, vector<vector<vector<float>>>(                                        \
             c, vector<vector<float>>(h, vector<float>(w))))

#define quote_vector4D(name) vector<vector<vector<vector<float>>>> &name
#define quote_vector3D(name) vector<vector<vector<float>>> &name
#define quote_vector2D(name) vector<vector<float>> &name
#define quote_vector1D(name) vector<float> &name

#if !defined(REDUX_1D) && !defined(REDUX_2D)
#define REDUX_1D
#endif

/**
 * Macro to encapsulate a function call in the main function to enable
 * its profiling with the PolyBench harness.
 *
 *
 */
#define polybench_profile_one_function(X, name, features, flops, steps)        \
  {                                                                            \
    polybench_prepare_instruments;                                             \
    polybench_start_instruments;                                               \
    for (int t = 0; t < steps; ++t) {                                          \
      X;                                                                       \
    }                                                                          \
    polybench_stop_instruments;                                                \
    printf("%s,%s,%ld,", name, features, flops);                               \
    polybench_print_instruments_flops(flops);                                  \
  }

#define release4D(name) vector<vector<vector<vector<float>>>>(name).swap(name)
#define release3D(name) vector<vector<vector<float>>>(name).swap(name)
#define release2D(name) vector<vector<float>>(name).swap(name)
#define release(name) vector<float>(name).swap(name)

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
#define DATA_PRINTF_MODIFIER "%0.2f "
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

static void print_1darray(std::vector<float> C) {
  int i, j;
  int n = C.size();
  for (i = 0; i < n; i++) {
    fprintf(stderr, DATA_PRINTF_MODIFIER, C[i]);
    if (i % 20 == 0)
      fprintf(stderr, "\n");
  }
  fprintf(stderr, "\n");
}

static void print_2darray(quote_vector2D(C)) {
  int i, j;
  int n = C.size();
  int m = C[0].size();

  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      fprintf(stderr, DATA_PRINTF_MODIFIER, C[i][j]);
      if (i % 20 == 0)
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
  }
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

#if defined(REDUX_1D)
#define red_vec(x) quote_vector1D(x)
#define print_array(x) print_1darray(x)
#else
#define red_vec(x) quote_vector2D(x)
#define print_array(x) print_2darray(x)
#endif

#ifndef DATA_TYPE
#define DATA_TYPE float
#else
#undef DATA_TYPE
#define DATA_TYPE float
#endif

long redux_flops(red_vec(v));
void redux(red_vec(v));

long redux2_flops(red_vec(v));
void redux2(red_vec(v0), red_vec(v1));

long redux4_flops(red_vec(v));
void redux4(red_vec(v0), red_vec(v1), red_vec(v2), red_vec(v3));

long redux8_flops(red_vec(v));
void redux8(red_vec(v0), red_vec(v1), red_vec(v2), red_vec(v3), red_vec(v4),
            red_vec(v5), red_vec(v6), red_vec(v7));

long redux_fma_flops(red_vec(v));
void redux_fma(red_vec(v));

long redux_mul_flops(red_vec(v));
void redux_mul(red_vec(v));

long redux_svml_flops(red_vec(v));
void redux_svml(red_vec(v));

long redux_2d_flops(red_vec(v));
void redux_2d(red_vec(v));

#endif
