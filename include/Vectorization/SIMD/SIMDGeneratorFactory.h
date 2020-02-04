/**
 * File              : SIMDGeneratorFactory.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 14:43:18 MST
 * Last Modified Date: Lun 13 Xan 2020 19:05:18 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_SIMDGENERATORFACTORY_H
#define MACVETH_SIMDGENERATORFACTORY_H

#include "include/MVOptions.h"
#include "include/Vectorization/SIMD/AVX2Gen.h"
#include "include/Vectorization/SIMD/SIMDGenerator.h"
#include <string>

namespace macveth {

/// Wrapper for avoiding low level details when creating the different
/// backends
class SIMDGeneratorFactory {
public:
  /// Return a specific backend given as input
  static SIMDGenerator *getBackend(MVISA ISA) {
    SIMDGenerator *G;
    switch (ISA) {
    case NATIVE:
    case SSE:
    case AVX:
    case AVX2:
    case AVX512:
      G = new AVX2Gen();
      G->clearMappings();
      return G;
    }
    return nullptr;
  }
};

} // namespace macveth
#endif
