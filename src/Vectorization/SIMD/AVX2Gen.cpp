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
  std::string Pattern = CostTable::getPattern(AVX2Gen::NArch, Op);

  // Replace fills in pattern
  std::string AVXFunc =
      SIMDGenerator::replacePatterns(Pattern, Width, DataType, PrefS, SuffS);

  // Generate SIMD inst
  SIMDGenerator::SIMDInst I(Res, AVXFunc, Args, "", {}, TacID);

  // Retrieving cost of function
  I.Cost += CostTable::getLatency(AVX2Gen::NArch, Op);
  I.SType = SType;

  return I;
}

// ---------------------------------------------
SIMDGenerator::SIMDInst AVX2Gen::genMultAccOp(SIMDGenerator::SIMDInst Mul,
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
AVX2Gen::fuseAddSubMult(SIMDGenerator::SIMDInstListType I) {
  SIMDGenerator::SIMDInstListType IL;
  SIMDGenerator::SIMDInstListType PotentialFuse;
  SIMDGenerator::SIMDInstListType SkipList;
  std::map<SIMDGenerator::SIMDInst, SIMDGenerator::SIMDInst> Fuses;

  // Search replacements
  for (auto Inst : I) {
    // If we find a multiply operation it may be a FMADD
    if (Inst.SType == SIMDGenerator::SIMDType::VMUL) {
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
          SIMDGenerator::SIMDInst NewFuse = AVX2Gen::genMultAccOp(P, Inst);
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
  std::string Type = VDTypeToCType[DT];
  std::string AuxArr =
      getNextArrRegister(Type, 256 / VectorIR::VDataTypeWidthBits[DT]);
  return AuxArr;
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

  int NElems = VIL[0].W / VectorIR::VDataTypeWidthBits[VIL[0].DT];
  int Steps = log2(NElems + (NElems % 2));
  int NumRedux = TIL.size();
  if (NumRedux > NElems) {
    assert(false && "You can not have more reductions than elements in vector");
  }
  std::vector<std::string> VAccm;
  std::vector<std::string> VRedux;
  for (int t = 0; t < NumRedux; ++t) {
    VAccm.push_back(VIL[t].Args.front());
    VRedux.push_back(VIL[t].Result);
  }

  // FIXME:
  auto OP = (VIL[0].FuncName == ("add") || VIL[0].FuncName == ("+")) ? "hsub"
                                                                     : "hadd";

  int S = (NumRedux + (NumRedux % 2)) / 2;
  while (true) {
    addSIMDInst(VIL[0].VOPResult, OP, "", "",
                {VAccm[0], (NumRedux > 1) ? VAccm[1] : VAccm[0]},
                SIMDType::VREDUC, &IL, VAccm[0]);
    if (S > 1) {
      addSIMDInst(VIL[2].VOPResult, OP, "", "",
                  {VAccm[2], (NumRedux > 3) ? VAccm[3] : VAccm[2]},
                  SIMDType::VREDUC, &IL, VAccm[2]);
      VAccm[1] = VAccm[2];
    }
    if (S > 2) {
      addSIMDInst(VIL[4].VOPResult, OP, "", "",
                  {VAccm[4], (NumRedux > 5) ? VAccm[5] : VAccm[4]},
                  SIMDType::VREDUC, &IL, VAccm[4]);
      VAccm[3] = VAccm[4];
    }
    if (S > 3) {
      addSIMDInst(VIL[6].VOPResult, OP, "", "",
                  {VAccm[6], (NumRedux > 7) ? VAccm[7] : VAccm[6]},
                  SIMDType::VREDUC, &IL, VAccm[6]);
      VAccm[5] = VAccm[6];
    }
    S = (S + (S % 2)) / 2;
    // Final permutation
    if (S == 1) {
      std::string PermS = "";
      std::string Mask = "";
      std::string PermW = MapWidth[VIL[0].W];
      if (VIL[0].W == VectorIR::VWidth::W256) {
        if (VIL[0].DT == VectorIR::VDataType::DOUBLE) {
          PermS = "4x64";
          Mask = "0xd8";
        } else if (VIL[0].DT == VectorIR::VDataType::FLOAT) {
          PermS = "var8x32";
          Mask = genGenericFunc(
              replacePatterns(CostTable::getPattern(AVX2Gen::NArch, "set"),
                              PermW, "epi32", "", ""),
              {"7", "3", "6", "2", "5", "1", "4", "0"});
        }
      }
      std::string Perm =
          replacePatterns(CostTable::getPattern(AVX2Gen::NArch, "permute"),
                          PermW, MapType[VIL[0].DT], "", PermS);
      std::string Op1 = genGenericFunc(Perm, {VAccm[0], Mask});
      std::string Op2 = VAccm[0];
      if (NumRedux > 2)
        Op2 = genGenericFunc(Perm, {VAccm[1], Mask});
      addSIMDInst(VIL[0].VOPResult, OP, "", "", {Op1, Op2}, SIMDType::VREDUC,
                  &IL, VAccm[0]);

      break;
    }
  }

  std::string TD = "";
  std::string TW = "";
  std::string TSuffix = "";
  // FIXME: Store approach
  std::string AuxArray = declareAuxArray(VIL[0].DT);
  addSIMDInst(VIL[0].VOPResult, "store", "", "u", {AuxArray, VAccm[0]},
              SIMDType::VSTORER, &IL);
  std::vector<int> VIdx;
  if (VIL[0].DT == VectorIR::VDataType::DOUBLE) {
    VIdx = {0, 2, 1, 3};
  }
  if (VIL[0].DT == VectorIR::VDataType::FLOAT) {
    VIdx = {0, 1, 4, 5, 2, 3, 6, 7};
  }
  for (int R = 0; R < NumRedux; ++R) {
    std::string Idx = "[" + std::to_string(VIdx[R]) + "]";
    addNonSIMDInst(VRedux[R], AuxArray + Idx, SIMDType::VSEQR, &IL);
  }

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
AVX2Gen::fuseReductions(SIMDGenerator::SIMDInstListType TIL) {
  // Copy list
  SIMDGenerator::SIMDInstListType IL;
  SIMDGenerator::SIMDInstListType SkipList;
  // List of reduction candidates to be fused
  std::map<std::string, SIMDGenerator::SIMDInstListType> LRedux;
  std::map<SIMDGenerator::SIMDInst, SIMDGenerator::SIMDInstListType>
      ReplaceFusedRedux;
  for (auto I : TIL) {
    if (I.SType == SIMDGenerator::SIMDType::VREDUC) {
      LRedux[I.VOPResult.getOperandLoop()].push_back(I);
    }
    if (LRedux.size() == 0) {
      continue;
    }
    // For each reduction found, check whether they can be fused together or
    // not:
    // they can be fused if and only if they are in the same loop dimension/name
    // and the same loop, basically (because they can have same name but be
    // different loops). This is important for coherence in the program
    for (auto const &L : LRedux) { /* this is valid >=C++11 */
      int MaxFusableRedux =
          (L.second.front().VOPResult.DType == VectorIR::VDataType::DOUBLE) ? 4
                                                                            : 8;
      // Only compute reductions if max found
      if (MaxFusableRedux > L.second.size()) {
        continue;
      }
      auto FusedRedux = generalReductionFusion(L.second);
      ReplaceFusedRedux[L.second.back()] = FusedRedux;
      for (auto SInst : L.second) {
        if (SInst == L.second.back()) {
          break;
        }
        SkipList.push_back(SInst);
      }
      LRedux[L.first].clear();
    }
  }

  for (auto const &L : LRedux) { /* this is valid >=C++11 */
    int MaxFusableRedux =
        (L.second.front().VOPResult.DType == VectorIR::VDataType::DOUBLE) ? 4
                                                                          : 8;
    if (L.second.size() == 0) {
      continue;
    }
    auto FusedRedux = generalReductionFusion(L.second);
    ReplaceFusedRedux[L.second.back()] = FusedRedux;
    for (auto SInst : L.second) {
      if (SInst == L.second.back()) {
        break;
      }
      SkipList.push_back(SInst);
    }
  }

  // Reorder SIMDInstructions yet
  for (auto Inst : TIL) {
    if (std::find(SkipList.begin(), SkipList.end(), Inst) != SkipList.end()) {
      continue;
    }
    if (ReplaceFusedRedux.count(Inst) > 0) {
      IL.splice(IL.end(), ReplaceFusedRedux.at(Inst));
      continue;
    }
    IL.push_back(Inst);
  }

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
AVX2Gen::peepholeOptimizations(SIMDGenerator::SIMDInstListType I) {
  SIMDGenerator::SIMDInstListType IL(I);

  // Fuse reductions if any and fusable
  IL = fuseReductions(IL);
  // Fuse operations: find potential and applicable FMADD/FMSUB
  IL = fuseAddSubMult(IL);

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
SIMDGenerator::SIMDInst
AVX2Gen::addSIMDInst(VectorIR::VOperand V, std::string Op, std::string PrefS,
                     std::string SuffS, std::list<std::string> Args,
                     SIMDGenerator::SIMDType SType,
                     SIMDGenerator::SIMDInstListType *IL, std::string NameOp,
                     std::string MVFunc, std::list<std::string> MVArgs) {
  // Get the function
  std::string Pattern = CostTable::getPattern(AVX2Gen::NArch, Op);

  // Replace fills in pattern
  std::string AVXFunc =
      replacePatterns(Pattern, getMapWidth(V.getWidth()),
                      getMapType(V.getDataType()), PrefS, SuffS);

  // Generate SIMD inst
  SIMDGenerator::SIMDInst I((NameOp == "") ? V.getName() : NameOp, AVXFunc,
                            Args, MVFunc, MVArgs, V.Order);

  // Retrieving cost of function
  I.Cost += CostTable::getLatency(AVX2Gen::NArch, Op);
  I.SType = SType;

  // Data type and width
  I.DT = V.getDataType();
  I.W = V.getWidth();
  I.VOPResult = V;

  // Adding instruction to the list
  IL->push_back(I);

  return I;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vpack(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO: generate preffix
  std::string PrefS = "";
  // TODO: generate suffix
  std::string SuffS = "u";
  // Mask
  PrefS += (V.IsPartial) ? "mask" : "";

  // TODO:
  // Type of load: load/u
  // [1]
  // https://software.intel.com/en-us/forums/intel-isa-extensions/topic/752392
  // [2] https://stackoverflow.com/questions/36191748/difference-between-\
  // load1-and-broadcast-intrinsics
  std::string Op = "load";

  // TODO: check
  // List of parameters
  std::list<std::string> Args;
  Args.push_back(getOpName(V, true, true));

  // Adding SIMD inst to the list
  addSIMDInst(V, Op, PrefS, SuffS, Args, SIMDType::VPACK, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vbcast(VectorIR::VOperand V) {
  std::string Op = "broadcast";
  SIMDGenerator::SIMDInstListType IL;
  // TODO: generate preffix
  std::string PrefS = "";
  // TODO: generate suffix
  std::string SuffS = "";
  // Mask
  PrefS += (V.IsPartial) ? "mask" : "";

  // TODO: check
  // List of parameters
  std::list<std::string> Args;
  Args.push_back(getOpName(V, true, true));

  if (V.getDataType() == VectorIR::VDataType::DOUBLE) {
    V.DType = VectorIR::VDataType::SDOUBLE;
  }

  // FIXME:
  // V.Width = VectorIR::VWidth::W256;

  // Adding SIMD inst to the list
  addSIMDInst(V, Op, PrefS, SuffS, Args, SIMDType::VBCAST, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vgather(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO: generate preffix
  std::string PrefS = "";
  // TODO: generate suffix
  std::string SuffS = "";
  // Mask
  PrefS += (V.IsPartial) ? "mask" : "";

  // TODO:
  std::string Op = "gather";

  // TODO: check
  // List of parameters
  std::list<std::string> Args;
  Args.push_back(V.UOP[0]->getValue());

  // Adding SIMD inst to the list
  addSIMDInst(V, Op, PrefS, SuffS, Args, SIMDType::VGATHER, &IL);

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
    Args.push_back((V.UOP[0] != NULL) ? V.UOP[0]->getRegisterValue() : "0");
  } else {
    for (int n = 0; n < V.VSize; n++) {
      Args.push_back((V.UOP[n] != NULL) ? V.UOP[n]->getValue() : "0.0");
    }
  }
  for (int t = V.VSize; t < (V.Width / VectorIR::VDataTypeWidthBits[V.DType]);
       ++t) {
    Args.push_back("0");
  }
  Args.reverse();

  // Adding SIMD inst to the list
  addSIMDInst(V, Op, "", SuffS, Args, SIMDType::VSET, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vstore(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO: generate preffix
  std::string PrefS = "";
  // TODO: generate suffix
  std::string SuffS = "";
  // Mask
  PrefS += (V.R.IsPartial) ? "mask" : "";

  // TODO:
  std::string Op = "store";

  // TODO: check
  // List of parameters
  std::list<std::string> Args;
  Args.push_back(getOpName(V.R, true, true));
  Args.push_back(getOpName(V.R, false, false));
  if (V.R.EqualVal) {
    if (V.R.DType == VectorIR::DOUBLE) {
      V.R.DType = VectorIR::SDOUBLE;
      V.R.Width = VectorIR::W64;
    } else if (V.R.DType == VectorIR::FLOAT) {
      V.R.DType = VectorIR::SFLOAT;
      V.R.Width = VectorIR::W32;
    }
  } else {
    // Use unaligned whenever it is possible
    SuffS = "u";
  }

  // Adding SIMD inst to the list
  addSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VSTORE, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vscatter(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO: generate preffix
  std::string PrefS = "";
  // TODO: generate suffix
  std::string SuffS = "";
  // Mask
  PrefS += (V.R.IsPartial) ? "mask" : "";

  // TODO:
  std::string Op = "scatter";

  // TODO: check
  // List of parameters
  std::list<std::string> Args;
  Args.push_back(V.OpA.getName());
  Args.push_back(V.OpB.getName());

  // Adding SIMD inst to the list
  addSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VSCATTER, &IL);

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
  addSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VADD, &IL);

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
  addSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VMUL, &IL);

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
  addSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VSUB, &IL);

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
  addSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VDIV, &IL);

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
  addSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VMOD, &IL);

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
  addSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VFUNC, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vreduce(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  SIMDGenerator::SIMDInstListType TIL;
  std::string RegType = getRegisterType(V.DT, V.VW);
  std::string RegAccm = getNextAccmRegister(V.R.Name);
  SIMDGenerator::addRegToDeclare(RegType, RegAccm, {0});

  // Check that the reduction is binary, this should never ever happen
  if (!V.isBinOp()) {
    assert(false && "Reduction is not a binary operation!!!");
  }

  // Some needed values
  std::string TmpReduxVar = (V.R.Name == V.OpB.Name) ? V.OpA.Name : V.OpB.Name;
  auto VCopy = V;
  VCopy.OpA.Name = TmpReduxVar;
  VCopy.R.Name = RegAccm;
  VCopy.OpB.Name = RegAccm;

  // Get type of binary operation, reduction is not the same if
  // addition/substraction than multiplication or division
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
    assert(false && "Reduction must be from an addition, substraction, "
                    "multiplication or division");
  }
  IL.splice(IL.end(), TIL);
  std::string ReduxVar =
      (V.R.Name == V.OpA.Name) ? V.OpA.getRegName() : V.OpB.getRegName();

  /// This is a 'hack': when a reduction is found, instead of generate all the
  /// instructions needed for it, wait until the peephole optimization says
  /// if there are any other reductions that can be packed and fused together.
  /// P.S. creating hacks make me a hacker? lol
  addSIMDInst(V.R, "VREDUX", "", "", {RegAccm}, SIMDType::VREDUC, &IL, ReduxVar,
              V.getMVOp().toString());

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vseq(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;

  // Adding SIMD inst to the list
  addNonSIMDInst(V, SIMDType::VSEQ, &IL);

  return IL;
}
