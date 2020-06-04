/**
 * File              : SIMDGenerator.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 22 Dec 2019 20:50:04 MST
 * Last Modified Date: Mér 15 Xan 2020 12:26:46 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Vectorization/SIMD/SIMDGenerator.h"
#include "include/MVOptions.h"
#include "include/Utils.h"
#include "include/Vectorization/SIMD/CostTable.h"
#include "clang/AST/OperationKinds.h"
#include "clang/AST/Type.h"
#include <regex>
#include <sstream>
#include <unistd.h>
#if __GNUC__ >= 8
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

using namespace macveth;

// ---------------------------------------------
void SIMDGenerator::populateTable(MVISA ISA) {
  std::string PathISA = "/src/Vectorization/SIMD/CostsArch/" + MVISAStr[ISA];
  char buff[PATH_MAX];
  getcwd(buff, PATH_MAX);
  std::string cwd(buff);
  PathISA = cwd + PathISA;
  std::string Arch = MVArchStr[MVOptions::Arch];
  std::ifstream F(PathISA);
  std::string L, W;
  Utils::printDebug("CostsTable", "PathISA = " + PathISA);
  if (F.is_open()) {
    while (getline(F, L)) {
      if (L.rfind("#", 0) == 0) {
        // Check if line is a comment
        continue;
      }
      std::stringstream TMP(L);
      std::vector<std::string> Args;
      while (getline(TMP, W, ',')) {
        Args.push_back(W);
      }
      if (((Args[0] == Arch) || (Args[0] == "UNDEF")) && (Args.size() == 4)) {
        CostTable::addRow(Arch, Args[1], std::stoi(Args[2]), Args[3]);
      } else if (((Args[0] == Arch) || (Args[0] == "UNDEF")) &&
                 (Args.size() == 6)) {
        CostTable::addRow(Arch, Args[1], std::stoi(Args[2]), std::stod(Args[3]),
                          std::stoi(Args[4]), Args[5]);
      }
    }
    F.close();
  } else {
    Utils::printDebug(
        "SIMDGenerator",
        "This should never happen: PATH for costs file not found");
    llvm::llvm_unreachable_internal();
  }
}

// ---------------------------------------------
std::string SIMDGenerator::getOpName(VectorIR::VOperand V, bool Ptr,
                                     bool RegVal) {
  if ((Ptr) && (V.IsStore || V.IsLoad)) {
    return "&" + V.getRegName();
  }
  if (RegVal) {
    return V.getRegName();
  }
  return V.Name;
}

// ---------------------------------------------
std::string SIMDGenerator::SIMDInst::render() {
  if (MVOptions::MacroFree) {
    Utils::printDebug("SIMDGenerator", FuncName);
    std::string FullFunc = ((Result == "") || (SType == SIMDType::VSTORE))
                               ? FuncName + "("
                               : Result + " = " + FuncName + "(";
    std::list<std::string>::iterator Op;
    int i = 0;
    for (Op = Args.begin(); Op != Args.end(); ++Op) {
      FullFunc += (i++ == (Args.size() - 1)) ? (*Op + ")") : (*Op + ", ");
    }
    return FullFunc;
  } else {
    std::string FullFunc = ((Result == "") || (SType == SIMDType::VSTORE))
                               ? MVFuncName + "("
                               : Result + " = " + MVFuncName + "(";
    std::list<std::string>::iterator Op;
    int i = 0;
    for (Op = MVArgs.begin(); Op != MVArgs.end(); ++Op) {
      FullFunc += (i++ == (MVArgs.size() - 1)) ? (*Op + ")") : (*Op + ", ");
    }
    return FullFunc;
  }
}

// ---------------------------------------------
SIMDGenerator::SIMDInfo SIMDGenerator::computeSIMDCost(SIMDInstListType S) {
  std::map<std::string, long> CostOp;
  std::map<std::string, long> NumOp;
  std::list<std::string> L;
  long TotCost = 0;
  for (SIMDInst I : S) {
    CostOp[I.FuncName] = I.Cost;
    NumOp[I.FuncName]++;
    TotCost += I.Cost;
  }
  SIMDInfo SI(S, CostOp, NumOp, TotCost);
  return SI;
}

// ---------------------------------------------
std::list<std::string> SIMDGenerator::renderSIMDRegister(SIMDInstListType S) {
  std::list<std::string> L;
  // Render register declarations
  for (auto It = RegDeclared.begin(); It != RegDeclared.end(); ++It) {
    std::string TypeRegDecl = It->first + " ";
    int i = 0;
    for (auto N = It->second.begin(); N != It->second.end(); ++N) {
      TypeRegDecl += (i++ == (It->second.size() - 1)) ? *N : (*N + ", ");
    }
    L.push_back(TypeRegDecl + ";");
  }
  return L;
}

// ---------------------------------------------
std::list<std::string> SIMDGenerator::renderSIMDasString(SIMDInstListType S) {
  std::list<std::string> L;
  // Render instructions
  for (SIMDInst I : S) {
    std::string Inst = I.render() + ";\t// cost = " + std::to_string(I.Cost);
    L.push_back(Inst);
  }
  return L;
}

// ---------------------------------------------
bool equalValues(int VL, Node **N) {
  for (int n = 1; n < VL; ++n) {
    if (N[0]->getValue() != N[n]->getValue()) {
      return false;
    }
  }
  return true;
}

// ---------------------------------------------
bool SIMDGenerator::getSIMDVOperand(VectorIR::VOperand V,
                                    SIMDInstListType *IL) {
  SIMDInstListType TIL;
  if (V.IsLoad) {
    // Need to determine which type of memory/register load it is
    // We will say that it is a load if all the operands are contiguous and
    // need to be retrieved from memory
    //
    // We will say that it is a bcast if we need to replicate the values from
    // memory or a registers
    //
    // We will say it is a gather if we have to use an index to retrieve data
    // from memory (not contiguous)
    //
    // We will say it is a set if we have to explicity set the values of the
    // vector operand

    // FIXME
    bool EqualVal = equalValues(V.VSize, V.UOP);
    bool ContMem = V.MemOp && !(V.Shuffle & 0x0);
    bool ScatterMem = V.MemOp && !ContMem;
    bool ExpVal = !V.MemOp;

    // 0, 1, 0, 0
    if ((!EqualVal) && (ContMem) && (!ScatterMem)) {
      TIL = vpack(V);
      // 0, X, 1
    } else if ((!EqualVal) && (ScatterMem)) {
      TIL = vgather(V);
      // 1, X, 0, 1
    } else if ((EqualVal) && (!ScatterMem) && (!ExpVal)) {
      TIL = vbcast(V);
    } else {
      // 1, X, 0, 1
      TIL = vset(V);
    }

  } else {
    return false;
  }
  // Update the list
  IL->splice(IL->end(), TIL);

  return true;
}

// ---------------------------------------------
void SIMDGenerator::mapOperation(VectorIR::VectorOP V, SIMDInstListType *TI) {
  SIMDInstListType TIL;
  // Arranging the operands: maybe they need load, set, bcast...
  getSIMDVOperand(V.OpA, TI);
  getSIMDVOperand(V.OpB, TI);

  if (V.isBinOp()) {
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
  } else {
    // TODO decide what todo when we have the custom operations
  }

  // If there is a store
  if (V.R.IsStore) {
    TIL.splice(TIL.end(), vstore(V));
  }
  TI->splice(TI->end(), TIL);
}

// ---------------------------------------------
void SIMDGenerator::reduceOperation(VectorIR::VectorOP V,
                                    SIMDInstListType *TI) {
  SIMDInstListType TIL;

  // Retrieve operands
  if (V.R.Name == V.OpA.Name)
    getSIMDVOperand(V.OpB, TI);
  if (V.R.Name == V.OpB.Name)
    getSIMDVOperand(V.OpA, TI);

  // Let the magic happens
  TIL = vreduce(V);
  TI->splice(TI->end(), TIL);
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
SIMDGenerator::getSIMDfromVectorOP(VectorIR::VectorOP V) {
  SIMDInstListType IL;

  std::string RegType = getRegisterType(V.DT, V.VW);

  // Arranging the operation
  switch (V.VT) {
  case VectorIR::VType::MAP:
    Utils::printDebug("SIMDGen", "map");
    mapOperation(V, &IL);
    break;
  case VectorIR::VType::REDUCE:
    Utils::printDebug("SIMDGen", "reduce");
    reduceOperation(V, &IL);
    break;
  case VectorIR::VType::SEQ:
    Utils::printDebug("SIMDGen", "sequential");
    IL = vseq(V);
    break;
  };

  // Registers used
  addRegToDeclare(RegType, V.R.getName());
  addRegToDeclare(RegType, V.OpA.getName());
  addRegToDeclare(RegType, V.OpB.getName());

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
SIMDGenerator::getSIMDfromVectorOP(std::list<VectorIR::VectorOP> VList) {
  SIMDInstListType I;

  // Get list of SIMD instructions
  for (VectorIR::VectorOP V : VList) {
    for (SIMDInst Inst : getSIMDfromVectorOP(V)) {
      I.push_back(Inst);
    }
  }

  // Then optimizations can be done, for instance, combine operatios such as
  // addition + multiplication. It will depend on the architecture/ISA.
  I = peepholeOptimizations(I);

  return I;
}

// ---------------------------------------------
void SIMDGenerator::addRegToDeclare(std::string Type, std::string Name) {
  if (!(std::find(RegDeclared[Type].begin(), RegDeclared[Type].end(), Name) !=
        RegDeclared[Type].end())) {
    RegDeclared[Type].push_back(Name);
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
  for (auto &X : RegDeclared) {
    X.second.clear();
  }
  RegDeclared.clear();
  AccmReg = 0;
}
