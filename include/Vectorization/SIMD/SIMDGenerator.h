/**
 * File              : SIMDGenerator.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 15:32:33 MST
 * Last Modified Date: Sáb 04 Xan 2020 11:32:30 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_SIMDGENERATOR_H
#define MACVETH_SIMDGENERATOR_H

#include <list>
#include <map>
#include <string>

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
    /// Reduce operation
    VREDUC,
    /// Sequential operation
    VSEQ
  };

  /// Wrap for representing the SIMDInst not just as single strings to print,
  /// but as a set of fields
  struct SIMDInst {
    /// Result register name
    std::string Result;
    /// Signature of the function
    std::string FuncName;
    /// Type of the function
    SIMDType SType;
    /// List of *sorted* arguments of the function
    std::list<std::string> OPS;
    /// TODO list of instructions needed?
    std::list<SIMDInst> DependsOn;
    /// Cost of the instruction
    int Cost = 0;

    /// Render instruction as a string
    std::string render();

    /// Empty constructor
    SIMDInst(){};

    /// Constructor
    SIMDInst(std::string R, std::string FN, std::list<std::string> OPS)
        : Result(R), FuncName(FN), OPS(OPS) {}
  };

  /// Alias for list of SIMDInst structures
  typedef std::list<SIMDInst> SIMDInstListType;

  /// Return value when generating new code
  struct SIMDInfo {
    SIMDInstListType SIMDList;
    unsigned int Cost;
  };

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
  virtual SIMDInstListType vstore(VectorIR::VOperand V) = 0;
  /// Generate store instructions to memory using an index
  virtual SIMDInstListType vscatter(VectorIR::VOperand V) = 0;

  // Binary operations

  virtual SIMDInstListType vmul(VectorIR::VectorOP V) = 0;
  virtual SIMDInstListType vsub(VectorIR::VectorOP V) = 0;
  virtual SIMDInstListType vadd(VectorIR::VectorOP V) = 0;
  virtual SIMDInstListType vdiv(VectorIR::VectorOP V) = 0;
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
  virtual std::map<VectorIR::VWidth, std::string> getMapWidth() = 0;

  /// Map of VectorIR types to the concrete architecture
  virtual std::map<VectorIR::VDataType, std::string> getMapType() = 0;

  /// Render SIMD instructions as a list of strings, where each element
  /// represents a new line
  std::list<std::string> renderSIMDasString(SIMDInstListType S);

  /// Insert the SIMDInst in the list given an VOperand
  bool getSIMDVOperand(VectorIR::VOperand V, SIMDInstListType *IL);

  /// Auxiliary function for replacing patterns in a string
  std::string replacePatterns(std::string Pattern, std::string W, std::string D,
                              std::string P, std::string S);

  /// Entry point: this method basically redirects to any of the operations
  /// supported
  SIMDInstListType getSIMDfromVectorOP(VectorIR::VectorOP V);

  /// Basically calls its other overloaded function iterating over the input
  /// list of VectorOP
  SIMDInstListType getSIMDfromVectorOP(std::list<VectorIR::VectorOP> VList);

  /// Map data types to their size in bytes
  inline static std::map<std::string, int> SizeOf = {
      {"double", 8}, {"float", 4}, {"char", 1}, {"unsigned int", 4}};

  /// Empty constructor
  SIMDGenerator(){};

  /// Destructor
  virtual ~SIMDGenerator(){};

protected:
  /// Add register to declare
  static void addRegToDeclare(std::string Type, std::string Name);

protected:
  /// List of registers declared
  inline static std::map<std::string, std::list<std::string>> RegDeclared;
  /// Clean the list of registers declared
  static void clearMappings();
};

} // namespace macveth
#endif
