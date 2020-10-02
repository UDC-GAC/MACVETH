/**
 * File					 : src/Vectorization/SIMD/AVX512Gen.cpp
 * Author        : Marcos Horro
 * Date					 : Ven 02 Out 2020 12:52:29 CET
 *
 * Last Modified : Ven 02 Out 2020 12:52:29 CET
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

#include "include/Vectorization/SIMD/AVX512Gen.h"
#include "include/Utils.h"
#include "include/Vectorization/SIMD/CostTable.h"
#include <algorithm>
#include <map>
#include <utility>

using namespace macveth;

// ---------------------------------------------
SIMDGenerator::SIMDInst
AVX512Gen::createSIMDInst(std::string Op, std::string Res, std::string Width,
                          std::string DataType, std::string PrefS,
                          std::string SuffS, std::list<std::string> Args,
                          SIMDGenerator::SIMDType SType, int TacID) {
  // Get the function
  std::string Pattern = CostTable::getPattern(AVX512Gen::NArch, Op);

  // Replace fills in pattern
  std::string AVXFunc =
      SIMDGenerator::replacePatterns(Pattern, Width, DataType, PrefS, SuffS);

  // Generate SIMD inst
  SIMDGenerator::SIMDInst I(Res, AVXFunc, Args, "", {}, TacID);

  // Retrieving cost of function
  SIMDGenerator::SIMDCostInfo C(CostTable::getRow(AVX512Gen::NArch, Op));
  I.Cost = C;
  I.SType = SType;

  return I;
}

// ---------------------------------------------
SIMDGenerator::SIMDInst AVX512Gen::genMultAccOp(SIMDGenerator::SIMDInst Mul,
                                                SIMDGenerator::SIMDInst Acc) {
  SIMDGenerator::SIMDInst Fuse;

  std::string Res = Acc.Result;
  std::string Width = getMapWidth(Mul.W);
  std::string DataType = getMapType(Mul.DT);
  std::string PrefS = "";
  std::string SuffS = "";
  std::string Op = (Acc.SType == SIMDType::VADD) ? "fmadd" : "fmsub";
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

  return Fuse;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
AVX512Gen::fuseAddSubMult(SIMDGenerator::SIMDInstListType I) {
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
          SIMDGenerator::SIMDInst NewFuse = AVX512Gen::genMultAccOp(P, Inst);
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
std::string AVX512Gen::declareAuxArray(VectorIR::VDataType DT) {
  return getNextArrRegister(VDTypeToCType[DT],
                            getMaxWidth() / VectorIR::VDataTypeWidthBits[DT]);
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
AVX512Gen::horizontalSingleReduction(SIMDGenerator::SIMDInstListType TIL) {
  // Horizontal operations
  SIMDGenerator::SIMDInstListType IL;
  std::vector<SIMDGenerator::SIMDInst> VIL{std::begin(TIL), std::end(TIL)};

  int NElems = VIL[0].W / VectorIR::VDataTypeWidthBits[VIL[0].DT];
  auto OpRedux = VIL[0].MVOP;
  auto OpReduxType = OpRedux.getType();
  auto OpName = VIL[0].MVOP.toString();
  auto Type = VIL[0].DT;
  auto RegType = getRegisterType(Type, VectorIR::VWidth::W128);
  std::string LoRes = "__mv_lo" + getMapWidth(VIL[0].W);
  std::string HiRes = "__mv_hi" + getMapWidth(VIL[0].W);
  SIMDGenerator::addRegToDeclare(RegType, LoRes, {0});
  SIMDGenerator::addRegToDeclare(RegType, HiRes, {0});
  std::string AccmReg = getAccmReg(VIL[0].VOPResult.Name);
  auto Pos = MVSourceLocation::Position::POSORDER;

  if (Type == VectorIR::VDataType::DOUBLE) {
    // If 4 elements
    if (NElems > 2) {
      // __m128d lo = _mm256_castpd256_pd128(v); zero latency
      genSIMDInst(LoRes, "castpd", "256", "pd128", VectorIR::VWidth::W256, Type,
                  {AccmReg}, SIMDType::VREDUC, &IL, -1, Pos);
      // __m128d hi = _mm256_extractf128_pd(v,0x1);
      genSIMDInst(HiRes, "extract", "", "f128", VectorIR::VWidth::W256, Type,
                  {AccmReg, "0x1"}, SIMDType::VREDUC, &IL, -1, Pos);
      // lo = op(lo,hi);
      genSIMDInst(LoRes, OpName, "", "", VectorIR::VWidth::W128, Type,
                  {LoRes, HiRes}, SIMDType::VREDUC, &IL, -1, Pos);
    } else {
      LoRes = AccmReg;
    }
    // When only 2 elements
    // hi = _mm_shuffle_pd(lo,lo,0x1);
    genSIMDInst(HiRes, "shuffle", "", "", VectorIR::VWidth::W128, Type,
                {LoRes, LoRes, "0x1"}, SIMDType::VREDUC, &IL, -1, Pos);
    // val = _mm_cvtsd_f64(op(lo,hi));
    genSIMDInst(LoRes, OpName, "", "", VectorIR::VWidth::W128, Type,
                {LoRes, HiRes}, SIMDType::VREDUC, &IL, -1, Pos);
    genSIMDInst(VIL[0].Result, "cvtsd", "", "f64", VectorIR::VWidth::W128, Type,
                {LoRes}, SIMDType::VREDUC, &IL, -1, Pos);
  } else if (Type == VectorIR::VDataType::FLOAT) {
    // 8 elements
    if (NElems > 4) {
      // __m128 lo =
      genSIMDInst(LoRes, "castps", "256", "ps128", VectorIR::VWidth::W256, Type,
                  {AccmReg}, SIMDType::VREDUC, &IL, -1, Pos);
      // __m128 lo = cast(v)
      genSIMDInst(HiRes, "extract", "", "f128", VectorIR::VWidth::W256, Type,
                  {AccmReg, "0x1"}, SIMDType::VREDUC, &IL, -1, Pos);

      // lo = op(lo,hi);
      genSIMDInst(LoRes, OpName, "", "", VectorIR::VWidth::W128, Type,
                  {LoRes, HiRes}, SIMDType::VREDUC, &IL, -1, Pos);
    } else {
      LoRes = AccmReg;
    }
    // 4 elements
    // hi = movehl(lo);
    genSIMDInst(HiRes, "move", "", "hl", VectorIR::VWidth::W128, Type,
                {LoRes, LoRes}, SIMDType::VREDUC, &IL, -1, Pos);
    // lo = op(lo,hi);
    genSIMDInst(LoRes, OpName, "", "", VectorIR::VWidth::W128, Type,
                {LoRes, HiRes}, SIMDType::VREDUC, &IL, -1, Pos);
    // 2 elements
    // hi = shuffle(lo);
    genSIMDInst(HiRes, "shuffle", "", "", VectorIR::VWidth::W128, Type,
                {LoRes, LoRes, "0b00001110"}, SIMDType::VREDUC, &IL, -1, Pos);
    // lo = op(lo,hi);
    genSIMDInst(LoRes, OpName, "", "", VectorIR::VWidth::W128, Type,
                {LoRes, HiRes}, SIMDType::VREDUC, &IL, -1, Pos);

    genSIMDInst(VIL[0].Result, "cvtss", "", "f32", VectorIR::VWidth::W128, Type,
                {LoRes}, SIMDType::VREDUC, &IL, -1, Pos);
  } else {
    // FIXME: should we implement this for integer arithmetic?
    MVErr("Not implemented this approach yet for other types than float or "
          "doubles");
  }

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
AVX512Gen::horizontalReductionFusion(SIMDGenerator::SIMDInstListType TIL) {
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
  int Loc = 0;
  std::vector<std::string> VAccm;
  std::vector<std::string> VRedux;
  for (int t = 0; t < NumRedux; ++t) {
    VAccm.push_back(VIL[t].Args.front());
    VRedux.push_back(VIL[t].Result);
    Loc = std::max(Loc, VIL[t].VOPResult.Order);
  }

  Utils::printDebug("AVX512Gen", "Location is = " + std::to_string(Loc) +
                                     ", otherwise it would be " +
                                     std::to_string(VIL[0].VOPResult.Order));

  int S = (NumRedux + (NumRedux % 2)) / 2;
  std::string OP =
      (OpRedux.ClangOP == BinaryOperator::Opcode::BO_Add) ? "hadd" : "hsub";

  auto Stride = 2;
  while (true) {
    for (int i = 0; i < NumRedux; i += Stride) {
      genSIMDInst(VIL[0].VOPResult, OP, "", "",
                  {VAccm[i], (NumRedux > 1) ? VAccm[i + 1] : VAccm[i]},
                  SIMDType::VREDUC, &IL, Loc, Pos, VAccm[i]);
      if (i > 0) {
        VAccm[i - 1] = VAccm[i];
      }
    }
    Stride *= 2;
    S = (S + (S % 2)) / 2;
    // Final permutation
    if (S == 1) {
      // README: for further information
      // https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction
      // https://stackoverflow.com/questions/49941645/get-sum-of-values-stored-in-m256d-with-sse-avx
      auto W = MapWidth[VIL[0].W];
      // Operand 1
      auto Blend =
          replacePatterns(CostTable::getPattern(AVX512Gen::NArch, "blend"), W,
                          MapType[VIL[0].DT], "", "");
      auto Mask1 =
          VIL[0].DT == VectorIR::VDataType::DOUBLE ? "0b1100" : "0b11110000";
      // Operand 2
      auto PermS = "2f128";
      auto Mask2 = "0x21";
      auto Perm =
          replacePatterns(CostTable::getPattern(AVX512Gen::NArch, "permute"), W,
                          MapType[VIL[0].DT], "", PermS);
      auto V0 = VAccm[0];
      auto V1 =
          (NumRedux > 4) ? VAccm[4] : (NumRedux > 2) ? VAccm[2] : VAccm[1];
      auto Op1 = genGenericFunc(Blend, {V0, V1, Mask1});
      auto Op2 = genGenericFunc(Perm, {V0, V1, Mask2});

      OP = (OP == "hadd") ? "add" : "sub";
      genSIMDInst(VIL[0].VOPResult, OP, "", "", {Op1, Op2}, SIMDType::VREDUC,
                  &IL, Loc, Pos, VAccm[0]);

      break;
    }
  }

  auto AuxArray = declareAuxArray(VIL[0].DT);
  genSIMDInst(VIL[0].VOPResult, "store", "", "u", {AuxArray, VAccm[0]},
              SIMDType::VSTORE, &IL, Loc, Pos);
  for (int R = 0; R < NumRedux; ++R) {
    auto Idx = "[" + std::to_string(R) + "]";
    addNonSIMDInst(VRedux[R], AuxArray + Idx, SIMDType::VSEQ, &IL, Loc, Pos);
  }

  return IL;
}

// ---------------------------------------------
std::string AVX512Gen::shuffleArguments(std::string A1, std::string A2,
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
      replacePatterns(CostTable::getPattern(AVX512Gen::NArch, "shuffle"),
                      getMapWidth(Width), getMapType(I.DT), "", ""),
      {A1, A2, Mask});
}

// ---------------------------------------------
std::string AVX512Gen::permuteArguments(std::string A1, std::string A2,
                                        SIMDGenerator::SIMDInst I, int Pos) {
  std::string Mask = "0x21";
  std::string PermW = getMapWidth(I.W);
  std::string PermT = getMapType(I.DT);
  return genGenericFunc(
      replacePatterns(CostTable::getPattern(AVX512Gen::NArch, "permute"), PermW,
                      PermT, "", "2f128"),
      {A1, A2, Mask});
}

// ---------------------------------------------
std::string AVX512Gen::extractArgument(std::string A, SIMDGenerator::SIMDInst I,
                                       int Hi) {
  std::string Mask = (Hi == 0) ? "0x00" : "0x01";
  std::string PermW = getMapWidth(I.W);
  std::string PermT = getMapType(I.DT);
  return genGenericFunc(
      replacePatterns(CostTable::getPattern(AVX512Gen::NArch, "extract"), PermW,
                      PermT, "", "f128"),
      {A, Mask});
}

// ---------------------------------------------
std::list<std::string> AVX512Gen::renderSIMDRegister(SIMDInstListType S) {
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
AVX512Gen::generalReductionFusion(SIMDGenerator::SIMDInstListType TIL) {
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

  auto Pos = MVSourceLocation::Position::POSORDER;

  // Shuffles
  for (int i = 0; i < NumRedux; i += 2) {
    auto A0 =
        shuffleArguments(VAccm[i], (NumRedux > i + 1) ? VAccm[i + 1] : VAccm[i],
                         VIL[i].W, VIL[i], 0);
    auto A1 =
        shuffleArguments(VAccm[i], (NumRedux > i + 1) ? VAccm[i + 1] : VAccm[i],
                         VIL[i].W, VIL[i], 1);
    genSIMDInst(VIL[i].VOPResult, OP, "", "", {A0, A1}, SIMDType::VREDUC, &IL,
                VIL[i].TacID, Pos, VAccm[i]);
    if (i > 0) {
      VAccm[i - 1] = VAccm[i];
    }
  }

  // Permutations
  for (int i = 0; i < NumRedux; i += 4) {
    auto W = MapWidth[VIL[0].W];
    // Operand 1
    auto Blend =
        replacePatterns(CostTable::getPattern(AVX512Gen::NArch, "blend"), W,
                        MapType[VIL[0].DT], "", "");
    auto Mask =
        VIL[0].DT == VectorIR::VDataType::DOUBLE ? "0b1100" : "0b11110000";
    auto A0 = genGenericFunc(
        Blend, {VAccm[i], (NumRedux > i + 1) ? VAccm[i + 1] : VAccm[i], Mask});
    auto A1 = permuteArguments(
        VAccm[i], (NumRedux > i + 1) ? VAccm[i + 1] : VAccm[i], VIL[i], 1);
    genSIMDInst(VIL[i].VOPResult, OP, "", "", {A0, A1}, SIMDType::VREDUC, &IL,
                VIL[i].TacID, Pos, VAccm[i]);
    if (i > 0)
      VAccm[i - 1] = VAccm[i];
  }

  // Extract values depending if 4 or 8 elements, different approaches due to
  // intrisics design
  std::vector<int> VIdx = {0, 1, 2, 3, 4, 5, 6, 7};
  std::string AuxArray = declareAuxArray(VIL[0].DT);
  if (NElems <= 4) {
    // FIXME: Store approach
    genSIMDInst(VIL[0].VOPResult, "store", "", "u", {AuxArray, VAccm[0]},
                SIMDType::VSTORE, &IL);
    for (int R = 0; R < NumRedux; ++R) {
      std::string Idx = "[" + std::to_string(VIdx[R]) + "]";
      addNonSIMDInst(VRedux[R], AuxArray + Idx, SIMDType::VSEQ, &IL);
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
                  SIMDType::VREDUC, &IL, -1, Pos);
      if (NumRedux > 1) {
        genSIMDInst(Res, "store", "", "u", VectorIR::VWidth::W128, VIL[R].DT,
                    {"&" + AuxArray + "[" + std::to_string(R) + "]", Res},
                    SIMDType::VSTORE, &IL);
      }
    }
    if (NumRedux == 1) {
      // A simple conversion
      auto Res = getNextAuxRegister(VIL[0].VOPResult.Name);
      auto Suf = VIL[0].DT == VectorIR::VDataType::DOUBLE ? "f64" : "f32";
      auto Type = VIL[0].DT == VectorIR::VDataType::DOUBLE
                      ? VectorIR::VDataType::SDOUBLE
                      : VectorIR::VDataType::SFLOAT;
      genSIMDInst(VRedux[0], "cvt", "", Suf, VectorIR::VWidth::W128, Type,
                  {Res}, SIMDType::VSEQ, &IL);
    } else {
      // FIXME: not sure this works properly
      for (int R = 0; R < NumRedux; ++R) {
        std::string Idx = "[" + std::to_string(VIdx[R]) + "]";
        addNonSIMDInst(VRedux[R], AuxArray + Idx, SIMDType::VSEQ, &IL);
      }
    }
  }
  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
AVX512Gen::fuseReductions(SIMDGenerator::SIMDInstListType TIL) {
  // Copy list
  SIMDGenerator::SIMDInstListType IL;
  SIMDGenerator::SIMDInstListType SkipList;
  // List of reduction candidates to be fused
  std::map<int, std::map<std::string, SIMDGenerator::SIMDInstListType>> LRedux;
  std::map<int,
           std::map<SIMDGenerator::SIMDInst, SIMDGenerator::SIMDInstListType>>
      ReplaceFusedRedux;
  for (auto I : TIL) {
    if (I.SType == SIMDGenerator::SIMDType::VREDUC) {
      LRedux[I.VOPResult.Size][I.VOPResult.getOperandLoop() + I.MVOP.toString()]
          .push_back(I);
    }
    if (LRedux.size() == 0) {
      continue;
    }
    // For each reduction found, check whether they can be fused together or
    // not:
    // they can be fused if and only if they are in the same loop dimension/name
    // and the same loop, basically (because they can have same name but be
    // different loops). This is important for coherence in the program
    for (auto const &R : LRedux) {
      for (auto const &L : R.second) { /* this is valid >=C++11 */
        auto LInsSize = L.second.size();
        auto Ins = L.second.front();
        auto W = L.second.front().VOPResult.Width;
        auto DT = L.second.front().VOPResult.DType;
        auto DW = VectorIR::VDataTypeWidthBits[DT];
        // Only compute reductions if max found
        if (W / DW > LInsSize) {
          continue;
        }
        auto OpReduxType = L.second.front().MVOP.T;
        auto OpRedux = L.second.front().MVOP;
        SIMDGenerator::SIMDInstListType FusedRedux;
        // This is the horizontal approach, only valid for AVX512 and additions
        // and subtraction
        if (LInsSize == 1) {
          Utils::printDebug("AVX512Gen", "Single reduction approach (" +
                                             std::to_string(LInsSize) + ")");
          FusedRedux = horizontalSingleReduction(L.second);
        } else if (((OpReduxType == MVOpType::CLANG_BINOP) &&
                    ((OpRedux.ClangOP == BinaryOperator::Opcode::BO_Add) ||
                     (OpRedux.ClangOP == BinaryOperator::Opcode::BO_Sub)))) {
          // Horizontal approach only worth it when we have two or more
          // reductions
          Utils::printDebug("AVX512Gen", "Horizontal approach (" +
                                             std::to_string(LInsSize) + ")");
          FusedRedux = horizontalReductionFusion(L.second);
        } else {
          Utils::printDebug("AVX512Gen", "General approach approach (" +
                                             std::to_string(LInsSize) + ")");
          FusedRedux = generalReductionFusion(L.second);
        }
        ReplaceFusedRedux[R.first][L.second.back()] = FusedRedux;
        for (auto SInst : L.second) {
          if (SInst == L.second.back()) {
            break;
          }
          SkipList.push_back(SInst);
        }
        LRedux[R.first][L.first].clear();
      }
    }
  }

  for (auto const &R : LRedux) {
    for (auto const &L : R.second) { /* this is valid >=C++11 */
      auto LInsSize = L.second.size();
      if (LInsSize == 0) {
        continue;
      }
      auto OpReduxType = L.second.front().MVOP.T;
      auto OpRedux = L.second.front().MVOP;
      SIMDGenerator::SIMDInstListType FusedRedux;
      // This is the horizontal approach, only valid for AVX512 and additions
      // and subtraction
      if (LInsSize == 1) {
        Utils::printDebug("AVX512Gen", "Single reduction approach (" +
                                           std::to_string(LInsSize) + ")");
        FusedRedux = horizontalSingleReduction(L.second);
      } else if (((OpReduxType == MVOpType::CLANG_BINOP) &&
                  ((OpRedux.ClangOP == BinaryOperator::Opcode::BO_Add) ||
                   (OpRedux.ClangOP == BinaryOperator::Opcode::BO_Sub)))) {
        // Horizontal approach only worth it when we have two or more
        // reductions
        Utils::printDebug("AVX512Gen", "Horizontal approach (" +
                                           std::to_string(LInsSize) + ")");
        FusedRedux = horizontalReductionFusion(L.second);
      } else {
        Utils::printDebug("AVX512Gen", "General approach approach (" +
                                           std::to_string(LInsSize) + ")");
        FusedRedux = generalReductionFusion(L.second);
      }

      ReplaceFusedRedux[R.first][L.second.back()] = FusedRedux;
      for (auto SInst : L.second) {
        if (SInst == L.second.back()) {
          break;
        }
        SkipList.push_back(SInst);
      }
    }
  }
  // Reorder SIMDInstructions yet
  for (auto I : TIL) {
    if (std::find(SkipList.begin(), SkipList.end(), I) != SkipList.end()) {
      continue;
    }
    for (auto &R : ReplaceFusedRedux) {
      if (R.second.count(I) > 0) {
        IL.splice(IL.end(), R.second.at(I));
        continue;
      }
    }
    IL.push_back(I);
  }

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
AVX512Gen::peepholeOptimizations(SIMDGenerator::SIMDInstListType I) {
  SIMDGenerator::SIMDInstListType IL(I);

  // Fuse reductions if any and fuse them
  IL = fuseReductions(IL);
  // Fuse operations: find potential and applicable FMADD/FMSUB, if not disabled
  if (!MVOptions::DisableFMA) {
    IL = fuseAddSubMult(IL);
  }

  // TODO: eliminate duplicated instructions (?)
  // IL = eliminateDeadIns();

  return IL;
}

