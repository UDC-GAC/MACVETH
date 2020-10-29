/*
 * File	: src/Vectorization/SIMD/SIMDBackEnd.cpp
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 03:56 +02:00
 * Modified By	 : Marcos Horro (marcos.horro@udc.gal>)
 *
 * MIT License
 *
 * Copyright (c) 2020 Colorado State University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "include/Vectorization/SIMD/SIMDBackEnd.h"
#include "include/CostModel/CostTable.h"
#include "include/MVOptions.h"
#include "include/PlcmntAlgo.h"
#include "include/Utils.h"
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
void SIMDBackEnd::populateTable(MVCPUInfo::MVISA ISA) {
  auto PathISA = "/ISA/" + MVISAStr[ISA] + "/" + MVArchStr[MVOptions::Arch];
  std::string dir(__FILE__);
  dir = dir.substr(0, dir.find_last_of("\\/"));
  PathISA = dir + PathISA;
  auto Arch = MVArchStr[MVOptions::Arch];
  std::ifstream F(PathISA);
  std::string L, W;
  Utils::printDebug("SIMDBackEnd", PathISA + " path");
  if (F.is_open()) {
    while (getline(F, L)) {
      if ((L.rfind("#", 0) == 0) || (L == "")) {
        // Check if line is a comment
        continue;
      }

      //   0 ,    1     , 2 ,    3    , 4 ,  5  , 6 , 7, 8  , 9
      // MVOP,intrinsics,ASM,XED_iform,ISA,CPUID,lat,th,uops,ports

      std::stringstream TMP(L);
      std::vector<std::string> Args;
      while (getline(TMP, W, '|')) {
        Args.push_back(W);
      }

      CostTable::addRow(Args[0], Args[1], Args[2], Args[3], Args[6], Args[7],
                        Args[8], Args[9]);
    }
    F.close();
  } else {
    Utils::printDebug(
        "SIMDBackEnd",
        "This should never happen: PATH for costs file not found");
    llvm::llvm_unreachable_internal();
  }
}

// ---------------------------------------------
std::string SIMDBackEnd::getOpName(VectorIR::VOperand V, bool Ptr,
                                   bool RegVal) {
  if ((Ptr) && (V.IsStore || V.IsLoad)) {
    // Pointer to Rvalue
    return "&" + V.getRegName();
  }
  if (RegVal) {
    // Rvalue
    return V.getRegName();
  }
  // Other
  return V.Name;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInst
SIMDBackEnd::addNonSIMDInst(VectorIR::VectorOP OP, SIMDBackEnd::SIMDType SType,
                            MVSourceLocation MVSL,
                            SIMDBackEnd::SIMDInstListType *IL) {
  // Generate SIMD inst
  std::string Rhs;
  auto Lhs = OP.R.getName();

  SIMDBackEnd::SIMDInst I(Lhs, Rhs, MVSL);
  // Retrieving cost of function
  I.SType = SType;

  // Adding instruction to the list
  IL->push_back(I);
  return I;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInst
SIMDBackEnd::addNonSIMDInst(std::string Lhs, std::string Rhs,
                            SIMDBackEnd::SIMDType SType, MVSourceLocation MVSL,
                            SIMDBackEnd::SIMDInstListType *IL) {
  SIMDBackEnd::SIMDInst I(Lhs, Rhs, MVSL);

  // Retrieving cost of function
  I.SType = SType;

  // Adding instruction to the list
  IL->push_back(I);
  return I;
}

// ---------------------------------------------
std::string SIMDBackEnd::SIMDInst::render() {
  std::string FN = (MVOptions::MacroCode) ? MVFuncName : FuncName;
  std::string FullFunc = ((Result == "") || (SType == SIMDType::VSTORE))
                             ? FN
                             : Result + " = " + FN;
  if ((SType == SIMDType::VSEQ) ||
      ((Args.size() == 0) && (SType == SIMDType::VOPT))) {
    return FullFunc;
  }
  int i = 0;
  FullFunc += "(";
  for (auto Op = Args.begin(); Op != Args.end(); ++Op) {
    FullFunc += (i++ == (Args.size() - 1)) ? (*Op) : (*Op + ", ");
  }
  FullFunc += ")";
  return FullFunc;
}

// ---------------------------------------------
std::list<std::string> SIMDBackEnd::renderSIMDasString(SIMDInstListType S) {
  std::list<std::string> L;
  // Render instructions
  for (auto I : S) {
    auto Inst = I.render() + ";\t";
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
bool SIMDBackEnd::getSIMDVOperand(VectorIR::VOperand V, SIMDInstListType *IL) {
  if (!V.IsLoad) {
    return false;
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
  auto EqualVal = equalValues(V.VSize, V.UOP);
  auto ContMem = V.MemOp && (V.Contiguous);
  auto ScatterMem = V.MemOp && !ContMem;
  auto ExpVal = !V.MemOp;

  auto NullIndex = false;
  if (V.Idx.size() > 0) {
    auto I0 = V.Idx[0];
    for (int i = 1; i < V.VSize; ++i) {
      if (V.Idx[i] == I0) {
        NullIndex = true;
        break;
      }
    }
  }

  Utils::printDebug("SIMDBackEnd",
                    "V = " + V.Name +
                        "; EqualVal = " + std::to_string(EqualVal) +
                        "; ContMem = " + std::to_string(ContMem) +
                        "; ScatterMem = " + std::to_string(ScatterMem) +
                        "; SameVec = " + std::to_string(V.SameVector) +
                        "; ExpVal = " + std::to_string(ExpVal) +
                        "; NullIndex = " + std::to_string(NullIndex));

  // Load contiguous from memory
  if ((!EqualVal) && (ContMem) && (!ScatterMem)) {
    TIL = vpack(V);
    // Load from memory but not contiguous
  } else if ((!EqualVal) && (ScatterMem) && (V.SameVector) && (!NullIndex)) {
    TIL = vgather(V);
    // Replicate value
  } else if ((EqualVal) && (!ScatterMem) && (!ExpVal)) {
    TIL = vbcast(V);
  } else {
    // Set values directly
    TIL = vset(V);
  }

  // Update the list
  IL->splice(IL->end(), TIL);

  return true;
}

// ---------------------------------------------
void SIMDBackEnd::mapOperation(VectorIR::VectorOP V, SIMDInstListType *TI) {
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
    default:
      TIL = vseq(V);
    }
  } else {
    // TODO decide what todo when we have the custom operations
    Utils::printDebug("SIMDBackEnd", "it is not a binary operation");
    TIL = vfunc(V);
  }

  // If there is a store
  if (V.R.IsStore) {
    if (V.R.Contiguous) {
      TIL.splice(TIL.end(), vstore(V));
    } else {
      TIL.splice(TIL.end(), vscatter(V));
    }
  }
  TI->splice(TI->end(), TIL);
}

// ---------------------------------------------
void SIMDBackEnd::reduceOperation(VectorIR::VectorOP V, SIMDInstListType *TI) {
  SIMDInstListType TIL;

  // Retrieve operands
  if (V.R.Name == V.OpA.Name) {
    getSIMDVOperand(V.OpB, TI);
  }
  if (V.R.Name == V.OpB.Name) {
    getSIMDVOperand(V.OpA, TI);
  }

  // Let the magic happens
  TIL = vreduce(V);
  TI->splice(TI->end(), TIL);
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
SIMDBackEnd::getSIMDfromVectorOP(VectorIR::VectorOP V) {
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
  };

  auto RegType = getRegisterType(V.DT, V.VW);
  // Registers used
  addRegToDeclare(RegType, V.R.getName());
  addRegToDeclare(RegType, V.OpA.getName());
  addRegToDeclare(RegType, V.OpB.getName());

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
SIMDBackEnd::getSIMDfromVectorOP(std::list<VectorIR::VectorOP> VList) {
  SIMDInstListType IL;

  // Get list of SIMD instructions
  for (VectorIR::VectorOP V : VList) {
    IL.splice(IL.end(), getSIMDfromVectorOP(V));
  }

  // Then optimizations can be done, for instance, combine operatios such as
  // addition + multiplication. It will depend on the architecture/ISA.
  IL = peepholeOptimizations(IL);

  return IL;
}

// ---------------------------------------------
void SIMDBackEnd::addRegToDeclare(std::string Type, std::string Name,
                                  int InitVal) {
  for (auto V : RegDeclared[Type]) {
    if (std::get<0>(V) == Name) {
      return;
    }
  }
  std::vector<std::string> L;
  L.push_back(std::to_string(InitVal));
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
  for (int i = 1; i < L.size(); ++i) {
    R += "," + L[i];
  }
  return R + ")";
}

// ---------------------------------------------
std::string SIMDBackEnd::replacePatterns(std::string Pattern, std::string W,
                                         std::string D, std::string P,
                                         std::string S) {
  return "_mm" + W + "_" + P + Pattern + S + "_" + D;
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
