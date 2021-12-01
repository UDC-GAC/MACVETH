// MACVETH - SIMDBackEnd.h
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

#ifndef MACVETH_SIMDGENERATOR_H
#define MACVETH_SIMDGENERATOR_H

#include "include/CDAG.h"
#include "include/CostModel/CostTable.h"
#include "include/CostModel/RandomPackingTable.h"
#include "include/MVExpr/MVDataType.h"
#include "include/MVSourceLocation.h"
#include "include/Vectorization/VectorIR.h"

using namespace macveth;

namespace macveth {

using MVStrVector = std::vector<std::string>;

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
    /// Scalar or sequential operation
    VSEQ,
    /// If the SIMD instruction is the result of an optimization (e.g. fuse
    /// multiply-accumulation) we will call it VOPT
    VOPT,
    /// Initializing some value, for instance, in reductions
    INIT,
    /// Template
    VTEMPLATE
  };

  /// Wrap for representing the SIMDInst not just as single strings to print,
  /// but as a set of fields
  struct SIMDInst {

    /// Empty constructor
    SIMDInst(){};

    /// Constructor for not SIMD code
    SIMDInst(const std::string &LHS, const std::string &RHS,
             const MVSourceLocation &SL)
        : Result(LHS), FuncName(RHS), MVSL(SL) {
      SIMD_UID = SIMDInst::UID++;
    }
    /// Constructor
    SIMDInst(const std::string &R, const std::string &FN,
             const MVStrVector &Args, const std::string &MVFunc,
             const MVStrVector &MVArgs, const MVSourceLocation &SL)
        : Result(R), FuncName(FN), Args(Args), MVFuncName(MVFunc),
          MVArgs(MVArgs), MVSL(SL) {
      SIMD_UID = SIMDInst::UID++;
    }

    /// VOperand result
    VOperand VOPResult;
    /// Unique identifier generator
    static inline unsigned int UID = 0;
    /// Unique identifier
    unsigned int SIMD_UID = 0;
    /// Result register name
    std::string Result;
    /// Type of operation
    MVOp MVOP;
    /// Signature of the function
    std::string FuncName;
    /// List of *sorted* arguments of the function
    MVStrVector Args;
    /// Signature of the function (macro approach)
    std::string MVFuncName;
    /// Type of the function
    SIMDType SType;
    /// List of *sorted* arguments of the function (macro approach)
    MVStrVector MVArgs;
    /// Source location according to our TACs
    MVSourceLocation MVSL;
    /// Cost of the instruction
    // InstCostInfo Cost;
    /// Data type
    MVDataType::VDataType DT;
    /// Width
    MVDataType::VWidth W;
    /// Cost
    unsigned int Cost = 10;
    /// For random packing templates
    bool Template = false;

    const unsigned int getCost() { return Cost; }
    const bool isTemplate() { return Template; }

    std::string getResultValue() { return Result; }

    /// Render instruction as a string
    std::string render();

    /// Get the source location of the SIMD instruction
    MVSourceLocation getMVSourceLocation() { return MVSL; }

    /// Check if the vectorial type of the operation is sequential
    bool isSequential() { return SType == SIMDType::VSEQ; }

    /// Check if the vectorial type of the operation is a reduction
    bool isReduction() { return SType == SIMDType::VREDUC; }

    bool isAddition() { return SType == SIMDType::VADD; }

    /// Comparison operator
    bool operator==(const SIMDInst &S) const {
      return (S.SIMD_UID == SIMD_UID);
    }

    /// This is util if we want to use SIMDInst in std::map
    bool operator<(const SIMDInst &S) const { return S.SIMD_UID < SIMD_UID; }
  };

  /// Alias for list of SIMDInst structures
  using SIMDInstListType = std::list<SIMDInst>;

  /// Empty constructor

  /// Generate non SIMD instructions, as we may have sequential operations or
  /// other type of not vectorized instructions, given a VectorOP
  SIMDInst addNonSIMDInst(VectorOP &OP, SIMDType SType, MVSourceLocation MVSL,
                          SIMDInstListType *IL);
  /// Generate non SIMD instructions, as we may have sequential operations or
  /// other type of not vectorized instructions specifying explicitly the LHS
  /// and the RHS
  SIMDInst addNonSIMDInst(std::string Lhs, std::string Rhs, SIMDType SType,
                          MVSourceLocation MVSL, SIMDInstListType *IL);

  // VectorAPI: instructions to implement by the specific backends

  // Operands instructions

  /// Generate pack instructions, e.g. load/loadu
  virtual SIMDInstListType vload(VOperand V) = 0;
  /// Generate broadcast instructions. Broadcasts are used to replicate known
  /// values (i.e. values in registers, not memory addresses)
  virtual SIMDInstListType vbcast(VOperand V) = 0;
  /// Generate gather instructions. Gathers are meant to retrieve data from the
  /// memory using indices
  virtual SIMDInstListType vpack(VOperand V) = 0;
  /// Generate set instrucctions. Set is meant for literal values
  virtual SIMDInstListType vset(VOperand V) = 0;
  virtual SIMDInstListType vregisterpacking(VOperand V) = 0;

  /// Generate store instructions to memory
  virtual SIMDInstListType vstore(VectorOP V) = 0;
  /// Generate store instructions to memory using an index
  virtual SIMDInstListType vscatter(VectorOP V) = 0;
  virtual SIMDInstListType singleElementScatterOp(VectorOP V) = 0;

  // Binary operations

  /// Generate multiplication operations
  virtual SIMDInstListType vmul(VectorOP V) = 0;
  /// Generate subtraction operations
  virtual SIMDInstListType vsub(VectorOP V) = 0;
  /// Generate addition operations
  virtual SIMDInstListType vadd(VectorOP V) = 0;
  /// Generate division operations
  virtual SIMDInstListType vdiv(VectorOP V) = 0;
  /// Generate modulo operations
  virtual SIMDInstListType vmod(VectorOP V) = 0;

  /// Generate custom functions
  virtual SIMDInstListType vfunc(VectorOP V) = 0;

  // Reduction operations

  /// Operations are reduce given a certain condicitons.
  virtual SIMDInstListType vreduce(VectorOP V) = 0;

  // Scalar operations

  /// If operation is not binary, pararel or reduce, then we just emit a
  /// sequential operation. Backend will be in charge of performing any
  /// optimizations if needed
  virtual SIMDInstListType vseq(VectorOP V) = 0;

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
  virtual MVStrVector renderSIMDRegister(SIMDInstListType S) = 0;

  /// Perform some peephole optimizations after generating SIMD instructions
  virtual SIMDInstListType peepholeOptimizations(SIMDInstListType I) = 0;

  /// Get initial values of a VectorOP. Must be implemented
  virtual std::vector<std::string> getInitValues(VectorOP V) = 0;

  /// Add SIMD instruction
  virtual SIMDInst genSIMDInst(std::string Result, std::string Op,
                               std::string PrefS, std::string SuffS,
                               MVDataType::VWidth Width,
                               MVDataType::VDataType Type, MVStrVector Args,
                               SIMDType SType, MVSourceLocation SL,
                               SIMDInstListType *IL, std::string NameOp = "",
                               std::string MVFunc = "", MVStrVector MVArgs = {},
                               MVOp MVOP = MVOp()) = 0;

  virtual SIMDInst genSIMDInst(VOperand V, std::string Op, std::string PrefS,
                               std::string SuffS, MVStrVector OPS,
                               SIMDType SType, MVSourceLocation SL,
                               SIMDInstListType *IL = nullptr,
                               std::string NameOp = "", std::string MVFunc = "",
                               MVStrVector MVArgs = {}, MVOp MVOP = MVOp()) = 0;

  /// Get headers needed (include files)
  virtual std::vector<std::string> getHeadersNeeded() = 0;

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
  static std::string replacePatterns(const std::string &Pattern,
                                     const std::string &Width,
                                     const std::string &DataSuffix,
                                     const std::string &Prefix,
                                     const std::string &Suffix);

  /// Render SIMD instructions as a list of strings, where each element
  /// represents a new line
  std::vector<std::string> renderSIMDasString(SIMDInstListType &S);

  /// Auxiliary function to retrieve properly the operands
  std::string getOpName(const VOperand &V, bool Ptr, bool RegVal,
                        int Position = 0, int Offset = 0);

  /// Insert the SIMDInst in the list given an VOperand
  bool getSIMDVOperand(VOperand V, SIMDInstListType *IL,
                       bool Reduction = false);

  std::string genGenericFunc(std::string F, std::vector<std::string> L);

  /// Entry point: this method basically redirects to any of the operations
  /// supported
  SIMDInstListType getSIMDfromVectorOP(VectorOP &V);

  /// Basically calls its other overloaded function iterating over the input
  /// list of VectorOP
  SIMDInstListType getSIMDfromVectorOP(const VectorOPListT &VList);

  /// Get list of initializations
  SIMDInstListType getInitReg() { return InitReg; }

  SIMDBackEnd(){};

  /// Destructor
  virtual ~SIMDBackEnd(){};

  using RegistersMapT =
      std::map<std::string,
               std::vector<std::tuple<std::string, std::vector<std::string>>>>;

  /// Get list of registers declared
  RegistersMapT getRegDeclared() { return RegDeclared; }

  const RandomPackingTable &getRPTable() const { return RPTable; }

