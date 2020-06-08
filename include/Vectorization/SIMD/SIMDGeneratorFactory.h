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
    /// TODO: only AVX2 implemented yet...
    switch (ISA) {
    case SSE:
    case AVX:
    case AVX512:
      assert(false && "Architecture not implemented yet!");
    case NATIVE:
    // FIXME: remove this at some point: native case should be determined at
    // some point, not to be contemplated exactly here
    case AVX2:
      G = new AVX2Gen();
      G->clearMappings();
      return G;
    }

    assert(false && "No backend chosen!");
    return nullptr;
  }
};

} // namespace macveth
#endif /* !MACVETH_SIMDGENERATORFACTORY_H */