// ---------------------------------------------
std::string AVX512Gen::getRegisterType(VectorIR::VDataType DT,
                                       VectorIR::VWidth W) {
  auto Width = W;
  assert((W != 512) && "Width not permitted in AVX512!!");
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
SIMDGenerator::SIMDInst AVX512Gen::genSIMDInst(
    std::string Result, std::string Op, std::string PrefS, std::string SuffS,
    VectorIR::VWidth Width, VectorIR::VDataType Type,
    std::list<std::string> Args, SIMDGenerator::SIMDType SType,
    SIMDGenerator::SIMDInstListType *IL, int Order,
    MVSourceLocation::Position P, std::string NameOp, std::string MVFunc,
    std::list<std::string> MVArgs, MVOp MVOP) {
  // Get the function
  std::string Pattern = CostTable::getPattern(AVX512Gen::NArch, Op);

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

  // Retrieving cost of function
  SIMDGenerator::SIMDCostInfo C(CostTable::getRow(AVX512Gen::NArch, Op));
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
SIMDGenerator::SIMDInst AVX512Gen::genSIMDInst(
    VectorIR::VOperand V, std::string Op, std::string PrefS, std::string SuffS,
    std::list<std::string> Args, SIMDGenerator::SIMDType SType,
    SIMDGenerator::SIMDInstListType *IL, int Order,
    MVSourceLocation::Position P, std::string NameOp, std::string MVFunc,
    std::list<std::string> MVArgs, MVOp MVOP) {
  // Get the function
  std::string Pattern = CostTable::getPattern(AVX512Gen::NArch, Op);

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
  SIMDGenerator::SIMDCostInfo C(CostTable::getRow(AVX512Gen::NArch, Op));
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
std::string AVX512Gen::getMask(unsigned int MaskVect, int NElems,
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
  std::string Indices = std::to_string(MaskVect & 0x1);
  for (int i = 1; i < NElems; ++i) {
    int M = (MaskVect >> i) & 0x1;
    Indices = std::to_string(M) + "," + Indices;
  }
  Mask += Indices + ")";
  return Mask;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX512Gen::vpack(VectorIR::VOperand V) {
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

  // Hack: if data type is integer then use the si128/si256 notation instead of
  // epi
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
SIMDGenerator::SIMDInstListType AVX512Gen::vbcast(VectorIR::VOperand V) {
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
SIMDGenerator::SIMDInstListType AVX512Gen::vgather(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  auto Op = "gather";

  // List of parameters
  std::list<std::string> Args;

  // To gather elements
  // Generate preffix: must be i32 or i64, depending on the VIndex width
  std::string Scale = "8";
  auto VIndexSuffix = "epi64x";
  std::string PrefS = "";

  // Generation of the preffix
  if (V.IsPartial) {
    // Who tf did intel intrinsics??????? I mean, it makes no sense at
    // all that for loads we use "maskload", then for gathers
    // "mask_[i32|i64]gather", why the underscore? It should not be difficult to
    // normalize the API so the signature of all functions are constructed the
    // same way
    PrefS += "mask_";
    std::string NeutralReg = "_mm" + getMapWidth(V.getWidth()) + "_setzero_" +
                             getMapType(V.getDataType()) + "()";
    Args.push_back(NeutralReg);
  }
  PrefS += "i64";
  if (V.VSize > 4) {
    PrefS = "i32";
    VIndexSuffix = "epi32";
    Scale = std::to_string(4);
  }

  // FIXME: fuck me, this is garbage
  auto VIndex = "_mm" + MapWidth[V.Width] + "_set_" + VIndexSuffix + "(";
  auto CopyIdx = V.Idx;
  auto MinIdx = std::min_element(CopyIdx.begin(), CopyIdx.end());
  auto MinVal = abs(*MinIdx);
  auto BaseIdx = V.UOP[MinIdx - CopyIdx.begin()]->getValue();
  // This way we avoid negative indices
  if (MinVal < 0) {
    for (auto T = 0; T < CopyIdx.size(); ++T) {
      CopyIdx[T] += MinVal;
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
    Args.push_back(getMask(V.Mask, V.Size, V.getWidth()));
  }

  // Scale can only be: 1, 2, 4, 8
  Args.push_back(Scale);

  // Adding SIMD inst to the list
  genSIMDInst(V, Op, PrefS, "", Args, SIMDType::VGATHER, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX512Gen::vset(VectorIR::VOperand V) {
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
SIMDGenerator::SIMDInstListType AVX512Gen::vstore(VectorIR::VectorOP V) {
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
SIMDGenerator::SIMDInstListType AVX512Gen::vscatter(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;

  // TODO: check if any contiguous in order to optimize minimally this
  for (int N = 0; N < V.R.VSize; ++N) {
    std::string Idx = "[" + std::to_string(N) + "]";
    addNonSIMDInst(V.R.UOP[N]->getRegisterValue(), V.R.getName() + Idx,
                   SIMDType::VSCATTER, &IL, V.Order);
  }

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX512Gen::vadd(VectorIR::VectorOP V) {
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
SIMDGenerator::SIMDInstListType AVX512Gen::vmul(VectorIR::VectorOP V) {
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
SIMDGenerator::SIMDInstListType AVX512Gen::vsub(VectorIR::VectorOP V) {
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
SIMDGenerator::SIMDInstListType AVX512Gen::vdiv(VectorIR::VectorOP V) {
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
SIMDGenerator::SIMDInstListType AVX512Gen::vmod(VectorIR::VectorOP V) {
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
SIMDGenerator::SIMDInstListType AVX512Gen::vfunc(VectorIR::VectorOP V) {
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
std::vector<std::string> AVX512Gen::getInitValues(VectorIR::VectorOP V) {
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

  for (int i = 0; i < VS - 1 + (VS % 2); ++i) {
    InitVal.push_back(NeutralValue);
  }
  InitVal.push_back(V.R.UOP[0]->getRegisterValue());

  std::list<std::string> InitValList(InitVal.begin(), InitVal.end());
  std::string Reg = V.R.Name;
  // Fuck, this is awful...
  if (getAccmReg(Reg) != "") {
    Reg = getAccmReg(Reg);
  } else if (getAuxReg(Reg) != "") {
    Reg = getAuxReg(Reg);
  }
  InitReg.push_back(createSIMDInst("set", Reg, getMapWidth(V.R.Width),
                                   getMapType(V.R.DType), "", "", InitValList,
                                   SIMDType::INIT, V.Order));
  return InitVal;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX512Gen::vreduce(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  SIMDGenerator::SIMDInstListType TIL;
  auto RegType = getRegisterType(V.DT, V.VW);
  auto IsANewReduction = !hasAlreadyBeenMapped(V.R.Name);
  Utils::printDebug("AVX512Gen",
                    "this is a reduction = " + std::to_string(IsANewReduction) +
                        "; name = " + V.R.Name);
  auto RegAccm = getNextAccmRegister(V.R.Name);
  if (IsANewReduction) {
    // FIXME: perform initializations in another way
    SIMDGenerator::addRegToDeclareInitVal(RegType, RegAccm, getInitValues(V));
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

  auto Pos = MVSourceLocation::Position::POSORDER;
  /// This is a 'hack': when a reduction is found, instead of generate all the
  /// instructions needed for it, wait until the peephole optimization says
  /// if there are any other reductions that can be packed and fused together.
  /// P.S. creating hacks make me a hacker? lol
  if (IsANewReduction) {
    genSIMDInst(V.R, "VREDUX", "", "", {RegAccm}, SIMDType::VREDUC, &IL,
                V.Order, Pos, ReduxVar, "", {}, V.getMVOp());
  } else {
    Utils::printDebug("AVX512Gen", "it is not a new reduction");
  }

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX512Gen::vseq(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;

  // Adding SIMD inst to the list
  addNonSIMDInst(V, SIMDType::VSEQ, &IL, V.R.Order);

  return IL;
}