private:
  const RandomPackingTable &RPTable = RandomPackingTable();

  /// Auxiliary function to dispatch the VectorOP operation
  void mapOperation(VectorOP &V, SIMDInstListType *TI);

  /// Auxiliary function to dispatch the VectorOP operation
  void reduceOperation(VectorOP &V, SIMDInstListType *TI);

protected:
  /// Add register to declare
  static void addRegToDeclare(std::string Type, std::string Name,
                              int InitVal = 0);

  /// Add register to declare
  static void addRegToDeclareInitVal(std::string Type, std::string Name,
                                     std::vector<std::string> InitVal);

  static inline std::map<std::string, std::string> MapReduxVarToAccm;
  static inline std::map<std::string, VOperand> MapReduxVOp;

  /// List of registers declared
  static inline RegistersMapT RegDeclared;

  /// List of SIMD instructions which represent the initialization of some
  /// registers
  static inline SIMDInstListType InitReg;

  /// Prefix of the auxiliary regsiters
  static inline const std::string AUX_PREFIX = "__mv_aux";
  /// Prefix of the auxiliary arrays
  static inline const std::string ARR_PREFIX = "__mv_arr";
  /// Prefix of the
  static inline const std::string VEC_PREFIX = "__mv_accm";

  /// AccmReg numbering for auxiliary operations such as reduce
  static inline int AccmReg = 0;

  using MapRegT = std::map<std::string, int>;

  /// Map of the operands mapped to the accumulator
  static inline MapRegT AccmToReg;
  /// Map of the operands mapped to the accumulator which are dirty
  static inline MapRegT AccmDirty;

  /// Get the next available accumulator register
  static std::string getNextAccmRegister(const std::string &V) {
    if (AccmToReg.count(V) == 0) {
      AccmDirty[V] = 0;
      AccmToReg[V] = AccmReg++;
    }
    return VEC_PREFIX + std::to_string(AccmToReg.at(V));
  }

  /// Check if the accumulator register is in a dirty state
  static bool isAccmClean(const std::string &V) { return !AccmDirty[V]; }

  /// Mark the accumulator register as dirty
  static void markDirtyAccm(const std::string &V) { AccmDirty[V] = 1; }

  /// Get the current accumulator register
  static std::string getAccmReg(const std::string &V) {
    if (AccmToReg.count(V) == 0) {
      return V;
    }
    return VEC_PREFIX + std::to_string(AccmToReg.at(V));
  }

  /// Auxiliar register unique ID
  static inline int AuxRegId = 0;

  /// Map of auxiliar registers
  static inline MapRegT AuxReg;

  /// Get the name of the auxiliar register for a operand and increment
  static std::string getNextAuxRegister(const std::string &V) {
    if (AuxReg.count(V) == 0) {
      AuxReg[V] = AuxRegId++;
    }
    return AUX_PREFIX + std::to_string(AuxReg.at(V));
  }

  /// Check if value has been already been mapped for an accumulator
  static bool hasAlreadyBeenMapped(const std::string &V) {
    return !(AccmToReg.count(V) == 0);
  }

  /// Return auxiliar register given a type and a vector name to map
  std::string mapNewAuxRegister(const std::string &Type,
                                const std::string &Name, int InitVal = 0) {
    auto Reg = getNextAuxRegister(Name);
    addRegToDeclare(Type, Reg, InitVal);
    return Reg;
  }

  /// Auxiliary array numbering for auxiliary operations such as reduce
  static inline int AuxArrayReg = 0;
  static std::string getNextArrRegister(const std::string &Type,
                                        const int &Size) {
    auto Name = ARR_PREFIX + std::to_string(AuxArrayReg++);
    addRegToDeclare(Type, Name + "[" + std::to_string(Size) + "]");
    return Name;
  }

public:
  static void clear() {
    AuxReg.clear();
    AuxRegId = 0;
    AuxArrayReg = 0;
    AccmToReg.clear();
    AccmDirty.clear();
    AccmReg = 0;
    InitReg.clear();
    RegDeclared.clear();
    MapReduxVarToAccm.clear();
    MapReduxVOp.clear();
    SIMDInst::UID = 0;
  }
};

} // namespace macveth
#endif /* !MACVETH_SIMDGENERATOR_H */
