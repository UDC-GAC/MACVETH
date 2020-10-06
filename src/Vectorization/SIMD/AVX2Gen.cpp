/**
 * File					 : src/Vectorization/SIMD/AVX2Gen.cpp
 * Author        : Marcos Horro
 * Date					 : Wed 26 Feb 2020 06:05 +01:00
 *
 * Last Modified : Wed 10 Jun 2020 10:26 +02:00
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

#include "include/Vectorization/SIMD/AVX2Gen.h"
#include "include/Utils.h"
#include "include/Vectorization/SIMD/CostTable.h"
#include <algorithm>
#include <bits/stdc++.h>
#include <map>
#include <utility>

using namespace macveth;

// ---------------------------------------------
SIMDGenerator::SIMDInst createSIMDInst(std::string Op, std::string Res,
                                       std::string Width, std::string DataType,
                                       std::string PrefS, std::string SuffS,
                                       std::list<std::string> Args,
                                       SIMDGenerator::SIMDType SType,
                                       int TacID) {
  // Get the function
  auto Pattern = CostTable::getPattern(AVX2Gen::NArch, Op);

  // Replace fills in pattern
  auto AVXFunc =
      SIMDGenerator::replacePatterns(Pattern, Width, DataType, PrefS, SuffS);

  // Generate SIMD inst
  SIMDGenerator::SIMDInst I(Res, AVXFunc, Args, "", {}, TacID);

  // Retrieving cost of function
  SIMDGenerator::SIMDCostInfo C(CostTable::getRow(AVX2Gen::NArch, Op));
  I.Cost = C;
  I.SType = SType;

  return I;
}

// ---------------------------------------------
SIMDGenerator::SIMDInst AVX2Gen::genMultAccOp(SIMDGenerator::SIMDInst Mul,
                                              SIMDGenerator::SIMDInst Acc) {
  SIMDGenerator::SIMDInst Fuse;

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
                        SIMDGenerator::SIMDType::VOPT, Acc.TacID);
  Fuse.VOPResult = Acc.VOPResult;
  Fuse.MVSL = Acc.MVSL;

  return Fuse;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
AVX2Gen::fuseAddSubMult(SIMDGenerator::SIMDInstListType I) {
  SIMDGenerator::SIMDInstListType IL;
  SIMDGenerator::SIMDInstListType PotentialFuse;
  SIMDGenerator::SIMDInstListType SkipList;
  std::map<SIMDGenerator::SIMDInst, SIMDGenerator::SIMDInst> Fuses;

  // Search replacements
  for (auto Inst : I) {
    // If we find a multiply operation it may be a FMADD
    if ((Inst.SType == SIMDGenerator::SIMDType::VMUL) &&
        ((Inst.DT == VectorIR::VDataType::FLOAT) ||
         (Inst.DT == VectorIR::VDataType::DOUBLE))) {
      PotentialFuse.push_back(Inst);
    }
    // If after the multiplication, there is a store, then this is not a
    // potential fuse anymore
    if (Inst.SType == SIMDGenerator::SIMDType::VSTORE) {
      SIMDGenerator::SIMDInstListType AuxL;
      for (auto P : PotentialFuse) {
        if (Inst.Result != P.Result) {
          AuxL.push_back(P);
        }
      }
      PotentialFuse = AuxL;
    }
    // Check if we have any add/sub adding the result of a previous
    // multiplication
    if ((Inst.SType == SIMDGenerator::SIMDType::VADD) ||
        (Inst.SType == SIMDGenerator::SIMDType::VSUB)) {
      for (auto P : PotentialFuse) {
        // if (Utils::contains(Inst.Args, P.Result)) {
        if (std::find(Inst.Args.begin(), Inst.Args.end(), P.Result) !=
            Inst.Args.end()) {
          auto NewFuse = AVX2Gen::genMultAccOp(P, Inst);
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
std::string AVX2Gen::declareAuxArray(VectorIR::VDataType DT) {
  return getNextArrRegister(VDTypeToCType[DT],
                            getMaxWidth() / VectorIR::VDataTypeWidthBits[DT]);
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
AVX2Gen::horizontalSingleReduction(SIMDGenerator::SIMDInstListType TIL) {
  // Horizontal operations
  SIMDGenerator::SIMDInstListType IL;
  std::vector<SIMDGenerator::SIMDInst> VIL{std::begin(TIL), std::end(TIL)};

  int NElems = VIL[0].W / VectorIR::VDataTypeWidthBits[VIL[0].DT];
  auto OpRedux = VIL[0].MVOP;
  auto OpReduxType = OpRedux.getType();
  auto OpName = VIL[0].MVOP.toString();
  auto Type = VIL[0].DT;
  auto RegType = getRegisterType(Type, VectorIR::VWidth::W128);
  auto LoRes = "__mv_lo" + getMapWidth(VIL[0].W);
  auto HiRes = "__mv_hi" + getMapWidth(VIL[0].W);
  SIMDGenerator::addRegToDeclare(RegType, LoRes, {0});
  SIMDGenerator::addRegToDeclare(RegType, HiRes, {0});
  auto AccmReg = getAccmReg(VIL[0].VOPResult.Name);
  auto Pos = MVSourceLocation::Position::POSORDER;
  auto Loc = VIL[0].VOPResult.Order;
  auto OP = VIL[0].MVOP.toString();
  if (Type == VectorIR::VDataType::DOUBLE) {
    // If 4 elements
    if (NElems > 2) {
      // __m128d lo = _mm256_castpd256_pd128(v); zero latency
      genSIMDInst(LoRes, "castpd", "256", "pd128", VectorIR::VWidth::W256, Type,
                  {AccmReg}, SIMDType::VOPT, &IL, Loc, Pos);
      // __m128d hi = _mm256_extractf128_pd(v,0x1);
      genSIMDInst(HiRes, "extract", "", "f128", VectorIR::VWidth::W256, Type,
                  {AccmReg, "0x1"}, SIMDType::VOPT, &IL, Loc, Pos);
      // lo = op(lo,hi);
      genSIMDInst(LoRes, OpName, "", "", VectorIR::VWidth::W128, Type,
                  {LoRes, HiRes}, SIMDType::VOPT, &IL, Loc, Pos);
    } else {
      LoRes = AccmReg;
    }
    // When only 2 elements
    // hi = _mm_shuffle_pd(lo,lo,0x1);
    genSIMDInst(HiRes, "shuffle", "", "", VectorIR::VWidth::W128, Type,
                {LoRes, LoRes, "0x1"}, SIMDType::VPERM, &IL, Loc, Pos);
    // val = _mm_cvtsd_f64(op(lo,hi));
    genSIMDInst(LoRes, OpName, "", "", VectorIR::VWidth::W128, Type,
                {LoRes, HiRes}, SIMDType::VOPT, &IL, Loc, Pos);
    // genSIMDInst(VIL[0].Result, "cvtsd", "", "f64", VectorIR::VWidth::W128,
    // Type,
    //             {LoRes}, SIMDType::VOPT, &IL, Loc, Pos);
    addNonSIMDInst(VIL[0].Result, VIL[0].Result + OP + LoRes + "[0]",
                   SIMDType::VOPT, &IL, Loc, Pos);
  } else if (Type == VectorIR::VDataType::FLOAT) {
    // 8 elements
    if (NElems > 4) {
      // __m128 lo =
      genSIMDInst(LoRes, "castps", "256", "ps128", VectorIR::VWidth::W256, Type,
                  {AccmReg}, SIMDType::VOPT, &IL, Loc, Pos);
      // __m128 lo = cast(v)
      genSIMDInst(HiRes, "extract", "", "f128", VectorIR::VWidth::W256, Type,
                  {AccmReg, "0x1"}, SIMDType::VOPT, &IL, Loc, Pos);

      // lo = op(lo,hi);
      genSIMDInst(LoRes, OpName, "", "", VectorIR::VWidth::W128, Type,
                  {LoRes, HiRes}, SIMDType::VOPT, &IL, Loc, Pos);
    } else {
      LoRes = AccmReg;
    }
    // 4 elements
    // hi = movehl(lo);
    genSIMDInst(HiRes, "move", "", "hl", VectorIR::VWidth::W128, Type,
                {LoRes, LoRes}, SIMDType::VOPT, &IL, Loc, Pos);
    // lo = op(lo,hi);
    genSIMDInst(LoRes, OpName, "", "", VectorIR::VWidth::W128, Type,
                {LoRes, HiRes}, SIMDType::VOPT, &IL, Loc, Pos);
    // 2 elements
    // hi = shuffle(lo);
    genSIMDInst(HiRes, "shuffle", "", "", VectorIR::VWidth::W128, Type,
                {LoRes, LoRes, "0b00001110"}, SIMDType::VPERM, &IL, Loc, Pos);
    // lo = op(lo,hi);
    genSIMDInst(LoRes, OpName, "", "", VectorIR::VWidth::W128, Type,
                {LoRes, HiRes}, SIMDType::VOPT, &IL, Loc, Pos);

    // genSIMDInst(VIL[0].Result, "cvtss", "", "f32", VectorIR::VWidth::W128,
    // Type,
    //             {LoRes}, SIMDType::VOPT, &IL, Loc, Pos);
    addNonSIMDInst(VIL[0].Result, VIL[0].Result + OP + LoRes + "[0]",
                   SIMDType::VOPT, &IL, Loc, Pos);
  } else {
    // FIXME: should we implement this for integer arithmetic?
    MVErr("Not implemented this approach yet for other types than float or "
          "doubles");
  }

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
AVX2Gen::horizontalReductionFusion(SIMDGenerator::SIMDInstListType TIL) {
  // Horizontal operations
  SIMDGenerator::SIMDInstListType IL;
  std::vector<SIMDGenerator::SIMDInst> VIL{std::begin(TIL), std::end(TIL)};

  int NElems = VIL[0].W / VectorIR::VDataTypeWidthBits[VIL[0].DT];
  auto OpRedux = VIL[0].MVOP;
  auto OpReduxType = OpRedux.getType();
  auto OpName = VIL[0].MVOP.toString();
  int NumRedux = TIL.size();
  auto Pos = MVSourceLocation::Position::POSORDER;
  if (NumRedux > NElems) {
    MVAssert(false, "You can not have more reductions than elements in vector");
  }
  auto Loc = 0;
  std::vector<std::string> VAccm;
  std::unordered_set<std::string> VRedux;
  std::vector<std::string> VReduxList;
  for (int t = 0; t < NumRedux; ++t) {
    VAccm.push_back(VIL[t].Args.front());
    VRedux.insert(VIL[t].Result);
    VReduxList.push_back(VIL[t].Result);
    Loc = std::max(Loc, VIL[t].VOPResult.Order);
  }

  auto S = (NumRedux + (NumRedux % 2)) / 2;
  auto OP =
      (OpRedux.ClangOP == BinaryOperator::Opcode::BO_Add) ? "hadd" : "hsub";

  auto Stride = VIL[0].DT == VectorIR::VDataType::DOUBLE ? 4 : 2;
  while (true) {
    for (int i = 0; i < NumRedux; i += Stride) {
      genSIMDInst(VIL[0].VOPResult, OP, "", "",
                  {VAccm[i], (NumRedux > 1) ? VAccm[i + 1] : VAccm[i]},
                  SIMDType::VOPT, &IL, Loc, Pos, VAccm[i]);
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
      auto Blend =
          replacePatterns(CostTable::getPattern(AVX2Gen::NArch, "blend"), W,
                          MapType[VIL[0].DT], "", "");
      auto Mask1 =
          VIL[0].DT == VectorIR::VDataType::DOUBLE ? "0b1100" : "0b11110000";
      // Operand 2
      auto PermS = "2f128";
      auto Mask2 = "0x21";
      auto Perm =
          replacePatterns(CostTable::getPattern(AVX2Gen::NArch, "permute"), W,
                          MapType[VIL[0].DT], "", PermS);
      auto V0 = VAccm[0];
      auto V1 =
          (NumRedux > 4) ? VAccm[4] : (NumRedux > 2) ? VAccm[2] : VAccm[1];
      auto Op1 = genGenericFunc(Blend, {V0, V1, Mask1});
      auto Op2 = genGenericFunc(Perm, {V0, V1, Mask2});

      OP = (OP == "hadd") ? "add" : "sub";
      genSIMDInst(VIL[0].VOPResult, OP, "", "", {Op1, Op2}, SIMDType::VOPT, &IL,
                  Loc, Pos, VAccm[0]);
      break;
    } else if (S == 1) {
      break;
    }
  }

  auto No = 0;
  OP = (OpRedux.ClangOP == BinaryOperator::Opcode::BO_Add) ? " + " : " - ";
  std::set<std::string> Visited;
  for (auto R : VReduxList) {
    if (Visited.find(R) != Visited.end()) {
      continue;
    }
    Visited.insert(R);
    auto Idx = "[" + std::to_string(No++) + "]";
    addNonSIMDInst(R, R + OP + VAccm[0] + Idx, SIMDType::VOPT, &IL, Loc, Pos);
  }

  return IL;
}

// ---------------------------------------------
std::string AVX2Gen::shuffleArguments(std::string A1, std::string A2,
                                      VectorIR::VWidth Width,
                                      SIMDGenerator::SIMDInst I, int Pos) {
  std::string Mask;
  if (I.DT == VectorIR::VDataType::DOUBLE) {
    Mask = (Pos == 0) ? "0x00" : "0xff";
  } else if (I.DT == VectorIR::VDataType::FLOAT) {
    Mask = (Pos == 0) ? "0x44" : "0xee";
  }

  if (Width == VectorIR::VWidth::W128) {
    Mask = (Pos == 0) ? "0b10001000" : "0b11011101";
  }

  return genGenericFunc(
      replacePatterns(CostTable::getPattern(AVX2Gen::NArch, "shuffle"),
                      getMapWidth(Width), getMapType(I.DT), "", ""),
      {A1, A2, Mask});
}

// ---------------------------------------------
std::string AVX2Gen::permuteArguments(std::string A1, std::string A2,
                                      SIMDGenerator::SIMDInst I, int Pos) {
  std::string Mask = "0x21";
  std::string PermW = getMapWidth(I.W);
  std::string PermT = getMapType(I.DT);
  return genGenericFunc(
      replacePatterns(CostTable::getPattern(AVX2Gen::NArch, "permute"), PermW,
                      PermT, "", "2f128"),
      {A1, A2, Mask});
}

// ---------------------------------------------
std::string AVX2Gen::extractArgument(std::string A, SIMDGenerator::SIMDInst I,
                                     int Hi) {
  std::string Mask = (Hi == 0) ? "0x00" : "0x01";
  std::string PermW = getMapWidth(I.W);
  std::string PermT = getMapType(I.DT);
  return genGenericFunc(
      replacePatterns(CostTable::getPattern(AVX2Gen::NArch, "extract"), PermW,
                      PermT, "", "f128"),
      {A, Mask});
}

// ---------------------------------------------
std::list<std::string> AVX2Gen::renderSIMDRegister(SIMDInstListType S) {
  std::list<std::string> L;
  // Render register declarations
  for (auto It : RegDeclared) {
    auto TypeRegDecl = It.first + " ";
    auto VAuxRegs = It.second;
    if (std::get<0>(VAuxRegs[0]) != "") {
      TypeRegDecl += std::get<0>(VAuxRegs[0]);
    }
    for (int i = 1; i < VAuxRegs.size(); ++i) {
      if (std::get<0>(VAuxRegs[i]) != "") {
        TypeRegDecl += ", " + std::get<0>(VAuxRegs[i]);
      }
    }
    L.push_back(TypeRegDecl + ";");
  }
  return L;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
AVX2Gen::generalReductionFusion(SIMDGenerator::SIMDInstListType TIL) {
  // TODO:
  // This is quite complex: it is not the same doing reductions of additions
  // subtractions than multiplications or divisions. This way, we have to
  // explictly differentiate them. General algorithm (4 doubles) for those
  // cases:
  //          DATA IN YMM0
  //         ymm0 = _mm256_OP_pd(ymm0, _mm256_permute_pd(ymm0, 0x05));
  //         ymm1 = _mm256_OP_pd(ymm0, _mm256_permute2f128_pd(ymm0, ymm0,
  //         0x01));
  //          STORE YMM2
  // For additions and subtraction it is different: it is based on horizontal
  // operations
  SIMDGenerator::SIMDInstListType IL;
  std::vector<SIMDInst> VIL{std::begin(TIL), std::end(TIL)};

  auto Pos = MVSourceLocation::Position::POSORDER;
  auto NElems = VIL[0].W / VectorIR::VDataTypeWidthBits[VIL[0].DT];
  auto OpRedux = VIL[0].MVOP;
  // auto OpReduxType = OpRedux.getType();
  auto OpName = VIL[0].MVOP.toString();
  auto NumRedux = TIL.size();
  if (NumRedux > NElems) {
    MVAssert(false, "You can not have more reductions than elements in vector");
  }
  std::vector<std::string> VAccm;
  std::vector<std::string> VRedux;
  for (int t = 0; t < NumRedux; ++t) {
    VAccm.push_back(VIL[t].Args.front());
    VRedux.push_back(VIL[t].Result);
  }

  auto OP = OpRedux.toString();
  auto Loc = VIL[NumRedux - 1].VOPResult.Order;
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
  for (int i = 0; i < NumRedux; i += 2) {
    auto A0 =
        shuffleArguments(VAccm[i], (NumRedux > i + 1) ? VAccm[i + 1] : VAccm[i],
                         VIL[i].W, VIL[i], 0);
    auto A1 =
        shuffleArguments(VAccm[i], (NumRedux > i + 1) ? VAccm[i + 1] : VAccm[i],
                         VIL[i].W, VIL[i], 1);
    genSIMDInst(VIL[i].VOPResult, OP, "", "", {A0, A1}, SIMDType::VOPT, &IL,
                Loc, Pos, VAccm[i]);
    if (i > 0) {
      VAccm[i - 1] = VAccm[i];
    }
  }

  // Permutations
  for (int i = 0; i < NumRedux; i += 4) {
    auto W = MapWidth[VIL[0].W];
    // Operand 1
    auto Blend = replacePatterns(CostTable::getPattern(AVX2Gen::NArch, "blend"),
                                 W, MapType[VIL[0].DT], "", "");
    auto Mask =
        VIL[0].DT == VectorIR::VDataType::DOUBLE ? "0b1100" : "0b11110000";
    auto A0 = genGenericFunc(
        Blend, {VAccm[i], (NumRedux > i + 1) ? VAccm[i + 1] : VAccm[i], Mask});
    auto A1 = permuteArguments(
        VAccm[i], (NumRedux > i + 1) ? VAccm[i + 1] : VAccm[i], VIL[i], 1);
    genSIMDInst(VIL[i].VOPResult, OP, "", "", {A0, A1}, SIMDType::VOPT, &IL,
                Loc, Pos, VAccm[i]);
    if (i > 0)
      VAccm[i - 1] = VAccm[i];
  }

  // Extract values depending if 4 or 8 elements, different approaches due to
  // intrisics design

  std::string AuxArray = declareAuxArray(VIL[0].DT);
  if (NElems <= 4) {
    for (int R = 0; R < NumRedux; ++R) {
      std::string Idx = "[" + std::to_string(R) + "]";
      addNonSIMDInst(VRedux[R], VRedux[R] + OP + VAccm[0] + Idx, SIMDType::VSEQ,
                     &IL, Loc, Pos);
    }
    return IL;
  } else {
    // Need to do the last extraction, shuffle and operation
    for (int R = 0; R < NumRedux; R += 4) {
      // op(shuffle(hi,lo,mask), shuffle(hi,lo,mask))
      auto Lo = extractArgument(VAccm[R], VIL[R], 0);
      auto Hi = extractArgument(VAccm[R], VIL[R], 1);
      auto A1 = shuffleArguments(Lo, Hi, VectorIR::VWidth::W128, VIL[R], 0);
      auto A2 = shuffleArguments(Lo, Hi, VectorIR::VWidth::W128, VIL[R], 1);
      auto RegType = getRegisterType(VIL[R].DT, VectorIR::VWidth::W128);
      auto Res = getNextAuxRegister(VIL[R].VOPResult.Name);
      SIMDGenerator::addRegToDeclare(RegType, Res, {0});
      genSIMDInst(Res, OP, "", "", VectorIR::VWidth::W128, VIL[R].DT, {A1, A2},
                  SIMDType::VOPT, &IL, Loc, Pos);
      if (NumRedux > 1) {
        genSIMDInst(Res, "store", "", "u", VectorIR::VWidth::W128, VIL[R].DT,
                    {"&" + AuxArray + "[" + std::to_string(R) + "]", Res},
                    SIMDType::VSTORE, &IL, Loc, Pos);
      }
    }
    if (NumRedux == 1) {
      addNonSIMDInst(VRedux[0], VRedux[0] + OP + VAccm[0] + "[0]",
                     SIMDType::VSEQ, &IL, Loc, Pos);
    } else {
      for (int R = 0; R < NumRedux; ++R) {
        std::string Idx = "[" + std::to_string(R) + "]";
        addNonSIMDInst(VRedux[R], VRedux[R] + OP + AuxArray + Idx,
                       SIMDType::VSEQ, &IL, Loc, Pos);
      }
    }
  }
  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
AVX2Gen::fuseReductionsList(SIMDGenerator::SIMDInstListType L) {
  // For each reduction found, check whether they can be fused together or
  // not:
  // they can be fused if and only if they are in the same loop dimension/name
  // and the same loop, basically (because they can have same name but be
  // different loops). This is important for coherence in the program
  auto LInsSize = L.size();
  auto OpReduxType = L.front().MVOP.T;
  auto OpRedux = L.front().MVOP;
  SIMDGenerator::SIMDInstListType FusedRedux;
  if (LInsSize == 0) {
    return FusedRedux;
  }

  // This is the horizontal approach, only valid for AVX2 and additions
  // and subtraction
  if (LInsSize == 1) {
    Utils::printDebug("AVX2Gen", "Single reduction approach (" +
                                     std::to_string(LInsSize) + ")");
    FusedRedux = horizontalSingleReduction(L);
  } else if (((OpReduxType == MVOpType::CLANG_BINOP) &&
              ((OpRedux.ClangOP == BinaryOperator::Opcode::BO_Add) ||
               (OpRedux.ClangOP == BinaryOperator::Opcode::BO_Sub)))) {
    // Horizontal approach only worth it when we have two or more
    // reductions
    Utils::printDebug("AVX2Gen",
                      "Horizontal approach (" + std::to_string(LInsSize) + ")");
    FusedRedux = horizontalReductionFusion(L);
  } else {
    Utils::printDebug("AVX2Gen", "General approach approach (" +
                                     std::to_string(LInsSize) + ")");
    FusedRedux = generalReductionFusion(L);
  }

  // FIXME: Clean accumulators
  for (auto R : L) {
    auto Accm = getAccmReg(R.VOPResult.getName());
    if (!isAccmClean(Accm)) {
      genSIMDInst(Accm, "setzero", "", "", R.W, R.DT, {}, SIMDType::VSET,
                  &FusedRedux, R.VOPResult.Order,
                  MVSourceLocation::Position::PREORDER);
    }
  }
  /// Mark accumulators
  for (auto R : L) {
    auto Accm = getAccmReg(R.VOPResult.getName());
    markDirtyAccm(Accm);
  }

  return FusedRedux;
}

// ---------------------------------------------
bool AVX2Gen::hasRawDependencies(SIMDGenerator::SIMDInstListType L,
                                 SIMDGenerator::SIMDInst I) {
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
bool AVX2Gen::reductionIsContiguous(SIMDGenerator::SIMDInstListType L,
                                    SIMDGenerator::SIMDInst I) {
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
SIMDGenerator::SIMDInstListType
AVX2Gen::fuseReductions(SIMDGenerator::SIMDInstListType TIL) {
  SIMDGenerator::SIMDInstListType IL;
  SIMDGenerator::SIMDInstListType SkipList;
  // List of reduction candidates to be fused
  std::map<int, SIMDGenerator::SIMDInstListType> LRedux;
  std::map<SIMDGenerator::SIMDInst, SIMDGenerator::SIMDInstListType>
      ReplaceFusedRedux;
  for (auto I : TIL) {
    if (I.isReduction()) {
      auto S = I.VOPResult.Size;
      auto L = LRedux[S];
      if (reductionIsContiguous(L, I)) {
        SIMDGenerator::SIMDInstListType Aux;
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
      auto DW = VectorIR::VDataTypeWidthBits[L.second.front().VOPResult.DType];
      // Only compute reductions if max found
      if (W / DW > LInsSize) {
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
SIMDGenerator::SIMDInstListType
AVX2Gen::peepholeOptimizations(SIMDGenerator::SIMDInstListType I) {
  SIMDGenerator::SIMDInstListType IL(I);

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
std::string AVX2Gen::getRegisterType(VectorIR::VDataType DT,
                                     VectorIR::VWidth W) {
  auto Width = W;
  assert((W != 512) && "Width not permitted in AVX2!!");
  if (W < 256) {
    Width = VectorIR::VWidth::W128;
  }
  std::string Suffix = "";
  if (DT == VectorIR::VDataType::DOUBLE) {
    Suffix = "d";
  } else if (DT == VectorIR::VDataType::FLOAT) {
    Suffix = "";
  } else {
    Suffix = "i";
  }
  return "__m" + std::to_string(Width) + Suffix;
}

// ---------------------------------------------
SIMDGenerator::SIMDInst AVX2Gen::genSIMDInst(
    std::string Result, std::string Op, std::string PrefS, std::string SuffS,
    VectorIR::VWidth Width, VectorIR::VDataType Type,
    std::list<std::string> Args, SIMDGenerator::SIMDType SType,
    SIMDGenerator::SIMDInstListType *IL, int Order,
    MVSourceLocation::Position P, std::string NameOp, std::string MVFunc,
    std::list<std::string> MVArgs, MVOp MVOP) {
  // Get the function
  std::string Pattern = CostTable::getPattern(AVX2Gen::NArch, Op);

  // Replace fills in pattern
  std::string AVXFunc = replacePatterns(Pattern, getMapWidth(Width),
                                        getMapType(Type), PrefS, SuffS);

  // Custom order
  auto O = 0;
  if (Order != -1) {
    O = Order;
  }

  // Generate SIMD inst
  SIMDGenerator::SIMDInst I(NameOp, AVXFunc, Args, MVFunc, MVArgs, O);
  I.MVSL.setOrder(O);
  I.MVSL.setPosition(P);

  // Retrieving cost of function
  SIMDGenerator::SIMDCostInfo C(CostTable::getRow(AVX2Gen::NArch, Op));
  I.Cost = C;
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
SIMDGenerator::SIMDInst AVX2Gen::genSIMDInst(
    VectorIR::VOperand V, std::string Op, std::string PrefS, std::string SuffS,
    std::list<std::string> Args, SIMDGenerator::SIMDType SType,
    SIMDGenerator::SIMDInstListType *IL, int Order,
    MVSourceLocation::Position P, std::string NameOp, std::string MVFunc,
    std::list<std::string> MVArgs, MVOp MVOP) {
  // Get the function
  std::string Pattern = CostTable::getPattern(AVX2Gen::NArch, Op);

  // Replace fills in pattern
  std::string AVXFunc =
      replacePatterns(Pattern, getMapWidth(V.getWidth()),
                      getMapType(V.getDataType()), PrefS, SuffS);

  // Custom order
  auto O = V.Order;
  if (Order != -1) {
    O = Order;
  }

  // Generate SIMD inst
  SIMDGenerator::SIMDInst I((NameOp == "") ? V.getName() : NameOp, AVXFunc,
                            Args, MVFunc, MVArgs, O);

  I.MVSL.setOrder(O);
  I.MVSL.setPosition(P);

  // Retrieving cost of function
  SIMDGenerator::SIMDCostInfo C(CostTable::getRow(AVX2Gen::NArch, Op));
  I.Cost = C;
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
std::string AVX2Gen::getMask(unsigned int MaskVect, int NElems,
                             VectorIR::VWidth Width, VectorIR::VDataType Type) {
  std::string Mask = "";
  std::string SetPref = "";
  std::string SetSuff = getMapType(Type);
  if (Width == VectorIR::VWidth::W256) {
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
std::string AVX2Gen::getMask(unsigned int MaskVect, int NElems,
                             VectorIR::VWidth Width) {
  std::string Mask = "";
  std::string SetPref = "";
  std::string SetSuff = "epi32";
  if (Width == VectorIR::VWidth::W256) {
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
SIMDGenerator::SIMDInstListType AVX2Gen::vpack(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  // Suffix: we are going to assume that all the load are unaligned.
  std::string SuffS = (V.IsPartial) ? "" : "u";
  // Mask
  std::string Mask = (V.IsPartial) ? "mask" : "";

  // Type of load: load/u
  // [1]
  // https://software.intel.com/en-us/forums/intel-isa-extensions/topic/752392
  // [2] https://stackoverflow.com/questions/36191748/difference-between-\
  // load1-and-broadcast-intrinsics
  auto Op = Mask + "load";

  // FIXME: if line crosses cache boundary consider using lddqu instead of
  // loadu. Approach only valid for integers... Fuck integers.

  // List of parameters
  std::list<std::string> Args;

  // Hack: if data type is integer then use the si128/si256 notation instead
  // of epi
  if (V.DType > VectorIR::VDataType::SFLOAT) {
    V.DType = (V.VSize > 4) ? VectorIR::VDataType::UNDEF256
                            : VectorIR::VDataType::UNDEF128;
    std::string Cast = getRegisterType(V.DType, V.Width);
    Args.push_back("(" + Cast + "*)" + getOpName(V, true, true));
  } else {
    Args.push_back(getOpName(V, true, true));
  }

  if (V.IsPartial) {
    // TODO: calculate mask
    // std::string MaskLoad = getMask();
    std::string MaskLoad = getMask(V.Mask, V.Size, V.getWidth());
    Args.push_back(MaskLoad);
  }

  // Adding SIMD inst to the list
  genSIMDInst(V, Op, "", SuffS, Args, SIMDType::VPACK, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vbcast(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  auto Op = "broadcast";

  // There are no preffixes for broadcasts operations
  // TODO: generate suffix
  auto SuffS = "";
  // Mask
  auto PrefS = (V.IsPartial) ? "mask" : "";

  // TODO: check
  // List of parameters
  std::list<std::string> Args;
  Args.push_back(getOpName(V, true, true));

  if (V.EqualVal) {
    if (V.getDataType() == VectorIR::VDataType::DOUBLE) {
      V.DType = VectorIR::VDataType::SDOUBLE;
    }
    if (V.getDataType() == VectorIR::VDataType::FLOAT) {
      V.DType = VectorIR::VDataType::SFLOAT;
    }
  }

  // Adding SIMD inst to the list
  genSIMDInst(V, Op, "", SuffS, Args, SIMDType::VBCAST, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vgather(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  auto Op = "gather";

  // List of parameters
  std::list<std::string> Args;

  // To gather elements
  // Generate preffix: must be i32 or i64, depending on the VIndex width
  std::string Scale =
      (V.getDataType() == VectorIR::VDataType::DOUBLE) ? "8" : "4";
  auto VIndexSuffix = "epi64x";
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
  auto PrefGather = "i64";
  VIndexSuffix = "epi64x";
  if (V.VSize > 4) {
    PrefGather = "i32";
    VIndexSuffix = "epi32";
    Scale = std::to_string(4);
  }
  PrefS += PrefGather;

  auto VIndex = "_mm" + MapWidth[V.Width] + "_set_" + VIndexSuffix + "(";
  auto CopyIdx = V.Idx;
  auto MinIdx = std::min_element(CopyIdx.begin(), CopyIdx.end());
  auto MinVal = *MinIdx;
  auto BaseIdx = V.UOP[MinIdx - CopyIdx.begin()]->getValue();
  // This way we avoid negative indices
  if (MinVal < 0) {
    for (auto T = 0; T < CopyIdx.size(); ++T) {
      CopyIdx[T] += abs(MinVal);
    }
  }
  std::reverse(CopyIdx.begin(), CopyIdx.end());
  for (auto T = 0; T < V.VSize % 2; ++T) {
    VIndex += std::to_string(0) + ",";
  }
  for (auto T = 0; T < CopyIdx.size(); ++T) {
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

  // Adding SIMD inst to the list
  genSIMDInst(V, Op, PrefS, "", Args, SIMDType::VGATHER, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vset(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  std::string SuffS = "";
  std::string Op = "set";

  // List of parameters
  std::list<std::string> Args;
  if (V.EqualVal) {
    SuffS += "1";
    Args.push_back((V.UOP[0] != nullptr) ? V.UOP[0]->getRegisterValue() : "0");
  } else {
    for (int n = 0; n < V.VSize; n++) {
      Args.push_back((V.UOP[n] != nullptr) ? V.UOP[n]->getValue() : "0.0");
    }
  }
  for (int t = V.VSize; t < (V.Width / VectorIR::VDataTypeWidthBits[V.DType]);
       ++t) {
    Args.push_back("0");
  }
  Args.reverse();

  // Adding SIMD inst to the list
  genSIMDInst(V, Op, "", SuffS, Args, SIMDType::VSET, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vstore(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  std::string SuffS = (V.R.IsPartial) ? "" : "u";
  // Mask
  std::string PrefS = (V.R.IsPartial) ? "mask" : "";

  std::string Op = "store";

  // List of parameters
  std::list<std::string> Args;
  // Hack: if data type is integer then use the si128/si256 notation instead
  // of epi
  if (V.R.DType > VectorIR::VDataType::SFLOAT) {
    V.R.DType = (V.R.VSize > 4) ? VectorIR::VDataType::UNDEF256
                                : VectorIR::VDataType::UNDEF128;
    std::string Cast = getRegisterType(V.R.DType, V.R.Width);
    Args.push_back("(" + Cast + "*)" + getOpName(V.R, true, true));
    if (V.R.IsPartial) {
      Args.push_back(getMask(V.R.Mask, V.R.Size, V.R.getWidth()));
    }
    Args.push_back(getOpName(V.R, false, false));
  } else {
    Args.push_back(getOpName(V.R, true, true));
    if (V.R.IsPartial) {
      Args.push_back(getMask(V.R.Mask, V.R.Size, V.R.getWidth()));
    }
    Args.push_back(getOpName(V.R, false, false));
    if (V.R.EqualVal) {
      if (V.R.DType == VectorIR::DOUBLE) {
        V.R.DType = VectorIR::SDOUBLE;
        V.R.Width = VectorIR::W64;
      } else if (V.R.DType == VectorIR::FLOAT) {
        V.R.DType = VectorIR::SFLOAT;
        V.R.Width = VectorIR::W32;
      }
      SuffS = "";
    }
  }

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VSTORE, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vscatter(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;

  // TODO: FIXME: check if any contiguous in order to optimize minimally this
  for (int N = 0; N < V.R.VSize; ++N) {
    std::string Idx = "[" + std::to_string(N) + "]";
    addNonSIMDInst(V.R.UOP[N]->getRegisterValue(), V.R.getName() + Idx,
                   SIMDType::VOPT, &IL, V.Order);
  }

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vadd(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
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

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VADD, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vmul(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO: generate preffix
  std::string PrefS = "";
  // TODO: generate suffix
  std::string SuffS = "";
  // TODO:
  std::string Op = "mul";

  // TODO: check
  // List of parameters
  std::list<std::string> Args;
  Args.push_back(V.OpA.getName());
  Args.push_back(V.OpB.getName());

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VMUL, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vsub(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
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

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VSUB, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vdiv(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
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

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VDIV, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vmod(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
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

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VMOD, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vfunc(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
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

  // Adding SIMD inst to the list
  genSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VFUNC, &IL);

  return IL;
}

// ---------------------------------------------
std::vector<std::string> AVX2Gen::getInitValues(VectorIR::VectorOP V) {
  std::vector<std::string> InitVal;
  std::string NeutralValue = "0";
  if (V.isBinOp()) {
    switch (V.getBinOp()) {
    case clang::BO_Mul:
    case clang::BO_Div:
      NeutralValue = "1";
      break;
    }
  }
  auto VS = V.R.VSize;

  // for (int i = 0; i < VS - 1 + (VS % 2); ++i) {
  for (int i = 0; i < VS; ++i) {
    InitVal.push_back(NeutralValue);
  }
  // InitVal.push_back(V.R.UOP[0]->getRegisterValue());

  std::list<std::string> InitValList(InitVal.begin(), InitVal.end());
  std::string Reg = V.R.Name;
  // Fuck, this is awful...
  if (getAccmReg(Reg) != "") {
    Reg = getAccmReg(Reg);
  }
  if (V.isBinOp() &&
      ((V.getBinOp() == clang::BO_Mul) || (V.getBinOp() == clang::BO_Div))) {
    InitReg.push_back(createSIMDInst("set", Reg, getMapWidth(V.R.Width),
                                     getMapType(V.R.DType), "", "", InitValList,
                                     SIMDType::INIT, V.Order));
  } else {
    InitReg.push_back(createSIMDInst("setzero", Reg, getMapWidth(V.R.Width),
                                     getMapType(V.R.DType), "", "", {},
                                     SIMDType::INIT, V.Order));
  }
  InitVal[VS - 1] = V.R.UOP[0]->getRegisterValue();
  return InitVal;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vreduce(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  SIMDGenerator::SIMDInstListType TIL;
  auto RegType = getRegisterType(V.DT, V.VW);
  auto IsANewReduction = !hasAlreadyBeenMapped(V.R.Name);
  auto RegAccm = getNextAccmRegister(V.R.Name);
  if (IsANewReduction) {
    auto InitValues = getInitValues(V);
    SIMDGenerator::addRegToDeclareInitVal(RegType, RegAccm, InitValues);
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
  std::string ReduxVar =
      (V.R.Name == V.OpA.Name) ? V.OpA.getRegName() : V.OpB.getRegName();

  /// This is a 'hack': when a reduction is found, instead of generate all the
  /// instructions needed for it, wait until the peephole optimization says
  /// if there are any other reductions that can be packed and fused together.
  /// P.S. creating hacks make me a hacker? lol
  genSIMDInst(V.R, "VREDUX", "", "", {RegAccm}, SIMDType::VREDUC, &IL, V.Order,
              MVSourceLocation::Position::POSORDER, ReduxVar, "", {},
              V.getMVOp());
  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vseq(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;

  // Adding SIMD inst to the list
  addNonSIMDInst(V, SIMDType::VSEQ, &IL, V.R.Order,
                 MVSourceLocation::Position::INORDER);

  return IL;
}