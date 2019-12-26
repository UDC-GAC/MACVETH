/**
 * File              : SIMDGenerator.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 15:32:33 MST
 * Last Modified Date: Xov 26 Dec 2019 15:04:13 MST
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

  struct SIMDInst {
    std::string Result;
    std::string FuncName;
    std::list<std::string> OPS;

    std::string render() {
      std::string OPSstr = "";
      for (std::string OP : OPS) {
        OPSstr += OP + ",";
      }
    }
  };

  typedef std::list<SIMDInst> SIMDInstListType;

  /// Return value when generating new code
  struct SIMDInfo {
    SIMDInstListType SIMDList;
    unsigned int Cost;
  };

public:
  /// Generating
  virtual SIMDInfo generateIntrinsics(std::list<VectorIR::VectorOP> V);

private:
  SIMDInst genFullFunction(std::string Name, std::list<std::string> OPS);

private:
  /// List of registers declared
  inline static std::list<std::string> RegDeclared;
  /// List of temporal registers declared
  inline static std::list<std::string> TempRegDeclared;
  /// Map of registers declared and its correspondant in Node terms
  inline static std::map<std::string, std::string> RegMap;

  static void clearMappings();
};

} // namespace macveth
#endif
