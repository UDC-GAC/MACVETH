/**
 * File              : AVX2Gen.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 22 Dec 2019 20:50:29 MST
 * Last Modified Date: Mar 14 Xan 2020 14:31:53 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_AVX2GEN_H
#define MACVETH_AVX2GEN_H

#include "include/Vectorization/SIMD/SIMDGenerator.h"
#include "include/Vectorization/VectorIR.h"

using namespace macveth;

namespace macveth {

/// SIMD generator for AVX2 architectures
class AVX2Gen : public SIMDGenerator {
public:
  /// Name of the architecture
  static inline std::string NArch = "x86";
  /// Name of the ISA
  static inline std::string NISA = "AVX2";
  /// Name of the headers needed
  static inline std::list<std::string> Headers = {"immintrin.h"};

  /// Get headers
  virtual std::list<std::string> getHeadersNeeded() override {
    return AVX2Gen::Headers;
  }

  // Operand operations

  /// Pack memory operands
  virtual SIMDInstListType vpack(VectorIR::VOperand V) override;

  /// Broadcast values
  virtual SIMDInstListType vbcast(VectorIR::VOperand V) override;

  /// Gather data from memory
  virtual SIMDInstListType vgather(VectorIR::VOperand V) override;

  /// Set values to registers
  virtual SIMDInstListType vset(VectorIR::VOperand V) override;

  /// Store values in memory
  virtual SIMDInstListType vstore(VectorIR::VectorOP V) override;

  /// Store values in memory using an index
  virtual SIMDInstListType vscatter(VectorIR::VectorOP V) override;

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

  /// Sequential operation
  virtual SIMDInstListType vseq(VectorIR::VectorOP V) override;

  /// Perform some peephole optimizations after generating SIMD instructions
  virtual SIMDInstListType peepholeOptimizations(SIMDInstListType I) override;

  /// Set values for registers which need to be initalized
  virtual std::list<std::string> renderSIMDRegister(SIMDInstListType S);

  /// One of the optimizations included in AVX2
  SIMDGenerator::SIMDInstListType
  fuseAddSubMult(SIMDGenerator::SIMDInstListType I);

  /// Horizontal reduction approach
  SIMDGenerator::SIMDInstListType
  horizontalSingleReduction(SIMDGenerator::SIMDInstListType TIL);

  /// Horizontal reduction approach
  SIMDGenerator::SIMDInstListType
  horizontalReductionFusion(SIMDGenerator::SIMDInstListType TIL);

  /// General reduction approach, based on vertical operations
  SIMDGenerator::SIMDInstListType
  generalReductionFusion(SIMDGenerator::SIMDInstListType TIL);

  SIMDGenerator::SIMDInstListType
  fuseReductionsList(SIMDGenerator::SIMDInstListType TIL);

  bool hasRawDependencies(SIMDGenerator::SIMDInstListType L,
                          SIMDGenerator::SIMDInst I);

  bool reductionIsContiguous(SIMDGenerator::SIMDInstListType L,
                             SIMDGenerator::SIMDInst I);

  /// Fusing reductions: peephole optimization
  SIMDGenerator::SIMDInstListType
  fuseReductions(SIMDGenerator::SIMDInstListType I);

  /// Peephole optimization for fusing reductions
  SIMDGenerator::SIMDInst genMultAccOp(SIMDGenerator::SIMDInst Mul,
                                       SIMDGenerator::SIMDInst Acc);

  /// Generate mask given a number of elements and width
  std::string getMask(unsigned int MaskVect, int NElems,
                      VectorIR::VWidth Width);

  std::string getMask(unsigned int MaskVect, int NElems, VectorIR::VWidth Width,
                      VectorIR::VDataType Type);

  /// Get name of AVX architecture
  virtual std::string getNArch() override { return AVX2Gen::NArch; }

  /// Get name of AVX architecture
  virtual std::string getNISA() override { return AVX2Gen::NISA; }

  /// Get the translation between VectorIR data widths and AVX2's
  virtual std::string getMapWidth(VectorIR::VWidth V) override {
    MVAssert((V != VectorIR::VWidth::W512),
             "Width too wide for AVX2 (512 bits not supported)!!");
    if (V <= 128) {
      return MapWidth[VectorIR::VWidth::W128];
    } else {
      return MapWidth[VectorIR::VWidth::W256];
    }
  }

  /// Get the translation between VectorIR data types and AVX2's
  virtual std::string getMapType(VectorIR::VDataType D) override {
    return MapType[D];
  }

  /// Get max width
  virtual int getMaxWidth() override { return AVX2Gen::MaxWidth; }

  /// Get the type of register according to the VDataType and VWidth. This
  /// method is abstract because each architecture may have different types.
  virtual std::string getRegisterType(VectorIR::VDataType DT,
                                      VectorIR::VWidth W) override;

  /// Get initial values of a VectorOP
  virtual std::vector<std::string> getInitValues(VectorIR::VectorOP V);

  /// Destructor
  virtual ~AVX2Gen(){};

  /// Singleton pattern
  static SIMDGenerator *getSingleton() {
    if (AVX2Gen::_instance == 0) {
      AVX2Gen::_instance = new AVX2Gen();
    }
    return AVX2Gen::_instance;
  };

private:
  /// Constructor
  AVX2Gen() : SIMDGenerator() { SIMDGenerator::populateTable(MVISA::AVX2); }

  /// Singletton pattern
  static inline SIMDGenerator *_instance = 0;

  /// Add SIMD instruction
  SIMDGenerator::SIMDInst genSIMDInst(
      std::string Result, std::string Op, std::string PrefS, std::string SuffS,
      VectorIR::VWidth Width, VectorIR::VDataType Type,
      std::list<std::string> Args, SIMDGenerator::SIMDType SType,
      SIMDGenerator::SIMDInstListType *IL, int Order = -1,
      MVSourceLocation::Position P = MVSourceLocation::Position::INORDER,
      std::string NameOp = "", std::string MVFunc = "",
      std::list<std::string> MVArgs = {}, MVOp MVOP = MVOp());

  /// Auxiliary function for generating the SIMDInst to the list
  SIMDGenerator::SIMDInst genSIMDInst(
      VectorIR::VOperand V, std::string Op, std::string PrefS,
      std::string SuffS, std::list<std::string> OPS,
      SIMDGenerator::SIMDType SType,
      SIMDGenerator::SIMDInstListType *IL = nullptr, int Order = -1,
      MVSourceLocation::Position P = MVSourceLocation::Position::INORDER,
      std::string NameOp = "", std::string MVFunc = "",
      std::list<std::string> MVArgs = {}, MVOp MVOP = MVOp()) override;

  /// Shuffle method for reductions
  std::string shuffleArguments(std::string A1, std::string A2,
                               VectorIR::VWidth Width,
                               SIMDGenerator::SIMDInst I, int Pos);
  /// Shuffle method for reductions
  std::string permuteArguments(std::string A1, std::string A2,
                               SIMDGenerator::SIMDInst I, int Pos);

  /// Extract high or low part
  std::string extractArgument(std::string A, SIMDGenerator::SIMDInst I, int Hi);

  /// Auxiliary method for declaring auxiliary arrays
  std::string declareAuxArray(VectorIR::VDataType DT);

  /// Specific instruction for loading data according to the operand
  bool genLoadInst(VectorIR::VOperand V, SIMDGenerator::SIMDInstListType *L);

  /// Max width
  static inline const int MaxWidth = 256;

  /// Mapping the width types with its name in AVX2
  static inline std::map<VectorIR::VWidth, std::string> MapWidth = {
      {VectorIR::VWidth::W128, ""}, {VectorIR::VWidth::W256, "256"}};

  /// Map of VectorIR types and its translation in the AVX2 architecture
  static inline std::map<VectorIR::VDataType, std::string> MapType = {
      {VectorIR::VDataType::FLOAT, "ps"},
      {VectorIR::VDataType::SFLOAT, "ss"},
      {VectorIR::VDataType::DOUBLE, "pd"},
      {VectorIR::VDataType::SDOUBLE, "sd"},
      {VectorIR::VDataType::INT8, "epi8"},
      {VectorIR::VDataType::INT16, "epi16"},
      {VectorIR::VDataType::INT32, "epi32"},
      {VectorIR::VDataType::INT64, "epi64"},
      {VectorIR::VDataType::UINT8, "epu8"},
      {VectorIR::VDataType::UINT16, "epu16"},
      {VectorIR::VDataType::UINT32, "epu32"},
      {VectorIR::VDataType::UINT64, "epu64"},
      {VectorIR::VDataType::UNDEF128, "si128"},
      {VectorIR::VDataType::UNDEF256, "si256"},
      {VectorIR::VDataType::IN_INT128, "m128i"},
      {VectorIR::VDataType::IN_INT256, "m256i"},
      {VectorIR::VDataType::IN_FLOAT128, "m128"},
      {VectorIR::VDataType::IN_FLOAT256, "m256"},
      {VectorIR::VDataType::IN_DOUBLE128, "m128d"},
      {VectorIR::VDataType::IN_DOUBLE256, "m128d"}};
};

} // namespace macveth
#endif /* !MACVETH_AVX2GEN_H */
