/*
 * File          : include/Vectorization/SIMD/SIMDBackEnd.h
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

#ifndef MACVETH_SIMDGENERATOR_H
#define MACVETH_SIMDGENERATOR_H

#include "include/CDAG.h"
#include "include/CostModel/CostTable.h"
//#include "include/CostModel/SIMDCostInfo.h"
#include "include/MVExpr/MVDataType.h"
#include "include/MVSourceLocation.h"
#include "include/Vectorization/VectorIR.h"

using namespace macveth;

namespace macveth {

/// Abstract class implemented by each architecture (AVX, AVX2, AVX512, etc.) to
/// generate specific intrinsics and calculate the associated cost for the
/// operations provided by the VectorAPI
class SIMDBackEnd {
public:
  /// Types of SIMD instructions
  enum SIMDType {
    /// Custom function
    VFUNC,
    /// Load, pack memory addresses
    VPACK,
    /// Broadcast values
    VBCAST,
    /// Gather values from memory
    VGATHER,
    /// Set values
    VSET,
    /// Store values in contiguous memory
    VSTORE,
    /// Store values in a scattered fashion
    VSCATTER,
    /// Multiplication
    VMUL,
    /// Addition
    VADD,
    /// Substraction
    VSUB,
    /// Division
    VDIV,
    /// Modulo
    VMOD,
    /// Permutation or shuffle
    VPERM,
    /// Reduce operation
    VREDUC,
    /// Sequential operation
    VSEQ,
    /// If the SIMD instruction is the result of an optimization (e.g. fuse
    /// multiply-accumulation) we will call it VOPT
    VOPT,
    /// Initializing some value, for instance, in reductions
    INIT
  };

  /// Wrap for representing the SIMDInst not just as single strings to print,
  /// but as a set of fields
  struct SIMDInst {
    /// VOperand result
    VectorIR::VOperand VOPResult;
    /// Unique identifier generator
    static inline unsigned int UID = 0;
    /// Unique identifier
    unsigned int SIMD_UID = 0;
    /// Source location according to our TACs
    MVSourceLocation MVSL;
    /// Result register name
    std::string Result;
    /// Type of operation
    MVOp MVOP;
    /// Signature of the function
    std::string FuncName;
    /// Signature of the function (macro approach)
    std::string MVFuncName;
    /// Type of the function
    SIMDType SType;
    /// List of *sorted* arguments of the function
    std::list<std::string> Args;
    /// List of *sorted* arguments of the function (macro approach)
    std::list<std::string> MVArgs;
    /// Cost of the instruction
    // InstCostInfo Cost;
    /// Data type
    MVDataType::VDataType DT;
    /// Width
    MVDataType::VWidth W;

    /// Render instruction as a string
    std::string render();

    MVSourceLocation getMVSourceLocation() { return MVSL; }

    /// Check if the vectorial type of the operation is sequential
    bool isSequential() { return this->SType == SIMDBackEnd::SIMDType::VSEQ; }

    /// Check if the vectorial type of the operation is a reduction
    bool isReduction() { return this->SType == SIMDType::VREDUC; }

    /// Comparison operator
    bool operator==(const SIMDInst &S) const {
      return (S.SIMD_UID == this->SIMD_UID);
    }

    /// This is util if we want to use SIMDInst in std::map
    bool operator<(const SIMDInst &S) const {
      return S.SIMD_UID < this->SIMD_UID;
    }

    /// Empty constructor
    SIMDInst(){};

    /// Constructor for not SIMD code
    SIMDInst(std::string LHS, std::string RHS, MVSourceLocation SL)
        : Result(LHS), FuncName(RHS), MVSL(SL) {
      this->SIMD_UID = SIMDInst::UID++;
    }
    /// Constructor
    SIMDInst(std::string R, std::string FN, std::list<std::string> Args,
             std::string MVFunc, std::list<std::string> MVArgs,
             MVSourceLocation SL)
        : Result(R), FuncName(FN), Args(Args), MVFuncName(MVFunc),
          MVArgs(MVArgs), MVSL(SL) {
      this->SIMD_UID = SIMDInst::UID++;
    }
  };

  /// Alias for list of SIMDInst structures
  using SIMDInstListType = std::list<SIMDInst>;

  /// Empty constructor

  /// Generate non SIMD instructions, as we may have sequential operations or
  /// other type of not vectorized instructions, given a VectorOP
  SIMDBackEnd::SIMDInst addNonSIMDInst(VectorIR::VectorOP OP,
                                       SIMDBackEnd::SIMDType SType,
                                       MVSourceLocation MVSL,
                                       SIMDBackEnd::SIMDInstListType *IL);
  /// Generate non SIMD instructions, as we may have sequential operations or
  /// other type of not vectorized instructions specifying explicitly the LHS
  /// and the RHS
  SIMDBackEnd::SIMDInst addNonSIMDInst(std::string Lhs, std::string Rhs,
                                       SIMDBackEnd::SIMDType SType,
                                       MVSourceLocation MVSL,
                                       SIMDBackEnd::SIMDInstListType *IL);

  // VectorAPI: instructions to implement by the specific backends

  // Operands instructions

  /// Generate pack instructions, e.g. load/loadu
  virtual SIMDInstListType vpack(VectorIR::VOperand V) = 0;
  /// Generate broadcast instructions. Broadcasts are used to replicate known
  /// values (i.e. values in registers, not memory addresses)
  virtual SIMDInstListType vbcast(VectorIR::VOperand V) = 0;
  /// Generate gather instructions. Gathers are meant to retrieve data from the
  /// memory using indices
  virtual SIMDInstListType vgather(VectorIR::VOperand V) = 0;
  /// Generate set instrucctions. Set is meant for literal values
  virtual SIMDInstListType vset(VectorIR::VOperand V) = 0;

  /// Generate store instructions to memory
  virtual SIMDInstListType vstore(VectorIR::VectorOP V) = 0;
  /// Generate store instructions to memory using an index
  virtual SIMDInstListType vscatter(VectorIR::VectorOP V) = 0;

  // Binary operations

  /// Generate multiplication operations
  virtual SIMDInstListType vmul(VectorIR::VectorOP V) = 0;
  /// Generate subtraction operations
  virtual SIMDInstListType vsub(VectorIR::VectorOP V) = 0;
  /// Generate addition operations
  virtual SIMDInstListType vadd(VectorIR::VectorOP V) = 0;
  /// Generate division operations
  virtual SIMDInstListType vdiv(VectorIR::VectorOP V) = 0;
  /// Generate modulo operations
  virtual SIMDInstListType vmod(VectorIR::VectorOP V) = 0;

  /// Generate custom functions
  virtual SIMDInstListType vfunc(VectorIR::VectorOP V) = 0;

  // Reduction operations

  /// Operations are reduce given a certain condicitons.
  virtual SIMDInstListType vreduce(VectorIR::VectorOP V) = 0;

  // Sequential operations

  /// If operation is not binary, pararel or reduce, then we just emit a
  /// sequential operation. Backend will be in charge of performing any
  /// optimizations if needed
  virtual SIMDInstListType vseq(VectorIR::VectorOP V) = 0;

  /// Get the type of register according to the VDataType and VWidth. This
  /// method is abstract because each architecture may have different types.
  virtual std::string getRegisterType(MVDataType::VDataType DT,
                                      MVDataType::VWidth W) = 0;

  /// Get max width
  virtual int getMaxWidth() = 0;

  /// Get name of the concrete architecture
  virtual std::string getNISA() = 0;

  /// Get name of the concrete architecture
  virtual std::string getNArch() = 0;

  /// Map of VectorIR widths to the concrete architecture
  virtual std::string getMapWidth(MVDataType::VWidth V) = 0;

  /// Map of VectorIR types to the concrete architecture
  virtual std::string getMapType(MVDataType::VDataType D) = 0;

  /// Generate the declaration of the necessary registers for the operations
  virtual std::list<std::string> renderSIMDRegister(SIMDInstListType S) = 0;

  /// Perform some peephole optimizations after generating SIMD instructions
  virtual SIMDInstListType peepholeOptimizations(SIMDInstListType I) = 0;

  /// Get initial values of a VectorOP. Must be implemented
  virtual std::vector<std::string> getInitValues(VectorIR::VectorOP V) = 0;

  /// Add SIMD instruction
  virtual SIMDBackEnd::SIMDInst
  genSIMDInst(std::string Result, std::string Op, std::string PrefS,
              std::string SuffS, MVDataType::VWidth Width,
              MVDataType::VDataType Type, std::list<std::string> Args,
              SIMDBackEnd::SIMDType SType, MVSourceLocation SL,
              SIMDBackEnd::SIMDInstListType *IL, std::string NameOp = "",
              std::string MVFunc = "", std::list<std::string> MVArgs = {},
              MVOp MVOP = MVOp()) = 0;

  virtual SIMDBackEnd::SIMDInst
  genSIMDInst(VectorIR::VOperand V, std::string Op, std::string PrefS,
              std::string SuffS, std::list<std::string> OPS,
              SIMDBackEnd::SIMDType SType, MVSourceLocation SL,
              SIMDBackEnd::SIMDInstListType *IL = nullptr,
              std::string NameOp = "", std::string MVFunc = "",
              std::list<std::string> MVArgs = {}, MVOp MVOP = MVOp()) = 0;

  /// Get headers needed (include files)
  virtual std::list<std::string> getHeadersNeeded() = 0;

  /// Get maximum vector operands size
  virtual int getMaxVectorSize(std::string Type) {
    return getMaxWidth() / (MVDataType::SizeOf[Type] * 8);
  };

  /// Clean the list of registers declared
  static void clearMappings();

  /// Populate the table only when creating the object, to avoid overloading the
  /// memory from the start
  static void populateTable(MVCPUInfo::MVISA ISA);

  /// Auxiliary function for replacing patterns in a string
  static std::string replacePatterns(std::string Pattern, std::string W,
                                     std::string D, std::string P,
                                     std::string S);

  /// Render SIMD instructions as a list of strings, where each element
  /// represents a new line
  std::list<std::string> renderSIMDasString(SIMDInstListType S);

  /// Auxiliary function to retrieve properly the operands
  std::string getOpName(VectorIR::VOperand V, bool Ptr, bool RegVal);

  /// Insert the SIMDInst in the list given an VOperand
  bool getSIMDVOperand(VectorIR::VOperand V, SIMDInstListType *IL);

  std::string genGenericFunc(std::string F, std::vector<std::string> L);

  /// Entry point: this method basically redirects to any of the operations
  /// supported
  SIMDInstListType getSIMDfromVectorOP(VectorIR::VectorOP V);

  /// Basically calls its other overloaded function iterating over the input
  /// list of VectorOP
  SIMDInstListType getSIMDfromVectorOP(std::list<VectorIR::VectorOP> VList);

  /// Get list of initializations
  SIMDInstListType getInitReg() { return InitReg; }

  /// Setting CDAG
  void setCDAG(CDAG *C) { this->C = C; }

  /// Getting CDAG
  CDAG *getCDAG() { return this->C; }

  SIMDBackEnd(){};

  /// Destructor
  virtual ~SIMDBackEnd(){};

  using RegistersMapT =
      std::map<std::string,
               std::vector<std::tuple<std::string, std::vector<std::string>>>>;

  /// Get list of registers declared
  RegistersMapT getRegDeclared() { return SIMDBackEnd::RegDeclared; }

private:
  /// Auxiliary function to dispatch the VectorOP operation
  void mapOperation(VectorIR::VectorOP V, SIMDInstListType *TI);

  /// Auxiliary function to dispatch the VectorOP operation
  void reduceOperation(VectorIR::VectorOP V, SIMDInstListType *TI);

  /// CDAG information
  CDAG *C;

protected:
  /// Add register to declare
  static void addRegToDeclare(std::string Type, std::string Name,
                              int InitVal = 0);

  /// Add register to declare
  static void addRegToDeclareInitVal(std::string Type, std::string Name,
                                     std::vector<std::string> InitVal);

  /// List of registers declared
  inline static RegistersMapT RegDeclared;

  /// List of SIMD instructions which represent the initialization of some
  /// registers
  inline static SIMDInstListType InitReg;

  /// Prefix of the auxiliary regsiters
  inline static const std::string AUX_PREFIX = "__mv_aux";
  /// Prefix of the auxiliary arrays
  inline static const std::string ARR_PREFIX = "__mv_arr";
  /// Prefix of the
  inline static const std::string VEC_PREFIX = "__mv_accm";

  /// AccmReg numbering for auxiliary operations such as reduce
  inline static int AccmReg = 0;

  /// Map of the operands mapped to the accumulator
  inline static std::map<std::string, int> AccmToReg;
  /// Map of the operands mapped to the accumulator which are dirty
  inline static std::map<std::string, int> AccmDirty;

  /// Get the next available accumulator register
  static std::string getNextAccmRegister(std::string V) {
    if (AccmToReg.count(V) == 0) {
      AccmDirty[V] = 0;
      AccmToReg[V] = SIMDBackEnd::AccmReg++;
    }
    return VEC_PREFIX + std::to_string(AccmToReg.at(V));
  }

  static bool isAccmClean(std::string V) { return !AccmDirty[V]; }
  static void markDirtyAccm(std::string V) { AccmDirty[V] = 1; }

  /// Get the current accumulator register
  static std::string getAccmReg(std::string V) {
    if (AccmToReg.count(V) == 0) {
      return "";
    }
    return VEC_PREFIX + std::to_string(AccmToReg.at(V));
  }

  /// Auxiliar register unique ID
  inline static int AuxRegId = 0;

  /// Map of auxiliar registers
  inline static std::map<std::string, int> AuxReg;

  /// Get the name of the auxiliar register for a operand and increment
  static std::string getNextAuxRegister(std::string V) {
    if (AuxReg.count(V) == 0) {
      AuxReg[V] = SIMDBackEnd::AuxRegId++;
    }
    return AUX_PREFIX + std::to_string(AuxReg.at(V));
  }

  /// Get the name of the auxiliar register for a operand
  static std::string getAuxReg(std::string V) {
    if (AuxReg.count(V) == 0) {
      return "";
    }
    return AUX_PREFIX + std::to_string(AuxReg.at(V));
  }

  /// Check if value has been already been mapped for an accumulator
  static bool hasAlreadyBeenMapped(std::string V) {
    return !(AccmToReg.count(V) == 0);
  }

  /// Auxiliary array numbering for auxiliary operations such as reduce
  inline static int AuxArrayReg = 0;
  static std::string getNextArrRegister(std::string Type, int Size) {
    auto Name = ARR_PREFIX + std::to_string(AuxArrayReg++);
    addRegToDeclare(Type, Name + "[" + std::to_string(Size) + "]");
    return Name;
  }
};

} // namespace macveth
#endif /* !MACVETH_SIMDGENERATOR_H */
