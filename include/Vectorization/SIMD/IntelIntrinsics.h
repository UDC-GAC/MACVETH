// MACVETH - IntelIntrinsics.h
//
// Copyright (c) Colorado State University. 2019-2021
// Copyright (c) Universidade da Coruña. 2020-2021
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Authors:
//     Marcos Horro <marcos.horro@udc.es>
//     Louis-Noël Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Marcos Horro <marcos.horro@udc.es>

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
      {MVDataType::VDataType::HALF_FLOAT, "pi"},
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