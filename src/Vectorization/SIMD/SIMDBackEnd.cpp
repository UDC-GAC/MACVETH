// MACVETH - SIMDBackEnd.cpp
//
// Copyright (c) Colorado State University. 2019-2021
// Copyright (c) Universidade da Coruña. 2020-2021
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Authors:
//     Marcos Horro <marcos.horro@udc.es>
//     Louis-Noël Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Marcos Horro <marcos.horro@udc.es>

#include "include/Vectorization/SIMD/SIMDBackEnd.h"
#include "include/CostModel/CostTable.h"
#include "include/Debug.h"
#include "include/MVOptions.h"
#include "include/PlcmntAlgo.h"
#include "include/Utils.h"
#include "clang/AST/OperationKinds.h"
#include "clang/AST/Type.h"
#include <algorithm>
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
void SIMDBackEnd::populateTable(MVCPUInfo::MVISA ISA) {
  std::string dir(__FILE__);
  dir = dir.substr(0, dir.find_last_of("\\/"));
  auto PathISA =
      dir + "/ISA/" + MVISAStr[ISA] + "/" + MVArchStr[MVOptions::Arch] + ".mvi";
  std::ifstream F(PathISA);
  assert(!F.fail() && "File does not exist for SIMDBackend");
  if (F.is_open()) {
    std::string L, W;
    while (getline(F, L)) {
      if ((L.rfind("#", 0) == 0) || (L.empty())) {
        // Check if line is a comment
        continue;
      }
      //   0 ,    1     , 2 ,    3    , 4 ,  5  , 6 , 7, 8  , 9
      // MVOP,intrinsics,ASM,XED_iform,ISA,CPUID,lat,th,uops,ports
      std::vector<std::string> Args = Split<'|'>::split(L);
      CostTable::addRow(Args[0], Args[1], Args[2], Args[3], Args[6], Args[7],
                        Args[8], Args[9]);
    }
    F.close();
    return;
  }
}

// ---------------------------------------------
std::string SIMDBackEnd::getOpName(const VOperand &V, bool Ptr, bool RegVal,
                                   int Position, int Offset) {
  if ((Ptr) && (V.IsStore || V.IsLoad))
    return "&" + V.getRegName(Position, Offset);
  if (RegVal)
    return V.getRegName(Position, Offset);
  return V.getName();
}

