// MACVETH - AVX2BackEnd.cpp
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

#include "include/Vectorization/SIMD/AVX2/AVX2BackEnd.h"
#include "include/CostModel/CostTable.h"
#include "include/Debug.h"
#include "include/MVExpr/MVExprArray.h"
#include "include/Utils.h"
#include <algorithm>
#include <map>
#include <regex>
#include <unordered_set>
#include <utility>

using namespace macveth;

// ---------------------------------------------
std::map<std::string, std::string> OpEq = {
    {"+", "add"},
    {"-", "sub"},
    {"*", "mul"},
    {"/", "div"},
};

// ---------------------------------------------
VectorIR::VOperand getSubVOperand(const VectorIR::VOperand &V, std::string Name,
                                  int VSize, int Size, MVDataType::VWidth Width,
                                  MVDataType::VDataType DType, int Offset) {
  VectorIR::VOperand NewVOp = V;
  NewVOp.Name = Name;
  NewVOp.Size = Size;
  NewVOp.VSize = VSize;
  NewVOp.Width = Width;
  NewVOp.DType = DType;
  NewVOp.IsPartial = (Size != VSize);
  NewVOp.LowBits = (Offset % 4 == 0);
  NewVOp.HighBits = !NewVOp.LowBits;
  NewVOp.VSize = (NewVOp.Size > 4) ? 8 : 4;
  for (int Idx = 0; Idx < Size; ++Idx) {
    NewVOp.UOP[Idx] = V.UOP[Idx + Offset];
    NewVOp.Idx[Idx] = V.Idx[Idx + Offset];
  }
  return NewVOp;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInst createSIMDInst(std::string Op, std::string Res,
                                     std::string Width, std::string DataType,
                                     std::string PrefS, std::string SuffS,
                                     MVStrVector Args,
                                     SIMDBackEnd::SIMDType SType,
                                     MVSourceLocation MVSL) {
  Op = OpEq[Op] != "" ? OpEq[Op] : Op;

  // Replace fills in pattern
  auto AVXFunc =
      SIMDBackEnd::replacePatterns(Op, Width, DataType, PrefS, SuffS);
  // Generate SIMD inst
  SIMDBackEnd::SIMDInst I(Res, AVXFunc, Args, "", {}, MVSL);
  I.SType = SType;

  return I;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInst AVX2BackEnd::genMultAccOp(SIMDBackEnd::SIMDInst Mul,
                                                SIMDBackEnd::SIMDInst Acc) {
  SIMDBackEnd::SIMDInst Fuse;

  auto Res = Acc.getResultValue();
  auto Width = getMapWidth(Mul.W);
  auto DataType = getMapType(Mul.DT);
  auto Op = (Acc.isAddition()) ? "fmadd" : "fmsub";
  MVStrVector Args;

  // Format is: (a * b) + c
  for (auto OP : Mul.Args) {
    Args.push_back(OP);
  }
  for (auto OP : Acc.Args) {
    if (OP != Mul.Result) {
      Args.push_back(OP);
    }
  }
  // Adding SIMD inst to the list
  Fuse = createSIMDInst(Op, Res, Width, DataType, "", "", Args,
                        SIMDBackEnd::SIMDType::VOPT, Acc.MVSL);
  Fuse.VOPResult = Acc.VOPResult;
  Fuse.MVSL = Acc.MVSL;

  return Fuse;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::fuseAddSubMult(SIMDBackEnd::SIMDInstListType I) {
  SIMDBackEnd::SIMDInstListType IL;
  std::map<MVDataType::VWidth, SIMDBackEnd::SIMDInstListType> PotentialFuse;
  SIMDBackEnd::SIMDInstListType SkipList;
  std::map<SIMDBackEnd::SIMDInst, SIMDBackEnd::SIMDInst> Fuses;
  // Search replacements
  for (auto Inst : I) {
    // If we find a multiply operation it may be a FMADD
    if ((Inst.SType == SIMDBackEnd::SIMDType::VMUL) &&
        ((Inst.DT == MVDataType::VDataType::FLOAT) ||
         (Inst.DT == MVDataType::VDataType::DOUBLE))) {
      PotentialFuse[Inst.W].push_back(Inst);
      continue;
    }
    // If after the multiplication, there is a store, then this is not a
    // potential fuse anymore.
    if (Inst.SType == SIMDBackEnd::SIMDType::VSTORE) {
      SIMDBackEnd::SIMDInstListType AuxL;
      for (auto P : PotentialFuse[Inst.W]) {
        if (Inst.Result != P.Result) {
          AuxL.push_back(P);
        }
      }
      PotentialFuse[Inst.W] = AuxL;
      continue;
    }

    // Check liveness
    SIMDBackEnd::SIMDInstListType AuxL;
    for (auto P : PotentialFuse[Inst.W]) {
      auto Live = VectorIR::LiveOut[P.Result];
      if (Live <= (int)Inst.MVSL.getOrder()) {
        AuxL.push_back(P);
      }
    }
    PotentialFuse[Inst.W] = AuxL;

    // Check if we have any add/sub adding the result of a previous
    // multiplication
    if ((Inst.isAddition()) || (Inst.SType == SIMDBackEnd::SIMDType::VSUB)) {
      for (auto P : PotentialFuse[Inst.W]) {
        if (std::find(Inst.Args.begin(), Inst.Args.end(), P.Result) !=
            Inst.Args.end()) {
          auto NewFuse = AVX2BackEnd::genMultAccOp(P, Inst);
          // We will like to skip this function later
          SkipList.push_back(P);
          // We will want to replace the add/sub by this
          Fuses[Inst] = NewFuse;
        }
      }
    }
  }

  // Perform replacements if any
  for (auto Inst : I) {
    // Skip instruction because it was fused
    if (std::find(SkipList.begin(), SkipList.end(), Inst) != SkipList.end()) {
      continue;
    }
    // Find if this Instruction is to be fused, then put the instruction fused
    if (Fuses.count(Inst) > 0) {
      IL.push_back(Fuses.at(Inst));
      continue;
    }
    IL.push_back(Inst);
  }
  return IL;
}

// ---------------------------------------------
std::string AVX2BackEnd::declareAuxArray(MVDataType::VDataType DT) {
  return getNextArrRegister(MVDataType::VDTypeToCType[DT],
                            getMaxWidth() / MVDataType::VDataTypeWidthBits[DT]);
}

using ReductionsMap = std::map<std::string, std::vector<int>>;

// ---------------------------------------------
ReductionsMap getReductionsMapFromVector(VectorIR::VOperand V) {
  ReductionsMap Map;
  // if (V.Idx.size() == 0) {
  std::string PrevVal = V.UOP[0]->getRegisterValue();
  Map[PrevVal].push_back(0);
  MACVETH_DEBUG("AVX2Backend", "getReductionsMapFromVector. VSize = " +
                                   std::to_string(V.VSize) +
                                   "; Size = " + std::to_string(V.Size));
  for (int T = 1; T < (int)V.VSize; ++T) {
    std::string Val = V.UOP[T]->getRegisterValue();
    Map[Val].push_back(T);
    MACVETH_DEBUG("AVX2Backend", "getReductionsMapFromVector = " + Val +
                                     "; idx = " + std::to_string(T));
  }
  return Map;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::reduceOneRegister(VectorIR::VOperand V, std::string OpName,
                               MVSourceLocation MVSL) {
  MACVETH_DEBUG("AVX2Backend", "reduceOneRegister");
  auto Type = V.DType;
  auto AccmReg = getAccmReg(V.getName());
  auto RegType = getRegisterType(Type, MVDataType::VWidth::W128);
  auto NElems = V.VSize;
  std::string LoRes = "__mv_lo128";
  std::string HiRes = "__mv_hi128";
  // MVSourceLocation MVSL(Pos, V.Order, V.Offset);
  SIMDBackEnd::SIMDInstListType IL;
  // if (Type == MVDataType::VDataType::DOUBLE) {
  //   // If 4 elements
  //   if (NElems > 2) {
  //     // __m128d lo = _mm256_castpd256_pd128(v); zero latency
  //     genSIMDInst(LoRes, "castpd", "256", "pd128", MVDataType::VWidth::W256,
  //                 Type, {AccmReg}, SIMDType::VOPT, MVSL, &IL);
  //     // __m128d hi = _mm256_extractf128_pd(v,0x1);
  //     genSIMDInst(HiRes, "extract", "", "f128", MVDataType::VWidth::W256,
  //     Type,
  //                 {AccmReg, "0x1"}, SIMDType::VOPT, MVSL, &IL);
  //     // lo = op(lo,hi);
  //     genSIMDInst(LoRes, OpName, "", "", MVDataType::VWidth::W128, Type,
  //                 {LoRes, HiRes}, SIMDType::VOPT, MVSL, &IL);
  //   } else {
  //     LoRes = AccmReg;
  //   }
  //   // When only 2 elements
  //   // hi = _mm_shuffle_pd(lo,lo,0x1);
  //   genSIMDInst(HiRes, "shuffle", "", "", MVDataType::VWidth::W128, Type,
  //               {LoRes, LoRes, "0x1"}, SIMDType::VPERM, MVSL, &IL);
  //   // val = _mm_cvtsd_f64(op(lo,hi));
  //   genSIMDInst(LoRes, OpName, "", "", MVDataType::VWidth::W128, Type,
  //               {LoRes, HiRes}, SIMDType::VOPT, MVSL, &IL);
  // } else
  if (Type == MVDataType::VDataType::FLOAT) {
    // 8 elements
    if (NElems > 4) {
      // __m128 lo = cast()
      genSIMDInst(LoRes, "castps", "256", "ps128", MVDataType::VWidth::W256,
                  Type, {AccmReg}, SIMDType::VOPT, MVSL, &IL);
      // __m128 lo = cast(v)
      genSIMDInst(HiRes, "extract", "", "f128", MVDataType::VWidth::W256, Type,
                  {AccmReg, "0x1"}, SIMDType::VOPT, MVSL, &IL);

      // lo = op(lo, hi);
      genSIMDInst(LoRes, OpName, "", "", MVDataType::VWidth::W128, Type,
                  {LoRes, HiRes}, SIMDType::VOPT, MVSL, &IL);
    } else {
      LoRes = AccmReg;
    }
    if (NElems > 2) {
      // 4 elements
      // hi = movehl(lo);
      genSIMDInst(HiRes, "move", "", "hl", MVDataType::VWidth::W128, Type,
                  {LoRes, LoRes}, SIMDType::VOPT, MVSL, &IL);
      // lo = op(lo, hi);
      genSIMDInst(LoRes, OpName, "", "", MVDataType::VWidth::W128, Type,
                  {LoRes, HiRes}, SIMDType::VOPT, MVSL, &IL);
    }
    // 2 elements
    // hi = shuffle(lo, lo, mask);
    genSIMDInst(HiRes, "shuffle", "", "", MVDataType::VWidth::W128, Type,
                {LoRes, LoRes, "0b00000001"}, SIMDType::VPERM, MVSL, &IL);
    // lo = op(lo,hi);
    genSIMDInst("__mv_lo128", OpName, "", "", MVDataType::VWidth::W128, Type,
                {LoRes, HiRes}, SIMDType::VOPT, MVSL, &IL);

  } else {
    // FIXME: should we implement this for integer arithmetic?
    MVErr("Not implemented this approach yet for other types than float");
  }
  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::reduceMultipleValuesInRegisterSymmetric(VectorIR::VOperand V,
                                                     std::string OpName,
                                                     MVSourceLocation MVSL) {
  MACVETH_DEBUG("AVX2Backend", "reduceMultipleValuesInRegisterSymmetric");
  SIMDBackEnd::SIMDInstListType IL;
  auto Type = V.DType;
  auto AccmReg = getAccmReg(V.getName());
  auto NElems = V.VSize;
  auto RegType = getRegisterType(Type, MVDataType::VWidth::W256);
  std::string LoRes = "__mv_lo256";
  std::string HiRes = "__mv_hi256";
  SIMDBackEnd::addRegToDeclare(RegType, LoRes, {0});
  SIMDBackEnd::addRegToDeclare(RegType, HiRes, {0});
  // MVSourceLocation MVSL(Pos, V.Order, V.Offset);
  if (Type == MVDataType::VDataType::FLOAT) {
    // 8 elements
    if (NElems > 4) {
      auto TmpReg0 = "__tmp0_256";
      auto TmpReg1 = "__tmp1_256";
      auto TmpReg2 = "__tmp2_256";
      SIMDBackEnd::addRegToDeclare(RegType, TmpReg0, {0});
      SIMDBackEnd::addRegToDeclare(RegType, TmpReg1, {0});
      SIMDBackEnd::addRegToDeclare(RegType, TmpReg2, {0});
      genSIMDInst(TmpReg0, "permute", "", "", MVDataType::VWidth::W256, Type,
                  {AccmReg, "0b00001110"}, SIMDType::VPERM, MVSL, &IL);
      genSIMDInst(TmpReg1, "add", "", "", MVDataType::VWidth::W256, Type,
                  {AccmReg, TmpReg0}, SIMDType::VADD, MVSL, &IL);
      genSIMDInst(TmpReg2, "shuffle", "", "", MVDataType::VWidth::W256, Type,
                  {TmpReg1, TmpReg1, "0x01"}, SIMDType::VPERM, MVSL, &IL);
      genSIMDInst(LoRes, "add", "", "", MVDataType::VWidth::W256, Type,
                  {TmpReg1, TmpReg2}, SIMDType::VADD, MVSL, &IL);
    } else if (NElems > 2) {
      RegType = getRegisterType(Type, MVDataType::VWidth::W128);
      auto TmpReg0 = "__tmp0_128";
      auto TmpReg1 = "__tmp1_128";
      LoRes = "__mv_lo128";
      SIMDBackEnd::addRegToDeclare(RegType, TmpReg0, {0});
      SIMDBackEnd::addRegToDeclare(RegType, TmpReg1, {0});
      std::string PermuteMask = "0b0000110001";
      genSIMDInst(TmpReg0, "permute", "", "", MVDataType::VWidth::W128, Type,
                  {AccmReg, PermuteMask}, SIMDType::VPERM, MVSL, &IL);
      genSIMDInst(LoRes, "add", "", "", MVDataType::VWidth::W128, Type,
                  {AccmReg, TmpReg0}, SIMDType::VADD, MVSL, &IL);
    }
  }
  // else {
  //   if (NElems > 2) {
  //     auto TmpReg0 = "__tmp0_256";
  //     auto TmpReg1 = "__tmp1_256";
  //     LoRes = "__tmp1_256";
  //     SIMDBackEnd::addRegToDeclare(RegType, TmpReg0, {0});
  //     SIMDBackEnd::addRegToDeclare(RegType, TmpReg1, {0});
  //     genSIMDInst(TmpReg0, "permute", "", "2f128", MVDataType::VWidth::W256,
  //                 Type, {AccmReg, AccmReg, "0b10000001"}, SIMDType::VPERM,
  //                 MVSL, &IL);
  //     genSIMDInst(TmpReg1, "add", "", "", MVDataType::VWidth::W256, Type,
  //                 {AccmReg, TmpReg0}, SIMDType::VADD, MVSL, &IL);
  //   }
  // }
  return IL;
}

// ---------------------------------------------
bool getSymmetry(ReductionsMap ReduxMap) {
  if (ReduxMap.size() == 1) {
    return true;
  }
  int Size = -1;
  for (auto K : ReduxMap) {
    auto RVector = K.second;
    // Check if elements to reduce are contiguous or not
    for (int i = 1; i < (int)RVector.size(); ++i) {
      if (RVector[i] - 1 != RVector[i - 1]) {
        MACVETH_DEBUG("AVX2Backend", "getSymmetry = false");
        return false;
      }
    }
    if (Size == -1) {
      Size = RVector.size();
    }
    if ((Size != (int)RVector.size())) {
      MACVETH_DEBUG("AVX2Backend", "getSymmetry = false");
      return false;
    }
  }
  MACVETH_DEBUG("AVX2Backend", "getSymmetry = false");
  return true;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::reduceMultipleValuesInRegister(
    VectorIR::VOperand V, std::string OpName, MVSourceLocation MVSL) {
  MACVETH_DEBUG("AVX2Backend", "reduceMultipleValuesInRegister");
  SIMDBackEnd::SIMDInstListType IL;
  auto AccmReg = getAccmReg(V.getName());
  auto NElems = V.VSize;
  // MVSourceLocation MVSL(Pos, V.Order, V.Offset);
  ReductionsMap ReduxMap;
  int NReductions = 0;
  bool Symmetric = true;
  bool SkipRedux = false;
  int Stride = 2;
  if (NElems > 4) {
    // Divide and conquer
    auto NameFirstHalve = "_mm256_castps256_ps128(" + AccmReg + ")";
    auto RegFirstHalve = getNextAccmRegister(NameFirstHalve);
    SIMDBackEnd::addRegToDeclareInitVal("__m128", RegFirstHalve, {});
    genSIMDInst(RegFirstHalve, "castps", "256", "ps128",
                MVDataType::VWidth::W256, MVDataType::VDataType::FLOAT,
                {AccmReg}, SIMDType::VOPT, MVSL, &IL);
    auto VFirstHalve =
        getSubVOperand(V, NameFirstHalve, 4, 4, MVDataType::VWidth::W128,
                       MVDataType::VDataType::FLOAT, 0);
    ReduxMap = getReductionsMapFromVector(VFirstHalve);
    NReductions = ReduxMap.size();
    Symmetric = getSymmetry(ReduxMap);
    if (NReductions == 1) {
      IL.splice(IL.end(), reduceOneRegister(VFirstHalve, OpName, MVSL));
    } else {
      if (Symmetric) {
        IL.splice(IL.end(), reduceMultipleValuesInRegisterSymmetric(
                                VFirstHalve, OpName, MVSL));
      } else {
        IL.splice(IL.end(),
                  reduceMultipleValuesInRegister(VFirstHalve, OpName, MVSL));
        SkipRedux = true;
      }
    }

    if (!SkipRedux) {
      Stride = (NReductions > 1) ? 2 : 4;
      for (unsigned R = 0; R < 4; R += Stride) {
        addNonSIMDInst(V.StoreValues[R],
                       V.StoreValues[R] + OpName + "__mv_lo128[" +
                           std::to_string(R) + "]",
                       SIMDType::VOPT, MVSL, &IL);
      }
    }

    SkipRedux = false;
    auto NameSecondHalve = getNextAccmRegister("__tmp_extract_128");
    auto RegSecondHalve = getNextAccmRegister(NameSecondHalve);
    SIMDBackEnd::addRegToDeclareInitVal("__m128", RegSecondHalve, {});
    genSIMDInst(RegSecondHalve, "extract", "", "f128", MVDataType::VWidth::W256,
                MVDataType::VDataType::FLOAT, {AccmReg, "0x1"}, SIMDType::VOPT,
                MVSL, &IL);
    auto VSecondHalve = getSubVOperand(V, NameSecondHalve, NElems - 4, 4,
                                       MVDataType::VWidth::W128,
                                       MVDataType::VDataType::FLOAT, 4);
    ReduxMap = getReductionsMapFromVector(VSecondHalve);
    NReductions = ReduxMap.size();
    Symmetric = getSymmetry(ReduxMap);
    if (NReductions == 1) {
      IL.splice(IL.end(), reduceOneRegister(VSecondHalve, OpName, MVSL));
    } else {
      if (Symmetric) {
        IL.splice(IL.end(), reduceMultipleValuesInRegisterSymmetric(
                                VSecondHalve, OpName, MVSL));
      } else {
        IL.splice(IL.end(),
                  reduceMultipleValuesInRegister(VSecondHalve, OpName, MVSL));
        SkipRedux = true;
      }
    }
    if (!SkipRedux) {
      Stride = (NReductions > 1) ? 2 : 4;
      for (unsigned R = 0; R < 4; R += Stride) {
        addNonSIMDInst(V.StoreValues[R + 4],
                       V.StoreValues[R + 4] + OpName + "__mv_lo128[" +
                           std::to_string(R) + "]",
                       SIMDType::VOPT, MVSL, &IL);
      }
    }

  } else {
    // FIXME:
    for (size_t N = 0; N < V.VSize; ++N) {
      addNonSIMDInst(V.UOP[N]->getRegisterValue(),
                     V.UOP[N]->getRegisterValue() + OpName + AccmReg + "[" +
                         std::to_string(N) + "]",
                     SIMDType::VOPT, MVSL, &IL);
    }
  }
  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::horizontalSingleVectorReduction(SIMDBackEnd::SIMDInstListType TIL,
                                             MVSourceLocation::Position Pos) {
  std::vector<SIMDBackEnd::SIMDInst> VIL{std::begin(TIL), std::end(TIL)};
  MACVETH_DEBUG("AVX2Backend", "horizontalSingleVectorReduction");
  auto ReduxIns = VIL[0];
  auto V = ReduxIns.VOPResult;
  auto NElems = V.VSize;
  ReductionsMap ReduxMap = getReductionsMapFromVector(V);
  MACVETH_DEBUG("AVX2Backend",
                "horizontalSingleVectorReduction. ReduxMap.size() = " +
                    std::to_string(ReduxMap.size()));
  int NReductions = ReduxMap.size();
  bool Symmetric = getSymmetry(ReduxMap);

  MACVETH_DEBUG("AVX2Backend",
                "horizontalSingleVectorReduction. Symmetry = " +
                    std::to_string(Symmetric) +
                    "; NReductions = " + std::to_string(NReductions));
  // Horizontal operations
  SIMDBackEnd::SIMDInstListType IL;
  auto OpRedux = ReduxIns.MVOP;
  auto OP = ReduxIns.MVOP.toString();
  auto AccmReg = getAccmReg(V.getName());
  // MVSourceLocation MVSL(Pos, V.Order, V.Offset);
  MVSourceLocation MVSL = VIL[0].MVSL;
  std::string LoRes = "__mv_lo128";
  std::string HiRes = "__mv_hi128";
  SIMDBackEnd::addRegToDeclare("__m128", LoRes, {0});
  SIMDBackEnd::addRegToDeclare("__m128", HiRes, {0});
  if (NReductions == 1) {
    IL = reduceOneRegister(V, OP, MVSL);
  } else {
    if (Symmetric) {
      LoRes = "__mv_lo256";
      IL = reduceMultipleValuesInRegisterSymmetric(V, OP, MVSL);
    } else {
      IL = reduceMultipleValuesInRegister(V, OP, MVSL);
    }
  }
  // Perfect cases:
  // * [A,A,B,B,C,C,D,D]
  // * [A,A,A,A,C,C,C,C]
  // * [A,A,A,A,A,A,A,A]
  if ((Symmetric && ((NReductions == 4) || (NReductions == 2))) ||
      (NReductions == 1)) {
    int Stride = 1 + ((NElems - 1) / NReductions);
    unsigned UB = NElems;
    if (NElems < 4) {
      // UB = NElems > 1 ? (NElems - 1) : NElems;
      UB = 1;
      Stride = 1;
    }
    for (unsigned R = 0; R < UB; R += Stride) {
      if (OpRedux.isBinaryOperation()) {
        addNonSIMDInst(V.StoreValues[R],
                       V.StoreValues[R] + OP + LoRes + "[" + std::to_string(R) +
                           "]",
                       SIMDType::VOPT, MVSL, &IL);
      } else {
        OP = OpRedux.getOpPrefix() + OP;
        addNonSIMDInst(V.StoreValues[R],
                       OP + "(" + V.StoreValues[R] + ", " + LoRes + "[" +
                           std::to_string(R) + "])",
                       SIMDType::VOPT, MVSL, &IL);
      }
    }
  }

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::horizontalReductionFusion(SIMDBackEnd::SIMDInstListType TIL,
                                       MVSourceLocation::Position Pos) {
  // Horizontal operations
  SIMDBackEnd::SIMDInstListType IL;
  std::vector<SIMDBackEnd::SIMDInst> VIL{std::begin(TIL), std::end(TIL)};

  int NElems = VIL[0].W / MVDataType::VDataTypeWidthBits[VIL[0].DT];
  auto OpRedux = VIL[0].MVOP;
  auto OpName = VIL[0].MVOP.toString();
  int NumRedux = TIL.size();
  if (NumRedux > NElems) {
    MVAssert(false, "You can not have more reductions than elements in vector");
  }
  auto Loc = 0;
  auto Off = 0;
  std::vector<std::string> VAccm;
  std::unordered_set<std::string> VRedux;
  std::vector<std::string> VReduxList;
  for (int t = 0; t < NumRedux; ++t) {
    VAccm.push_back(VIL[t].Args.front());
    VRedux.insert(VIL[t].Result);
    VReduxList.push_back(VIL[t].Result);
    Loc = std::max(Loc, VIL[t].VOPResult.Order);
    Off = std::max(Off, VIL[t].VOPResult.Offset);
  }

  auto OP =
      (OpRedux.ClangOP == BinaryOperator::Opcode::BO_Add) ? "hadd" : "hsub";

  auto Stride = 2;
  auto S = (NumRedux + (NumRedux % 2)) / 2;
  MVSourceLocation MVSL(Pos, Loc, Off);
  while (true) {
    for (int i = 0; i < NumRedux; i += Stride) {
      genSIMDInst(VIL[0].VOPResult, OP, "", "",
                  {VAccm[i], (NumRedux > 1) ? VAccm[i + 1] : VAccm[i]},
                  SIMDType::VOPT, MVSL, &IL, VAccm[i]);
      if (i > 0) {
        VAccm[i - 1] = VAccm[i];
      }
    }
    S = (S + (S % 2)) / 2;
    Stride *= 2;
    // Final permutation
    if ((S == 1) && (NElems > 2)) {
      // README: for further information
      // https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction
      // https://stackoverflow.com/questions/49941645/get-sum-of-values-stored-in-m256d-with-sse-avx
      auto W = MapWidth[VIL[0].W];
      // Operand 1
      auto Blend = replacePatterns("blend", W, MapType[VIL[0].DT], "", "");
      auto Mask1 =
          VIL[0].DT == MVDataType::VDataType::DOUBLE ? "0b1100" : "0b11110000";
      // Operand 2
      auto Perm =
          replacePatterns("permute", W, MapType[VIL[0].DT], "", "2f128");
      auto V0 = VAccm[0];
      auto V1 = (NumRedux > 4)   ? VAccm[4]
                : (NumRedux > 2) ? VAccm[2]
                                 : VAccm[1];
      auto Op1 = genGenericFunc(Blend, {V0, V1, Mask1});
      auto Op2 = genGenericFunc(Perm, {V0, V1, "0x21"});

      OP = (OpRedux.ClangOP == BinaryOperator::Opcode::BO_Add) ? "add" : "sub";
      genSIMDInst(VIL[0].VOPResult, OP, "", "", {Op1, Op2}, SIMDType::VOPT,
                  MVSL, &IL, VAccm[0]);
      break;
    } else if (S == 1) {
      break;
    }
  }

  auto No = 0;
  OP = (OpRedux.ClangOP == BinaryOperator::Opcode::BO_Add) ? " + " : " - ";
  std::set<std::string> Visited;
  // Storing elements and adding them to the reduction
  for (auto R : VReduxList) {
    if (Visited.find(R) != Visited.end()) {
      continue;
    }
    Visited.insert(R);
    auto Idx = "[" + std::to_string(No++) + "]";
    addNonSIMDInst(R, R + OP + VAccm[0] + Idx, SIMDType::VOPT, MVSL, &IL);
  }

  return IL;
}

// ---------------------------------------------
std::string AVX2BackEnd::shuffleArguments(std::string A1, std::string A2,
                                          MVDataType::VWidth Width,
                                          SIMDBackEnd::SIMDInst I, int Pos) {
  std::string Mask;
  if (I.DT == MVDataType::VDataType::DOUBLE) {
    Mask = (Pos == 0) ? "0x00" : "0xff";
  } else if (I.DT == MVDataType::VDataType::FLOAT) {
    Mask = (Pos == 0) ? "0x44" : "0xee";
  }

  if (Width == MVDataType::VWidth::W128) {
    Mask = (Pos == 0) ? "0b10001000" : "0b11011101";
  }

  return genGenericFunc(
      replacePatterns("shuffle", getMapWidth(Width), getMapType(I.DT), "", ""),
      {A1, A2, Mask});
}

// ---------------------------------------------
std::string AVX2BackEnd::permuteArguments(std::string A1, std::string A2,
                                          SIMDBackEnd::SIMDInst I, int Pos) {
  std::string Mask = "0x21";
  std::string PermW = getMapWidth(I.W);
  std::string PermT = getMapType(I.DT);
  return genGenericFunc(replacePatterns("permute", PermW, PermT, "", "2f128"),
                        {A1, A2, Mask});
}

// ---------------------------------------------
std::string AVX2BackEnd::extractArgument(std::string A, SIMDBackEnd::SIMDInst I,
                                         int Hi) {
  std::string Mask = (Hi == 0) ? "0x00" : "0x01";
  std::string PermW = getMapWidth(I.W);
  std::string PermT = getMapType(I.DT);
  return genGenericFunc(replacePatterns("extract", PermW, PermT, "", "f128"),
                        {A, Mask});
}

// ---------------------------------------------
MVStrVector AVX2BackEnd::renderSIMDRegister(SIMDInstListType S) {
  MVStrVector L;
  // Render register declarations
  for (auto It : RegDeclared) {
    auto TypeRegDecl = It.first + " ";
    auto VAuxRegs = It.second;
    if (std::get<0>(VAuxRegs[0]) != "") {
      TypeRegDecl += std::get<0>(VAuxRegs[0]);
    }
    for (unsigned i = 1; i < VAuxRegs.size(); ++i) {
      if (std::get<0>(VAuxRegs[i]) != "") {
        TypeRegDecl += ", " + std::get<0>(VAuxRegs[i]);
      }
    }
    L.push_back(TypeRegDecl + ";");
  }
  return L;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::generalReductionFusion(SIMDBackEnd::SIMDInstListType TIL,
                                    MVSourceLocation::Position Pos) {
  // TODO:
  // This is quite complex: it is not the same doing reductions of additions
  // subtractions than multiplications or divisions. This way, we have to
  // explicitly differentiate them. General algorithm (4 doubles) for those
  // cases:
  //          DATA IN YMM0
  //         ymm0 = _mm256_OP_pd(ymm0, _mm256_permute_pd(ymm0, 0x05));
  //         ymm1 = _mm256_OP_pd(ymm0, _mm256_permute2f128_pd(ymm0, ymm0,
  //         0x01));
  //          STORE YMM2
  // For additions and subtraction it is different: it is based on horizontal
  // operations
  SIMDBackEnd::SIMDInstListType IL;
  std::vector<SIMDInst> VIL{std::begin(TIL), std::end(TIL)};

  auto NElems = (size_t)(VIL[0].W / MVDataType::VDataTypeWidthBits[VIL[0].DT]);
  auto OpRedux = VIL[0].MVOP;
  // auto OpReduxType = OpRedux.getType();
  auto OpName = VIL[0].MVOP.toString();
  auto NumRedux = TIL.size();
  if (NumRedux > NElems) {
    MVAssert(false, "You can not have more reductions than elements in vector");
  }
  std::vector<std::string> VAccm;
  std::vector<std::string> VRedux;
  for (size_t t = 0; t < NumRedux; ++t) {
    VAccm.push_back(VIL[t].Args.front());
    VRedux.push_back(VIL[t].Result);
  }

  auto OP = OpRedux.toString();
  auto Loc = VIL[NumRedux - 1].VOPResult.Order;
  auto Off = VIL[NumRedux - 1].VOPResult.Offset;
  MVSourceLocation MVSL(Pos, Loc, Off);
  // Pseudo-algorithm:
  // -----------------
  // An = op(shuffle(OPn,mask1), shuffle(OPn+1,mask2))
  // An = op(permute2(An,An+1,mask1), permute2(An,An+1,mask1))
  // if (Nelems < 4)
  //  return An;
  // else
  //  for each i in [0, nelems)
  //    Vi = op(extract(An), shuffle(extract(An)))

  // IMPORTANT: This approach assumes NumRedux > 1

  // Shuffles
  for (size_t i = 0; i < NumRedux; i += 2) {
    auto A0 =
        shuffleArguments(VAccm[i], (NumRedux > i + 1) ? VAccm[i + 1] : VAccm[i],
                         VIL[i].W, VIL[i], 0);
    auto A1 =
        shuffleArguments(VAccm[i], (NumRedux > i + 1) ? VAccm[i + 1] : VAccm[i],
                         VIL[i].W, VIL[i], 1);
    genSIMDInst(VIL[i].VOPResult, OP, "", "", {A0, A1}, SIMDType::VOPT, MVSL,
                &IL, VAccm[i]);
    if (i > 0) {
      VAccm[i - 1] = VAccm[i];
    }
  }

  // Permutations
  for (size_t i = 0; i < NumRedux; i += 4) {
    auto W = MapWidth[VIL[0].W];
    // Operand 1
    auto Blend = replacePatterns("blend", W, MapType[VIL[0].DT], "", "");
    auto Mask =
        VIL[0].DT == MVDataType::VDataType::DOUBLE ? "0b1100" : "0b11110000";
    auto A0 = genGenericFunc(
        Blend, {VAccm[i], (NumRedux > i + 1) ? VAccm[i + 1] : VAccm[i], Mask});
    auto A1 = permuteArguments(
        VAccm[i], (NumRedux > i + 1) ? VAccm[i + 1] : VAccm[i], VIL[i], 1);
    genSIMDInst(VIL[i].VOPResult, OP, "", "", {A0, A1}, SIMDType::VOPT, MVSL,
                &IL, VAccm[i]);
    if (i > 0)
      VAccm[i - 1] = VAccm[i];
  }

  // Extract values depending if 4 or 8 elements, different approaches due to
  // intrinsics design

  std::string AuxArray = declareAuxArray(VIL[0].DT);
  std::string Res = "";

  if (NElems > 4) {
    // Need to do the last extraction, shuffle and operation
    for (unsigned R = 0; R < NumRedux; R += 4) {
      // op(shuffle(hi,lo,mask), shuffle(hi,lo,mask))
      auto Lo = extractArgument(VAccm[R], VIL[R], 0);
      auto Hi = extractArgument(VAccm[R], VIL[R], 1);
      auto A1 = shuffleArguments(Lo, Hi, MVDataType::VWidth::W128, VIL[R], 0);
      auto A2 = shuffleArguments(Lo, Hi, MVDataType::VWidth::W128, VIL[R], 1);
      auto RegType = getRegisterType(VIL[R].DT, MVDataType::VWidth::W128);
      Res = getNextAuxRegister(VIL[R].VOPResult.getName());
      SIMDBackEnd::addRegToDeclare(RegType, Res, {0});
      genSIMDInst(Res, OP, "", "", MVDataType::VWidth::W128, VIL[R].DT,
                  {A1, A2}, SIMDType::VOPT, MVSL, &IL);
      if (NumRedux > 1) {
        genSIMDInst(Res, "store", "", "u", MVDataType::VWidth::W128, VIL[R].DT,
                    {"&" + AuxArray + "[" + std::to_string(R) + "]", Res},
                    SIMDType::VSTORE, MVSL, &IL);
      }
    }
  }

  auto ResRegister = (Res != "") ? Res : VAccm[0];
  if ((NElems > 4) && (NumRedux > 1)) {
    ResRegister = AuxArray;
  }

  for (unsigned R = 0; R < NumRedux; ++R) {
    std::string Idx = "[" + std::to_string(R) + "]";
    if (OpRedux.isBinaryOperation()) {
      addNonSIMDInst(VRedux[R], VRedux[R] + OP + ResRegister + Idx,
                     SIMDType::VSEQ, MVSL, &IL);
    } else {
      OP = OpRedux.getOpPrefix() + OP;
      addNonSIMDInst(VRedux[R],
                     OP + "(" + VRedux[R] + ", " + ResRegister + Idx + ")",
                     SIMDType::VSEQ, MVSL, &IL);
    }
  }
  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::fuseReductionsList(SIMDBackEnd::SIMDInstListType L) {
  // For each reduction found, check whether they can be fused together or
  // not:
  // they can be fused if and only if they are in the same loop dimension/name
  // and the same loop, basically (because they can have same name but be
  // different loops). This is important for coherence in the program
  SIMDBackEnd::SIMDInstListType LC;
  std::string Accm = "";
  for (auto I : L) {
    if (I.Args.front() != Accm) {
      Accm = I.Args.front();
      LC.push_back(I);
    }
  }

  auto LInsSize = LC.size();
  SIMDBackEnd::SIMDInstListType FusedRedux;
  if (LInsSize == 0) {
    return FusedRedux;
  }

  auto OpRedux = LC.back().MVOP;
  auto InitPos = MVSourceLocation::Position::PREORDER;
  auto ReduxPos = MVSourceLocation::Position::POSORDER;

  // This is the horizontal approach, only valid for AVX2 and additions
  // and subtraction
  if (LInsSize == 1) {
    MACVETH_DEBUG("AVX2BackEnd", "Single reduction approach (" +
                                     std::to_string(LInsSize) + ")");
    FusedRedux = horizontalSingleVectorReduction(LC, ReduxPos);
  } else if (((OpRedux.isBinaryOperation()) && (OpRedux.isAddOrSub()))) {
    // Horizontal approach only worth it when we have two or more
    // reductions
    MACVETH_DEBUG("AVX2BackEnd",
                  "Horizontal approach (" + std::to_string(LInsSize) + ")");
    FusedRedux = horizontalReductionFusion(LC, ReduxPos);
  } else {
    MACVETH_DEBUG("AVX2BackEnd", "General approach approach (" +
                                     std::to_string(LInsSize) + ")");
    FusedRedux = generalReductionFusion(LC, ReduxPos);
  }

  // Clean accumulators
  for (auto R : LC) {
    auto Accm = getAccmReg(R.VOPResult.getName());
    if (!isAccmClean(Accm)) {
      MVSourceLocation MVSL(InitPos, R.MVSL.getOrder(), R.MVSL.getOffset());
      // genSIMDInst(Accm, "setzero", "", "", R.W, R.DT, {}, SIMDType::VSET,
      // MVSL,
      //            &FusedRedux);
    }
  }

  /// Mark accumulators
  for (auto R : LC) {
    auto Accm = getAccmReg(R.VOPResult.getName());
    markDirtyAccm(Accm);
  }

  return FusedRedux;
}

// ---------------------------------------------
bool AVX2BackEnd::hasRawDependencies(SIMDBackEnd::SIMDInstListType L,
                                     SIMDBackEnd::SIMDInst I) {
  auto Last = I.VOPResult.Order;
  auto RAW = getCDAG().getRAWs();
  for (auto Ins : L) {
    auto Order = Ins.VOPResult.Order;
    auto It = RAW.find(Order);
    if (It != RAW.end()) {
      auto S = It->second;
      if (Last > *(S.begin())) {
        return true;
      }
    }
  }
  return false;
}

// ---------------------------------------------
bool AVX2BackEnd::reductionIsContiguous(SIMDBackEnd::SIMDInstListType L,
                                        SIMDBackEnd::SIMDInst I) {
  auto Last = I.VOPResult.Order;
  auto LastAccm = I.getResultValue();
  auto RAW = getCDAG().getRAWs();

  MACVETH_DEBUG("AVX2Backend", "reductionIsContiguous: i.result = " + LastAccm +
                                   "; size L = " + std::to_string(L.size()));
  for (auto Ins : L) {
    MACVETH_DEBUG("AVX2Backend",
                  "ins.result = " + Ins.Result + "; last = " + LastAccm);
    auto Order = Ins.VOPResult.Order;
    auto Accm = Ins.Result;
    auto It = RAW.find(Order);
    if (It != RAW.end()) {
      auto S = It->second;
      if (Last == *(S.rbegin()) && (Accm == LastAccm)) {
        return true;
      }
    }
  }
  return false;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::fuseReductions(SIMDBackEnd::SIMDInstListType TIL) {
  SIMDBackEnd::SIMDInstListType IL;
  SIMDBackEnd::SIMDInstListType SkipList;
  // List of reduction candidates to be fused
  std::map<int, SIMDBackEnd::SIMDInstListType> LRedux;
  std::map<SIMDBackEnd::SIMDInst, SIMDBackEnd::SIMDInstListType>
      ReplaceFusedRedux;
  MACVETH_DEBUG("AVX2BackEnd", "=== FUSE REDUCTIONS ===");
  for (auto I : TIL) {
    if (I.isReduction()) {
      auto S = I.VOPResult.VSize;
      auto L = LRedux[S];
      MACVETH_DEBUG("AVX2BackEnd",
                    "Size = " + std::to_string(S) + "; I = " + I.render());
      if (reductionIsContiguous(L, I)) {
        SIMDBackEnd::SIMDInstListType Aux;
        MACVETH_DEBUG("AVX2BackEnd", "reduction is contiguous");
        // FIXME: isn't a better way of doing this????
        for (auto R : L) {
          if (R.getResultValue() != I.getResultValue()) {
            Aux.push_back(R);
          }
        }
        LRedux[S] = Aux;
      } else if (hasRawDependencies(L, I)) {
        MACVETH_DEBUG("AVX2BackEnd", "has dependencies");
        ReplaceFusedRedux[L.back()] = fuseReductionsList(L);
        auto it = L.end();
        SkipList.splice(SkipList.end(), L, L.begin(), --it);
        LRedux[S].clear();
      }
      LRedux[S].push_back(I);
    }
    // For each reduction found, check whether they can be fused together or
    // not:
    // they can be fused if and only if they are in the same loop
    // dimension/name and the same loop, basically (because they can have same
    // name but be different loops). This is important for coherence in the
    // program
    for (auto &L : LRedux) {
      if ((L.second.size() == 0)) {
        MACVETH_DEBUG("AVX2BackEnd",
                      "no reductions for size  = " + std::to_string(L.first));
        continue;
      }
      auto LInsSize = L.second.size();
      auto W = L.second.front().VOPResult.Width;
      auto DW =
          MVDataType::VDataTypeWidthBits[L.second.front().VOPResult.DType];
      // Only compute reductions if max found
      if ((unsigned)W / DW > LInsSize) {
        MACVETH_DEBUG("AVX2BackEnd",
                      "Skip => W / DW = " + std::to_string((W / DW)) +
                          "; LInsSize " + std::to_string(LInsSize));
        continue;
      }
      ReplaceFusedRedux[L.second.back()] = fuseReductionsList(L.second);
      auto it = L.second.end();
      SkipList.splice(SkipList.end(), L.second, L.second.begin(), --it);
      LRedux[L.first].clear();
    }
  }

  // Compute the rest
  for (auto &L : LRedux) {
    if ((L.second.size() == 0)) {
      continue;
    }
    MACVETH_DEBUG("AVX2BackEnd", "no fusion");
    ReplaceFusedRedux[L.second.back()] = fuseReductionsList(L.second);
    auto it = L.second.end();
    SkipList.splice(SkipList.end(), L.second, L.second.begin(), --it);
  }

  // Reorder SIMDInstructions yet
  for (auto I : TIL) {
    if (std::find(SkipList.begin(), SkipList.end(), I) != SkipList.end()) {
      continue;
    }
    if (ReplaceFusedRedux.count(I)) {
      IL.splice(IL.end(), ReplaceFusedRedux.at(I));
      continue;
    }
    IL.push_back(I);
  }
  MACVETH_DEBUG("AVX2BackEnd", "=== END FUSE REDUCTIONS ===");
  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::noFuseReductions(SIMDBackEnd::SIMDInstListType TIL) {
  SIMDBackEnd::SIMDInstListType IL;
  SIMDBackEnd::SIMDInstListType SkipList;
  // List of reduction candidates to be fused
  std::map<int, SIMDBackEnd::SIMDInstListType> LRedux;
  std::map<SIMDBackEnd::SIMDInst, SIMDBackEnd::SIMDInstListType>
      ReplaceFusedRedux;
  MACVETH_DEBUG("AVX2BackEnd", "=== FUSE REDUCTIONS ===");
  int S = 0;
  for (auto I : TIL) {
    if (I.isReduction()) {
      LRedux[S++].push_back(I);
    }
  }

  // Compute the rest
  for (auto &L : LRedux) {
    if ((L.second.size() == 0)) {
      continue;
    }
    ReplaceFusedRedux[L.second.back()] = fuseReductionsList(L.second);
    auto it = L.second.end();
    SkipList.splice(SkipList.end(), L.second, L.second.begin(), --it);
  }

  // Reorder SIMDInstructions yet
  for (auto I : TIL) {
    if (std::find(SkipList.begin(), SkipList.end(), I) != SkipList.end()) {
      continue;
    }
    if (ReplaceFusedRedux.count(I)) {
      IL.splice(IL.end(), ReplaceFusedRedux.at(I));
      continue;
    }
    IL.push_back(I);
  }
  MACVETH_DEBUG("AVX2BackEnd", "=== END FUSE REDUCTIONS ===");
  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::peepholeOptimizations(SIMDBackEnd::SIMDInstListType I) {
  SIMDBackEnd::SIMDInstListType IL(I);
  // Fuse reductions if any and fuse them
  // IL = fuseReductions(IL);
  IL = noFuseReductions(IL);

  // Fuse operations: find potential and applicable FMADD/FMSUB, if not
  // disabled
  if (!MVOptions::DisableFMA) {
    IL = fuseAddSubMult(IL);
  }

  return IL;
}

// ---------------------------------------------
std::string AVX2BackEnd::getRegisterType(MVDataType::VDataType DT,
                                         MVDataType::VWidth W) {
  auto Width = W;
  assert((W != 512) && "Width not permitted in AVX2!!");
  if (W < 256) {
    Width = MVDataType::VWidth::W128;
  }
  std::string Suffix = "";
  if (DT == MVDataType::VDataType::DOUBLE) {
    Suffix = "d";
  } else if (DT == MVDataType::VDataType::FLOAT) {
    Suffix = "";
  } else {
    Suffix = "i";
  }
  return "__m" + std::to_string(Width) + Suffix;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInst
AVX2BackEnd::genSIMDInst(std::string Result, std::string Op, std::string PrefS,
                         std::string SuffS, MVDataType::VWidth Width,
                         MVDataType::VDataType Type, MVStrVector Args,
                         SIMDBackEnd::SIMDType SType, MVSourceLocation SL,
                         SIMDBackEnd::SIMDInstListType *IL, std::string NameOp,
                         std::string MVFunc, MVStrVector MVArgs, MVOp MVOP) {

  auto WStr = getMapWidth(Width);
  auto TStr = getMapType(Type);

  // Special case
  // Almost all intrinsics are:
  // _mm#W_#Pop#S_#D where #W is "" if width = 128 bits, "256" if width = 256
  // bits. #P is a preffix, e.g. "mask_", #S is a suffix, e.g. "f128" for
  // inserts. #D is the datatype, e.g. "epi8/.../ps/pd".
  // Nonetheless, for cast intrinsics the format be like: _mm#W_op#P_#S
  auto F = Op.find("cast");
  if (F != std::string::npos) {
    TStr = SuffS;
    SuffS = PrefS;
    PrefS = "";
  }

  if (OpEq[Op] != "") {
    Op = OpEq[Op];
  }

  if (Op == "set") {
    if (TStr == "epi64") {
      TStr += "x"; // epi64x
    }
  }

  // Replace fills in pattern
  std::string AVXFunc = replacePatterns(Op, WStr, TStr, PrefS, SuffS);

  // Generate SIMD inst
  SIMDBackEnd::SIMDInst I(NameOp, AVXFunc, Args, MVFunc, MVArgs, SL);

  // Retrieving cost of function
  I.SType = SType;

  // Data type and width
  I.DT = Type;
  I.W = Width;

  // MVOp
  I.MVOP = MVOP;
  I.Result = Result;

  // Adding instruction to the list
  if (IL != nullptr) {
    IL->push_back(I);
  }

  return I;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInst
AVX2BackEnd::genSIMDInst(VectorIR::VOperand V, std::string Op,
                         std::string PrefS, std::string SuffS, MVStrVector Args,
                         SIMDBackEnd::SIMDType SType, MVSourceLocation SL,
                         SIMDBackEnd::SIMDInstListType *IL, std::string NameOp,
                         std::string MVFunc, MVStrVector MVArgs, MVOp MVOP) {

  if (OpEq[Op] != "") {
    Op = OpEq[Op];
  }

  // Replace fills in pattern
  std::string AVXFunc = replacePatterns(
      Op, getMapWidth(V.getWidth()), getMapType(V.getDataType()), PrefS, SuffS);

  // Generate SIMD inst
  SIMDBackEnd::SIMDInst I((NameOp == "") ? V.getName() : NameOp, AVXFunc, Args,
                          MVFunc, MVArgs, SL);

  // Retrieving cost of function
  // InstCostInfo C(CostTable::getOpInfo(Op));
  // I.Cost = C;
  I.SType = SType;

  // Data type and width
  I.DT = V.getDataType();
  I.W = V.getWidth();
  I.VOPResult = V;

  // MVOp
  I.MVOP = MVOP;

  // Adding instruction to the list
  if (IL != nullptr) {
    IL->push_back(I);
  }
  return I;
}

// ---------------------------------------------
std::string AVX2BackEnd::getMask(unsigned int MaskVect, int NElems,
                                 MVDataType::VWidth Width,
                                 MVDataType::VDataType Type) {
  std::string Mask = "";
  std::string SetPref = "";
  std::string SetSuff = getMapType(Type);
  if (Width == MVDataType::VWidth::W256) {
    SetPref = "256";
  }
  Mask = "_mm" + SetPref + "_set_" + SetSuff + "(";
  long long M = (MaskVect & 0x1) == 1 ? 0xFFFFFFFFFFFFFFFF : 0;
  std::string Indices = std::to_string(M);
  for (int i = 1; i < NElems; ++i) {
    M = (MaskVect >> i & 0x1) == 1 ? 0xFFFFFFFFFFFFFFFF : 0;
    Indices = std::to_string(M) + "," + Indices;
  }
  Mask += Indices + ")";
  return Mask;
}

// ---------------------------------------------
std::string AVX2BackEnd::getMask(unsigned int MaskVect, int NElems,
                                 MVDataType::VWidth Width) {
  std::string Mask = "";
  std::string SetPref = "";
  std::string SetSuff = "epi32";
  if (Width == MVDataType::VWidth::W256) {
    SetPref = "256";
    if (NElems == 4) {
      SetSuff = "epi64x";
    }
  } else {
    if (NElems == 2) {
      SetSuff = "epi64x";
    }
  }
  Mask = "_mm" + SetPref + "_set_" + SetSuff + "(";
  long long M = (MaskVect & 0x1) == 1 ? 0xFFFFFFFFFFFFFFFF : 0;
  std::string Indices = std::to_string(M);
  for (int i = 1; i < NElems; ++i) {
    M = (MaskVect >> i & 0x1) == 1 ? 0xFFFFFFFFFFFFFFFF : 0;
    Indices = std::to_string(M) + "," + Indices;
  }
  Mask += Indices + ")";
  return Mask;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vload(VectorIR::VOperand V) {
  SIMDBackEnd::SIMDInstListType IL;
  // Suffix: we are going to assume that all the load are unaligned.
  std::string SuffS = (V.IsPartial) || (V.Size < V.VSize) ? "" : "u";
  // Mask
  std::string Mask = (V.IsPartial) && (V.VSize != 2) ? "mask" : "";

  // Type of load: load/u
  auto Op = Mask + "load";

  // List of parameters
  std::list<std::string> Args;

  // Hack: if data type is integer then use the si128/si256 notation instead
  // of epi
  if (V.DType > MVDataType::VDataType::SFLOAT) {
    V.DType = (V.VSize > 4) ? MVDataType::VDataType::UNDEF256
                            : MVDataType::VDataType::UNDEF128;
    auto Cast = getRegisterType(V.DType, V.Width);
    Args.push_back("(" + Cast + "*)" + getOpName(V, true, true));
  } else {
    Args.push_back(getOpName(V, true, true));
  }

  if (V.IsPartial) {
    if (V.VSize == 2) {
      Args.push_front(V.getName());
      SuffS += (V.LowBits) ? "l" : "";
      SuffS += (V.HighBits) ? "h" : "";
      if (V.isFloat()) {
        V.DType = MVDataType::VDataType::HALF_FLOAT;
      }
    } else {
      auto MaskLoad = getMask(V.Mask, V.Size, V.getWidth());
      Args.push_back(MaskLoad);
    }
  }

  if (V.isDouble() && V.ContiguousHalves) {
    Args.push_front(getOpName(V, true, true, 2));
    V.DType = MVDataType::VDataType::IN_DOUBLE256;
    SuffS += "2";
  }

  MVStrVector ArgsVector{std::make_move_iterator(Args.begin()),
                         std::make_move_iterator(Args.end())};
  // Adding SIMD inst to the list
  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);
  genSIMDInst(V, Op, "", SuffS, ArgsVector, SIMDType::VPACK, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vbcast(VectorIR::VOperand V) {
  SIMDBackEnd::SIMDInstListType IL;
  auto Op = "broadcast";

  // There are no preffixes for broadcasts operations
  // TODO: generate suffix
  auto SuffS = "";

  // List of parameters
  MVStrVector Args;
  Args.push_back(getOpName(V, true, true));

  if (V.EqualVal) {
    if (V.getDataType() == MVDataType::VDataType::DOUBLE) {
      V.DType = MVDataType::VDataType::SDOUBLE;
    }
    if (V.getDataType() == MVDataType::VDataType::FLOAT) {
      V.DType = MVDataType::VDataType::SFLOAT;
    }
  }

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);
  // Adding SIMD inst to the list
  genSIMDInst(V, Op, "", SuffS, Args, SIMDType::VBCAST, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vgather(VectorIR::VOperand V) {
  SIMDBackEnd::SIMDInstListType IL;
  auto Op = "gather";
  std::string PrefS = "";

  // List of parameters
  MVStrVector Args;

  // To gather elements
  // Generate preffix: must be i32 or i64, depending on the VIndex width
  auto Scale = "4";
  auto VIndexSuffix = "epi32";
  auto PrefGather = "i32";

  // Generation of the preffix
  if (V.IsPartial) {
    PrefS += "mask_";
    std::string NeutralReg = "_mm" + getMapWidth(V.getWidth()) + "_setzero_" +
                             getMapType(V.getDataType()) + "()";
    Args.push_back(NeutralReg);
  }

  auto MapWidthSet = MapWidth[MVDataType::VWidth::W256];
  auto LeftOver = V.Size - V.VSize;
  if (((V.getDataType() == MVDataType::VDataType::FLOAT) && (V.VSize <= 4)) ||
      ((V.getDataType() == MVDataType::VDataType::DOUBLE) && (V.VSize <= 2))) {
    MapWidthSet = MapWidth[MVDataType::VWidth::W128];
  }
  if (V.getDataType() == MVDataType::VDataType::DOUBLE) {
    PrefGather = "i64";
    Scale = "8";
    VIndexSuffix = "epi64x";
  }

  PrefS += PrefGather;
  auto VIndex = "_mm" + MapWidthSet + "_set_" + VIndexSuffix + "(";
  auto CopyIdx = std::vector<long>(V.Idx.begin(), V.Idx.begin() + V.Size);
  auto MinIdx = std::min_element(CopyIdx.begin(), CopyIdx.end());
  auto MinVal = *MinIdx;
  auto BaseIdx = V.UOP[MinIdx - CopyIdx.begin()]->getValue();
  // This way we avoid negative indices
  if (MinVal < 0) {
    for (size_t T = 0; T != CopyIdx.size(); ++T) {
      CopyIdx[T] += abs(MinVal);
    }
  }
  std::reverse(CopyIdx.begin(), CopyIdx.end());
  for (size_t T = 0; T < LeftOver; ++T) {
    VIndex += std::to_string(0) + ",";
  }
  for (size_t T = LeftOver; T < V.Size; ++T) {
    VIndex += std::to_string(CopyIdx[T]) + ((T == (V.Size - 1)) ? "" : ", ");
  }
  VIndex += ")";

  // Memory address
  Args.push_back("&" + BaseIdx);
  Args.push_back(VIndex);

  // Mask argument if needed
  if (V.IsPartial) {
    Args.push_back(getMask(V.Mask, V.Size, V.getWidth(), V.getDataType()));
  }

  // Scale can only be: 1, 2, 4, 8
  Args.push_back(Scale);

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V, Op, PrefS, "", Args, SIMDType::VGATHER, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
std::vector<std::tuple<int, int>> getContiguityRanges(VectorIR::VOperand V) {
  int ActualStride = 1;
  std::vector<std::tuple<int, int>> Ranges;
  for (int Idx = 1; Idx < (int)V.VSize; ++Idx) {
    // Check longest stride of contiguous elements
    if (V.Idx[Idx - 1] + 1 != V.Idx[Idx]) {
      Ranges.push_back(std::make_tuple(Idx - ActualStride, Idx - 1));
      ActualStride = 0;
    }
    ActualStride++;
  }
  Ranges.push_back(std::make_tuple(V.Size - ActualStride, V.Size - 1));
  return Ranges;
}

// ---------------------------------------------
void AVX2BackEnd::insertLowAndHighBits(VectorIR::VOperand V, std::string Hi,
                                       std::string Lo, MVSourceLocation MVSL,
                                       SIMDBackEnd::SIMDInstListType *IL) {
  VectorIR::VOperand NewVOp = V;
  NewVOp.Width = MVDataType::VWidth::W256;
  NewVOp.DType = (V.DType == MVDataType::VDataType::DOUBLE)
                     ? MVDataType::VDataType::IN_DOUBLE128
                     : MVDataType::VDataType::IN_FLOAT128;
  genSIMDInst(NewVOp, "set", "", "", {Hi, Lo}, SIMDType::VPACK, MVSL, IL);
}

// ---------------------------------------------
void AVX2BackEnd::store(VectorIR::VOperand V, MVStrVector Args,
                        MVSourceLocation MVSL,
                        SIMDBackEnd::SIMDInstListType *IL) {
  genSIMDInst(V, "storeu", "", "", Args, SIMDType::VSTORE, MVSL, IL);
}

// ---------------------------------------------
void AVX2BackEnd::load(VectorIR::VOperand V, MVStrVector Args,
                       MVSourceLocation MVSL,
                       SIMDBackEnd::SIMDInstListType *IL) {
  genSIMDInst(V, "loadu", "", "", Args, SIMDType::VPACK, MVSL, IL);
}

// ---------------------------------------------
void AVX2BackEnd::loads(VectorIR::VOperand V, MVStrVector Args,
                        MVSourceLocation MVSL,
                        SIMDBackEnd::SIMDInstListType *IL) {
  V.DType = (V.DType == MVDataType::FLOAT) ? MVDataType::VDataType::SFLOAT
                                           : MVDataType::VDataType::SDOUBLE;
  V.Width = MVDataType::VWidth::W128;
  genSIMDInst(V, "load", "", "", Args, SIMDType::VPACK, MVSL, IL);
}

// ---------------------------------------------
void AVX2BackEnd::moves(VectorIR::VOperand V, MVStrVector Args,
                        MVSourceLocation MVSL,
                        SIMDBackEnd::SIMDInstListType *IL) {
  V.DType = (V.DType == MVDataType::FLOAT) ? MVDataType::VDataType::SFLOAT
                                           : MVDataType::VDataType::SDOUBLE;
  V.Width = MVDataType::VWidth::W128;
  genSIMDInst(V, "move", "", "", Args, SIMDType::VPACK, MVSL, IL);
}

// ---------------------------------------------
void AVX2BackEnd::blend(VectorIR::VOperand V, MVStrVector Args,
                        MVSourceLocation MVSL,
                        SIMDBackEnd::SIMDInstListType *IL) {
  genSIMDInst(V, "blend", "", "", Args, SIMDType::VPACK, MVSL, IL);
}

// ---------------------------------------------
void AVX2BackEnd::insert(VectorIR::VOperand V, MVStrVector Args,
                         MVSourceLocation MVSL,
                         SIMDBackEnd::SIMDInstListType *IL) {
  genSIMDInst(V, "insert", "", "", Args, SIMDType::VPACK, MVSL, IL);
}

// ---------------------------------------------
std::string getContiguityStr(VectorIR::VOperand V) {
  std::string Cont = "";
  for (int Idx = 1; Idx < (int)V.VSize; ++Idx) {
    Cont = "_" + std::to_string(V.Idx[Idx - 1] + 1 == V.Idx[Idx]) + Cont;
  }
  return Cont;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vpack(VectorIR::VOperand V) {
  // Search for the pattern in the templates
  auto SearchStr = MVDataType::VDTypeToCType[V.getDataType()] + "_n" +
                   std::to_string(V.VSize) + getContiguityStr(V);
  // Generate search string, search, get template
  // For each line, substitute
  auto Template = getRPTable().getTemplate(SearchStr);
  if (Template.has_value()) {
    MACVETH_DEBUG("AVX2BackEnd", "Found cooking recipe for = " + SearchStr);
    SIMDBackEnd::SIMDInstListType IL;
    auto RPTemplate = Template.value();
    std::string Output = "";
    // Iterate all lines, and substitute elements
    for (const auto &Line : RPTemplate.getTemplates()) {
      std::regex OutputRegex("(#output:REG#)");
      Output = std::regex_replace(Line, OutputRegex, V.getName());
      std::regex ValRegex("(#[-a-z0-9:]+#)+", std::regex::icase);
      auto i = std::sregex_iterator(Line.begin(), Line.end(), ValRegex);
      for (; i != std::sregex_iterator(); ++i) {
        auto Match = (*i).str();                        // this is the match
        auto Split = Match.substr(1, Match.size() - 2); // remove # characters
        auto Value = TemplateOperand(Split);
        // Value to replace
        std::string ReplaceStr = "";
        if (Value.isMem()) {
          auto *NodeArray =
              dyn_cast<MVExprArray>(V.UOP[Value.getMemPos()]->getMVExpr());
          if (NodeArray) {
            ReplaceStr = (Value.getOffset() != 0)
                             ? ReplaceStr = NodeArray->toStringWithOffset(
                                   Value.getOffset())
                             : NodeArray->getExprStrRaw();
          } else {
            /// FIXME: not tested this...
            ReplaceStr = V.UOP[Value.getMemPos()]->getRegisterValue();
          }
        } else {
          // Get auxiliar register
          std::string Type = "__m" + std::to_string(Value.getMOD());
          ReplaceStr = mapNewAuxRegister(Type, Value.getID(), 0);
        }
        // Last but not least
        std::regex NewReplace(Match);
        Output = std::regex_replace(Output, NewReplace, ReplaceStr);
      }
      // ADD SIMD instruction
      MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order,
                            V.Offset);
      addNonSIMDInst("", Output, SIMDType::VTEMPLATE, MVSL, &IL);
    }
    return IL;
  }
  // Fallback: in case gather is more profitable or packing not found...
  MACVETH_DEBUG("AVX2BackEnd", "Not found cooking recipe for = " + SearchStr);
  return vgather(V);
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::vregisterpacking(VectorIR::VOperand V) {
  SIMDBackEnd::SIMDInstListType IL;
  std::string SuffS = "";
  std::string Op = "set";

  MACVETH_DEBUG("AVX2BackEnd", "Random register packing: " + V.toString());

  // List of parameters
  std::vector<std::string> Args;
  unsigned i = 0;
  assert(V.RegIdx.size() == V.VSize && "Random packing invalid");
  for (; i < V.VSize; ++i) {
    auto T = V.RegIdx[i];
    std::string NewIdx =
        std::get<0>(T) + "[" + std::to_string(std::get<1>(T)) + "]";
    Args.push_back(NewIdx);
  }
  for (; i < V.Size; ++i) {
    Args.push_back("0");
  }
  std::reverse(Args.begin(), Args.end());
  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);
  // Adding SIMD inst to the list
  genSIMDInst(V, Op, "", SuffS, Args, SIMDType::VSET, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vset(VectorIR::VOperand V) {
  SIMDBackEnd::SIMDInstListType IL;
  std::string SuffS = "";
  std::string Op = "set";

  // List of parameters
  std::list<std::string> Args;
  if (V.EqualVal) {
    SuffS += "1";
    Args.push_front((V.UOP[0] != nullptr) ? V.UOP[0]->getRegisterValue() : "0");
  } else {
    for (size_t n = 0; n < V.VSize; n++) {
      Args.push_front((V.UOP[n] != nullptr) ? V.UOP[n]->getValue() : "0.0");
    }
    for (int t = V.VSize;
         t < (int)(V.Width / MVDataType::VDataTypeWidthBits[V.DType]); ++t) {
      Args.push_front("0");
    }
  }
  Args.reverse();
  MVStrVector ArgsVector{std::make_move_iterator(Args.begin()),
                         std::make_move_iterator(Args.end())};
  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);
  // Adding SIMD inst to the list
  genSIMDInst(V, Op, "", SuffS, ArgsVector, SIMDType::VSET, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vstore(VectorIR::VectorOP V) {
  SIMDBackEnd::SIMDInstListType IL;
  MACVETH_DEBUG("AVX2BackEnd", V.getResult().toString());
  std::string SuffS = (V.getResult().IsPartial) ? "" : "u";
  auto NeedsMask = (V.getResult().IsPartial) &&
                   (!V.getResult().LowBits && !V.getResult().HighBits);
  std::string PrefS = (NeedsMask) ? "mask" : "";

  std::string Op = "store";

  // List of parameters
  MVStrVector Args;
  // Hack: if data type is integer then use the si128/si256 notation instead
  // of epi
  if (V.getResult().DType > MVDataType::VDataType::SFLOAT) {
    V.R.DType = (V.getResult().VSize > 4) ? MVDataType::VDataType::UNDEF256
                                          : MVDataType::VDataType::UNDEF128;
    std::string Cast =
        getRegisterType(V.getResult().DType, V.getResult().Width);
    Args.push_back("(" + Cast + "*)" + getOpName(V.getResult(), true, true));
    if (NeedsMask) {
      Args.push_back(getMask(V.getResult().Mask, V.getResult().Size,
                             V.getResult().getWidth()));
    }
    Args.push_back(getOpName(V.getResult(), false, false));
  } else {
    Args.push_back(getOpName(V.getResult(), true, true));
    if (NeedsMask) {
      Args.push_back(getMask(V.getResult().Mask, V.getResult().Size,
                             V.getResult().getWidth()));
    }
    Args.push_back(getOpName(V.getResult(), false, false));
    if (V.getResult().EqualVal) {
      if (V.getResult().DType == MVDataType::VDataType::DOUBLE) {
        V.R.DType = MVDataType::VDataType::SDOUBLE;
        V.R.Width = MVDataType::VWidth::W64;
      } else if (V.getResult().DType == MVDataType::VDataType::FLOAT) {
        V.R.DType = MVDataType::VDataType::SFLOAT;
        V.R.Width = MVDataType::VWidth::W32;
      }
      SuffS = "";
    }
  }

  if (!NeedsMask) {
    if ((V.getResult().Size == 2) || (V.getResult().VSize == 2)) {
      SuffS = (V.getResult().LowBits) ? "l" : SuffS;
      SuffS = (V.getResult().HighBits) ? "h" : SuffS;
      if (V.getResult().isFloat()) {
        V.R.DType = MVDataType::VDataType::HALF_FLOAT;
      }
    }
  }

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.getResult(), Op, PrefS, SuffS, Args, SIMDType::VSTORE, MVSL,
              &IL);

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::vscatterAVX512(VectorIR::VectorOP VOP) {
  auto V = VOP.R;
  SIMDBackEnd::SIMDInstListType IL;
  auto Op = "scatter";

  // List of parameters
  MVStrVector Args;

  // To gather elements
  // Generate preffix: must be i32 or i64, depending on the VIndex width
  std::string Scale =
      (V.getDataType() == MVDataType::VDataType::DOUBLE) ? "8" : "4";
  std::string VIndexSuffix = "epi32";
  std::string PrefS = "";

  // Generation of the preffix
  if (V.IsPartial) {
    // Who tf did intel intrinsics??????? I mean, it makes no sense at
    // all that for loads we use "maskload", then for gathers
    // "mask_[i32|i64]gather", why the underscore? It should not be difficult
    // to normalize the API so the signature of all functions are constructed
    // the same way
    PrefS += "mask_";
    std::string NeutralReg = "_mm" + getMapWidth(V.getWidth()) + "_setzero_" +
                             getMapType(V.getDataType()) + "()";
    Args.push_back(NeutralReg);
  }
  auto PrefScatter = "i32";
  PrefS += PrefScatter;

  auto VIndex = "_mm" + MapWidth[V.Width] + "_set_" + VIndexSuffix + "(";
  auto CopyIdx = std::vector<long>(V.Idx.begin(), V.Idx.begin() + V.Size);
  auto MinIdx = std::min_element(CopyIdx.begin(), CopyIdx.end());
  auto MinVal = *MinIdx;
  auto BaseIdx = V.UOP[MinIdx - CopyIdx.begin()]->getRegisterValue();
  // This way we avoid negative indices
  if (MinVal < 0) {
    for (size_t T = 0; T != CopyIdx.size(); ++T) {
      CopyIdx[T] += abs(MinVal);
    }
  }
  std::reverse(CopyIdx.begin(), CopyIdx.end());
  for (size_t T = 0; T != V.VSize % 2; ++T) {
    VIndex += std::to_string(0) + ",";
  }
  for (size_t T = 0; T != CopyIdx.size(); ++T) {
    VIndex +=
        std::to_string(CopyIdx[T]) + ((T == (CopyIdx.size() - 1)) ? "" : ", ");
  }
  VIndex += ")";

  // Memory address
  Args.push_back("&" + BaseIdx);
  Args.push_back(VIndex);
  Args.push_back(V.getName());

  // Mask argument if needed
  if (V.IsPartial) {
    Args.push_back(getMask(V.Mask, V.Size, V.getWidth(), V.getDataType()));
  }

  // Scale can only be: 1, 2, 4, 8
  Args.push_back(Scale);

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V, Op, PrefS, "", Args, SIMDType::VSCATTER, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::singleElementScatterOp(VectorIR::VectorOP VOP) {
  SIMDBackEnd::SIMDInstListType IL;
  MACVETH_DEBUG("AVX2BackEnd", "manual scatter with op = " + VOP.toString());
  auto V = VOP.getOpB();
  auto R = VOP.getResult();
  auto OP = VOP.getMVOp().toString();
  MVSourceLocation MVSL(MVSourceLocation::Position::POSORDER, R.Order,
                        R.Offset);

  for (size_t N = 0; N < V.VSize; ++N) {
    auto Reg = V.getName() + "[" + std::to_string(N) + "]";
    if (V.RequiresRegisterPacking) {
      auto TmpReg = V.RegIdx[N];
      Reg =
          std::get<0>(TmpReg) + "[" + std::to_string(std::get<1>(TmpReg)) + "]";
    }
    addNonSIMDInst(R.UOP[N]->getRegisterValue(),
                   R.UOP[N]->getRegisterValue() + OP + Reg, SIMDType::VOPT,
                   MVSL, &IL);
  }
  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::singleElementScatter(VectorIR::VectorOP VOP) {
  MACVETH_DEBUG("AVX2BackEnd", "manual scatter = " + VOP.toString());
  SIMDBackEnd::SIMDInstListType IL;
  auto R = VOP.getResult();
  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, R.Order, R.Offset);
  for (size_t N = 0; N < R.VSize; ++N) {
    addNonSIMDInst(R.UOP[N]->getRegisterValue(),
                   R.getName() + "[" + std::to_string(N) + "]", SIMDType::VOPT,
                   MVSL, &IL);
  }
  return IL;
}

// ---------------------------------------------
bool AVX2BackEnd::vscatter4elements(VectorIR::VectorOP VOP,
                                    MVDataType::VWidth Width,
                                    SIMDBackEnd::SIMDInstListType *IL) {
  MACVETH_DEBUG("AVX2BackEnd", "VSCATTER 4 elements = " + VOP.toString());
  auto V = VOP.R;
  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);
  std::string TypeReg = (V.DType == MVDataType::FLOAT) ? "__m128" : "__m128d";
  int Half = (int)V.VSize / 2;
  bool FullVector = false;
  bool FirstHalve = false;
  bool SecondHalve = false;
  auto Ranges = getContiguityRanges(V);
  for (auto Range : Ranges) {
    FullVector |= ((std::get<0>(Range) == 0) &&
                   (std::get<1>(Range) == (int)(V.VSize - 1)));
    FirstHalve |=
        ((std::get<0>(Range) == 0) && (std::get<1>(Range) == (Half - 1)));
    SecondHalve |= ((std::get<0>(Range) == (Half)) &&
                    (std::get<1>(Range) == (2 * Half - 1)));
  }

  if (FullVector) {
    IL->splice(IL->end(), vstore(VOP));
    return true;
  }

  if (FirstHalve) {
    VectorIR::VOperand NewVOp =
        getSubVOperand(V, V.getName(), V.VSize, Half, MVDataType::VWidth::W128,
                       MVDataType::VDataType::HALF_FLOAT, 0);
    VOP.R = NewVOp;
    IL->splice(IL->end(), vstore(VOP));
  }
  if (SecondHalve) {
    VectorIR::VOperand NewVOp =
        getSubVOperand(V, V.getName(), V.VSize, Half, MVDataType::VWidth::W128,
                       MVDataType::VDataType::HALF_FLOAT, Half);
    VOP.R = NewVOp;
    IL->splice(IL->end(), vstore(VOP));
  }

  if (!FirstHalve && SecondHalve) {
    for (size_t N = 0; N < 2; ++N) {
      std::string Idx = "[" + std::to_string(N) + "]";
      addNonSIMDInst(V.UOP[N]->getRegisterValue(), V.getName() + Idx,
                     SIMDType::VOPT, MVSL, IL);
    }
  }

  if (!SecondHalve && FirstHalve) {
    for (size_t N = 2; N < 4; ++N) {
      std::string Idx = "[" + std::to_string(N) + "]";
      addNonSIMDInst(V.UOP[N]->getRegisterValue(), V.getName() + Idx,
                     SIMDType::VOPT, MVSL, IL);
    }
  }

  if ((!FirstHalve) && (!SecondHalve)) {
    return false;
  }
  return true;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vscatter(VectorIR::VectorOP V) {
  SIMDBackEnd::SIMDInstListType IL;
  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER,
                        V.getResult().Order, V.getResult().Offset);
  auto VOP = V.getResult();
  auto Ranges = getContiguityRanges(VOP);

  MACVETH_DEBUG("AVX2BackEnd",
                "vscatter main function; Size = " + std::to_string(VOP.VSize) +
                    "; Ranges size = " + std::to_string(Ranges.size()));

  if (VOP.VSize < 4) {
    return singleElementScatterOp(V);
  }

  // Add AVX-512 scatter using 256 bits if architecture permits and
  // ISA chosen is >=(AVX512F + AVX512VL) If scatter is available in machine
  // Just do a manual scatter
  if (Ranges.size() == VOP.VSize) {
    // Check if ISA available
    if ((MVOptions::ISA == MVCPUInfo::MVISA::AVX512) ||
        (MVOptions::ScatterInstruction)) {
      return vscatterAVX512(V);
    }
    return singleElementScatter(V);
  }

  int Half = (int)VOP.VSize / 2;

  auto FullReg = V.R;
  if (VOP.Size > 4) {
    // 4 elements strategy
    auto NameFirstHalve = "_mm256_castps256_ps128(" + VOP.getName() + ")";
    auto VFirstHalve =
        getSubVOperand(VOP, NameFirstHalve, 4, Half, MVDataType::VWidth::W128,
                       MVDataType::VDataType::FLOAT, 0);
    V.setResult(VFirstHalve);
    bool FirstHalve = vscatter4elements(V, MVDataType::VWidth::W128, &IL);

    // FIXME:
    auto NameSecondHalve = "__tmp_extract_128";
    SIMDBackEnd::addRegToDeclare("__m128", NameSecondHalve, {0});
    genSIMDInst(NameSecondHalve, "extract", "", "f128",
                MVDataType::VWidth::W256, MVDataType::VDataType::FLOAT,
                {VOP.getName(), "0x1"}, SIMDType::VOPT, MVSL, &IL);
    auto VSecondHalve =
        getSubVOperand(VOP, NameSecondHalve, 4, Half, MVDataType::VWidth::W128,
                       MVDataType::VDataType::FLOAT, Half);
    V.setResult(VSecondHalve);
    bool SecondHalve = vscatter4elements(V, MVDataType::VWidth::W128, &IL);
    if (!FirstHalve && !SecondHalve) {
      V.setResult(FullReg);
      if ((MVOptions::ISA == MVCPUInfo::MVISA::AVX512) ||
          (MVOptions::ScatterInstruction)) {
        return vscatterAVX512(V);
      }
      return singleElementScatter(V);
    }
    V.setResult(FullReg);
    if (FirstHalve && !SecondHalve) {
      if ((MVOptions::ISA == MVCPUInfo::MVISA::AVX512) ||
          (MVOptions::ScatterInstruction)) {
        V.setResult(VSecondHalve);
        IL.splice(IL.end(), vscatterAVX512(V));
      } else {
        for (size_t N = 4; N < 8; ++N) {
          std::string Idx = "[" + std::to_string(N) + "]";
          addNonSIMDInst(V.getResult().UOP[N]->getRegisterValue(),
                         V.getResult().getName() + Idx, SIMDType::VOPT, MVSL,
                         &IL);
        }
      }
    }
    if (!FirstHalve && SecondHalve) {
      if ((MVOptions::ISA == MVCPUInfo::MVISA::AVX512) ||
          (MVOptions::ScatterInstruction)) {
        V.setResult(VFirstHalve);
        IL.splice(IL.end(), vscatterAVX512(V));
      } else {
        for (size_t N = 0; N < 4; ++N) {
          std::string Idx = "[" + std::to_string(N) + "]";
          addNonSIMDInst(V.getResult().UOP[N]->getRegisterValue(),
                         V.getResult().getName() + Idx, SIMDType::VOPT, MVSL,
                         &IL);
        }
      }
    }
  } else {
    if (!vscatter4elements(V, VOP.getWidth(), &IL)) {
      if ((MVOptions::ISA == MVCPUInfo::MVISA::AVX512) ||
          (MVOptions::ScatterInstruction)) {
        IL.splice(IL.end(), vscatterAVX512(V));
      } else {
        IL.splice(IL.end(), singleElementScatter(V));
      }
    }
  }

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vadd(VectorIR::VectorOP V) {
  SIMDBackEnd::SIMDInstListType IL;
  // TODO: generate preffix
  std::string PrefS = "";
  // TODO: generate suffix
  std::string SuffS = "";

  // TODO:
  std::string Op = "add";

  // TODO: check
  // List of parameters
  MVStrVector Args;
  Args.push_back(V.getOpA().getName());
  Args.push_back(V.getOpB().getName());

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.getResult(), Op, PrefS, SuffS, Args, SIMDType::VADD, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vmul(VectorIR::VectorOP V) {
  SIMDBackEnd::SIMDInstListType IL;
  std::string Op = "mul";

  MVAssertSkip(V.DT != MVDataType::VDataType::INT64,
               "64bit integer multiplication has not been implemented yet",
               GotoStartScop);

  std::string SuffS = "";
  if (V.DT == MVDataType::VDataType::INT32) {
    SuffS += "lo";
  }

  // List of parameters
  MVStrVector Args;
  Args.push_back(V.getOpA().getName());
  Args.push_back(V.getOpB().getName());

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.getResult(), Op, "", SuffS, Args, SIMDType::VMUL, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vsub(VectorIR::VectorOP V) {
  SIMDBackEnd::SIMDInstListType IL;
  // TODO: generate preffix
  std::string PrefS = "";
  // TODO: generate suffix
  std::string SuffS = "";
  // TODO:
  std::string Op = "sub";

  // TODO: check
  // List of parameters
  MVStrVector Args;
  Args.push_back(V.getOpA().getName());
  Args.push_back(V.getOpB().getName());

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.getResult(), Op, PrefS, SuffS, Args, SIMDType::VSUB, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vdiv(VectorIR::VectorOP V) {
  SIMDBackEnd::SIMDInstListType IL;
  // TODO: generate preffix
  std::string PrefS = "";
  // TODO: generate suffix
  std::string SuffS = "";
  // TODO:
  std::string Op = "div";

  // TODO: check
  // List of parameters
  MVStrVector Args;
  Args.push_back(V.getOpA().getName());
  Args.push_back(V.getOpB().getName());

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.getResult(), Op, PrefS, SuffS, Args, SIMDType::VDIV, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vmod(VectorIR::VectorOP V) {
  SIMDBackEnd::SIMDInstListType IL;
  // TODO: generate preffix
  std::string PrefS = "";
  // TODO: generate suffix
  std::string SuffS = "";
  // TODO:
  std::string Op = "mod";

  // TODO: check
  // List of parameters
  MVStrVector Args;
  Args.push_back(V.getOpA().getName());
  Args.push_back(V.getOpB().getName());

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.getResult(), Op, PrefS, SuffS, Args, SIMDType::VMOD, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vfunc(VectorIR::VectorOP V) {
  SIMDBackEnd::SIMDInstListType IL;
  // TODO: generate preffix
  std::string PrefS = "";
  // TODO: generate suffix
  std::string SuffS = "";
  // TODO:
  std::string Op = V.getMVOp().toString();

  // TODO: check
  // List of parameters
  MVStrVector Args;
  Args.push_back(V.getOpA().getName());
  if (V.getOpB().getName() != "") {
    Args.push_back(V.getOpB().getName());
  }

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.getResult(), Op, PrefS, SuffS, Args, SIMDType::VFUNC, MVSL,
              &IL);

  return IL;
}

// ---------------------------------------------
MVStrVector AVX2BackEnd::getInitValues(VectorIR::VectorOP V) {
  MVStrVector InitVal;
  std::string NeutralValue = "0";
  if (V.isBinOp()) {
    switch (V.getBinOp()) {
    case clang::BO_Mul:
    case clang::BO_Div:
      NeutralValue = "1";
      break;
    default:
      break;
    }
  }

  auto VS = V.getResult().VSize;
  for (size_t i = 0; i < VS; ++i) {
    InitVal.push_back(NeutralValue);
  }

  MVStrVector InitValList(InitVal.begin(), InitVal.end());
  auto Reg = V.getResult().getName();
  // Fuck, this is awful...
  if (getAccmReg(Reg) != "") {
    Reg = getAccmReg(Reg);
  }
  MVSourceLocation MVSL(MVSourceLocation::Position::PREORDER,
                        V.getResult().Order, V.getResult().Offset);
  auto Op = "setzero";
  MVStrVector Values = {};
  if (V.isBinOp() &&
      ((V.getBinOp() == clang::BO_Mul) || (V.getBinOp() == clang::BO_Div))) {
    Op = "set";
    Values = InitValList;
  }
  InitReg.push_back(createSIMDInst(Op, Reg, getMapWidth(V.getResult().Width),
                                   getMapType(V.getResult().DType), "", "",
                                   Values, SIMDType::INIT, MVSL));
  InitVal[VS - 1] = V.getResult().UOP[0]->getRegisterValue();
  return InitVal;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vreduce(VectorIR::VectorOP V) {
  SIMDBackEnd::SIMDInstListType IL;
  SIMDBackEnd::SIMDInstListType TIL;
  auto RegType = getRegisterType(V.DT, V.VW);
  auto IsANewReduction = !hasAlreadyBeenMapped(V.getResult().getName());
  auto RegAccm = getNextAccmRegister(V.getResult().getName());
  if (IsANewReduction) {
    auto InitValues = getInitValues(V);
    SIMDBackEnd::addRegToDeclareInitVal(RegType, RegAccm, InitValues);
  }

  // FIXME: If there anything to accumulate yet?
  // Some needed values
  auto TmpReg = (V.getResult().getName() == V.getOpB().getName()) ? V.getOpA()
                                                                  : V.getOpB();
  std::string TmpReduxVar = TmpReg.getName();
  // if (TmpReg.RequiresRegisterPacking) {
  //   IL.splice(IL.end(), vregisterpacking(TmpReg));
  // }

  auto VCopy = V;
  VCopy.getResult().setName(RegAccm);
  VCopy.getOpA().setName(TmpReduxVar);
  VCopy.getOpB().setName(RegAccm);

  // Get type of binary operation, reduction is not the same if
  // addition/substraction than multiplication or division
  if (V.isBinOp()) {
    switch (V.getBinOp()) {
    case clang::BO_Add:
      TIL = vadd(VCopy);
      break;
    case clang::BO_Sub:
      TIL = vsub(VCopy);
      break;
    case clang::BO_Mul:
      TIL = vmul(VCopy);
      break;
    case clang::BO_Div:
      TIL = vdiv(VCopy);
      break;
    default:
      MVAssert(false, "Reduction must be from an addition, substraction, "
                      "multiplication or division");
    }
  } else {
    TIL = vfunc(VCopy);
  }
  IL.splice(IL.end(), TIL);

  // Need to fix this
  std::string ReduxVar = V.getResult().UOP[0]->getRegisterValue();

  MACVETH_DEBUG("AVX2Backend", "ReduxVar = " + ReduxVar);
  MACVETH_DEBUG("AVX2Backend", "Redux V.R = " + V.getResult().toString());
  MACVETH_DEBUG("AVX2Backend", "Redux V.OpA = " + V.getOpA().toString());
  MACVETH_DEBUG("AVX2Backend", "Redux V.OpB = " + V.getOpB().toString());

  /// This is a 'hack': when a reduction is found, instead of generate all
  /// the instructions needed for it, wait until the peephole optimization
  /// says if there are any other reductions that can be packed and fused
  /// together. P.S. creating hacks make me a hacker? lol
  // FIXME: get the latest order of all
  unsigned Order = V.Order;
  auto Offset = V.Offset;
  for (auto I : IL) {
    Order = std::max(Order, I.MVSL.getOrder());
  }
  MVSourceLocation MVSL(MVSourceLocation::Position::POSORDER, Order, Offset);

  genSIMDInst(V.getResult(), "VREDUX", "", "", {RegAccm}, SIMDType::VREDUC,
              MVSL, &IL, ReduxVar, "", {}, V.getMVOp());
  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vseq(VectorIR::VectorOP V) {
  SIMDBackEnd::SIMDInstListType IL;

  // Adding SIMD inst to the list
  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);
  addNonSIMDInst(V, SIMDType::VSEQ, MVSL, &IL);

  return IL;
}