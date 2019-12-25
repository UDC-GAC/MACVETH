/**
 * File              : AVX2Gen.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 22 Dec 2019 20:50:29 MST
 * Last Modified Date: Mar 24 Dec 2019 10:36:55 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Vectorization/SIMD/SIMDGenerator.h"

using namespace macveth;

namespace macveth {

class AVX2Gen : public SIMDGenerator {
public:
  static inline int MaxWidth = 256;
  static inline std::map<int, std::string> WidthTypeMap = {{128, "sp"},
                                                           {256, "dp"}};
};

} // namespace macveth
