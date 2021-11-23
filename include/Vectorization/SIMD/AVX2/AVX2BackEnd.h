// MACVETH - AVX2BackEnd.h
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

#ifndef MACVETH_AVX2BACKEND_H
#define MACVETH_AVX2BACKEND_H

#include "include/Vectorization/SIMD/IntelIntrinsics.h"
#include "include/Vectorization/SIMD/SIMDBackEnd.h"
#include "include/Vectorization/VectorIR.h"

using namespace macveth;

namespace macveth {

/// SIMD generator for AVX2 architectures
class AVX2BackEnd : public SIMDBackEnd, public IntelIntrinsics {
public:
  /// Name of the architecture
  static inline std::string NArch = "x86";
  /// Name of the ISA
  static inline std::string NISA = "AVX2";
  /// Name of the headers needed
  static inline std::vector<std::string> Headers = {"<immintrin.h>",
                                                    "\"macveth_api.h\""};

  /// Get headers
  virtual std::vector<std::string> getHeadersNeeded() override {
    return AVX2BackEnd::Headers;
  }

  // Operand operations

  /// Pack memory operands
  virtual SIMDInstListType vload(VOperand V) override;

  /// Broadcast values
  virtual SIMDInstListType vbcast(VOperand V) override;

  /// Gather data from memory
  virtual SIMDInstListType vpack(VOperand V) override;

  /// Intel intrinsics gather approach
  SIMDInstListType vgather(VOperand V);

  /// Set values to registers
  virtual SIMDInstListType vset(VOperand V) override;
  virtual SIMDInstListType vregisterpacking(VOperand V) override;

  /// Store values in memory
  virtual SIMDInstListType vstore(VectorOP V) override;

  /// Store values in memory using an index
  virtual SIMDInstListType vscatter(VectorOP V) override;
  virtual SIMDInstListType singleElementScatterOp(VectorOP V) override;

  SIMDInstListType singleElementScatter(VectorOP VOP);

  /// Store values in memory using an index
  SIMDInstListType vscatterAVX512(VectorOP VOP);

  /// Scattering of four elements taking into account the contiguity of data
  bool vscatter4elements(VectorOP V, MVDataType::VWidth Width,
                         SIMDBackEnd::SIMDInstListType *IL);

  void store(VOperand V, MVStrVector Args, MVSourceLocation MVSL,
             SIMDBackEnd::SIMDInstListType *IL);

  void load(VOperand V, MVStrVector Args, MVSourceLocation MVSL,
            SIMDBackEnd::SIMDInstListType *IL);

  void loads(VOperand V, MVStrVector Args, MVSourceLocation MVSL,
             SIMDBackEnd::SIMDInstListType *IL);

  void moves(VOperand V, MVStrVector Args, MVSourceLocation MVSL,
             SIMDBackEnd::SIMDInstListType *IL);

  void blend(VOperand V, MVStrVector Args, MVSourceLocation MVSL,
             SIMDBackEnd::SIMDInstListType *IL);

  void insert(VOperand V, MVStrVector Args, MVSourceLocation MVSL,
              SIMDBackEnd::SIMDInstListType *IL);

  // Binary operations

  /// Multiplication operation
  virtual SIMDInstListType vmul(VectorOP V) override;

  /// Substraction operation
  virtual SIMDInstListType vsub(VectorOP V) override;

  /// Add operation
  virtual SIMDInstListType vadd(VectorOP V) override;

  /// Division operation
  virtual SIMDInstListType vdiv(VectorOP V) override;

  /// Modulo operation
  virtual SIMDInstListType vmod(VectorOP V) override;

  /// Generate custom functions
  virtual SIMDInstListType vfunc(VectorOP V) override;

  /// Reduction operations
  virtual SIMDInstListType vreduce(VectorOP V) override;
  virtual SIMDInstListType computeAccmReduction(VectorOP V, std::string RegAccm,
                                                std::string TmpReg,
                                                std::string PrevVal,
                                                bool NeedsCast);

  /// Scalar operation
  virtual SIMDInstListType vseq(VectorOP V) override;

  /// Perform some peephole optimizations after generating SIMD instructions
  virtual SIMDInstListType peepholeOptimizations(SIMDInstListType I) override;

  /// Set values for registers which need to be initalized
  virtual MVStrVector renderSIMDRegister(SIMDInstListType S) override;

  /// Trick for inserting two 128-bit registers onto a 256-bit register
  void insertLowAndHighBits(VOperand V, std::string Hi, std::string Lo,
                            MVSourceLocation MVSL,
                            SIMDBackEnd::SIMDInstListType *IL);

  /// One of the optimizations included in AVX2
  SIMDBackEnd::SIMDInstListType fuseAddSubMult(SIMDBackEnd::SIMDInstListType I);

  /// Horizontal single reduction approach
  SIMDBackEnd::SIMDInstListType horizontalSingleVectorReduction(
      SIMDBackEnd::SIMDInstListType TIL,
      MVSourceLocation::Position Pos = MVSourceLocation::Position::POSORDER);

  /// Horizontal reduction approach
  SIMDBackEnd::SIMDInstListType horizontalReductionFusion(
      SIMDBackEnd::SIMDInstListType TIL,
      MVSourceLocation::Position Pos = MVSourceLocation::Position::POSORDER);

  /// General reduction approach, based on vertical operations
  SIMDBackEnd::SIMDInstListType generalReductionFusion(
      SIMDBackEnd::SIMDInstListType TIL,
      MVSourceLocation::Position Pos = MVSourceLocation::Position::POSORDER);

