/**
 * File              : SIMDGenerator.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 22 Dec 2019 20:50:04 MST
 * Last Modified Date: Xov 02 Xan 2020 13:32:51 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Vectorization/SIMD/SIMDGenerator.h"
#include "include/Vectorization/SIMD/CostTable.h"

using namespace macveth;

// ---------------------------------------------
std::string SIMDGenerator::SIMDInst::render() {
  std::string FullFunc =
      !Result.compare("") ? FuncName + "(" : Result + " = " + FuncName + "(";
  std::list<std::string>::iterator Op;
  int i = 0;
  for (Op = OPS.begin(); Op != OPS.end(); ++Op) {
    FullFunc += (i++ == (OPS.size() - 1)) ? *Op : (*Op + ", ");
  }
  FullFunc += ")";
  return FullFunc;
}

// ---------------------------------------------
std::list<std::string> SIMDGenerator::renderSIMDasString(SIMDInfo S) {
  std::list<std::string> L;
  // Render register declarations
  for (auto It = SIMDGenerator::RegDeclared.begin();
       It != SIMDGenerator::RegDeclared.end(); ++It) {
    std::string TypeRegDecl = It->first + " ";
    int i = 0;
    for (auto N = It->second.begin(); N != It->second.end(); ++N) {
      TypeRegDecl += (i++ == (It->second.size() - 1)) ? *N : (*N + ", ");
    }
    TypeRegDecl += ";";
    L.push_back(TypeRegDecl);
  }
  // For pretty printing: letting a blank line between declarations and the
  // actual instructions
  L.push_back("\n");
  // Render instructions
  for (SIMDInst I : S.SIMDList) {
    L.push_back(I.render() + ";");
  }
  return L;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
SIMDGenerator::getSIMDfromVectorOP(VectorIR::VectorOP V) {
  SIMDInstListType I;
  // Get width as string
  std::string WidthS = getMapWidth()[V.VW];
  // Get data type as string
  std::string DataTypeS = getMapType()[V.DT];
  // FIXME Is it needed prefix?
  std::string PrefS = "";
  // FIXME Is it needed suffix?
  std::string SuffS = "";
  // Get name of the function
  std::string Pattern = CostTable::getPattern(getNArch(), V.VN);
  // Replace fills in pattern
  std::string AVXFunc =
      replacePatterns(Pattern, WidthS, DataTypeS, PrefS, SuffS);

  VectorIR::VOperand VOpA = V.OpA;
  VectorIR::VOperand VOpB = V.OpB;
  // Generate load instructions if needed
  genLoadInst(VOpA, &R.SIMDList);
  genLoadInst(VOpB, &R.SIMDList);

  // Generate function
  SIMDInst Inst(V.R.getName(), AVXFunc, {VOpA.getName(), VOpB.getName()});
  I.push_back(Inst);

  // Registers used
  std::string RegType = getRegisterType(V.DT, V.VW);
  SIMDGenerator::addRegToDeclare(RegType, V.R.getName());
  SIMDGenerator::addRegToDeclare(RegType, VOpA.getName());
  SIMDGenerator::addRegToDeclare(RegType, VOpB.getName());

  return I;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
SIMDGenerator::getSIMDfromVectorOP(std::list<VectorIR::VectorOP> VList) {
  SIMDInstListType I;
  for (VectorIR::VectorOP V : VList) {
    for (SIMDInst Inst : getSIMDfromVectorOP(V)) {
      I.push_back(Inst);
    }
  }
  // TODO Peephole optimizations:
  // Then optimizations can be done, for instance, combine operatios such as
  // addition + multiplication
  return I;
}

// ---------------------------------------------
void SIMDGenerator::addRegToDeclare(std::string Type, std::string Name) {
  if (!Utils::contains(SIMDGenerator::RegDeclared[Type], Name)) {
    SIMDGenerator::RegDeclared[Type].push_back(Name);
  }
}

// ---------------------------------------------
void SIMDGenerator::clearMappings() {
  for (auto &X : SIMDGenerator::RegDeclared) {
    X.second.clear();
  }
  SIMDGenerator::RegDeclared.clear();
}
