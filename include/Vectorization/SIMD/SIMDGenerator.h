/**
 * File              : SIMDGenerator.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 15:32:33 MST
 * Last Modified Date: Mér 01 Xan 2020 15:07:59 MST
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
  /// Wrap for representing the SIMDInst not just as single strings to print,
  /// but as a set of fields
  struct SIMDInst {
    std::string Result;
    std::string FuncName;
    std::list<std::string> OPS;

    /// Render instruction as a string
    std::string render() {
      std::string FullFunc = !Result.compare("")
                                 ? FuncName + "("
                                 : Result + " = " + FuncName + "(";
      std::list<std::string>::iterator Op;
      int i = 0;
      for (Op = OPS.begin(); Op != OPS.end(); ++Op) {
        FullFunc += (i++ == (OPS.size() - 1)) ? *Op : (*Op + ", ");
      }
      FullFunc += ")";
      return FullFunc;
    }

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

  /// Empty constructor
  SIMDGenerator(){};

  /// Destructor
  virtual ~SIMDGenerator(){};

  /// Generating SIMD instructions from list of vector operations
  virtual SIMDInfo genSIMD(std::list<VectorIR::VectorOP> V) = 0;

  /// Get max width
  virtual int getMaxWidth() = 0;

  /// Render SIMD instructions as a list of strings, where each element
  /// represents a new line
  std::list<std::string> renderSIMDasString(SIMDInfo S);

  /// Map data types to their size in bytes
  inline static std::map<std::string, int> SizeOf = {
      {"double", 8}, {"float", 4}, {"char", 1}, {"unsigned int", 4}};

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
