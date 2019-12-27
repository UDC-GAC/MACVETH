/**
 * File              : SIMDGeneratorFactory.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 14:43:18 MST
 * Last Modified Date: Ven 27 Dec 2019 15:49:57 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_SIMDGENERATORFACTORY_H
#define MACVETH_SIMDGENERATORFACTORY_H

#include "include/Vectorization/SIMD/AVX2Gen.h"
#include "include/Vectorization/SIMD/SIMDGenerator.h"

#include <string>

namespace macveth {

class SIMDGeneratorFactory {
public:
  /// Types of architectures we currently support
  enum Arch { AVX, AVX2, AVX512 };

  /// Return a specific backend given as input
  static SIMDGenerator *getBackend(SIMDGeneratorFactory::Arch A) {
    switch (A) {
    case AVX:
    case AVX2:
    case AVX512:
      return new AVX2Gen();
    }
    return nullptr;
  }
};

} // namespace macveth
#endif
