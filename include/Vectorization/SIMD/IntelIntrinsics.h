/*
 * File 	 : include/Vectorization/SIMD/IntelIntrinsics.h
 * Author    : Marcos Horro
 * Date	     : Thu 12 Nov 2020 05:28 +01:00
 *
 * Last Modified : Thu 12 Nov 2020 05:29 +01:00
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

#ifndef MACVETH_INTELINTRINSICS_H
#define MACVETH_INTELINTRINSICS_H

#include "include/MVExpr/MVDataType.h"
#include <map>

namespace macveth {
class IntelIntrinsics {
public:
  /// Mapping the width types with its name in AVX2
  static inline std::map<MVDataType::VWidth, std::string> MapWidth = {
      {MVDataType::VWidth::W128, ""},
      {MVDataType::VWidth::W256, "256"},
      {MVDataType::VWidth::W512, "512"}};

  /// Map of VectorIR types and its translation in the AVX2 architecture
  static inline std::map<MVDataType::VDataType, std::string> MapType = {
      {MVDataType::VDataType::FLOAT, "ps"},
      {MVDataType::VDataType::SFLOAT, "ss"},
      {MVDataType::VDataType::DOUBLE, "pd"},
      {MVDataType::VDataType::SDOUBLE, "sd"},
      {MVDataType::VDataType::INT8, "epi8"},
      {MVDataType::VDataType::INT16, "epi16"},
      {MVDataType::VDataType::INT32, "epi32"},
      {MVDataType::VDataType::INT64, "epi64"},
      {MVDataType::VDataType::UINT8, "epu8"},
      {MVDataType::VDataType::UINT16, "epu16"},
      {MVDataType::VDataType::UINT32, "epu32"},
      {MVDataType::VDataType::UINT64, "epu64"},
      {MVDataType::VDataType::UNDEF128, "si128"},
      {MVDataType::VDataType::UNDEF256, "si256"},
      {MVDataType::VDataType::IN_INT128, "m128i"},
      {MVDataType::VDataType::IN_INT256, "m256i"},
      {MVDataType::VDataType::IN_FLOAT128, "m128"},
      {MVDataType::VDataType::IN_FLOAT256, "m256"},
      {MVDataType::VDataType::IN_DOUBLE128, "m128d"},
      {MVDataType::VDataType::IN_DOUBLE256, "m128d"}};
};
} // namespace macveth
#endif