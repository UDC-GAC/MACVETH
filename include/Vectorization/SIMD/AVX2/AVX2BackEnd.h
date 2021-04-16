/*
 * File	: include/Vectorization/SIMD/AVX2BackEnd.h
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:40 +02:00
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
  static inline std::list<std::string> Headers = {"immintrin.h"};

  /// Get headers
  virtual std::list<std::string> getHeadersNeeded() override {
    return AVX2BackEnd::Headers;
  }

  // Operand operations

  /// Pack memory operands
  virtual SIMDInstListType vload(VectorIR::VOperand V) override;

  /// Broadcast values
  virtual SIMDInstListType vbcast(VectorIR::VOperand V) override;

  /// Gather data from memory
  virtual SIMDInstListType vpack(VectorIR::VOperand V) override;

  /// Packing of four elements taking into account the contiguity of data
  bool vpack4elements(VectorIR::VOperand V, MVDataType::VWidth Width,
                      SIMDBackEnd::SIMDInstListType *IL);

  /// Intel intrisnics gather approach
  SIMDInstListType vgather(VectorIR::VOperand V);

  /// Set values to registers
  virtual SIMDInstListType vset(VectorIR::VOperand V) override;

  /// Store values in memory
  virtual SIMDInstListType vstore(VectorIR::VectorOP V) override;

  /// Store values in memory using an index
  virtual SIMDInstListType vscatter(VectorIR::VectorOP V) override;

  /// Store values in memory using an index
  SIMDInstListType vscatterAVX512(VectorIR::VectorOP VOP);

  /// Scattering of four elements taking into account the contiguity of data
  bool vscatter4elements(VectorIR::VectorOP V, MVDataType::VWidth Width,
                         SIMDBackEnd::SIMDInstListType *IL);

  void store(VectorIR::VOperand V, std::list<std::string> Args,
             MVSourceLocation MVSL, SIMDBackEnd::SIMDInstListType *IL);

  void load(VectorIR::VOperand V, std::list<std::string> Args,
            MVSourceLocation MVSL, SIMDBackEnd::SIMDInstListType *IL);

  void loads(VectorIR::VOperand V, std::list<std::string> Args,
             MVSourceLocation MVSL, SIMDBackEnd::SIMDInstListType *IL);

  void moves(VectorIR::VOperand V, std::list<std::string> Args,
             MVSourceLocation MVSL, SIMDBackEnd::SIMDInstListType *IL);

  void blend(VectorIR::VOperand V, std::list<std::string> Args,
             MVSourceLocation MVSL, SIMDBackEnd::SIMDInstListType *IL);

  void insert(VectorIR::VOperand V, std::list<std::string> Args,
              MVSourceLocation MVSL, SIMDBackEnd::SIMDInstListType *IL);

  // Binary operations

  /// Multiplication operation
  virtual SIMDInstListType vmul(VectorIR::VectorOP V) override;

  /// Substraction operation
  virtual SIMDInstListType vsub(VectorIR::VectorOP V) override;

  /// Add operation
  virtual SIMDInstListType vadd(VectorIR::VectorOP V) override;

  /// Division operation
  virtual SIMDInstListType vdiv(VectorIR::VectorOP V) override;

  /// Modulo operation
  virtual SIMDInstListType vmod(VectorIR::VectorOP V) override;

  /// Generate custom functions
  virtual SIMDInstListType vfunc(VectorIR::VectorOP V) override;

  /// Reduction operations
  virtual SIMDInstListType vreduce(VectorIR::VectorOP V) override;

  /// Scalar operation
  virtual SIMDInstListType vseq(VectorIR::VectorOP V) override;

  /// Perform some peephole optimizations after generating SIMD instructions
  virtual SIMDInstListType peepholeOptimizations(SIMDInstListType I) override;

  /// Set values for registers which need to be initalized
  virtual std::list<std::string> renderSIMDRegister(SIMDInstListType S);

  /// Trick for inserting two 128-bit registers onto a 256-bit register
  void insertLowAndHighBits(VectorIR::VOperand V, std::string Hi,
                            std::string Lo, MVSourceLocation MVSL,
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
  fuseReductionsList(SIMDBackEnd::SIMDInstListType TIL);

  /// Check whether an instruction has RAW dependencies within a list of
  /// instructions
  bool hasRawDependencies(SIMDBackEnd::SIMDInstListType L,
                          SIMDBackEnd::SIMDInst I);

  /// A reduction is contiguous if there are not any other
  bool reductionIsContiguous(SIMDBackEnd::SIMDInstListType L,
                             SIMDBackEnd::SIMDInst I);

  /// Fusing reductions: peephole optimization
  SIMDBackEnd::SIMDInstListType fuseReductions(SIMDBackEnd::SIMDInstListType I);

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
  virtual std::vector<std::string> getInitValues(VectorIR::VectorOP V);

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
              MVDataType::VDataType Type, std::list<std::string> Args,
              SIMDBackEnd::SIMDType SType, MVSourceLocation SL,
              SIMDBackEnd::SIMDInstListType *IL, std::string NameOp = "",
              std::string MVFunc = "", std::list<std::string> MVArgs = {},
              MVOp MVOP = MVOp()) override;

  /// Auxiliary function for generating the SIMDInst to the list
  virtual SIMDBackEnd::SIMDInst
  genSIMDInst(VectorIR::VOperand V, std::string Op, std::string PrefS,
              std::string SuffS, std::list<std::string> OPS,
              SIMDBackEnd::SIMDType SType, MVSourceLocation SL,
              SIMDBackEnd::SIMDInstListType *IL = nullptr,
              std::string NameOp = "", std::string MVFunc = "",
              std::list<std::string> MVArgs = {}, MVOp MVOP = MVOp()) override;

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
  static inline const int MaxWidth = 256;
};

} // namespace macveth
#endif /* !MACVETH_AVX2BACKEND_H */
