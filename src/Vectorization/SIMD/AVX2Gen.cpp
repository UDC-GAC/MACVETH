/**
 * File              : AVX2Gen.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 27 Dec 2019 09:00:11 MST
 * Last Modified Date: Mar 31 Dec 2019 18:07:19 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Vectorization/SIMD/AVX2Gen.h"
#include "Vectorization/SIMD/SIMDGenerator.h"
#include "include/Vectorization/SIMD/CostTable.h"

#include <llvm-10/llvm/Config/llvm-config.h>
#include <regex>

using namespace macveth;

// ---------------------------------------------
void AVX2Gen::populateTable() {
  // Sandy Bridge (Intel Q1 2011), Bulldozer (AMD Q3 2011)
  // Typical operations
  CostTable::addRow(NArch, "mul", 3, "_mm#W_#Pmul#S_#D");
  CostTable::addRow(NArch, "add", 1, "_mm#W_#Padd#S_#D");
  CostTable::addRow(NArch, "sub", 1, "_mm#W_#Psub#S_#D");
  CostTable::addRow(NArch, "div", 5, "_mm#W_#Pdiv#S_#D");
  CostTable::addRow(NArch, "fmadd", 5, "_mm#W_#Pfmadd#S_#D");
  CostTable::addRow(NArch, "avg", 5, "_mm#W_avg_#D");
  CostTable::addRow(NArch, "min", 5, "_mm#W_min_#D");
  CostTable::addRow(NArch, "max", 5, "_mm#W_max_#D");
  // Load operations
  CostTable::addRow(NArch, "load", 1, "_mm#W_#Pload#S_#D");
  CostTable::addRow(NArch, "gather", 1, "_mm#W_#Pgather#S_#D");
  // Store operation
  CostTable::addRow(NArch, "store", 1, "_mm#W_#Pstore#S_#D");
  CostTable::addRow(NArch, "stream", 1, "_mm#W_#Pstream#S_#D");
  // Trigonometry
  // CostTable::addRow(NArch, "cosine", 10, "_mm#W_#PREFIXcos#SUFFIX_#D");
  // CostTable::addRow(NArch, "sinus", 10, "_mm#W_#PREFIXsin#SUFFIX_#D");
  // CostTable::addRow(NArch, "hyperbolic", 10, "_mm#W_#PREFIXatanh#SUFFIX_#D");
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

// ---------------------------------------------
bool AVX2Gen::genLoadInst(VectorIR::VOperand V,
                          SIMDGenerator::SIMDInstListType *L) {
  if (!V.IsLoad) {
    // TODO maybe it would be OK to perform another action in this case
    std::cout << "NOT LOAD " << V.Name << std::endl;
    return false;
  }

  // Get width as string
  std::string WidthS = AVX2Gen::MapWidth[V.getWidth()];
  // Get data type as string
  std::string DataTypeS = AVX2Gen::MapType[V.getDataType()];
  // TODO generate prefix
  std::string PrefS = "";
  // TODO generate sufix
  std::string SuffS = "";
  // Mask
  PrefS += (V.Mask) ? "mask" : "";

  // TODO
  // Type of load: load, loadu, gather, set, broadcast
  // [1] software.intel.com/en-us/forums/intel-isa-extensions/topic/752392
  // [2] https://stackoverflow.com/questions/36191748/difference-between-\
  // load1-and-broadcast-intrinsics
  std::string Op = "load";

  // Get the function
  std::string Pattern = CostTable::getPattern(AVX2Gen::NArch, Op);

  // Replace fills in pattern
  std::string AVXFunc =
      replacePatterns(Pattern, WidthS, DataTypeS, PrefS, SuffS);

  /// TODO
  std::list<std::string> OPS;

  OPS.push_back(V.UOP[0]->getValue());

  // Generate function
  SIMDGenerator::SIMDInst I(V.getName(), AVXFunc, OPS);
  // Update the list
  L->push_back(I);

  return true;
}

// ---------------------------------------------
SIMDGenerator::SIMDInfo AVX2Gen::genSIMD(std::list<VectorIR::VectorOP> VL) {
  SIMDGenerator::SIMDInfo R;

  // Once we have the VectorIR (in between a middle IR and a low-level IR),
  // for generating the SIMD instructions, we first perform an instruction
  // selection which basically performs a pattern matching between the
  // operations an creates new SIMD instructions, AVX2 in this case
  for (auto V : VL) {
    // Get width as string
    std::string WidthS = MapWidth[V.VW];
    // Get data type as string
    std::string DataTypeS = MapType[V.DT];
    // FIXME Is it needed prefix?
    std::string PrefS = "";
    // FIXME Is it needed suffix?
    std::string SuffS = "";
    // Get name of the function
    std::string Pattern = CostTable::getPattern(NArch, V.VN);
    // Replace fills in pattern
    std::string AVXFunc =
        replacePatterns(Pattern, WidthS, DataTypeS, PrefS, SuffS);

    VectorIR::VOperand VOpA = V.OpA;
    VectorIR::VOperand VOpB = V.OpB;
    // Generate load instructions if needed
    genLoadInst(VOpA, &R.SIMDList);
    genLoadInst(VOpB, &R.SIMDList);

    // Generate function
    SIMDInst I(V.R.getName(), AVXFunc, {VOpA.getName(), VOpB.getName()});
    R.SIMDList.push_back(I);

    // Registers used
    std::string RegType = getRegisterType(V.DT, V.VW);
    SIMDGenerator::addRegToDeclare(RegType, V.R.getName());
    SIMDGenerator::addRegToDeclare(RegType, VOpA.getName());
    SIMDGenerator::addRegToDeclare(RegType, VOpB.getName());
  }

  // TODO Peephole optimizations:
  // Then optimizations can be done, for instance, combine operatios such as
  // addition + multiplication

  return R;
}
