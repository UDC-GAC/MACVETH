/**
 * File              : SIMDGenerator.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 15:32:33 MST
 * Last Modified Date: Mér 15 Xan 2020 10:57:54 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_SIMDGENERATOR_H
#define MACVETH_SIMDGENERATOR_H

#include "include/Vectorization/VectorIR.h"

using namespace macveth;

namespace macveth {

/// Abstract class implemented by each architecture (AVX, AVX2, AVX512, etc.) to
/// generate specific intrinsics and calculate the associated cost for the
/// operations provided by the VectorAPI
class SIMDGenerator {
public:
  /// Types of SIMD instructions
  enum SIMDType {
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
    VSTORER,
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
    VSEQR,
    /// If the SIMD instruction is the result of an optimization (e.g. fuse
    /// multiply-accumulation) we will call it VOPT
    VOPT
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
    /// TAC order
    int TacID = -1;
    /// Result register name
    std::string Result;
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
    int Cost = 0;
    /// Data type
    VectorIR::VDataType DT;
    /// Width
    VectorIR::VWidth W;

    /// Render instruction as a string
    std::string render();

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
    SIMDInst(std::string LHS, std::string RHS, int TacID)
        : Result(LHS), FuncName(RHS), TacID(TacID) {
      this->SIMD_UID = SIMDInst::UID++;
    }
    /// Constructor
    SIMDInst(std::string R, std::string FN, std::list<std::string> Args,
             std::string MVFunc, std::list<std::string> MVArgs, int TacID)
        : Result(R), FuncName(FN), Args(Args), MVFuncName(MVFunc),
          MVArgs(MVArgs), TacID(TacID) {
      this->SIMD_UID = SIMDInst::UID++;
    }
  };

  /// Alias for list of SIMDInst structures
  typedef std::list<SIMDInst> SIMDInstListType;

  /// Return value when generating new code
  struct SIMDInfo {
    /// List of SIMD instructions generated
    SIMDInstListType SIMDList;
    /// Cost of operations
    std::map<std::string, long> CostOp;
    /// Number of operations of each type
    std::map<std::string, long> NumOp;
    /// Total cost
    long TotCost;

    /// Constructor
    SIMDInfo(SIMDInstListType S, std::map<std::string, long> CostOp,
             std::map<std::string, long> NumOp, long TotCost)
        : SIMDList(S), CostOp(CostOp), NumOp(NumOp), TotCost(TotCost) {}

    /// Printing the cost
    void printCost() {
      std::cout << "---------- COST SIMD --------------\n";
      for (auto It = CostOp.begin(); It != CostOp.end(); ++It) {
        std::cout << It->first + "\t=\t" +
                         std::to_string(NumOp[It->first] * It->second) + "\t(" +
                         std::to_string(NumOp[It->first]) + ")"
                  << std::endl;
      }
      std::cout << "TOTAL = " + std::to_string(TotCost) << std::endl;
      std::cout << "-----------------------------------\n";
    }
  };

  /// Generate non SIMD instructions, as we may have sequential operations or
  /// other type of not vectorized instructions, given a VectorOP
  SIMDGenerator::SIMDInst addNonSIMDInst(VectorIR::VectorOP OP,
                                         SIMDGenerator::SIMDType SType,
                                         SIMDGenerator::SIMDInstListType *IL);
  /// Generate non SIMD instructions, as we may have sequential operations or
  /// other type of not vectorized instructions specifying explicitly the LHS
  /// and the RHS
  SIMDGenerator::SIMDInst addNonSIMDInst(std::string Lhs, std::string Rhs,
                                         SIMDGenerator::SIMDType SType,
                                         SIMDGenerator::SIMDInstListType *IL);

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
  virtual std::string getRegisterType(VectorIR::VDataType DT,
                                      VectorIR::VWidth W) = 0;

  /// Get max width
  virtual int getMaxWidth() = 0;

  /// Get name of the concrete architecture
  virtual std::string getNISA() = 0;

  /// Get name of the concrete architecture
  virtual std::string getNArch() = 0;

  /// Map of VectorIR widths to the concrete architecture
  virtual std::string getMapWidth(VectorIR::VWidth V) = 0;

  /// Map of VectorIR types to the concrete architecture
  virtual std::string getMapType(VectorIR::VDataType D) = 0;

  /// Populate the table only when creating the object, to avoid overloading the
  /// memory from the start
  static void populateTable(MVISA ISA);

  /// Generate the declaration of the necessary registers for the operations
  std::list<std::string> renderSIMDRegister(SIMDInstListType S);

  /// Render SIMD instructions as a list of strings, where each element
  /// represents a new line
  std::list<std::string> renderSIMDasString(SIMDInstListType S);

  /// Auxiliary function to retrieve properly the operands
  std::string getOpName(VectorIR::VOperand V, bool Ptr, bool RegVal);

  /// Just compute the cost of each SIMD inst
  SIMDInfo computeSIMDCost(SIMDInstListType S);

  /// Insert the SIMDInst in the list given an VOperand
  bool getSIMDVOperand(VectorIR::VOperand V, SIMDInstListType *IL);

  std::string genGenericFunc(std::string F, std::vector<std::string> L);

  /// Auxiliary function for replacing patterns in a string
  static std::string replacePatterns(std::string Pattern, std::string W,
                                     std::string D, std::string P,
                                     std::string S);

  /// Perform some peephole optimizations after generating SIMD instructions
  virtual SIMDInstListType peepholeOptimizations(SIMDInstListType I) = 0;

  /// Entry point: this method basically redirects to any of the operations
  /// supported
  SIMDInstListType getSIMDfromVectorOP(VectorIR::VectorOP V);

  /// Basically calls its other overloaded function iterating over the input
  /// list of VectorOP
  SIMDInstListType getSIMDfromVectorOP(std::list<VectorIR::VectorOP> VList);

  virtual SIMDGenerator::SIMDInst
  addSIMDInst(VectorIR::VOperand V, std::string Op, std::string PrefS,
              std::string SuffS, std::list<std::string> OPS,
              SIMDGenerator::SIMDType SType,
              SIMDGenerator::SIMDInstListType *IL, std::string NameOp = "",
              std::string MVFunc = "", std::list<std::string> MVArgs = {}) = 0;

  /// Clean the list of registers declared
  static void clearMappings();

  /// Get maximum vector operands size
  virtual int getMaxVectorSize(std::string Type) {
    return getMaxWidth() / (SizeOf[Type] * 8);
  };

  /// Table of equivalences between C/C++ basic numeric types and VectorIR's
  static inline std::map<VectorIR::VDataType, std::string> VDTypeToCType = {
      {VectorIR::VDataType::DOUBLE, "double"},
      {VectorIR::VDataType::FLOAT, "float"},
  };

  /// Map data types to their size in bytes
  inline static std::map<std::string, int> SizeOf = {
      {"double", 8}, {"float", 4}, {"char", 1}, {"unsigned int", 4}};

  /// Empty constructor
  SIMDGenerator(){};

  /// Destructor
  virtual ~SIMDGenerator(){};

private:
  /// Auxiliary function to dispatch the VectorOP operation
  void mapOperation(VectorIR::VectorOP V, SIMDInstListType *TI);

  /// Auxiliary function to dispatch the VectorOP operation
  void reduceOperation(VectorIR::VectorOP V, SIMDInstListType *TI);

protected:
  /// Add register to declare
  static void addRegToDeclare(std::string Type, std::string Name);

  /// List of registers declared
  inline static std::map<std::string, std::list<std::string>> RegDeclared;

  /// AccmReg numbering for auxiliary operations such as reduce
  inline static int AccmReg = 0;
  inline static std::map<std::string, int> AccmToReg;
  static std::string getNextAccmRegister(std::string V) {
    std::string PREFIX = "__mv_accm";
    if (AccmToReg.count(V) == 0) {
      AccmToReg[V] = SIMDGenerator::AccmReg++;
    }
    return PREFIX + std::to_string(AccmToReg.at(V));
  }

  /// Auxiliary array numbering for auxiliary operations such as reduce
  inline static int AuxArrayReg = 0;
  static std::string getNextArrRegister(std::string Type, int Size) {
    std::string Name = "__mv_arr" + std::to_string(AuxArrayReg++);
    addRegToDeclare(Type, Name + "[" + std::to_string(Size) + "]");
    return Name;
  }
};

} // namespace macveth
#endif
