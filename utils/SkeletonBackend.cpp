/**
 * File              : SkeletonBackend.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 27 Dec 2019 09:00:11 MST
 * Last Modified Date: Ven 03 Xan 2020 16:09:02 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Vectorization/SIMD/AVX2Gen.h"
#include "include/Vectorization/SIMD/CostTable.h"
#include "include/Vectorization/SIMD/SIMDGenerator.h"

#include <llvm-10/llvm/Config/llvm-config.h>
#include <regex>

using namespace macveth;

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vpack(VectorIR::VOperand V) {}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vbcast(VectorIR::VOperand V) {}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vgather(VectorIR::VOperand V) {}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vset(VectorIR::VOperand V) {}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vstore(VectorIR::VOperand V) {}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vadd(VectorIR::VectorOP V) {}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vmul(VectorIR::VectorOP V) {}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vsub(VectorIR::VectorOP V) {}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vdiv(VectorIR::VectorOP V) {}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vmod(VectorIR::VectorOP V) {}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vreduce(VectorIR::VectorOP V) {}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vseq(VectorIR::VectorOP V) {}

// ---------------------------------------------
void AVX2Gen::populateTable() {
  // Sandy Bridge (Intel Q1 2011), Bulldozer (AMD Q3 2011)
  // Math operations
  CostTable::addRow(NArch, "mul", 3, "_mm#W_#Pmul#S_#D");
  CostTable::addRow(NArch, "add", 1, "_mm#W_#Padd#S_#D");
  CostTable::addRow(NArch, "sub", 1, "_mm#W_#Psub#S_#D");
  CostTable::addRow(NArch, "div", 5, "_mm#W_#Pdiv#S_#D");
  CostTable::addRow(NArch, "fmadd", 5, "_mm#W_#Pfmadd#S_#D");
  CostTable::addRow(NArch, "avg", 5, "_mm#W_avg_#D");
  CostTable::addRow(NArch, "min", 5, "_mm#W_min_#D");
  CostTable::addRow(NArch, "max", 5, "_mm#W_max_#D");
  CostTable::addRow(NArch, "round", 10, "_mm#W_#Psvml_round#S_#D");
  CostTable::addRow(NArch, "trunc", 10, "_mm#W_#Ptrunc#S_#D");
  CostTable::addRow(NArch, "ceil", 10, "_mm#W_#Psvml_ceil#S_#D");
  // Load operations
  CostTable::addRow(NArch, "load", 1, "_mm#W_#Pload#S_#D");
  CostTable::addRow(NArch, "gather", 1, "_mm#W_#Pgather#S_#D");
  // Store operation
  CostTable::addRow(NArch, "store", 1, "_mm#W_#Pstore#S_#D");
  CostTable::addRow(NArch, "stream", 1, "_mm#W_#Pstream#S_#D");
  // Trigonometry
  CostTable::addRow(NArch, "cos", 10, "_mm#W_#Pcos#S_#D");
  CostTable::addRow(NArch, "sin", 10, "_mm#W_#Psin#S_#D");
  CostTable::addRow(NArch, "tan", 10, "_mm#W_#Ptan#S_#D");
  CostTable::addRow(NArch, "atanh", 10, "_mm#W_#Patanh#S_#D");
}

// ---------------------------------------------
std::string replacePattern(std::string P, std::regex R, std::string Rep) {
  return std::regex_replace(P, R, Rep);
}

// ---------------------------------------------
std::string replacePatterns(std::string Pattern, std::string W, std::string D,
                            std::string P, std::string S) {
  std::regex WidthRegex("(#W)");
  std::regex DataRegex("(#D)");
  std::regex PreRegex("(#P)");
  std::regex SuffRegex("(#S)");
  std::string NewFunc = Pattern;

  NewFunc = replacePattern(NewFunc, WidthRegex, W);
  NewFunc = replacePattern(NewFunc, DataRegex, D);
  NewFunc = replacePattern(NewFunc, PreRegex, P);
  NewFunc = replacePattern(NewFunc, SuffRegex, S);

  return NewFunc;
}

// ---------------------------------------------
std::string getRegisterType(VectorIR::VDataType DT, VectorIR::VWidth W) {
  std::string Suffix = "";
  if (DT == VectorIR::VDataType::DOUBLE) {
    Suffix = "d";
  } else if (DT == VectorIR::VDataType::FLOAT) {
    Suffix = "";
  } else {
    Suffix = "i";
  }
  return "__m" + std::to_string(W) + Suffix;
}
