/**
 * File              : IntrinsicsFactory.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 14:43:18 MST
 * Last Modified Date: Ven 20 Dec 2019 15:30:18 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_INTRINSICSFACTORY_H
#define MACVETH_INTRINSICSFACTORY_H

#include <string>

namespace macveth {

class IntrinsicsFactory {
public:
  static std::string genenerateInstruction();
};

} // namespace macveth
#endif