// ---------------------------------------------
SIMDBackEnd::SIMDInst
SIMDBackEnd::addNonSIMDInst(VectorOP &OP, SIMDBackEnd::SIMDType SType,
                            MVSourceLocation MVSL,
                            SIMDBackEnd::SIMDInstListType *IL) {
  auto Lhs = OP.getResult().getName();
  SIMDBackEnd::SIMDInst I(Lhs, "", MVSL);
  I.SType = SType;
  IL->push_back(I);
  return I;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInst
SIMDBackEnd::addNonSIMDInst(std::string Lhs, std::string Rhs,
                            SIMDBackEnd::SIMDType SType, MVSourceLocation MVSL,
                            SIMDBackEnd::SIMDInstListType *IL) {
  SIMDBackEnd::SIMDInst I(Lhs, Rhs, MVSL);
  I.SType = SType;
  IL->push_back(I);
  return I;
}

// ---------------------------------------------
std::string SIMDBackEnd::SIMDInst::render() {
  const std::string FN = (MVOptions::MacroCode) ? MVFuncName : FuncName;
  if ((SType == SIMDType::VTEMPLATE)) {
    return FN;
  }
  std::string FullFunc = ((Result.empty()) || (SType == SIMDType::VSTORE) ||
                          (SType == SIMDType::VSCATTER))
                             ? FN
                             : Result + " = " + FN;
  if ((SType == SIMDType::VSEQ) ||
      ((Args.size() == 0) && (SType == SIMDType::VOPT))) {
    return FullFunc;
  }
  size_t i = 0;
  FullFunc += "(";
  for (auto Op = Args.begin(); Op != Args.end(); ++Op) {
    FullFunc += (i++ == (Args.size() - 1)) ? (*Op) : (*Op + ", ");
  }
  FullFunc += ")";
  return FullFunc;
}

// ---------------------------------------------
std::vector<std::string> SIMDBackEnd::renderSIMDasString(SIMDInstListType &S) {
  std::vector<std::string> L;
  std::for_each(S.begin(), S.end(),
                [&L](auto I) { L.push_back(I.render() + ";\t"); });
  return L;
}

// ---------------------------------------------
bool equalValues(int VL, NodeVectorT N) {
  for (int n = 1; n < VL; ++n) {
    if (N[0]->getRegisterValue() != N[n]->getRegisterValue()) {
      return false;
    }
  }
  return true;
}

// ---------------------------------------------
bool SIMDBackEnd::getSIMDVOperand(VOperand V, SIMDInstListType *IL,
                                  bool Reduction) {
  MACVETH_DEBUG("SIMDBackEnd",
                "V = " + V.getName() + "; load = " + std::to_string(V.IsLoad) +
                    "; partial = " + std::to_string(V.IsPartial) +
                    "; regpack = " + std::to_string(V.RequiresRegisterPacking));
  if (((!V.IsLoad) && (!V.RequiresRegisterPacking) && (!V.IsPartial)) ||
      ((!V.MemOp) && (V.IsPartial) && (V.SameVector) && (Reduction))) {
    return false;
  }

  if ((V.RequiresRegisterPacking) ||
      //((!V.MemOp) && (!V.IsLoad) && (V.IsPartial) && (V.SameVector))) {
      ((!V.MemOp) && (V.IsPartial) && (V.SameVector))) {
    IL->splice(IL->end(), vregisterpacking(V));
    return true;
  }

  SIMDInstListType TIL;
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
  // We will say it is a set if we have to explicitly set the values of the
  // vector operand
  auto EqualVal = equalValues(V.VectorLength, V.UOP);
  auto ContMem = V.MemOp && (V.Contiguous);
  auto ScatterMem = V.MemOp && !ContMem;
  auto ExpVal = !V.MemOp;

  auto NullIndex = false;
  MACVETH_DEBUG("SIMDBackEnd",
                "V = " + V.getName() +
                    "; EqualVal = " + std::to_string(EqualVal) +
                    "; ContMem = " + std::to_string(ContMem) +
                    "; ScatterMem = " + std::to_string(ScatterMem) +
                    "; SameVec = " + std::to_string(V.SameVector) +
                    "; ExpVal = " + std::to_string(ExpVal) +
                    "; NullIndex = " + std::to_string(NullIndex));

  if ((!EqualVal) && (ContMem) && (!ScatterMem)) {
    // Load contiguous from memory
    TIL = vload(V);
  } else if ((!EqualVal) && (ScatterMem) && (V.SameVector) && (!NullIndex)) {
    // Load from memory but not contiguous
    TIL = vpack(V);
  } else if ((EqualVal) && (!ScatterMem) && (!ExpVal)) {
    // Replicate value
    TIL = vbcast(V);
  } else {
    // Set values directly
    TIL = vset(V);
  }
  IL->splice(IL->end(), TIL);

  return true;
}

// ---------------------------------------------
void SIMDBackEnd::mapOperation(VectorOP &V, SIMDInstListType *TI) {
  SIMDInstListType TIL;

  // Special case:
  // if (V.getResult().IsStore) {
  //   if ((!V.getResult().Contiguous) && (V.getOpB().RequiresRegisterPacking))
  //   {
  //     TI->splice(TI->end(), singleElementScatterOp(V));
  //     return;
  //   }
  // }

  // Arranging the operands: maybe they need load, set, bcast...
  getSIMDVOperand(V.getOpA(), TI);
  getSIMDVOperand(V.getOpB(), TI);

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
    default:
      TIL = vseq(V);
    }
  } else {
    MACVETH_DEBUG("SIMDBackEnd", "it is not a binary operation");
    TIL = vfunc(V);
  }

  // If there is a store
  if (V.getResult().IsStore) {
    if (V.getResult().Contiguous) {
      TIL.splice(TIL.end(), vstore(V));
    } else {
      TIL.splice(TIL.end(), vscatter(V));
    }
  }
  TI->splice(TI->end(), TIL);
}

