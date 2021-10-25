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
#include "include/Utils.h"
#include <algorithm>
#include <map>
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
SIMDBackEnd::SIMDInst createSIMDInst(std::string Op, std::string Res,
                                     std::string Width, std::string DataType,
                                     std::string PrefS, std::string SuffS,
                                     std::list<std::string> Args,
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

  auto Res = Acc.Result;
  auto Width = getMapWidth(Mul.W);
  auto DataType = getMapType(Mul.DT);
  auto Op = (Acc.SType == SIMDType::VADD) ? "fmadd" : "fmsub";
  std::string PrefS = "";
  std::string SuffS = "";
  std::list<std::string> Args;

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
  Fuse = createSIMDInst(Op, Res, Width, DataType, PrefS, SuffS, Args,
                        SIMDBackEnd::SIMDType::VOPT, Acc.MVSL);
  Fuse.VOPResult = Acc.VOPResult;
  Fuse.MVSL = Acc.MVSL;

  return Fuse;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::fuseAddSubMult(SIMDBackEnd::SIMDInstListType I) {
  SIMDBackEnd::SIMDInstListType IL;
  SIMDBackEnd::SIMDInstListType PotentialFuse;
  SIMDBackEnd::SIMDInstListType SkipList;
  std::map<SIMDBackEnd::SIMDInst, SIMDBackEnd::SIMDInst> Fuses;

  // Search replacements
  for (auto Inst : I) {
    // If we find a multiply operation it may be a FMADD
    if ((Inst.SType == SIMDBackEnd::SIMDType::VMUL) &&
        ((Inst.DT == MVDataType::VDataType::FLOAT) ||
         (Inst.DT == MVDataType::VDataType::DOUBLE))) {
      PotentialFuse.push_back(Inst);
    }
    // If after the multiplication, there is a store, then this is not a
    // potential fuse anymore
    if (Inst.SType == SIMDBackEnd::SIMDType::VSTORE) {
      SIMDBackEnd::SIMDInstListType AuxL;
      for (auto P : PotentialFuse) {
        if (Inst.Result != P.Result) {
          AuxL.push_back(P);
        }
      }
      PotentialFuse = AuxL;
    }
    // Check if we have any add/sub adding the result of a previous
    // multiplication
    if ((Inst.SType == SIMDBackEnd::SIMDType::VADD) ||
        (Inst.SType == SIMDBackEnd::SIMDType::VSUB)) {
      for (auto P : PotentialFuse) {
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

// ---------------------------------------------
int getNumberOfReductions(VectorIR::VOperand V) {
  int NReductions = 1;
  if (V.Idx.size() == 0) {
    std::string PrevVal = V.UOP[0]->getRegisterValue();
    for (int T = 1; T < (int)V.Size; ++T) {
      if (PrevVal != V.UOP[T]->getRegisterValue()) {
        PrevVal = V.UOP[T]->getRegisterValue();
        NReductions++;
      }
    }
    return NReductions;
  }
  int PrevVal = V.Idx[0];
  for (int T = 1; T < (int)V.Size; ++T) {
    if (PrevVal != V.Idx[T]) {
      PrevVal = V.Idx[T];
      NReductions++;
    }
  }
  return NReductions;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::horizontalSingleVectorReduction(SIMDBackEnd::SIMDInstListType TIL,
                                             MVSourceLocation::Position Pos) {
  std::vector<SIMDBackEnd::SIMDInst> VIL{std::begin(TIL), std::end(TIL)};
  // int NElems = VIL[0].W / MVDataType::VDataTypeWidthBits[VIL[0].DT];
  auto V = VIL[0].VOPResult;
  auto NElems = V.VSize;
  int NReductions = getNumberOfReductions(V);

  // Horizontal operations
  SIMDBackEnd::SIMDInstListType IL;
  auto OpRedux = VIL[0].MVOP;
  auto OpName = VIL[0].MVOP.toString();
  auto Type = VIL[0].DT;
  auto OP = VIL[0].MVOP.toString();
  auto AccmReg = getAccmReg(V.Name);
  MVSourceLocation MVSL(Pos, V.Order, V.Offset);
  std::string LoRes = "__mv_lo128";
  std::string HiRes = "__mv_hi128";
  if (NReductions == 1) {
    auto RegType = getRegisterType(Type, MVDataType::VWidth::W128);
    SIMDBackEnd::addRegToDeclare(RegType, LoRes, {0});
    SIMDBackEnd::addRegToDeclare(RegType, HiRes, {0});
    if (Type == MVDataType::VDataType::DOUBLE) {
      // If 4 elements
      if (NElems > 2) {
        // __m128d lo = _mm256_castpd256_pd128(v); zero latency
        genSIMDInst(LoRes, "castpd", "256", "pd128", MVDataType::VWidth::W256,
                    Type, {AccmReg}, SIMDType::VOPT, MVSL, &IL);
        // __m128d hi = _mm256_extractf128_pd(v,0x1);
        genSIMDInst(HiRes, "extract", "", "f128", MVDataType::VWidth::W256,
                    Type, {AccmReg, "0x1"}, SIMDType::VOPT, MVSL, &IL);
        // lo = op(lo,hi);
        genSIMDInst(LoRes, OpName, "", "", MVDataType::VWidth::W128, Type,
                    {LoRes, HiRes}, SIMDType::VOPT, MVSL, &IL);
      } else {
        LoRes = AccmReg;
      }
      // When only 2 elements
      // hi = _mm_shuffle_pd(lo,lo,0x1);
      genSIMDInst(HiRes, "shuffle", "", "", MVDataType::VWidth::W128, Type,
                  {LoRes, LoRes, "0x1"}, SIMDType::VPERM, MVSL, &IL);
      // val = _mm_cvtsd_f64(op(lo,hi));
      genSIMDInst(LoRes, OpName, "", "", MVDataType::VWidth::W128, Type,
                  {LoRes, HiRes}, SIMDType::VOPT, MVSL, &IL);
    } else if (Type == MVDataType::VDataType::FLOAT) {
      // 8 elements
      if (NElems > 4) {
        // __m128 lo = cast()
        genSIMDInst(LoRes, "castps", "256", "ps128", MVDataType::VWidth::W256,
                    Type, {AccmReg}, SIMDType::VOPT, MVSL, &IL);
        // __m128 lo = cast(v)
        genSIMDInst(HiRes, "extract", "", "f128", MVDataType::VWidth::W256,
                    Type, {AccmReg, "0x1"}, SIMDType::VOPT, MVSL, &IL);

        // lo = op(lo,hi);
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
        // lo = op(lo,hi);
        genSIMDInst(LoRes, OpName, "", "", MVDataType::VWidth::W128, Type,
                    {LoRes, HiRes}, SIMDType::VOPT, MVSL, &IL);
      }
      // 2 elements
      // hi = shuffle(lo);
      genSIMDInst(HiRes, "shuffle", "", "", MVDataType::VWidth::W128, Type,
                  {LoRes, LoRes, "0b00001110"}, SIMDType::VPERM, MVSL, &IL);
      // lo = op(lo,hi);
      genSIMDInst(LoRes, OpName, "", "", MVDataType::VWidth::W128, Type,
                  {LoRes, HiRes}, SIMDType::VOPT, MVSL, &IL);

    } else {
      // FIXME: should we implement this for integer arithmetic?
      MVErr("Not implemented this approach yet for other types than float or "
            "doubles");
    }

  } else {
    LoRes = "__mv_lo256";
    HiRes = "__mv_hi256";
    auto RegType = getRegisterType(Type, MVDataType::VWidth::W256);
    SIMDBackEnd::addRegToDeclare(RegType, LoRes, {0});
    SIMDBackEnd::addRegToDeclare(RegType, HiRes, {0});
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
        LoRes = "__tmp1_128";
        SIMDBackEnd::addRegToDeclare(RegType, TmpReg0, {0});
        SIMDBackEnd::addRegToDeclare(RegType, TmpReg1, {0});
        genSIMDInst(TmpReg0, "permute", "", "", MVDataType::VWidth::W128, Type,
                    {AccmReg, "0b00001110"}, SIMDType::VPERM, MVSL, &IL);
        genSIMDInst(TmpReg1, "add", "", "", MVDataType::VWidth::W128, Type,
                    {AccmReg, TmpReg0}, SIMDType::VADD, MVSL, &IL);
      }
    } else {
      if (NElems > 2) {
        auto TmpReg0 = "__tmp0_256";
        auto TmpReg1 = "__tmp1_256";
        LoRes = "__tmp1_256";
        SIMDBackEnd::addRegToDeclare(RegType, TmpReg0, {0});
        SIMDBackEnd::addRegToDeclare(RegType, TmpReg1, {0});
        genSIMDInst(TmpReg0, "permute", "", "2f128", MVDataType::VWidth::W256,
                    Type, {AccmReg, AccmReg, "0b10000001"}, SIMDType::VPERM,
                    MVSL, &IL);
        genSIMDInst(TmpReg1, "add", "", "", MVDataType::VWidth::W256, Type,
                    {AccmReg, TmpReg0}, SIMDType::VADD, MVSL, &IL);
      }
    }
  }

  // Perfect cases:
  // * [A,A,B,B,C,C,D,D]
  // * [A,A,A,A,C,C,C,C]
  // * [A,A,A,A,A,A,A,A]
  if ((NReductions == 4) || (NReductions == 2) || (NReductions == 1)) {
    int Stride = 1 + ((NElems - 1) / NReductions);
    unsigned UB = NElems;
    if (NElems < 4) {
      UB = NElems > 1 ? (NElems - 1) : NElems;
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
  // MACVETH_DEBUG("AVX2BackEnd", "Stride = " + std::to_string(Stride));
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
      auto PermS = "2f128";
      auto Mask2 = "0x21";
      auto Perm = replacePatterns("permute", W, MapType[VIL[0].DT], "", PermS);
      auto V0 = VAccm[0];
      auto V1 = (NumRedux > 4)   ? VAccm[4]
                : (NumRedux > 2) ? VAccm[2]
                                 : VAccm[1];
      auto Op1 = genGenericFunc(Blend, {V0, V1, Mask1});
      auto Op2 = genGenericFunc(Perm, {V0, V1, Mask2});

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
std::list<std::string> AVX2BackEnd::renderSIMDRegister(SIMDInstListType S) {
  std::list<std::string> L;
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
      Res = getNextAuxRegister(VIL[R].VOPResult.Name);
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
  auto RAW = this->getCDAG()->getRAWs();
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
  auto LastAccm = I.Result;
  auto RAW = this->getCDAG()->getRAWs();
  for (auto Ins : L) {
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
  for (auto I : TIL) {
    if (I.isReduction()) {
      auto S = I.VOPResult.Size;
      auto L = LRedux[S];
      if (reductionIsContiguous(L, I)) {
        SIMDBackEnd::SIMDInstListType Aux;
        // FIXME: isn't a better way of doing this????
        for (auto R : L) {
          if (R.Result != I.Result) {
            Aux.push_back(R);
          }
        }
        LRedux[S] = Aux;
      } else if (hasRawDependencies(L, I)) {
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
        continue;
      }
      auto LInsSize = L.second.size();
      auto W = L.second.front().VOPResult.Width;
      auto DW =
          MVDataType::VDataTypeWidthBits[L.second.front().VOPResult.DType];
      // Only compute reductions if max found
      if ((unsigned)W / DW > LInsSize) {
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
  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::peepholeOptimizations(SIMDBackEnd::SIMDInstListType I) {
  SIMDBackEnd::SIMDInstListType IL(I);

  // Fuse reductions if any and fuse them
  IL = fuseReductions(IL);
  // Fuse operations: find potential and applicable FMADD/FMSUB, if not
  // disabled
  if (!MVOptions::DisableFMA) {
    IL = fuseAddSubMult(IL);
  }

  // TODO: eliminate duplicated instructions (?)
  // IL = eliminateDeadIns();

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
SIMDBackEnd::SIMDInst AVX2BackEnd::genSIMDInst(
    std::string Result, std::string Op, std::string PrefS, std::string SuffS,
    MVDataType::VWidth Width, MVDataType::VDataType Type,
    std::list<std::string> Args, SIMDBackEnd::SIMDType SType,
    MVSourceLocation SL, SIMDBackEnd::SIMDInstListType *IL, std::string NameOp,
    std::string MVFunc, std::list<std::string> MVArgs, MVOp MVOP) {

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
SIMDBackEnd::SIMDInst AVX2BackEnd::genSIMDInst(
    VectorIR::VOperand V, std::string Op, std::string PrefS, std::string SuffS,
    std::list<std::string> Args, SIMDBackEnd::SIMDType SType,
    MVSourceLocation SL, SIMDBackEnd::SIMDInstListType *IL, std::string NameOp,
    std::string MVFunc, std::list<std::string> MVArgs, MVOp MVOP) {

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
  std::string Mask = (V.IsPartial) && (V.Size != 2) ? "mask" : "";

  // Type of load: load/u
  auto Op = Mask + "load";

  // List of parameters
  std::list<std::string> Args;

  // Hack: if data type is integer then use the si128/si256 notation instead
  // of epi
  if (V.DType > MVDataType::VDataType::SFLOAT) {
    V.DType = (V.VSize > 4) ? MVDataType::VDataType::UNDEF256
                            : MVDataType::VDataType::UNDEF128;
    std::string Cast = getRegisterType(V.DType, V.Width);
    Args.push_back("(" + Cast + "*)" + getOpName(V, true, true));
  } else {
    Args.push_back(getOpName(V, true, true));
  }

  if (V.IsPartial) {
    if (V.Size == 2) {
      Args.push_front(V.Name);
      SuffS += (V.LowBits) ? "l" : "";
      SuffS += (V.HighBits) ? "h" : "";
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

  // Adding SIMD inst to the list
  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);
  genSIMDInst(V, Op, "", SuffS, Args, SIMDType::VPACK, MVSL, &IL);

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
  std::list<std::string> Args;
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
  std::list<std::string> Args;

  // To gather elements
  // Generate preffix: must be i32 or i64, depending on the VIndex width
  auto Scale = "4";
  auto VIndexSuffix = "epi32";
  auto PrefGather = "i32";

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

  auto MapWidthSet = MapWidth[MVDataType::VWidth::W256];
  if (((V.getDataType() == MVDataType::VDataType::FLOAT) && (V.VSize == 4)) ||
      ((V.getDataType() == MVDataType::VDataType::DOUBLE) && (V.VSize == 2))) {
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
  for (int Idx = 1; Idx < (int)V.Size; ++Idx) {
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
VectorIR::VOperand getSubVOperand(VectorIR::VOperand V, std::string Name,
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
bool AVX2BackEnd::vpack4elements(VectorIR::VOperand V, MVDataType::VWidth Width,
                                 SIMDBackEnd::SIMDInstListType *IL) {
  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);
  std::string TypeReg = (V.DType == MVDataType::FLOAT) ? "__m128" : "__m256d";
  auto SingleType = (V.DType == MVDataType::FLOAT) ? MVDataType::HALF_FLOAT
                                                   : MVDataType::DOUBLE;
  unsigned Half = (int)V.VSize / 2;
  bool FullVector = false;
  bool FirstHalve = false;
  bool SecondHalve = false;
  auto Ranges = getContiguityRanges(V);
  for (auto Range : Ranges) {
    FullVector |= ((std::get<0>(Range) == 0) &&
                   ((unsigned)std::get<1>(Range) == (V.VSize - 1)));
    FirstHalve |= ((std::get<0>(Range) == 0) &&
                   ((unsigned)std::get<1>(Range) == (Half - 1)));
    SecondHalve |= (((unsigned)std::get<0>(Range) == (Half)) &&
                    ((unsigned)std::get<1>(Range) == (2 * Half - 1)));
  }

  if (FullVector) {
    auto NewVOp =
        getSubVOperand(V, V.Name, V.VSize, V.VSize, V.Width, V.DType, 0);
    SIMDBackEnd::addRegToDeclare(TypeReg, V.Name, {0});
    IL->splice(IL->end(), vload(NewVOp));
    return true;
  }

  if (V.DType == MVDataType::DOUBLE) {
    if (FirstHalve && SecondHalve) {
      V.ContiguousHalves = true;
      IL->splice(IL->end(), vload(V));
      return true;
    }
    return false;
  }

  SIMDBackEnd::addRegToDeclare(TypeReg, "__lo128", {0});
  auto WidthHalve = (V.DType == MVDataType::FLOAT) ? MVDataType::VWidth::W128
                                                   : MVDataType::VWidth::W256;
  // Fullfill all vector with size 4
  if (FirstHalve) {
    std::string Name = (FirstHalve && SecondHalve) ? V.Name : "__lo128";
    VectorIR::VOperand NewVOp =
        getSubVOperand(V, Name, V.VSize, Half, WidthHalve, SingleType, 0);
    SIMDBackEnd::addRegToDeclare(TypeReg, Name, {0});
    IL->splice(IL->end(), vload(NewVOp));
  }
  if (SecondHalve) {
    std::string Name = (FirstHalve && SecondHalve) ? V.Name : "__lo128";
    VectorIR::VOperand NewVOp =
        getSubVOperand(V, Name, V.VSize, Half, WidthHalve, SingleType, Half);
    SIMDBackEnd::addRegToDeclare(TypeReg, Name, {0});
    IL->splice(IL->end(), vload(NewVOp));
  }

  if ((FirstHalve && SecondHalve) || (FirstHalve && V.VSize == 2)) {
    return true;
  }

  VectorIR::VOperand NewVOp = V;
  NewVOp.Width = Width;
  SIMDBackEnd::addRegToDeclare(TypeReg, "__tmp0", {0});
  SIMDBackEnd::addRegToDeclare(TypeReg, "__tmp1", {0});
  SIMDBackEnd::addRegToDeclare(TypeReg, "__lo128", {0});
  SIMDBackEnd::addRegToDeclare(TypeReg, "__hi128", {0});
  // One stride of 2 at the beggining of at the end
  if (FirstHalve && !SecondHalve && V.VSize > 2) {
    // [C,C+1,X,Y]
    NewVOp.Name = "__tmp0";
    loads(NewVOp, {getOpName(V, true, true, 2)}, MVSL, IL);
    NewVOp.Name = V.Name;
    insert(NewVOp, {"__lo128", "__tmp0", "0b00100000"}, MVSL, IL);
    NewVOp.Name = "__tmp1";
    loads(NewVOp, {getOpName(V, true, true, 3)}, MVSL, IL);
    NewVOp.Name = V.Name;
    insert(NewVOp, {NewVOp.Name, "__tmp1", "0b00110000"}, MVSL, IL);
    return true;
  }
  if (!FirstHalve && SecondHalve && V.VSize > 2) {
    // [X,Y,C,C+1]
    NewVOp.Name = "__tmp0";
    loads(NewVOp, {getOpName(V, true, true, 0)}, MVSL, IL);
    NewVOp.Name = V.Name;
    moves(NewVOp, {"__lo128", "__tmp0"}, MVSL, IL);
    NewVOp.Name = "__tmp1";
    loads(NewVOp, {getOpName(V, true, true, 1)}, MVSL, IL);
    NewVOp.Name = V.Name;
    insert(NewVOp, {NewVOp.Name, "__tmp1", "0b00010000"}, MVSL, IL);
    return true;
  }
  // Strides of 3, or 2 in the middle
  if (!FirstHalve && !SecondHalve) {
    NewVOp.DType = V.DType;
    NewVOp.Name = V.Name;
    for (auto Range : Ranges) {
      // [C,C+1,C+2,X]
      if ((std::get<0>(Range) == 0) && (std::get<1>(Range) == (2))) {
        load(NewVOp, {getOpName(V, true, true, 0)}, MVSL, IL);
        NewVOp.Name = "__tmp0";
        loads(NewVOp, {getOpName(V, true, true, 3)}, MVSL, IL);
        NewVOp.Name = V.Name;
        insert(NewVOp, {V.Name, "__tmp0", "0b00110000"}, MVSL, IL);
        return true;
      }
      // [X,C,C+1,C+2]
      if ((std::get<0>(Range) == 1) && (std::get<1>(Range) == (3))) {
        NewVOp.Name = V.Name;
        load(NewVOp, {getOpName(V, true, true, 1, -1)}, MVSL, IL);
        NewVOp.Name = "__tmp0";
        loads(NewVOp, {getOpName(V, true, true, 0)}, MVSL, IL);
        NewVOp.Name = V.Name;
        moves(NewVOp, {V.Name, "__tmp0"}, MVSL, IL);
        return true;
      }
      // [X,C,C+1,Y]
      if ((std::get<0>(Range) == 1) && (std::get<1>(Range) == (2))) {
        IL->splice(IL->end(), vgather(V));
        // NewVOp.Name = V.Name;
        // load(NewVOp, {getOpName(V, true, true, 1, -1)}, MVSL, IL);
        // NewVOp.Name = "__tmp0";
        // loads(NewVOp, {getOpName(V, true, true, 0)}, MVSL, IL);
        // NewVOp.Name = V.Name;
        // moves(NewVOp, {V.Name, "__tmp0"}, MVSL, IL);
        // NewVOp.Name = "__tmp0";
        // loads(NewVOp, {getOpName(V, true, true, 3)}, MVSL, IL);
        // NewVOp.Name = V.Name;
        // insert(NewVOp, {V.Name, "__tmp0", "0b00110000"}, MVSL, IL);
        return true;
      }
    }
  }
  return false;
}

// ---------------------------------------------
void AVX2BackEnd::store(VectorIR::VOperand V, std::list<std::string> Args,
                        MVSourceLocation MVSL,
                        SIMDBackEnd::SIMDInstListType *IL) {
  genSIMDInst(V, "storeu", "", "", Args, SIMDType::VSTORE, MVSL, IL);
}

// ---------------------------------------------
void AVX2BackEnd::load(VectorIR::VOperand V, std::list<std::string> Args,
                       MVSourceLocation MVSL,
                       SIMDBackEnd::SIMDInstListType *IL) {
  genSIMDInst(V, "loadu", "", "", Args, SIMDType::VPACK, MVSL, IL);
}

// ---------------------------------------------
void AVX2BackEnd::loads(VectorIR::VOperand V, std::list<std::string> Args,
                        MVSourceLocation MVSL,
                        SIMDBackEnd::SIMDInstListType *IL) {
  V.DType = (V.DType == MVDataType::FLOAT) ? MVDataType::VDataType::SFLOAT
                                           : MVDataType::VDataType::SDOUBLE;
  V.Width = MVDataType::VWidth::W128;
  genSIMDInst(V, "load", "", "", Args, SIMDType::VPACK, MVSL, IL);
}

// ---------------------------------------------
void AVX2BackEnd::moves(VectorIR::VOperand V, std::list<std::string> Args,
                        MVSourceLocation MVSL,
                        SIMDBackEnd::SIMDInstListType *IL) {
  V.DType = (V.DType == MVDataType::FLOAT) ? MVDataType::VDataType::SFLOAT
                                           : MVDataType::VDataType::SDOUBLE;
  V.Width = MVDataType::VWidth::W128;
  genSIMDInst(V, "move", "", "", Args, SIMDType::VPACK, MVSL, IL);
}

// ---------------------------------------------
void AVX2BackEnd::blend(VectorIR::VOperand V, std::list<std::string> Args,
                        MVSourceLocation MVSL,
                        SIMDBackEnd::SIMDInstListType *IL) {
  genSIMDInst(V, "blend", "", "", Args, SIMDType::VPACK, MVSL, IL);
}

// ---------------------------------------------
void AVX2BackEnd::insert(VectorIR::VOperand V, std::list<std::string> Args,
                         MVSourceLocation MVSL,
                         SIMDBackEnd::SIMDInstListType *IL) {
  genSIMDInst(V, "insert", "", "", Args, SIMDType::VPACK, MVSL, IL);
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vpack(VectorIR::VOperand V) {
  // This is where magic happens.
  // This is a heavy and probably buggy function.
  auto Ranges = getContiguityRanges(V);
  // This is better implemented as a gather, since there is no contiguity in
  // retrieving operands
  if (Ranges.size() == V.VSize) {
    return vgather(V);
  }
  SIMDBackEnd::SIMDInstListType IL;
  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  int Half = (int)V.VSize / 2;
  bool FiveElements = false;
  bool SixElements = false;
  bool SevenElements = false;
  bool ThreeThreeElems =
      (Ranges.size() >= 2) && (std::get<0>(Ranges[0]) == 0) &&
      (std::get<1>(Ranges[0]) == 2) && (std::get<0>(Ranges[1]) == 3) &&
      (std::get<1>(Ranges[1]) == 5);

  for (auto Range : Ranges) {
    FiveElements |= ((std::get<0>(Range) + 4 == std::get<1>(Range)));
    SixElements |= ((std::get<0>(Range) + 5 == std::get<1>(Range)));
    SevenElements |= ((std::get<0>(Range) + 6 == std::get<1>(Range)));
  }

  if (V.Size > 4) {
    std::string TypeReg128 =
        (V.DType == MVDataType::FLOAT) ? "__m128" : "__m128d";
    std::string TypeReg256 =
        (V.DType == MVDataType::FLOAT) ? "__m256" : "__m256d";
    VectorIR::VOperand NewVOp = V;
    NewVOp.Width = V.Width;
    SIMDBackEnd::addRegToDeclare(TypeReg128, "__tmp0_128", {0});
    SIMDBackEnd::addRegToDeclare(TypeReg128, "__tmp1_128", {0});
    SIMDBackEnd::addRegToDeclare(TypeReg256, "__tmp0_256", {0});
    SIMDBackEnd::addRegToDeclare(TypeReg256, "__tmp1_256", {0});

    // 7 contiguous elements
    if (SevenElements) {
      for (auto Range : Ranges) {
        // [X|YYYYYYY]
        if ((std::get<0>(Range) == 1) && (std::get<1>(Range) == (7))) {
          load(NewVOp, {getOpName(V, true, true, 1, -1)}, MVSL, &IL);
          NewVOp.Name = "__tmp0_128";
          loads(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
          NewVOp.Name = V.Name;
          blend(NewVOp,
                {V.Name, "_mm256_castps128_ps256(__tmp0_128)", "0b00000001"},
                MVSL, &IL);
          return IL;
        }
        // [YYYYYYY|X]
        if ((std::get<0>(Range) == 0) && (std::get<1>(Range) == (6))) {
          load(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
          NewVOp.Name = "__tmp0_256";
          load(NewVOp, {getOpName(V, true, true, 7, -7)}, MVSL, &IL);
          NewVOp.Name = V.Name;
          blend(NewVOp, {V.Name, "__tmp0_256", "0b10000000"}, MVSL, &IL);
          return IL;
        }
      }
    }

    // 6 contiguous elements
    if (SixElements) {
      for (auto Range : Ranges) {
        if ((std::get<0>(Range) == 0) && (std::get<1>(Range) == (5))) {
          load(NewVOp, {getOpName(V, true, true, 0, 0)}, MVSL, &IL);
          NewVOp.Name = "__tmp0_256";
          load(NewVOp, {getOpName(V, true, true, 6, -6)}, MVSL, &IL);
          NewVOp.Name = V.Name;
          for (auto InnerRange : Ranges) {
            if ((std::get<0>(InnerRange) == 0) &&
                (std::get<1>(InnerRange) == (5))) {
              continue;
            }
            if ((std::get<0>(InnerRange) == 6) &&
                (std::get<1>(InnerRange) == (7))) {
              // [XXXXXX|YY]
              blend(NewVOp, {V.Name, "__tmp0_256", "0b11000000"}, MVSL, &IL);
            } else {
              // [XXXXXX|Y|Y]
              blend(NewVOp, {V.Name, "__tmp0_256", "0b01000000"}, MVSL, &IL);
              NewVOp.Name = "__tmp0_256";
              load(NewVOp, {getOpName(V, true, true, 7, -7)}, MVSL, &IL);
              NewVOp.Name = V.Name;
              blend(NewVOp, {V.Name, "__tmp0_256", "0b10000000"}, MVSL, &IL);
            }
            return IL;
          }
        }
        if ((std::get<0>(Range) == 1) && (std::get<1>(Range) == (6))) {
          // [Y|XXXXXX|Y]
          NewVOp.Name = V.Name;
          load(NewVOp, {getOpName(V, true, true, 1, -1)}, MVSL, &IL);
          NewVOp.Name = "__tmp0_256";
          load(NewVOp, {getOpName(V, true, true, 7, -7)}, MVSL, &IL);
          blend(NewVOp, {V.Name, "__tmp0_256", "0b10000000"}, MVSL, &IL);
          NewVOp.Name = "__tmp0_128";
          loads(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
          NewVOp.Name = V.Name;
          blend(NewVOp,
                {V.Name, "_mm256_castps128_ps256(__tmp0_128)", "0b00000001"},
                MVSL, &IL);
          return IL;
        }
        if ((std::get<0>(Range) == 2) && (std::get<1>(Range) == (7))) {
          load(NewVOp, {getOpName(V, true, true, 2, -2)}, MVSL, &IL);
          for (auto InnerRange : Ranges) {
            if ((std::get<0>(InnerRange) == 2) &&
                (std::get<1>(InnerRange) == (7))) {
              continue;
            }
            if ((std::get<0>(InnerRange) == 0) &&
                (std::get<1>(InnerRange) == (1))) {
              // [YY|XXXXXX]
              NewVOp.Name = "__tmp0_128";
              loads(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
              NewVOp.Name = V.Name;
              blend(
                  NewVOp,
                  {V.Name, "_mm256_castps128_ps256(__tmp0_128)", "0b00000011"},
                  MVSL, &IL);
            } else {
              // [Y|Y|XXXXXX]
              NewVOp.Name = "__tmp0_128";
              loads(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
              NewVOp.Name = V.Name;
              blend(
                  NewVOp,
                  {V.Name, "_mm256_castps128_ps256(__tmp0_128)", "0b00000001"},
                  MVSL, &IL);
              NewVOp.Name = "__tmp0_128";
              NewVOp.Width = MVDataType::VWidth::W128;
              load(NewVOp, {getOpName(V, true, true, 1, -1)}, MVSL, &IL);
              NewVOp.Name = V.Name;
              NewVOp.Width = V.Width;
              blend(
                  NewVOp,
                  {V.Name, "_mm256_castps128_ps256(__tmp0_128)", "0b00000010"},
                  MVSL, &IL);
            }
            return IL;
          }
        }
      }
    }

    // 5 contiguous elements
    if (FiveElements) {
      for (auto Range : Ranges) {
        if ((std::get<0>(Range) == 0) && (std::get<1>(Range) == (4))) {
          load(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
          for (auto InnerRange : Ranges) {
            if (((std::get<0>(InnerRange) == 0) &&
                 (std::get<1>(InnerRange) == (4))) ||
                ((std::get<0>(InnerRange) == 5) &&
                 (std::get<1>(InnerRange) == (5)))) {
              continue;
            }
            if ((std::get<0>(InnerRange) == 5) &&
                (std::get<1>(InnerRange) == (7))) {
              // [XXXXX|YYY]
              NewVOp.Name = "__tmp0_256";
              load(NewVOp, {getOpName(V, true, true, 5, -5)}, MVSL, &IL);
              NewVOp.Name = V.Name;
              blend(NewVOp, {V.Name, "__tmp0_256", "0b11100000"}, MVSL, &IL);
            } else if ((std::get<0>(InnerRange) == 6) &&
                       (std::get<1>(InnerRange) == (7))) {
              return vgather(V);
              // [XXXXX|Y|YY]
              // NewVOp.Name = "__tmp0_256";
              // load(NewVOp, {getOpName(V, true, true, 5, -5)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // blend(NewVOp, {V.Name, "__tmp0_256", "0b00100000"}, MVSL, &IL);
              // NewVOp.Name = "__tmp0_256";
              // load(NewVOp, {getOpName(V, true, true, 6, -6)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // blend(NewVOp, {V.Name, "__tmp0_256", "0b11000000"}, MVSL, &IL);
            } else if ((std::get<0>(InnerRange) == 5) &&
                       (std::get<1>(InnerRange) == (6))) {
              return vgather(V);
              // [XXXXX|YY|Y]
              // NewVOp.Name = "__tmp0_256";
              // load(NewVOp, {getOpName(V, true, true, 5, -5)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // blend(NewVOp, {V.Name, "__tmp0_256", "0b01100000"}, MVSL, &IL);
              // NewVOp.Name = "__tmp0_256";
              // load(NewVOp, {getOpName(V, true, true, 7, -7)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // blend(NewVOp, {V.Name, "__tmp0_256", "0b10000000"}, MVSL, &IL);
            } else {
              return vgather(V);
              // [XXXXX|Y|Y|Y]
              // NewVOp.Name = "__tmp0_256";
              // load(NewVOp, {getOpName(V, true, true, 5, -5)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // blend(NewVOp, {V.Name, "__tmp0_256", "0b00100000"}, MVSL, &IL);
              // NewVOp.Name = "__tmp0_256";
              // load(NewVOp, {getOpName(V, true, true, 6, -6)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // blend(NewVOp, {V.Name, "__tmp0_256", "0b01000000"}, MVSL, &IL);
              // NewVOp.Name = "__tmp0_256";
              // load(NewVOp, {getOpName(V, true, true, 7, -7)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // blend(NewVOp, {V.Name, "__tmp0_256", "0b10000000"}, MVSL, &IL);
            }
            return IL;
          }
        }
        if ((std::get<0>(Range) == 1) && (std::get<1>(Range) == (5))) {
          load(NewVOp, {getOpName(V, true, true, 1, -1)}, MVSL, &IL);
          NewVOp.Name = "__tmp0_128";
          loads(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
          NewVOp.Name = V.Name;
          blend(NewVOp,
                {V.Name, "_mm256_castps128_ps256(__tmp0_128)", "0b00000001"},
                MVSL, &IL);
          for (auto InnerRange : Ranges) {
            if (((std::get<0>(InnerRange) == 1) &&
                 (std::get<1>(InnerRange) == (5))) ||
                ((std::get<0>(InnerRange) == 0) &&
                 (std::get<1>(InnerRange) == (0)))) {
              continue;
            }
            if ((std::get<0>(InnerRange) == 6) &&
                (std::get<1>(InnerRange) == (7))) {
              // [Y|XXXXX|YY]
              NewVOp.Name = "__tmp0_256";
              load(NewVOp, {getOpName(V, true, true, 6, -6)}, MVSL, &IL);
              NewVOp.Name = V.Name;
              blend(NewVOp, {V.Name, "__tmp0_256", "0b11000000"}, MVSL, &IL);
            } else {
              return vgather(V);
              // [Y|XXXXX|Y|Y]
              // NewVOp.Name = "__tmp0_256";
              // load(NewVOp, {getOpName(V, true, true, 6, -6)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // blend(NewVOp, {V.Name, "__tmp0_256", "0b01000000"}, MVSL, &IL);
              // NewVOp.Name = "__tmp0_256";
              // load(NewVOp, {getOpName(V, true, true, 7, -7)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // blend(NewVOp, {V.Name, "__tmp0_256", "0b10000000"}, MVSL, &IL);
            }
            return IL;
          }
        }
        if ((std::get<0>(Range) == 2) && (std::get<1>(Range) == (6))) {
          load(NewVOp, {getOpName(V, true, true, 2, -2)}, MVSL, &IL);
          for (auto InnerRange : Ranges) {
            if ((std::get<0>(InnerRange) == 2) &&
                (std::get<1>(InnerRange) == (6))) {
              continue;
            }
            if ((std::get<0>(InnerRange) == 0) &&
                (std::get<1>(InnerRange) == (1))) {
              // [YY|XXXXX|Y]
              NewVOp.Name = "__tmp0_128";
              NewVOp.Width = MVDataType::VWidth::W128;
              load(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
              NewVOp.Name = V.Name;
              NewVOp.Width = V.Width;
              blend(
                  NewVOp,
                  {V.Name, "_mm256_castps128_ps256(__tmp0_128)", "0b00000011"},
                  MVSL, &IL);
            } else {
              return vgather(V);
              // [Y|Y|XXXXX|Y]
              // NewVOp.Name = "__tmp0_128";
              // loads(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // moves(NewVOp,
              //       {"_mm256_castps256_ps128(" + V.Name + ")", "__tmp0_128"},
              //       MVSL, &IL);
              // NewVOp.Name = "__tmp0_128";
              // NewVOp.Width = MVDataType::VWidth::W128;
              // load(NewVOp, {getOpName(V, true, true, 1, -1)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // NewVOp.Width = V.Width;
              // blend(
              //     NewVOp,
              //     {V.Name, "_mm256_castps128_ps256(__tmp0_128)",
              //     "0b00000010"}, MVSL, &IL);
            }
            NewVOp.Name = "__tmp0_256";
            load(NewVOp, {getOpName(V, true, true, 7, -7)}, MVSL, &IL);
            NewVOp.Name = V.Name;
            blend(NewVOp, {V.Name, "__tmp0_256", "0b10000000"}, MVSL, &IL);
            return IL;
          }
        }
        if ((std::get<0>(Range) == 3) && (std::get<1>(Range) == (7))) {
          load(NewVOp, {getOpName(V, true, true, 3, -3)}, MVSL, &IL);
          for (auto InnerRange : Ranges) {
            if (((std::get<0>(InnerRange) == 3) &&
                 (std::get<1>(InnerRange) == (7))) ||
                (((std::get<0>(InnerRange) == 0) &&
                  (std::get<1>(InnerRange) == (0))))) {
              continue;
            }
            if ((std::get<0>(InnerRange) == 0) &&
                (std::get<1>(InnerRange) == (2))) {
              // [YYY|XXXXX]
              NewVOp.Name = "__tmp0_128";
              NewVOp.Width = MVDataType::VWidth::W128;
              load(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
              NewVOp.Name = V.Name;
              NewVOp.Width = V.Width;
              blend(
                  NewVOp,
                  {V.Name, "_mm256_castps128_ps256(__tmp0_128)", "0b00000111"},
                  MVSL, &IL);
            } else if ((std::get<0>(InnerRange) == 1) &&
                       (std::get<1>(InnerRange) == (2))) {
              // [Y|YY|XXXXX]
              return vgather(V);
              // NewVOp.Name = "__tmp0_128";
              // loads(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // blend(
              //     NewVOp,
              //     {V.Name, "_mm256_castps128_ps256(__tmp0_128)",
              //     "0b00000001"}, MVSL, &IL);
              // NewVOp.Name = "__tmp0_128";
              // NewVOp.Width = MVDataType::VWidth::W128;
              // load(NewVOp, {getOpName(V, true, true, 2, -2)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // NewVOp.Width = V.Width;
              // blend(
              //     NewVOp,
              //     {V.Name, "_mm256_castps128_ps256(__tmp0_128)",
              //     "0b00000110"}, MVSL, &IL);
            } else if ((std::get<0>(InnerRange) == 0) &&
                       (std::get<1>(InnerRange) == (1))) {
              // [YY|Y|XXXXX]
              return vgather(V);
              // NewVOp.Name = "__tmp0_128";
              // NewVOp.Width = MVDataType::VWidth::W128;
              // load(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // NewVOp.Width = V.Width;
              // blend(
              //     NewVOp,
              //     {V.Name, "_mm256_castps128_ps256(__tmp0_128)",
              //     "0b00000011"}, MVSL, &IL);
              // NewVOp.Name = "__tmp0_128";
              // NewVOp.Width = MVDataType::VWidth::W128;
              // load(NewVOp, {getOpName(V, true, true, 2, -2)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // NewVOp.Width = V.Width;
              // blend(
              //     NewVOp,
              //     {V.Name, "_mm256_castps128_ps256(__tmp0_128)",
              //     "0b00000100"}, MVSL, &IL);
            } else {
              return vgather(V);
              // [Y|Y|Y|XXXXX]
              // NewVOp.Name = "__tmp0_128";
              // loads(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // moves(NewVOp,
              //       {"_mm256_castps256_ps128(" + V.Name + ")", "__tmp0_128"},
              //       MVSL, &IL);
              // NewVOp.Name = "__tmp0_128";
              // NewVOp.Width = MVDataType::VWidth::W128;
              // load(NewVOp, {getOpName(V, true, true, 1, -1)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // NewVOp.Width = V.Width;
              // blend(
              //     NewVOp,
              //     {V.Name, "_mm256_castps128_ps256(__tmp0_128)",
              //     "0b00000010"}, MVSL, &IL);
              // NewVOp.Name = "__tmp0_128";
              // NewVOp.Width = MVDataType::VWidth::W128;
              // load(NewVOp, {getOpName(V, true, true, 2, -2)}, MVSL, &IL);
              // NewVOp.Name = V.Name;
              // NewVOp.Width = V.Width;
              // blend(
              //     NewVOp,
              //     {V.Name, "_mm256_castps128_ps256(__tmp0_128)",
              //     "0b00000100"}, MVSL, &IL);
            }
            return IL;
          }
        }
      }
    }

    // 3+3+X elements
    if (ThreeThreeElems) {
      load(NewVOp, {getOpName(V, true, true)}, MVSL, &IL);
      NewVOp.Name = "__tmp0_256";
      load(NewVOp, {getOpName(V, true, true, 3, -3)}, MVSL, &IL);
      NewVOp.Name = V.Name;
      blend(NewVOp, {V.Name, "__tmp0_256", "0b00111000"}, MVSL, &IL);
      for (auto InnerRange : Ranges) {
        if (((std::get<0>(InnerRange) == 0) &&
             (std::get<1>(InnerRange) == (2))) ||
            ((std::get<0>(InnerRange) == 3) &&
             (std::get<1>(InnerRange) == (5)))) {
          continue;
        }
        if ((std::get<0>(InnerRange) == 6) &&
            (std::get<1>(InnerRange) == (7))) {
          NewVOp.Name = "__tmp0_256";
          load(NewVOp, {getOpName(V, true, true, 6, -6)}, MVSL, &IL);
          NewVOp.Name = V.Name;
          blend(NewVOp, {V.Name, "__tmp0_256", "0b11000000"}, MVSL, &IL);
        } else {
          return vgather(V);
          // NewVOp.Name = "__tmp0_256";
          // load(NewVOp, {getOpName(V, true, true, 6, -6)}, MVSL, &IL);
          // NewVOp.Name = V.Name;
          // blend(NewVOp, {V.Name, "__tmp0_256", "0b01000000"}, MVSL, &IL);
          // NewVOp.Name = "__tmp0_256";
          // load(NewVOp, {getOpName(V, true, true, 7, -7)}, MVSL, &IL);
          // NewVOp.Name = V.Name;
          // blend(NewVOp, {V.Name, "__tmp0_256", "0b10000000"}, MVSL, &IL);
        }
        return IL;
      }
    }

    auto Width = MVDataType::VWidth::W128;
    auto DataType = V.getDataType();

    // 4 elements strategy
    auto VFirstHalve =
        getSubVOperand(V, "__lo128", 4, Half, Width, DataType, 0);
    bool FirstHalve = vpack4elements(VFirstHalve, Width, &IL);
    auto VSecondHalve =
        getSubVOperand(V, "__hi128", 4, Half, Width, DataType, Half);
    bool SecondHalve = vpack4elements(VSecondHalve, Width, &IL);
    if (!FirstHalve && !SecondHalve) {
      return vgather(V);
    }
    if (FirstHalve && !SecondHalve) {
      IL.splice(IL.end(), vgather(VSecondHalve));
    }
    if (!FirstHalve && SecondHalve) {
      IL.splice(IL.end(), vgather(VFirstHalve));
    }
    insertLowAndHighBits(V, "__hi128", "__lo128", MVSL, &IL);
  } else {
    if (!vpack4elements(V, V.getWidth(), &IL)) {
      return vgather(V);
    }
  }

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
  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);
  // Adding SIMD inst to the list
  genSIMDInst(V, Op, "", SuffS, Args, SIMDType::VSET, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vstore(VectorIR::VectorOP V) {
  SIMDBackEnd::SIMDInstListType IL;
  std::string SuffS = (V.R.IsPartial) ? "" : "u";
  auto NeedsMask = (V.R.IsPartial) && (!V.R.LowBits && !V.R.HighBits);
  std::string PrefS = (NeedsMask) ? "mask" : "";

  std::string Op = "store";

  // List of parameters
  std::list<std::string> Args;
  // Hack: if data type is integer then use the si128/si256 notation instead
  // of epi
  if (V.R.DType > MVDataType::VDataType::SFLOAT) {
    V.R.DType = (V.R.VSize > 4) ? MVDataType::VDataType::UNDEF256
                                : MVDataType::VDataType::UNDEF128;
    std::string Cast = getRegisterType(V.R.DType, V.R.Width);
    Args.push_back("(" + Cast + "*)" + getOpName(V.R, true, true));
    if (NeedsMask) {
      Args.push_back(getMask(V.R.Mask, V.R.Size, V.R.getWidth()));
    }
    Args.push_back(getOpName(V.R, false, false));
  } else {
    Args.push_back(getOpName(V.R, true, true));
    if (NeedsMask) {
      Args.push_back(getMask(V.R.Mask, V.R.Size, V.R.getWidth()));
    }
    Args.push_back(getOpName(V.R, false, false));
    if (V.R.EqualVal) {
      if (V.R.DType == MVDataType::VDataType::DOUBLE) {
        V.R.DType = MVDataType::VDataType::SDOUBLE;
        V.R.Width = MVDataType::VWidth::W64;
      } else if (V.R.DType == MVDataType::VDataType::FLOAT) {
        V.R.DType = MVDataType::VDataType::SFLOAT;
        V.R.Width = MVDataType::VWidth::W32;
      }
      SuffS = "";
    }
  }

  if (!NeedsMask) {
    if (V.R.Size == 2) {
      SuffS = (V.R.LowBits) ? "l" : SuffS;
      SuffS = (V.R.HighBits) ? "h" : SuffS;
      if (V.R.isFloat()) {
        V.R.DType = MVDataType::VDataType::HALF_FLOAT;
      }
    }
  }

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VSTORE, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType
AVX2BackEnd::vscatterAVX512(VectorIR::VectorOP VOP) {
  auto V = VOP.R;
  SIMDBackEnd::SIMDInstListType IL;
  auto Op = "scatter";

  // List of parameters
  std::list<std::string> Args;

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

SIMDBackEnd::SIMDInstListType
AVX2BackEnd::singleElementScatter(VectorIR::VectorOP VOP) {
  // if (MVOptions::ISA != MVCPUInfo::MVISA::AVX512) {
  auto V = VOP.R;
  SIMDBackEnd::SIMDInstListType IL;
  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);
  for (size_t N = 0; N < V.VSize; ++N) {
    std::string Idx = "[" + std::to_string(N) + "]";
    addNonSIMDInst(V.UOP[N]->getRegisterValue(), V.getName() + Idx,
                   SIMDType::VOPT, MVSL, &IL);
  }
  return IL;
  //}
}

// ---------------------------------------------
bool AVX2BackEnd::vscatter4elements(VectorIR::VectorOP VOP,
                                    MVDataType::VWidth Width,
                                    SIMDBackEnd::SIMDInstListType *IL) {
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
        getSubVOperand(V, V.Name, V.VSize, Half, MVDataType::VWidth::W128,
                       MVDataType::VDataType::HALF_FLOAT, 0);
    VOP.R = NewVOp;
    IL->splice(IL->end(), vstore(VOP));
  }
  if (SecondHalve) {
    VectorIR::VOperand NewVOp =
        getSubVOperand(V, V.Name, V.VSize, Half, MVDataType::VWidth::W128,
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
  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.R.Order,
                        V.R.Offset);
  auto VOP = V.R;
  auto Ranges = getContiguityRanges(VOP);

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
    auto NameFirstHalve = "_mm256_castps256_ps128(" + VOP.Name + ")";
    auto VFirstHalve =
        getSubVOperand(VOP, NameFirstHalve, 4, Half, MVDataType::VWidth::W128,
                       MVDataType::VDataType::FLOAT, 0);
    V.R = VFirstHalve;
    bool FirstHalve = vscatter4elements(V, MVDataType::VWidth::W128, &IL);

    // FIXME:
    auto NameSecondHalve = "__tmp_extract_128";
    SIMDBackEnd::addRegToDeclare("__m128", NameSecondHalve, {0});
    genSIMDInst(NameSecondHalve, "extract", "", "f128",
                MVDataType::VWidth::W256, MVDataType::VDataType::FLOAT,
                {VOP.Name, "0x1"}, SIMDType::VOPT, MVSL, &IL);
    auto VSecondHalve =
        getSubVOperand(VOP, NameSecondHalve, 4, Half, MVDataType::VWidth::W128,
                       MVDataType::VDataType::FLOAT, Half);
    V.R = VSecondHalve;
    bool SecondHalve = vscatter4elements(V, MVDataType::VWidth::W128, &IL);
    if (!FirstHalve && !SecondHalve) {
      V.R = FullReg;
      if ((MVOptions::ISA == MVCPUInfo::MVISA::AVX512) ||
          (MVOptions::ScatterInstruction)) {
        return vscatterAVX512(V);
      }
      return singleElementScatter(V);
    }
    V.R = FullReg;
    if (FirstHalve && !SecondHalve) {
      // auto VSecondHalve =
      //     getSubVOperand(VOP, VOP.Name, 4, Half, MVDataType::VWidth::W128,
      //                    MVDataType::VDataType::FLOAT, Half);
      // V.R = VSecondHalve;
      // IL.splice(IL.end(), singleElementScatter(V));
      if ((MVOptions::ISA == MVCPUInfo::MVISA::AVX512) ||
          (MVOptions::ScatterInstruction)) {
        V.R = VSecondHalve;
        IL.splice(IL.end(), vscatterAVX512(V));
      } else {
        for (size_t N = 4; N < 8; ++N) {
          std::string Idx = "[" + std::to_string(N) + "]";
          addNonSIMDInst(V.R.UOP[N]->getRegisterValue(), V.R.getName() + Idx,
                         SIMDType::VOPT, MVSL, &IL);
        }
      }
    }
    if (!FirstHalve && SecondHalve) {
      if ((MVOptions::ISA == MVCPUInfo::MVISA::AVX512) ||
          (MVOptions::ScatterInstruction)) {
        V.R = VFirstHalve;
        IL.splice(IL.end(), vscatterAVX512(V));
      } else {
        for (size_t N = 0; N < 4; ++N) {
          std::string Idx = "[" + std::to_string(N) + "]";
          addNonSIMDInst(V.R.UOP[N]->getRegisterValue(), V.R.getName() + Idx,
                         SIMDType::VOPT, MVSL, &IL);
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
  std::list<std::string> Args;
  Args.push_back(V.OpA.getName());
  Args.push_back(V.OpB.getName());

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VADD, MVSL, &IL);

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
  std::list<std::string> Args;
  Args.push_back(V.OpA.getName());
  Args.push_back(V.OpB.getName());

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, "", SuffS, Args, SIMDType::VMUL, MVSL, &IL);

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
  std::list<std::string> Args;
  Args.push_back(V.OpA.getName());
  Args.push_back(V.OpB.getName());

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VSUB, MVSL, &IL);

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
  std::list<std::string> Args;
  Args.push_back(V.OpA.getName());
  Args.push_back(V.OpB.getName());

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VDIV, MVSL, &IL);

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
  std::list<std::string> Args;
  Args.push_back(V.OpA.getName());
  Args.push_back(V.OpB.getName());

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VMOD, MVSL, &IL);

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
  std::list<std::string> Args;
  Args.push_back(V.OpA.getName());
  if (V.OpB.getName() != "") {
    Args.push_back(V.OpB.getName());
  }

  MVSourceLocation MVSL(MVSourceLocation::Position::INORDER, V.Order, V.Offset);

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VFUNC, MVSL, &IL);

  return IL;
}

// ---------------------------------------------
std::vector<std::string> AVX2BackEnd::getInitValues(VectorIR::VectorOP V) {
  std::vector<std::string> InitVal;
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

  auto VS = V.R.VSize;
  for (size_t i = 0; i < VS; ++i) {
    InitVal.push_back(NeutralValue);
  }

  std::list<std::string> InitValList(InitVal.begin(), InitVal.end());
  auto Reg = V.R.Name;
  // Fuck, this is awful...
  if (getAccmReg(Reg) != "") {
    Reg = getAccmReg(Reg);
  }
  MVSourceLocation MVSL(MVSourceLocation::Position::PREORDER, V.R.Order,
                        V.R.Offset);
  auto Op = "setzero";
  std::list<std::string> Values = {};
  if (V.isBinOp() &&
      ((V.getBinOp() == clang::BO_Mul) || (V.getBinOp() == clang::BO_Div))) {
    Op = "set";
    Values = InitValList;
  }
  InitReg.push_back(createSIMDInst(Op, Reg, getMapWidth(V.R.Width),
                                   getMapType(V.R.DType), "", "", Values,
                                   SIMDType::INIT, MVSL));
  InitVal[VS - 1] = V.R.UOP[0]->getRegisterValue();
  return InitVal;
}

// ---------------------------------------------
SIMDBackEnd::SIMDInstListType AVX2BackEnd::vreduce(VectorIR::VectorOP V) {
  SIMDBackEnd::SIMDInstListType IL;
  SIMDBackEnd::SIMDInstListType TIL;
  auto RegType = getRegisterType(V.DT, V.VW);
  auto IsANewReduction = !hasAlreadyBeenMapped(V.R.Name);
  auto RegAccm = getNextAccmRegister(V.R.Name);
  if (IsANewReduction) {
    auto InitValues = getInitValues(V);
    SIMDBackEnd::addRegToDeclareInitVal(RegType, RegAccm, InitValues);
  }

  // Some needed values
  auto TmpReduxVar = (V.R.Name == V.OpB.Name) ? V.OpA.Name : V.OpB.Name;
  auto VCopy = V;
  VCopy.OpA.Name = TmpReduxVar;
  VCopy.R.Name = RegAccm;
  VCopy.OpB.Name = RegAccm;

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
  std::string ReduxVar = (V.R.Name == V.OpA.Name) ? V.OpA.getRegName(0, 0)
                                                  : V.OpB.getRegName(0, 0);

  /// This is a 'hack': when a reduction is found, instead of generate all
  /// the instructions needed for it, wait until the peephole optimization
  /// says if there are any other reductions that can be packed and fused
  /// together. P.S. creating hacks make me a hacker? lol
  MVSourceLocation MVSL(MVSourceLocation::Position::POSORDER, V.Order,
                        V.Offset);

  genSIMDInst(V.R, "VREDUX", "", "", {RegAccm}, SIMDType::VREDUC, MVSL, &IL,
              ReduxVar, "", {}, V.getMVOp());
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