  SIMDBackEnd::SIMDInstListType
  reduceOneRegister(VOperand V, std::string OpName, MVSourceLocation MVSL);

  SIMDBackEnd::SIMDInstListType
  reduceMultipleValuesInRegisterSymmetric(VOperand V, std::string OpName,
                                          MVSourceLocation MVSL);

  SIMDBackEnd::SIMDInstListType
  reduceMultipleValuesInRegister(VOperand V, std::string OpName,
                                 MVSourceLocation MVSL);

  SIMDBackEnd::SIMDInstListType
  fuseReductionsList(SIMDBackEnd::SIMDInstListType TIL);

  /// Fusing reductions: peephole optimization
  SIMDBackEnd::SIMDInstListType fuseReductions(SIMDBackEnd::SIMDInstListType I);
  SIMDBackEnd::SIMDInstListType
  groupReductions(SIMDBackEnd::SIMDInstListType I);

  void
  mergeReductions(std::map<int, SIMDBackEnd::SIMDInstListType> &LRedux,
                  std::map<SIMDBackEnd::SIMDInst, SIMDBackEnd::SIMDInstListType>
                      &ReplaceFusedRedux,
                  SIMDBackEnd::SIMDInstListType &SkipList);

  /// Peephole optimization for fusing reductions
  SIMDBackEnd::SIMDInst genMultAccOp(SIMDBackEnd::SIMDInst Mul,
                                     SIMDBackEnd::SIMDInst Acc);

  /// Generate mask given a number of elements and width
  std::string getMask(unsigned int MaskVect, int NElems,
                      MVDataType::VWidth Width);

  std::string getMask(unsigned int MaskVect, int NElems,
                      MVDataType::VWidth Width, MVDataType::VDataType Type);

  /// Get name of AVX architecture
  virtual std::string getNArch() override { return AVX2BackEnd::NArch; }

  /// Get name of AVX architecture
  virtual std::string getNISA() override { return AVX2BackEnd::NISA; }

  /// Get the translation between VectorIR data widths and AVX2's
  virtual std::string getMapWidth(MVDataType::VWidth V) override {
    MVAssert((V != MVDataType::VWidth::W512),
             "Width too wide for AVX2 (512 bits not supported)!!");
    if (V <= 128) {
      return MapWidth[MVDataType::VWidth::W128];
    } else {
      return MapWidth[MVDataType::VWidth::W256];
    }
  }

  /// Get the translation between VectorIR data types and AVX2's
  virtual std::string getMapType(MVDataType::VDataType D) override {
    return MapType[D];
  }

  /// Get max width
  virtual int getMaxWidth() override { return AVX2BackEnd::MaxWidth; }

  /// Get the type of register according to the VDataType and VWidth. This
  /// method is abstract because each architecture may have different types.
  virtual std::string getRegisterType(MVDataType::VDataType DT,
                                      MVDataType::VWidth W) override;

  /// Get initial values of a VectorOP
  virtual std::vector<std::string> getInitValues(VectorOP V);

  /// Destructor
  virtual ~AVX2BackEnd(){};

  /// Singleton pattern
  static SIMDBackEnd *getSingleton() {
    if (AVX2BackEnd::_instance == nullptr) {
      AVX2BackEnd::_instance = new AVX2BackEnd();
    }
    return AVX2BackEnd::_instance;
  };

private:
  std::map<std::string, std::string> MapReduxVarToAccm;
  std::map<std::string, VOperand> MapReduxVOp;

  /// Constructor
  AVX2BackEnd() : SIMDBackEnd() {
    SIMDBackEnd::populateTable(MVCPUInfo::MVISA::AVX2);
  }

  /// Singletton pattern
  static inline SIMDBackEnd *_instance = 0;

  /// Add SIMD instruction
  virtual SIMDBackEnd::SIMDInst
  genSIMDInst(std::string Result, std::string Op, std::string PrefS,
              std::string SuffS, MVDataType::VWidth Width,
              MVDataType::VDataType Type, MVStrVector Args,
              SIMDBackEnd::SIMDType SType, MVSourceLocation SL,
              SIMDBackEnd::SIMDInstListType *IL, std::string NameOp = "",
              std::string MVFunc = "", MVStrVector MVArgs = {},
              MVOp MVOP = MVOp()) override;

  /// Auxiliary function for generating the SIMDInst to the list
  virtual SIMDBackEnd::SIMDInst
  genSIMDInst(VOperand V, std::string Op, std::string PrefS, std::string SuffS,
              MVStrVector OPS, SIMDBackEnd::SIMDType SType, MVSourceLocation SL,
              SIMDBackEnd::SIMDInstListType *IL = nullptr,
              std::string NameOp = "", std::string MVFunc = "",
              MVStrVector MVArgs = {}, MVOp MVOP = MVOp()) override;

  /// Shuffle method for reductions
  std::string shuffleArguments(std::string A1, std::string A2,
                               MVDataType::VWidth Width,
                               SIMDBackEnd::SIMDInst I, int Pos);
  /// Shuffle method for reductions
  std::string permuteArguments(std::string A1, std::string A2,
                               SIMDBackEnd::SIMDInst I, int Pos);

  /// Extract high or low part
  std::string extractArgument(std::string A, SIMDBackEnd::SIMDInst I, int Hi);

  /// Auxiliary method for declaring auxiliary arrays
  std::string declareAuxArray(MVDataType::VDataType DT);

  /// Max width
  static constexpr int MaxWidth = 256;
};

} // namespace macveth
#endif /* !MACVETH_AVX2BACKEND_H */
