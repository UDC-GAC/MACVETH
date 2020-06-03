/**
 * File              : AVX2Gen.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 27 Dec 2019 09:00:11 MST
 * Last Modified Date: Mér 15 Xan 2020 11:07:02 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
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
    // if (Utils::contains(SkipList, Inst)) {
    if (std::find(SkipList.begin(), SkipList.end(), Inst) != SkipList.end()) {
      continue;
    }
    if (Fuses.count(Inst) > 0) {
      IL.push_back(Fuses.at(Inst));
      continue;
    }
    IL.push_back(Inst);
  }
  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
AVX2Gen::peepholeOptimizations(SIMDGenerator::SIMDInstListType I) {
  SIMDGenerator::SIMDInstListType IL;

  // Fuse operations: find potential and applicable FMADD/FMSUB
  IL = fuseAddSubMult(I);

  // TODO
  // Fuse reductions

  return IL;
}

// ---------------------------------------------
std::string AVX2Gen::getRegisterType(VectorIR::VDataType DT,
                                     VectorIR::VWidth W) {
  std::string Suffix = "";
  if (DT == VectorIR::VDataType::DOUBLE) {
    Suffix = "d";
  } else if (DT == VectorIR::VDataType::FLOAT) {
    Suffix = "";
  } else {
    Suffix = "i";
  }
  return "__m" + std::to_string(W) + Suffix;
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

  // Adding instruction to the list
  IL->push_back(I);

  return I;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vpack(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  PrefS += (V.IsPartial) ? "mask" : "";

  // TODO
  // Type of load: load/u
  // [1] software.intel.com/en-us/forums/intel-isa-extensions/topic/752392
  // [2] https://stackoverflow.com/questions/36191748/difference-between-\
  // load1-and-broadcast-intrinsics
  std::string Op = "load";

  // TODO check
  // List of parameters
  std::list<std::string> Args;
  Args.push_back(getOpName(V, true, true));

  // Adding SIMD inst to the list
  addSIMDInst(V, Op, PrefS, SuffS, Args, SIMDType::VPACK, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vbcast(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  PrefS += (V.IsPartial) ? "mask" : "";

  // TODO
  std::string Op = "broadcast";

  // TODO check
  // List of parameters
  std::list<std::string> Args;
  Args.push_back(getOpName(V, true, true));

  if (V.getDataType() == VectorIR::VDataType::DOUBLE) {
    V.DType = VectorIR::VDataType::SDOUBLE;
  }

  // Adding SIMD inst to the list
  addSIMDInst(V, Op, PrefS, SuffS, Args, SIMDType::VBCAST, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vgather(VectorIR::VOperand V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "gather";

  // TODO check
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
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";

  // TODO
  std::string Op = "set";

  // TODO check
  // List of parameters
  std::list<std::string> Args;
  for (int n = 0; n < V.VSize; n++) {
    Args.push_back((V.UOP[n] != NULL) ? V.UOP[n]->getValue() : "0.0");
  }
  Args.reverse();

  // Adding SIMD inst to the list
  addSIMDInst(V, Op, PrefS, SuffS, Args, SIMDType::VSET, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vstore(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  // PrefS += (V.R.Mask) ? "mask" : "";

  // TODO
  std::string Op = "store";

  // TODO check
  // List of parameters
  std::list<std::string> Args;
  Args.push_back(getOpName(V.R, true, true));
  if (V.R.EqualVal) {
    // FIXME
    PrefS += "mask";
    Args.push_back("_mm256_set_epi64x(0,0,0,0xffffffffffffffff)");
    // Args.push_back(getOpName(V.R, false, false));
    Args.push_back("_mm256_permute4x64_pd(" + getOpName(V.R, false, false) +
                   ", 0xff)");
  } else {
    Args.push_back(getOpName(V.R, false, false));
  }

  // Adding SIMD inst to the list
  addSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VSTORE, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vscatter(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  ////PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "scatter";

  // TODO check
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
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  // PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "add";

  // TODO check
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
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  // PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "mul";

  // TODO check
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
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  // PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "sub";

  // TODO check
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
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  // PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "div";

  // TODO check
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
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  // PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "mod";

  // TODO check
  // List of parameters
  std::list<std::string> Args;
  Args.push_back(V.OpA.getName());
  Args.push_back(V.OpB.getName());

  // Adding SIMD inst to the list
  addSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VMOD, &IL);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vreduce(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  SIMDGenerator::SIMDInstListType TIL;
  std::string RegType = getRegisterType(V.DT, V.VW);
  std::string RegAccm = getNextAccmRegister();
  SIMDGenerator::addRegToDeclare(RegType, RegAccm);

  std::string TmpReduxVar = (V.R.Name == V.OpB.Name) ? V.OpA.Name : V.OpB.Name;
  auto VCopy = V;
  VCopy.OpA.Name = TmpReduxVar;
  VCopy.R.Name = RegAccm;
  VCopy.OpB.Name = RegAccm;

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
  }
  IL.splice(IL.end(), TIL);
  std::string ReduxVar =
      (V.R.Name == V.OpA.Name) ? V.OpA.getRegName() : V.OpB.getRegName();
  /// New approach: horizontal adds work for AVX2, not sure for AVX512
  /// a = _mm256_hadd_pd(a,_mm256_permute4x64_pd(a,0x4e));
  /// a = _mm256_hadd_pd(a,a);
  /// b = _mm256_cvtsd_f64(a);
  /// FIXME: this only works for doubles...
  addSIMDInst(V.R, "hadd", "", "",
              {"_mm256_permute4x64_pd(" + RegAccm + ",0x4e)", RegAccm},
              SIMDType::VREDUC, &IL, RegAccm);
  addSIMDInst(V.R, "hadd", "", "", {RegAccm, RegAccm}, SIMDType::VREDUC, &IL,
              RegAccm);
  addSIMDInst(V.R, "cvtsd", "", "f64", {RegAccm}, SIMDType::VREDUC, &IL,
              ReduxVar);

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType AVX2Gen::vseq(VectorIR::VectorOP V) {
  SIMDGenerator::SIMDInstListType IL;
  // TODO generate preffix
  std::string PrefS = "";
  // TODO generate suffix
  std::string SuffS = "";
  // Mask
  // PrefS += (V.Mask) ? "mask" : "";

  // TODO
  std::string Op = "seq";

  // TODO check
  // List of parameters
  std::list<std::string> Args;
  Args.push_back(V.OpA.getName());
  Args.push_back(V.OpB.getName());

  // Adding SIMD inst to the list
  addSIMDInst(V.R, Op, PrefS, SuffS, Args, SIMDType::VSEQ, &IL);

  return IL;
}
