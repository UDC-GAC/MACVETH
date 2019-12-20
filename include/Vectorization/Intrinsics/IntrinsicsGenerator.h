/**
 * File              : IntrinsicsGenerator.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 15:32:33 MST
 * Last Modified Date: Ven 20 Dec 2019 16:26:17 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_INTRINSICSGENERATOR_H
#define MACVETH_INTRINSICSGENERATOR_H

#include <list>
#include <map>
#include <string>

namespace macveth {

typedef std::list<std::string> InstListType;

/// Abstract class implemented by each architecture (AVX, AVX2, AVX512, etc.) to
/// generate specific intrinsics and calculate the associated cost for the
/// operations provided by the VectorAPI
class IntrinsicsGenerator {

  /// Return value when generating new code
  struct IntrinsicsInfo {
    InstListType IntrinsicsList;
    int Cost;
  };

public:
  /// Computers a broadcast operation
  virtual IntrinsicsInfo vbroadcast();
  /// Computers a load operation
  virtual IntrinsicsInfo vload();
  /// Computers a store operation
  virtual IntrinsicsInfo vstore();
  /// Generates a FMADD
  virtual IntrinsicsInfo vfmadd();
  /// Generates a FMSUB
  virtual IntrinsicsInfo vfmsub();
  /// Generates a reduction
  virtual IntrinsicsInfo vreduction();

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
