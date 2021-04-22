// MACVETH - MVDataType.h
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
//     Louis-Nöel Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Louis-Nöel Pouchet <pouchet@colostate.edu>

#ifndef MACVETH_DATATYPE_H
#define MACVETH_DATATYPE_H

#include <map>
#include <string>

namespace macveth {

class MVDataType {
public:
  /// Vector data types: most of them are self explanatory
  enum VDataType {
    /// UNDEF
    UNDEF,
    /// This is the direct translation to "pd"
    DOUBLE,
    /// This is the direct translation onto "sd"
    SDOUBLE,
    /// This is the direct translation to "ps"
    FLOAT,
    ///
    HALF_FLOAT,
    /// This is the direct translation to "ss"
    SFLOAT,
    /// Unsigned 8-bit integer
    UINT8,
    /// Unsigned 16-bit integer
    UINT16,
    /// Unsigned 32-bit integer
    UINT32,
    /// Unsigned 64-bit integer
    UINT64,
    /// Signed 8-bit integer
    INT8,
    /// Signed 16-bit integer
    INT16,
    /// Signed 32-bit integer
    INT32,
    /// Signed 64-bit integer
    INT64,
    /// 128 bits, undefined type
    UNDEF128,
    /// 256 bits, undefined type
    UNDEF256,
    /// Input vector is INT128, output can be different
    IN_INT128,
    /// Input vector is FLOAT128, output can be different
    IN_FLOAT128,
    /// Input vector is DOUBLE128, output can be different
    IN_DOUBLE128,
    /// Input vector is INT256, output can be different
    IN_INT256,
    /// Input vector is FLOAT256, output can be different
    IN_FLOAT256,
    /// Input vector is DOUBLE256, output can be different
    IN_DOUBLE256
  };

  /// Table of equivalences between C/C++ basic numeric types and VectorIR's
  static inline std::map<std::string, VDataType> CTypeToVDataType = {
      {"double", DOUBLE},   {"float", FLOAT},     {"uint8_t", UINT8},
      {"uint16_t", UINT16}, {"uint32_t", UINT32}, {"uint64_t", UINT64},
      {"int8_t", INT8},     {"int16_t", INT16},   {"int32_t", INT32},
      {"int64_t", INT64},   {"int", INT32},       {"long", INT64},
  };

  /// Table of equivalences between C/C++ basic numeric types and VectorIR's
  static inline std::map<MVDataType::VDataType, std::string> VDTypeToCType = {
      {MVDataType::VDataType::DOUBLE, "double"},
      {MVDataType::VDataType::FLOAT, "float"},
      {MVDataType::VDataType::INT32, "int"},
      {MVDataType::VDataType::INT64, "long"},
      {MVDataType::VDataType::UINT32, "unsigned int"},
      {MVDataType::VDataType::UINT64, "unsigned long"},
  };

  /// Map data types to their size in bytes
  inline static std::map<std::string, int> SizeOf = {
      {"double", 8},
      {"float", 4},
      {"const unsigned long", 8},
      {"const long", 8},
      {"const float", 4},
      {"const double", 8},
      {"const char", 1},
      {"char", 1},
      {"const int", 4},
      {"const unsigned int", 4},
      {"unsigned int", 4},
      {"int", 4},
      {"long", 8},
      {"long long", 16},
      {"signed long long", 16},
      {"long long int", 16},
      {"signed long long int", 16}};

  /// Table of equivalences between the VDataTypes and the number of bits of
  /// type
  static inline std::map<VDataType, int> VDataTypeWidthBits = {
      {DOUBLE, 64}, {FLOAT, 32},  {SDOUBLE, 64}, {SFLOAT, 32},
      {UINT8, 8},   {UINT16, 16}, {UINT32, 32},  {UINT64, 64},
      {INT8, 8},    {INT16, 16},  {INT32, 32},   {INT64, 64},
  };

  /// Vector width possible types
  enum VWidth {
    W8 = 8,
    W16 = 16,
    W32 = 32,
    W64 = 64,
    W128 = 128,
    W256 = 256,
    W512 = 512
  };
};

} // namespace macveth

#endif /* !MACVETH_DATATYPE_H */