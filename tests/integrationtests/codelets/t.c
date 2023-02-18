/**
 * Copyright 2021 Marcos Horro
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
void kernel_spmv_fragment_0(float *restrict A, float *restrict x,
                            float *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;
#pragma macveth unroll i0 full

  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 0] += A[1 * i0 + 0] * x[1 * i0 + 256];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 8] += A[1 * i0 + 8] * x[1 * i0 + 264];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 16] += A[1 * i0 + 16] * x[1 * i0 + 272];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 24] += A[1 * i0 + 24] * x[1 * i0 + 280];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 32] += A[1 * i0 + 32] * x[1 * i0 + 288];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 40] += A[1 * i0 + 40] * x[1 * i0 + 296];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 48] += A[1 * i0 + 48] * x[1 * i0 + 304];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 56] += A[1 * i0 + 56] * x[1 * i0 + 312];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 64] += A[1 * i0 + 64] * x[1 * i0 + 320];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 72] += A[1 * i0 + 72] * x[1 * i0 + 328];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 80] += A[1 * i0 + 80] * x[1 * i0 + 336];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 88] += A[1 * i0 + 88] * x[1 * i0 + 360];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 96] += A[1 * i0 + 96] * x[1 * i0 + 368];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 104] += A[1 * i0 + 104] * x[1 * i0 + 376];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 112] += A[1 * i0 + 112] * x[1 * i0 + 384];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 120] += A[1 * i0 + 120] * x[1 * i0 + 392];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 128] += A[1 * i0 + 128] * x[0 * i0 + 16];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 129] += A[1 * i0 + 130] * x[0 * i0 + 17];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 130] += A[1 * i0 + 132] * x[0 * i0 + 18];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 131] += A[1 * i0 + 134] * x[0 * i0 + 19];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 132] += A[1 * i0 + 136] * x[0 * i0 + 20];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 133] += A[1 * i0 + 138] * x[0 * i0 + 21];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 134] += A[1 * i0 + 140] * x[0 * i0 + 22];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 135] += A[1 * i0 + 142] * x[0 * i0 + 23];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 136] += A[1 * i0 + 144] * x[0 * i0 + 24];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 137] += A[1 * i0 + 146] * x[0 * i0 + 25];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 138] += A[1 * i0 + 148] * x[0 * i0 + 26];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 139] += A[1 * i0 + 150] * x[0 * i0 + 27];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 140] += A[1 * i0 + 152] * x[0 * i0 + 28];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 141] += A[1 * i0 + 154] * x[0 * i0 + 29];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 142] += A[1 * i0 + 156] * x[0 * i0 + 30];
  }
  y[143] += A[158] * x[31];
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 144] += A[1 * i0 + 159] * x[1 * i0 + 256];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 152] += A[1 * i0 + 167] * x[1 * i0 + 264];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 160] += A[1 * i0 + 175] * x[1 * i0 + 272];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 168] += A[1 * i0 + 183] * x[1 * i0 + 280];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 176] += A[1 * i0 + 191] * x[0 * i0 + 16];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 177] += A[1 * i0 + 193] * x[0 * i0 + 17];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 178] += A[1 * i0 + 195] * x[0 * i0 + 18];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 179] += A[1 * i0 + 197] * x[0 * i0 + 19];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 180] += A[1 * i0 + 199] * x[0 * i0 + 20];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 181] += A[1 * i0 + 201] * x[0 * i0 + 21];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 182] += A[1 * i0 + 203] * x[0 * i0 + 22];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 183] += A[1 * i0 + 205] * x[0 * i0 + 23];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 184] += A[1 * i0 + 207] * x[0 * i0 + 24];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 185] += A[1 * i0 + 209] * x[0 * i0 + 25];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 186] += A[1 * i0 + 211] * x[0 * i0 + 26];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 187] += A[1 * i0 + 213] * x[0 * i0 + 27];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 188] += A[1 * i0 + 215] * x[0 * i0 + 28];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 189] += A[1 * i0 + 217] * x[0 * i0 + 29];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 190] += A[1 * i0 + 219] * x[0 * i0 + 30];
  }
  y[191] += A[221] * x[31];
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 192] += A[1 * i0 + 222] * x[1 * i0 + 0];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 200] += A[1 * i0 + 230] * x[1 * i0 + 8];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 208] += A[4 * i0 + 238] * x[1 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 208] += A[1 * i0 + 239] * x[1 * i0 + 32];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 208] += A[4 * i0 + 241] * x[1 * i0 + 256];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 209] += A[1 * i0 + 243] * x[1 * i0 + 33];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 210] += A[1 * i0 + 247] * x[1 * i0 + 34];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 211] += A[1 * i0 + 251] * x[1 * i0 + 35];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 212] += A[1 * i0 + 255] * x[1 * i0 + 36];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 213] += A[1 * i0 + 259] * x[1 * i0 + 37];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 214] += A[1 * i0 + 263] * x[1 * i0 + 38];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 215] += A[1 * i0 + 267] * x[1 * i0 + 39];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 216] += A[4 * i0 + 270] * x[1 * i0 + 8];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 216] += A[1 * i0 + 271] * x[1 * i0 + 40];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 216] += A[4 * i0 + 273] * x[1 * i0 + 264];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 217] += A[1 * i0 + 275] * x[1 * i0 + 41];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 218] += A[1 * i0 + 279] * x[1 * i0 + 42];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 219] += A[1 * i0 + 283] * x[1 * i0 + 43];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 220] += A[1 * i0 + 287] * x[1 * i0 + 44];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 221] += A[1 * i0 + 291] * x[1 * i0 + 45];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 222] += A[1 * i0 + 295] * x[1 * i0 + 46];
  }
  y[223] += A[299] * x[47];
  y[223] += A[300] * x[271];
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 224] += A[5 * i0 + 301] * x[1 * i0 + 0];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 224] += A[5 * i0 + 302] * x[1 * i0 + 256];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 224] += A[5 * i0 + 303] * x[1 * i0 + 288];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 224] += A[5 * i0 + 304] * x[1 * i0 + 304];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 224] += A[5 * i0 + 305] * x[1 * i0 + 360];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 232] += A[5 * i0 + 341] * x[1 * i0 + 8];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 232] += A[5 * i0 + 342] * x[1 * i0 + 264];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 232] += A[5 * i0 + 343] * x[1 * i0 + 296];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 232] += A[5 * i0 + 344] * x[1 * i0 + 312];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 232] += A[5 * i0 + 345] * x[1 * i0 + 368];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 240] += A[6 * i0 + 381] * x[1 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 240] += A[1 * i0 + 382] * x[1 * i0 + 48];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 240] += A[6 * i0 + 384] * x[1 * i0 + 256];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 240] += A[6 * i0 + 385] * x[1 * i0 + 304];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 240] += A[6 * i0 + 386] * x[1 * i0 + 360];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 241] += A[1 * i0 + 388] * x[1 * i0 + 49];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 242] += A[1 * i0 + 394] * x[1 * i0 + 50];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 243] += A[1 * i0 + 400] * x[1 * i0 + 51];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 244] += A[1 * i0 + 406] * x[1 * i0 + 52];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 245] += A[1 * i0 + 412] * x[1 * i0 + 53];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 246] += A[1 * i0 + 418] * x[1 * i0 + 54];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 247] += A[1 * i0 + 424] * x[1 * i0 + 55];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 248] += A[6 * i0 + 429] * x[1 * i0 + 8];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 248] += A[1 * i0 + 430] * x[1 * i0 + 56];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 248] += A[6 * i0 + 432] * x[1 * i0 + 264];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 248] += A[6 * i0 + 433] * x[1 * i0 + 312];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 248] += A[6 * i0 + 434] * x[1 * i0 + 368];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 249] += A[1 * i0 + 436] * x[1 * i0 + 57];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 250] += A[1 * i0 + 442] * x[1 * i0 + 58];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 251] += A[1 * i0 + 448] * x[1 * i0 + 59];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 252] += A[1 * i0 + 454] * x[1 * i0 + 60];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 253] += A[1 * i0 + 460] * x[1 * i0 + 61];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 254] += A[1 * i0 + 466] * x[1 * i0 + 62];
  }
  y[255] += A[472] * x[63];
  y[255] += A[473] * x[271];
  y[255] += A[474] * x[319];
  y[255] += A[475] * x[375];
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 256] += A[6 * i0 + 476] * x[1 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 256] += A[1 * i0 + 477] * x[1 * i0 + 64];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 256] += A[6 * i0 + 479] * x[1 * i0 + 256];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 256] += A[6 * i0 + 480] * x[1 * i0 + 304];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 256] += A[6 * i0 + 481] * x[1 * i0 + 360];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 257] += A[1 * i0 + 483] * x[1 * i0 + 65];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 258] += A[1 * i0 + 489] * x[1 * i0 + 66];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 259] += A[1 * i0 + 495] * x[1 * i0 + 67];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 260] += A[1 * i0 + 501] * x[1 * i0 + 68];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 261] += A[1 * i0 + 507] * x[1 * i0 + 69];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 262] += A[1 * i0 + 513] * x[1 * i0 + 70];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 263] += A[1 * i0 + 519] * x[1 * i0 + 71];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 264] += A[6 * i0 + 524] * x[1 * i0 + 8];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 264] += A[1 * i0 + 525] * x[1 * i0 + 72];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 264] += A[6 * i0 + 527] * x[1 * i0 + 264];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 264] += A[6 * i0 + 528] * x[1 * i0 + 312];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 264] += A[6 * i0 + 529] * x[1 * i0 + 368];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 265] += A[1 * i0 + 531] * x[1 * i0 + 73];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 266] += A[1 * i0 + 537] * x[1 * i0 + 74];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 267] += A[1 * i0 + 543] * x[1 * i0 + 75];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 268] += A[1 * i0 + 549] * x[1 * i0 + 76];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 269] += A[1 * i0 + 555] * x[1 * i0 + 77];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 270] += A[1 * i0 + 561] * x[1 * i0 + 78];
  }
  y[271] += A[567] * x[79];
  y[271] += A[568] * x[271];
  y[271] += A[569] * x[319];
  y[271] += A[570] * x[375];
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 272] += A[5 * i0 + 571] * x[1 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 272] += A[1 * i0 + 572] * x[1 * i0 + 80];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 272] += A[5 * i0 + 574] * x[1 * i0 + 256];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 272] += A[5 * i0 + 575] * x[1 * i0 + 320];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 273] += A[1 * i0 + 577] * x[1 * i0 + 81];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 274] += A[1 * i0 + 582] * x[1 * i0 + 82];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 275] += A[1 * i0 + 587] * x[1 * i0 + 83];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 276] += A[1 * i0 + 592] * x[1 * i0 + 84];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 277] += A[1 * i0 + 597] * x[1 * i0 + 85];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 278] += A[1 * i0 + 602] * x[1 * i0 + 86];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 279] += A[1 * i0 + 607] * x[1 * i0 + 87];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 280] += A[5 * i0 + 611] * x[1 * i0 + 8];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 280] += A[1 * i0 + 612] * x[1 * i0 + 88];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 280] += A[5 * i0 + 614] * x[1 * i0 + 264];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 280] += A[5 * i0 + 615] * x[1 * i0 + 328];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 281] += A[1 * i0 + 617] * x[1 * i0 + 89];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 282] += A[1 * i0 + 622] * x[1 * i0 + 90];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 283] += A[1 * i0 + 627] * x[1 * i0 + 91];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 284] += A[1 * i0 + 632] * x[1 * i0 + 92];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 285] += A[1 * i0 + 637] * x[1 * i0 + 93];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 286] += A[1 * i0 + 642] * x[1 * i0 + 94];
  }
  y[287] += A[647] * x[95];
  y[287] += A[648] * x[271];
  y[287] += A[649] * x[335];
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 288] += A[6 * i0 + 650] * x[1 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 288] += A[1 * i0 + 651] * x[1 * i0 + 96];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 288] += A[6 * i0 + 653] * x[1 * i0 + 256];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 288] += A[6 * i0 + 654] * x[1 * i0 + 320];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 288] += A[6 * i0 + 655] * x[1 * i0 + 345];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 289] += A[1 * i0 + 657] * x[1 * i0 + 97];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 290] += A[1 * i0 + 663] * x[1 * i0 + 98];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 291] += A[1 * i0 + 669] * x[1 * i0 + 99];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 292] += A[1 * i0 + 675] * x[1 * i0 + 100];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 293] += A[1 * i0 + 681] * x[1 * i0 + 101];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 294] += A[1 * i0 + 687] * x[1 * i0 + 102];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 295] += A[1 * i0 + 693] * x[1 * i0 + 103];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 296] += A[6 * i0 + 698] * x[1 * i0 + 8];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 296] += A[1 * i0 + 699] * x[1 * i0 + 104];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 296] += A[6 * i0 + 701] * x[1 * i0 + 264];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 296] += A[6 * i0 + 702] * x[1 * i0 + 328];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 296] += A[6 * i0 + 703] * x[1 * i0 + 353];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 297] += A[1 * i0 + 705] * x[1 * i0 + 105];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 298] += A[1 * i0 + 711] * x[1 * i0 + 106];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 299] += A[1 * i0 + 717] * x[1 * i0 + 107];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 300] += A[1 * i0 + 723] * x[1 * i0 + 108];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 301] += A[1 * i0 + 729] * x[1 * i0 + 109];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 302] += A[1 * i0 + 735] * x[1 * i0 + 110];
  }
  y[303] += A[741] * x[111];
  y[303] += A[742] * x[271];
  y[303] += A[743] * x[335];
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 304] += A[5 * i0 + 744] * x[1 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 304] += A[1 * i0 + 745] * x[1 * i0 + 112];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 304] += A[5 * i0 + 747] * x[1 * i0 + 256];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 304] += A[5 * i0 + 748] * x[1 * i0 + 320];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 305] += A[1 * i0 + 750] * x[1 * i0 + 113];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 306] += A[1 * i0 + 755] * x[1 * i0 + 114];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 307] += A[1 * i0 + 760] * x[1 * i0 + 115];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 308] += A[1 * i0 + 765] * x[1 * i0 + 116];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 309] += A[1 * i0 + 770] * x[1 * i0 + 117];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 310] += A[1 * i0 + 775] * x[1 * i0 + 118];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 311] += A[1 * i0 + 780] * x[1 * i0 + 119];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 312] += A[5 * i0 + 784] * x[1 * i0 + 8];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 312] += A[1 * i0 + 785] * x[1 * i0 + 120];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 312] += A[5 * i0 + 787] * x[1 * i0 + 264];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 312] += A[5 * i0 + 788] * x[1 * i0 + 328];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 313] += A[1 * i0 + 790] * x[1 * i0 + 121];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 314] += A[1 * i0 + 795] * x[1 * i0 + 122];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 315] += A[1 * i0 + 800] * x[1 * i0 + 123];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 316] += A[1 * i0 + 805] * x[1 * i0 + 124];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 317] += A[1 * i0 + 810] * x[1 * i0 + 125];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 318] += A[1 * i0 + 815] * x[1 * i0 + 126];
  }
  y[319] += A[820] * x[127];
  y[319] += A[821] * x[271];
  y[319] += A[822] * x[335];
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 320] += A[5 * i0 + 823] * x[1 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 320] += A[1 * i0 + 824] * x[1 * i0 + 128];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 320] += A[5 * i0 + 826] * x[1 * i0 + 256];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 320] += A[5 * i0 + 827] * x[1 * i0 + 345];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 321] += A[1 * i0 + 829] * x[1 * i0 + 129];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 322] += A[1 * i0 + 834] * x[1 * i0 + 130];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 323] += A[1 * i0 + 839] * x[1 * i0 + 131];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 324] += A[1 * i0 + 844] * x[1 * i0 + 132];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 325] += A[1 * i0 + 849] * x[1 * i0 + 133];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 326] += A[1 * i0 + 854] * x[1 * i0 + 134];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 327] += A[1 * i0 + 859] * x[1 * i0 + 135];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 328] += A[5 * i0 + 863] * x[1 * i0 + 8];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 328] += A[1 * i0 + 864] * x[1 * i0 + 136];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 328] += A[5 * i0 + 866] * x[1 * i0 + 264];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 328] += A[5 * i0 + 867] * x[1 * i0 + 353];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 329] += A[1 * i0 + 869] * x[1 * i0 + 137];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 330] += A[1 * i0 + 874] * x[1 * i0 + 138];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 331] += A[1 * i0 + 879] * x[1 * i0 + 139];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 332] += A[1 * i0 + 884] * x[1 * i0 + 140];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 333] += A[1 * i0 + 889] * x[1 * i0 + 141];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 334] += A[1 * i0 + 894] * x[1 * i0 + 142];
  }
  y[335] += A[899] * x[143];
  y[335] += A[900] * x[271];
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 336] += A[4 * i0 + 901] * x[1 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 336] += A[1 * i0 + 902] * x[1 * i0 + 144];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 336] += A[4 * i0 + 904] * x[1 * i0 + 256];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 337] += A[1 * i0 + 906] * x[1 * i0 + 145];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 338] += A[1 * i0 + 910] * x[1 * i0 + 146];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 339] += A[1 * i0 + 914] * x[1 * i0 + 147];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 340] += A[1 * i0 + 918] * x[1 * i0 + 148];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 341] += A[1 * i0 + 922] * x[1 * i0 + 149];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 342] += A[1 * i0 + 926] * x[1 * i0 + 150];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 343] += A[1 * i0 + 930] * x[1 * i0 + 151];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 344] += A[4 * i0 + 933] * x[1 * i0 + 8];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 344] += A[1 * i0 + 934] * x[1 * i0 + 152];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 344] += A[4 * i0 + 936] * x[1 * i0 + 264];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 345] += A[1 * i0 + 938] * x[1 * i0 + 153];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 346] += A[1 * i0 + 942] * x[1 * i0 + 154];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 347] += A[1 * i0 + 946] * x[1 * i0 + 155];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 348] += A[1 * i0 + 950] * x[1 * i0 + 156];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 349] += A[1 * i0 + 954] * x[1 * i0 + 157];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 350] += A[1 * i0 + 958] * x[1 * i0 + 158];
  }
  y[351] += A[962] * x[159];
  y[351] += A[963] * x[271];
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 352] += A[3 * i0 + 964] * x[1 * i0 + 160];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 352] += A[3 * i0 + 965] * x[1 * i0 + 272];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 352] += A[3 * i0 + 966] * x[1 * i0 + 288];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 360] += A[3 * i0 + 988] * x[1 * i0 + 168];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 360] += A[3 * i0 + 989] * x[1 * i0 + 280];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 360] += A[3 * i0 + 990] * x[1 * i0 + 296];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 368] += A[6 * i0 + 1012] * x[1 * i0 + 0];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 368] += A[1 * i0 + 1013] * x[1 * i0 + 176];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 368] += A[6 * i0 + 1015] * x[1 * i0 + 272];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 368] += A[6 * i0 + 1016] * x[1 * i0 + 304];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 368] += A[6 * i0 + 1017] * x[1 * i0 + 360];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 369] += A[1 * i0 + 1019] * x[1 * i0 + 177];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 370] += A[1 * i0 + 1025] * x[1 * i0 + 178];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 371] += A[1 * i0 + 1031] * x[1 * i0 + 179];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 372] += A[1 * i0 + 1037] * x[1 * i0 + 180];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 373] += A[1 * i0 + 1043] * x[1 * i0 + 181];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 374] += A[1 * i0 + 1049] * x[1 * i0 + 182];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 375] += A[1 * i0 + 1055] * x[1 * i0 + 183];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 376] += A[6 * i0 + 1060] * x[1 * i0 + 8];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 376] += A[1 * i0 + 1061] * x[1 * i0 + 184];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 376] += A[6 * i0 + 1063] * x[1 * i0 + 280];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 376] += A[6 * i0 + 1064] * x[1 * i0 + 312];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 376] += A[6 * i0 + 1065] * x[1 * i0 + 368];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 377] += A[1 * i0 + 1067] * x[1 * i0 + 185];
  }
#pragma endmacveth
}