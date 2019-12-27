/**
 * File              : SIMDGenerator.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 15:32:33 MST
 * Last Modified Date: Ven 27 Dec 2019 15:01:50 MST
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
protected:
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

    /// Constructor
    SIMDInst(std::string R, std::string FN, std::list<std::string> OPS)
        : Result(R), FuncName(FN), OPS(OPS) {}
  };

  /// Alias for list of SIMDInst structures
  typedef std::list<SIMDInst> SIMDInstListType;

public:
  /// Empty constructor
  SIMDGenerator(){};

  /// Destructor
  virtual ~SIMDGenerator(){};

  /// Return value when generating new code
  struct SIMDInfo {
    SIMDInstListType SIMDList;
    unsigned int Cost;
  };

  /// Generating SIMD instructions from list of vector operations
  virtual SIMDInfo genSIMD(std::list<VectorIR::VectorOP> V) = 0;

  /// Render SIMD instructions as a list of strings, where each element
  /// represents a new line
  std::list<std::string> renderSIMDasString(SIMDInfo S);

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
