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
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
using namespace std;
#define vector4D(name, n, c, h, w)                                             \
  vector<vector<vector<vector<float>>>> name(                                  \
      n, vector<vector<vector<float>>>(                                        \
             c, vector<vector<float>>(h, vector<float>(w))))
#define vector3D(name, c, h, w)                                                \
  vector<vector<vector<float>>> name(                                          \
      c, vector<vector<float>>(h, vector<float>(w)))
#define quote_vector4D(name) vector<vector<vector<vector<float>>>> &name
#define quote_vector3D(name) vector<vector<vector<float>>> &name

void relu(quote_vector3D(bottom));
void relu(vector<float> &bottom);

void dropout(vector<float> &bottom, float ratio);

void dropout(quote_vector3D(bottom), float ratio);

void fullconnection(const quote_vector3D(bottom), const quote_vector4D(weights),
                    const vector<float> &bias, vector<float> &top);

void fullconnection(const vector<float> &bottom,
                    const vector<vector<float>> &weights,
                    const vector<float> &bias, vector<float> &top);

void lrn(quote_vector3D(bottom), float k, float alpha, float beta,
         int local_size);

void softmax(vector<float> &bottom);

void add_pad(quote_vector3D(bottom), int pad);

void pooling(quote_vector3D(bottom), int kernel_size, int stride, int pad,
             quote_vector3D(top), string type);
void convolution(quote_vector3D(bottom), int stride, int pad,
                 const quote_vector4D(weights), const vector<float> &bias,
                 quote_vector3D(top), string layer_name);

void readimage(string ss, quote_vector3D(image));
void readdescriptor(char *s, vector<float> &descriptor);

void readweights(string ss, quote_vector4D(weights), vector<float> &bias);
void readweights(string ss, vector<vector<float>> &weights);
void readweights(string ss, vector<vector<float>> &weights,
                 vector<float> &bias);

void print_vec(char *s, const vector<float> &vec);
void print_vec(char *s, const vector<int> &vec);
void print_vec(char *s, const vector<vector<float>> &vec);

void EmbedLayer(int word_input, const vector<vector<float>> &embed_weights,
                vector<float> &input_sentence);

void PredictLayer(const vector<float> &bottom,
                  const vector<vector<float>> &weights,
                  const vector<float> &bias, vector<float> &top);

void Update(int cont_input, vector<float> &hc_rst);

void LSTMFullconnectionLayer(const vector<float> &bottom,
                             const vector<vector<float>> &weights,
                             vector<float> &top);

void Add_Result(vector<float> &gate_input_t, const vector<float> &xcstatic_rst,
                const vector<float> &Wxc_tm1, const vector<float> &Whc_tm1,
                const vector<float> &xc_bias);

float sigmoid(float x);
// float tanh(float x);
int argmax(const vector<float> &probs);

void LSTMlayer(int cont_input, const vector<float> &gate_input_t,
               vector<float> &c_tm1, vector<float> &hc_tm1);

// FLOPS
long relu_flops(quote_vector3D(bottom));
long relu_flops(vector<float> &bottom);

long dropout_flops(vector<float> &bottom, float ratio);

long dropout_flops(quote_vector3D(bottom), float ratio);

long fullconnection_flops(const quote_vector3D(bottom),
                          const quote_vector4D(weights),
                          const vector<float> &bias, vector<float> &top);

long fullconnection_flops(const vector<float> &bottom,
                          const vector<vector<float>> &weights,
                          const vector<float> &bias, vector<float> &top);

long lrn_flops(quote_vector3D(bottom), float k, float alpha, float beta,
               int local_size);

long softmax_flops(vector<float> &bottom);

long add_pad_flops(quote_vector3D(bottom), int pad);

long pooling_flops(quote_vector3D(bottom), int kernel_size, int stride, int pad,
                   quote_vector3D(top), string type);
long convolution_flops(quote_vector3D(bottom), int stride, int pad,
                       const quote_vector4D(weights), const vector<float> &bias,
                       quote_vector3D(top), string layer_name);

long readimage_flops(string ss, quote_vector3D(image));
long readdescriptor_flops(char *s, vector<float> &descriptor);

long readweights_flops(string ss, quote_vector4D(weights), vector<float> &bias);
long readweights_flops(string ss, vector<vector<float>> &weights);
long readweights_flops(string ss, vector<vector<float>> &weights,
                       vector<float> &bias);

long print_vec_flops(char *s, const vector<float> &vec);
long print_vec_flops(char *s, const vector<int> &vec);
long print_vec_flops(char *s, const vector<vector<float>> &vec);

long EmbedLayer_flops(int word_input,
                      const vector<vector<float>> &embed_weights,
                      vector<float> &input_sentence);

long PredictLayer_flops(const vector<float> &bottom,
                        const vector<vector<float>> &weights,
                        const vector<float> &bias, vector<float> &top);

long Update_flops(int cont_input, vector<float> &hc_rst);

long LSTMFullconnectionLayer_flops(const vector<float> &bottom,
                                   const vector<vector<float>> &weights,
                                   vector<float> &top);

long Add_Result_flops(vector<float> &gate_input_t,
                      const vector<float> &xcstatic_rst,
                      const vector<float> &Wxc_tm1,
                      const vector<float> &Whc_tm1,
                      const vector<float> &xc_bias);

long sigmoid_flops(float x);
// float tanh(float x);
long argmax_flops(const vector<float> &probs);

long LSTMlayer_flops(int cont_input, const vector<float> &gate_input_t,
                     vector<float> &c_tm1, vector<float> &hc_tm1);

#endif
