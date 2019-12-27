/**
 * File              : AVX2Gen.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 27 Dec 2019 09:00:11 MST
 * Last Modified Date: Ven 27 Dec 2019 10:28:13 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Vectorization/SIMD/AVX2Gen.h"
#include "include/Vectorization/SIMD/CostTable.h"

#include <regex>

// ---------------------------------------------
void AVX2Gen::populateTable() {
  // In
  CostTable::addRow(NArch, "mul", 3, "_mm#W_mul_#D");
  CostTable::addRow(NArch, "add", 1, "_mm#W_add_#D");
  CostTable::addRow(NArch, "sub", 1, "_mm#W_sub_#D");
  CostTable::addRow(NArch, "div", 5, "_mm#W_div_#D");
  CostTable::addRow(NArch, "fmadd", 5, "_mm#W_fmadd_#D");
  CostTable::addRow(NArch, "avg", 5, "_mm#W_avg_#D");
  CostTable::addRow(NArch, "min", 5, "_mm#W_min_#D");
  CostTable::addRow(NArch, "max", 5, "_mm#W_max_#D");
  // Load operations
  CostTable::addRow(NArch, "load", 1, "_mm#W_#Mload_#D");
  CostTable::addRow(NArch, "gather", 1, "_mm#W_#M#TDgather_#D");
}

// ---------------------------------------------
std::string replacePattern(std::string P, std::regex R, std::string Rep) {
  if (Rep.compare("")) {
    return std::regex_replace(P, R, Rep);
  }
  return P;
}

// ---------------------------------------------
std::string replacePatterns(std::string Pattern, std::string W, std::string M,
                            std::string TD, std::string D) {
  std::regex WidthRegex("(#W)");
  std::regex MaskRegex("(#M)");
  std::regex TypeDataRegex("(#TD)");
  std::regex DataRegex("(#D)");
  std::string NewFunc = Pattern;

  NewFunc = replacePattern(NewFunc, WidthRegex, W);
  NewFunc = replacePattern(NewFunc, MaskRegex, M);
  NewFunc = replacePattern(NewFunc, TypeDataRegex, TD);
  NewFunc = replacePattern(NewFunc, DataRegex, D);

  return NewFunc;
}

// ---------------------------------------------
SIMDGenerator::SIMDInfo AVX2Gen::genSIMD(std::list<VectorIR::VectorOP> V) {
  SIMDGenerator::SIMDInfo R;

  // Fuse operations
  // searchForFuseOperations();

  for (auto X : V) {
    // Get width as string
    std::string WIDTH = MapWidth[X.VW];
    // Get data type as string
    std::string DATA_TYPE = MapType[X.DT];
    // Get name of the function
    std::string Pattern = CostTable::getPattern(NArch, X.VN);
    // Replace fills in pattern
    std::string AVXFunc = replacePatterns(Pattern, WIDTH, "", "", DATA_TYPE);
    SIMDInst I(X.R.getName(), AVXFunc, {X.OpA.getName(), X.OpB.getName()});
    R.SIMDList.push_back(I);
  }

  return R;
}
