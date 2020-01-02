/**
 * File              : AVX2Gen.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 22 Dec 2019 20:50:29 MST
 * Last Modified Date: Xov 02 Xan 2020 15:04:43 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_AVX2GEN_H
#define MACVETH_AVX2GEN_H

#include "include/Vectorization/SIMD/SIMDGenerator.h"
#include "include/Vectorization/VectorIR.h"

using namespace macveth;

namespace macveth {

/// SIMD generator for AVX2 architectures
class AVX2Gen : public SIMDGenerator {
public:
  /// Name of the architecture
  static inline std::string NArch = "IntelX86";
  /// Name of the ISA
  static inline std::string NISA = "AVX2";

  /// Method to generate an AVX instruction
  // SIMDGenerator::SIMDInfo genSIMD(std::list<VectorIR::VectorOP> V) override;

  /// Get name of AVX architecture
  std::string getNArch() override { return AVX2Gen::NArch; }

  /// Get name of AVX architecture
  std::string getNISA() override { return AVX2Gen::NISA; }

  /// Get the traslation between VectorIR data widths and AVX2's
  std::map<VectorIR::VWidth, std::string> getMapWidth() override {
    return MapWidth;
  }

  /// Get the traslation between VectorIR data types and AVX2's
  std::map<VectorIR::VDataType, std::string> getMapType() override {
    return MapType;
  }

  /// Get max width
  int getMaxWidth() override { return AVX2Gen::MaxWidth; }

  /// Destructor
  virtual ~AVX2Gen(){};

  /// Constructor
  AVX2Gen() : SIMDGenerator() { AVX2Gen::populateTable(); }

private:
  /// Populate the table only when creating the object, to avoid overloading the
  /// memory from the start
  static void populateTable();
  /// Specific instruction for loading data according to the operand
  bool genLoadInst(VectorIR::VOperand V, SIMDGenerator::SIMDInstListType *L);
  /// Max width
  static inline int MaxWidth = 256;
  /// Mapping the width types with its name in AVX2
  static inline std::map<VectorIR::VWidth, std::string> MapWidth = {
      {VectorIR::VWidth::W128, ""}, {VectorIR::VWidth::W256, "256"}};
  /// Map of VectorIR types and its translation in the AVX2 architecture
  static inline std::map<VectorIR::VDataType, std::string> MapType = {
      {VectorIR::VDataType::FLOAT, "ps"},
      {VectorIR::VDataType::DOUBLE, "pd"},
      {VectorIR::VDataType::INT8, "epi8"},
      {VectorIR::VDataType::INT16, "epi16"},
      {VectorIR::VDataType::INT32, "epi32"},
      {VectorIR::VDataType::INT64, "epi64"},
      {VectorIR::VDataType::UINT8, "epu8"},
      {VectorIR::VDataType::UINT16, "epu16"},
      {VectorIR::VDataType::UINT32, "epu32"},
      {VectorIR::VDataType::UINT64, "epu64"},
      {VectorIR::VDataType::UNDEF128, "si128"},
      {VectorIR::VDataType::UNDEF256, "si256"},
      {VectorIR::VDataType::IN_INT128, "m128i"},
      {VectorIR::VDataType::IN_INT256, "m256i"},
      {VectorIR::VDataType::IN_FLOAT128, "m128"},
      {VectorIR::VDataType::IN_FLOAT256, "m256"},
      {VectorIR::VDataType::IN_DOUBLE128, "m128d"},
      {VectorIR::VDataType::IN_DOUBLE256, "m128d"}};
};

} // namespace macveth
#endif
