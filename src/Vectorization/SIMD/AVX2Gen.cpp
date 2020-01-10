/**
 * File              : AVX2Gen.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 27 Dec 2019 09:00:11 MST
 * Last Modified Date: Xov 09 Xan 2020 23:22:40 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Vectorization/SIMD/AVX2Gen.h"
#include "include/Vectorization/SIMD/CostTable.h"
#include "include/Vectorization/SIMD/SIMDGenerator.h"

using namespace macveth;

void printDebb(std::string S) { std::cout << "[AVX2] " << S << std::endl; }

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
  CostTable::addRow(NArch, "broadcast", 1, "_mm#W_#Pbroadcast#S_#D");
  CostTable::addRow(NArch, "set", 1, "_mm#W_#Pset#S_#D");
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
std::string AVX2Gen::getRegisterType(VectorIR::VDataType DT,
                                     VectorIR::VWidth W) {
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
void AVX2Gen::addSIMDInst(VectorIR::VOperand V, std::string Op,
                          std::string PrefS, std::string SuffS,
                          std::list<std::string> OPS,
                          SIMDGenerator::SIMDType SType,
                          SIMDGenerator::SIMDInstListType *IL) {
  // Get the function
  std::string Pattern = CostTable::getPattern(AVX2Gen::NArch, Op);

  // printDebb(Pattern);
  // printDebb(PrefS);
  // printDebb(SuffS);
  // printDebb(getMapWidth(V.getWidth()));
  // printDebb(getMapType(V.getDataType()));

  // Replace fills in pattern
  std::string AVXFunc =
      replacePatterns(Pattern, getMapWidth(V.getWidth()),
                      getMapType(V.getDataType()), PrefS, SuffS);

  // printDebb(AVXFunc);

  // Generate SIMD inst
  SIMDGenerator::SIMDInst I(V.getName(), AVXFunc, OPS);

  // Retrieving cost of function
  I.Cost += CostTable::getLatency(AVX2Gen::NArch, Op);
  I.SType = SType;

  printDebb(I.render());

  // Adding instruction to the list
  IL->push_back(I);
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vpack(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  PrefS += (V.Mask) ? "mask" : "";

  // TODO
  // Type of load: load/u
  // [1] software.intel.com/en-us/forums/intel-isa-extensions/topic/752392
  // [2] https://stackoverflow.com/questions/36191748/difference-between-\
  // load1-and-broadcast-intrinsics
  std::string Op = "load";

  // TODO check
  // List of parameters
  std::list<std::string> OPS;
  OPS.push_back(V.UOP[0]->getValue());

  // Adding SIMD inst to the list
  addSIMDInst(V, Op, PrefS, SuffS, OPS, SIMDType::VPACK, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vbcast(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "broadcast";

  // TODO check
  // List of parameters
  std::list<std::string> OPS;
  OPS.push_back(V.UOP[0]->getValue());

  // Adding SIMD inst to the list
  addSIMDInst(V, Op, PrefS, SuffS, OPS, SIMDType::VBCAST, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vgather(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "gather";

  // TODO check
  // List of parameters
  std::list<std::string> OPS;
  OPS.push_back(V.UOP[0]->getValue());

  // Adding SIMD inst to the list
  addSIMDInst(V, Op, PrefS, SuffS, OPS, SIMDType::VGATHER, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vset(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "set";

  // TODO check
  // List of parameters
  std::list<std::string> OPS;
  for (int n = 0; n < V.VSize; n++) {
    OPS.push_back((V.UOP[n] != NULL) ? V.UOP[n]->getValue() : "0.0");
  }

  // Adding SIMD inst to the list
  addSIMDInst(V, Op, PrefS, SuffS, OPS, SIMDType::VSET, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vstore(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "store";

  // TODO check
  // List of parameters
  std::list<std::string> OPS;
  OPS.push_back(V.UOP[0]->getValue());

  // Adding SIMD inst to the list
  addSIMDInst(V, Op, PrefS, SuffS, OPS, SIMDType::VSTORE, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vscatter(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "scatter";

  // TODO check
  // List of parameters
  std::list<std::string> OPS;
  OPS.push_back(V.UOP[0]->getValue());

  // Adding SIMD inst to the list
  addSIMDInst(V, Op, PrefS, SuffS, OPS, SIMDType::VSTORE, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vadd(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  // PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "add";

  // TODO check
  // List of parameters
  std::list<std::string> OPS;
  OPS.push_back(V.OpA.getName());
  OPS.push_back(V.OpB.getName());

  // Adding SIMD inst to the list
  addSIMDInst(V.R, Op, PrefS, SuffS, OPS, SIMDType::VADD, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vmul(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  // PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "mul";

  // TODO check
  // List of parameters
  std::list<std::string> OPS;
  OPS.push_back(V.OpA.getName());
  OPS.push_back(V.OpB.getName());

  // Adding SIMD inst to the list
  addSIMDInst(V.R, Op, PrefS, SuffS, OPS, SIMDType::VMUL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vsub(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  // PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "sub";

  // TODO check
  // List of parameters
  std::list<std::string> OPS;
  OPS.push_back(V.OpA.getName());
  OPS.push_back(V.OpB.getName());

  // Adding SIMD inst to the list
  addSIMDInst(V.R, Op, PrefS, SuffS, OPS, SIMDType::VSUB, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vdiv(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  // PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "div";

  // TODO check
  // List of parameters
  std::list<std::string> OPS;
  OPS.push_back(V.OpA.getName());
  OPS.push_back(V.OpB.getName());

  // Adding SIMD inst to the list
  addSIMDInst(V.R, Op, PrefS, SuffS, OPS, SIMDType::VDIV, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vmod(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  // PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "mod";

  // TODO check
  // List of parameters
  std::list<std::string> OPS;
  OPS.push_back(V.OpA.getName());
  OPS.push_back(V.OpB.getName());

  // Adding SIMD inst to the list
  addSIMDInst(V.R, Op, PrefS, SuffS, OPS, SIMDType::VMOD, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vreduce(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // List of parameters
  std::list<std::string> OPS;
  // Mask
  // PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = V.VN;
  printDebb(Op);
  /// Algorithm overview
  /// ymm0 = _mm256_loadu_pd(indata);
  /// ymm1 = _mm256_permute_pd(ymm0, 0x05);
  /// ymm2 = _mm256_#OP#_pd(ymm0, ymm1);
  /// ymm3 = _mm256_permute2f128_pd(ymm2, ymm2, 0x01);
  /// ymm4 = _mm256_#OP#_pd(ymm2, ymm3);
  /// [OPT] _mm256_storeu_pd(outdata, ymm4);

  // TODO check
  OPS.push_back(V.OpA.getName());
  OPS.push_back(V.OpB.getName());

  // Adding SIMD inst to the list
  addSIMDInst(V.R, Op, PrefS, SuffS, OPS, SIMDType::VREDUC, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vseq(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  // PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "seq";

  // TODO check
  // List of parameters
  std::list<std::string> OPS;
  OPS.push_back(V.OpA.getName());
  OPS.push_back(V.OpB.getName());

  // Adding SIMD inst to the list
  addSIMDInst(V.R, Op, PrefS, SuffS, OPS, SIMDType::VSEQ, &IL);

  return IL;
}
