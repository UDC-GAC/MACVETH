/**
 * File              : AVX2Gen.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 22 Dec 2019 20:50:29 MST
 * Last Modified Date: Ven 27 Dec 2019 09:18:25 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_AVX2GEN_H
#define MACVETH_AVX2GEN_H

#include "include/Vectorization/SIMD/CostTable.h"
#include "include/Vectorization/SIMD/SIMDGenerator.h"
#include "include/Vectorization/VectorIR.h"

#include <regex>

using namespace macveth;

namespace macveth {

/// SIMD generator for AVX2 architectures
class AVX2Gen : public SIMDGenerator {
public:
  /// Name of the architecture
  static inline std::string NArch = "AVX2";
  /// Max width
  static inline int MaxWidth = 256;
  static inline std::map<int, std::string> WidthTypeMap = {{128, "sp"},
                                                           {256, "dp"}};

  /// Populate the table only when creating the object, to avoid overloading the
  /// memory from the start
  void populateTable();

  /// Method to generate an AVX instruction
  virtual SIMDGenerator::SIMDInfo genSIMD(std::list<VectorIR::VectorOP> V);

  /// Constructor
  AVX2Gen() { populateTable(); }

private:
  static inline std::map<VectorIR::VWidth, std::string> MapWidth = {
      {VectorIR::VWidth::W256, "256"}};
  static inline std::map<VectorIR::VDataType, std::string> MapType = {
      {VectorIR::VDataType::DOUBLE, "pd"}};
};

} // namespace macveth
#endif