// ---------------------------------------------
void SIMDBackEnd::reduceOperation(VectorOP &V, SIMDInstListType *TI) {
  MACVETH_DEBUG("SIMDBackend", "reduction: R = " + V.getResult().getName() +
                                   "; B = " + V.getOpA().getName());
  getSIMDVOperand(V.getOpA(), TI);
  TI->splice(TI->end(), vreduce(V));
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType SIMDBackEnd::getSIMDfromVectorOP(VectorOP &V) {
  SIMDInstListType IL;

  // Arranging the operation
  switch (V.VT) {
  case VectorIR::VType::MAP:
    mapOperation(V, &IL);
    break;
  case VectorIR::VType::REDUCE:
    reduceOperation(V, &IL);
    break;
  case VectorIR::VType::SEQ:
    return vseq(V);
  case VectorIR::VType::SCATTER:
    return singleElementScatterOp(V);
  case VectorIR::VType::INDUCTION:
    MVErr("Induction not supported yet!!!");
  };

  auto RegType = getRegisterType(V.DT, V.VW);
  addRegToDeclare(RegType, V.getResult().getName());
  addRegToDeclare(RegType, V.getOpA().getName());
  if (!V.IsUnary)
    addRegToDeclare(RegType, V.getOpB().getName());

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
SIMDBackEnd::getSIMDfromVectorOP(const VectorOPListT &VList) {
  SIMDInstListType IL;

  // Get list of SIMD instructions
  std::for_each(VList.begin(), VList.end(), [&IL, this](auto V) {
    IL.splice(IL.end(), getSIMDfromVectorOP(V));
  });

  // Then optimizations can be done, for instance, combine operatios such as
  // addition + multiplication. It will depend on the architecture/ISA.
  return peepholeOptimizations(IL);
}

// ---------------------------------------------
void SIMDBackEnd::addRegToDeclare(std::string Type, std::string Name,
                                  int InitVal) {
  for (auto V : RegDeclared[Type]) {
    if (std::get<0>(V) == Name) {
      return;
    }
  }
  std::vector<std::string> L{std::to_string(InitVal)};
  RegDeclared[Type].push_back(std::make_tuple(Name, L));
}

// ---------------------------------------------
void SIMDBackEnd::addRegToDeclareInitVal(std::string Type, std::string Name,
                                         std::vector<std::string> InitVal) {
  for (auto V : RegDeclared[Type]) {
    if (std::get<0>(V) == Name) {
      return;
    }
  }
  RegDeclared[Type].push_back(std::make_tuple(Name, InitVal));
}

// ---------------------------------------------
std::string SIMDBackEnd::genGenericFunc(std::string F,
                                        std::vector<std::string> L) {
  auto R = F + "(";
  if (L.size() == 0) {
    return R + ")";
  }
  R += L[0];
  for (size_t i = 1; i < L.size(); ++i) {
    R += "," + L[i];
  }
  return R + ")";
}

// ---------------------------------------------
std::string SIMDBackEnd::replacePatterns(const std::string &Pattern,
                                         const std::string &Width,
                                         const std::string &DataSuffix,
                                         const std::string &Prefix,
                                         const std::string &Suffix) {
  return "_mm" + Width + "_" + Prefix + Pattern + Suffix + "_" + DataSuffix;
}

// ---------------------------------------------
void SIMDBackEnd::clearMappings() {
  for (auto &X : RegDeclared) {
    X.second.clear();
  }
  RegDeclared.clear();
  AccmToReg.clear();
  AuxArrayReg = 0;
  AccmReg = 0;
  AuxRegId = 0;
  AuxReg.clear();
  InitReg.clear();
}
