/**
 * File              : SIMDGenerator.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 22 Dec 2019 20:50:04 MST
 * Last Modified Date: Sáb 04 Xan 2020 22:14:38 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Vectorization/SIMD/SIMDGenerator.h"
#include "Vectorization/VectorIR.h"
#include "include/Vectorization/SIMD/CostTable.h"
#include "clang/AST/OperationKinds.h"
#include "clang/AST/Type.h"

#include <regex>

using namespace macveth;

//
void printDeb(std::string S) { std::cout << "[SIMDGEN] " << S << std::endl; }

// ---------------------------------------------
std::string SIMDGenerator::SIMDInst::render() {
  std::string FullFunc =
      !Result.compare("") ? FuncName + "(" : Result + " = " + FuncName + "(";
  std::list<std::string>::iterator Op;
  int i = 0;
  for (Op = OPS.begin(); Op != OPS.end(); ++Op) {
    FullFunc += (i++ == (OPS.size() - 1)) ? (*Op + ")") : (*Op + ", ");
  }
  return FullFunc;
}

// ---------------------------------------------
std::list<std::string> SIMDGenerator::renderSIMDasString(SIMDInstListType S) {
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
  for (SIMDInst I : S) {
    L.push_back(I.render() + ";");
  }
  return L;
}

// ---------------------------------------------
bool SIMDGenerator::getSIMDVOperand(VectorIR::VOperand V,
                                    SIMDGenerator::SIMDInstListType *IL) {
  if (!V.IsLoad) {
    // TODO maybe it would be OK to perform another action in this case
    std::cout << "NOT LOAD " << V.Name << std::endl;
    return false;
  }

  // Need to determine which type of memory/register load it is
  // We will say that it is a load if all the operands are contiguous and need
  // to be retrieved from memory
  //
  // We will say that it is a bcast if we need to replicate the values from
  // memory or a registers
  //
  // We will say it is a gather if we have to use an index to retrieve data from
  // memory (not contiguous)
  //
  // We will say it is a set if we have to explicity set the values of the
  // vector operand

  SIMDGenerator::SIMDInstListType TIL;
  bool EqualVal;
  bool ContMem = V.MemOp && !(V.Shuffle & 0x0);
  bool ScatterMem = V.MemOp && !ContMem;
  bool ExpVal = !V.MemOp;

  if ((ContMem) && (!ScatterMem)) {
    TIL = vpack(V);
  } else if ((!ContMem) && (ScatterMem)) {
    TIL = vgather(V);
  } else if ((!ContMem) && (!ScatterMem) && (!ExpVal)) {
    TIL = vbcast(V);
  } else if (ExpVal) {
    TIL = vset(V);
  }

  // Update the list
  for (auto I : TIL) {
    IL->push_back(I);
  }

  return true;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
SIMDGenerator::getMapOperation(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType TIL;
  printDeb("MapOperation");

  if (V.isBinOp()) {
    printDeb("Will crash here?");
    /// We can filter it by
    switch (V.getBinOp()) {
    case clang::BO_Add:
      TIL = vadd(V);
      break;
    case clang::BO_Sub:
      TIL = vsub(V);
      break;
    case clang::BO_Mul:
      TIL = vmul(V);
      break;
    case clang::BO_Div:
      TIL = vdiv(V);
      break;
    case clang::BO_Rem:
      TIL = vmod(V);
      break;
    }
    printDeb("or crash here?");
  } else {
    printDeb("or is just because of an empty fucking list");
  }

  return TIL;
}

// ---------------------------------------------
bool SIMDGenerator::getSIMDVOperation(VectorIR::VectorOP V,
                                      SIMDGenerator::SIMDInstListType *IL) {
  SIMDGenerator::SIMDInstListType TIL;

  printDeb("SIMDVOperation");

  switch (V.VT) {
  case VectorIR::VType::MAP:
    TIL = getMapOperation(V);
  case VectorIR::VType::REDUCE:
    TIL = vreduce(V);
    break;
  case VectorIR::VType::SEQ:
    TIL = vseq(V);
    break;
  };

  // Update the list
  for (auto I : TIL) {
    IL->push_back(I);
  }
  return true;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
SIMDGenerator::getSIMDfromVectorOP(VectorIR::VectorOP V) {
  SIMDInstListType IL;

  VectorIR::VOperand VOpA = V.OpA;
  VectorIR::VOperand VOpB = V.OpB;

  // Arranging the operands: maybe they need load, set, bcast...
  getSIMDVOperand(VOpA, &IL);
  getSIMDVOperand(VOpB, &IL);

  // Arranging the operation
  getSIMDVOperation(V, &IL);

  // Registers used
  std::string RegType = getRegisterType(V.DT, V.VW);
  SIMDGenerator::addRegToDeclare(RegType, V.R.getName());
  SIMDGenerator::addRegToDeclare(RegType, VOpA.getName());
  SIMDGenerator::addRegToDeclare(RegType, VOpB.getName());

  return IL;
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
std::string replacePattern(std::string P, std::regex R, std::string Rep) {
  return std::regex_replace(P, R, Rep);
}

// ---------------------------------------------
std::string SIMDGenerator::replacePatterns(std::string Pattern, std::string W,
                                           std::string D, std::string P,
                                           std::string S) {
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
void SIMDGenerator::clearMappings() {
  for (auto &X : SIMDGenerator::RegDeclared) {
    X.second.clear();
  }
  SIMDGenerator::RegDeclared.clear();
}
