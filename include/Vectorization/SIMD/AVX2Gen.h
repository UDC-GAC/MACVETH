/**
 * File              : AVX2Gen.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 22 Dec 2019 20:50:29 MST
 * Last Modified Date: Xov 26 Dec 2019 12:07:05 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_AVX2GEN_H
#define MACVETH_AVX2GEN_H

#include "include/Vectorization/SIMD/CostTable.h"
#include "include/Vectorization/SIMD/SIMDGenerator.h"
#include "include/Vectorization/VectorIR.h"

using namespace macveth;

namespace macveth {

class AVX2Gen : public SIMDGenerator {
public:
  static inline std::string NArch = "AVX2";
  static inline int MaxWidth = 256;
  static inline std::map<int, std::string> WidthTypeMap = {{128, "sp"},
                                                           {256, "dp"}};
  /// Populate the table only when creating the object, to avoid overloading the
  /// memory from the start
  void populateTable() {
    CostTable::addRow(NArch, "mul", 3, "_mm_mul");
    CostTable::addRow(NArch, "add", 1, "_mm_mul");
    CostTable::addRow(NArch, "sub", 1, "_mm_mul");
    CostTable::addRow(NArch, "", 1, "_mm_mul");
    CostTable::addRow(NArch, "mul", 1, "_mm_mul");
  }

  /// Method to generate an AVX instruction
  void generateIntrinsics(VectorIR::VectorOP V);

  /// Constructor
  AVX2Gen() { populateTable(); }
};

} // namespace macveth
#endif
