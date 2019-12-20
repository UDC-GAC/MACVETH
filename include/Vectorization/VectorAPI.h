/**
 * File              : VectorAPI.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 09:59:02 MST
 * Last Modified Date: Ven 20 Dec 2019 14:30:00 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_VECTORAPI_H
#define MACVETH_VECTORAPI_H

#include "include/Node.h"
#include "include/Utils.h"

using namespace macveth;

namespace macveth {
/// VectorAPI is a facade for the CDAG to compute the cost of the vectorization,
/// generate intrinsics and perform the optimizations.

class VectorAPI {
public:
  /// Given a set of operations from the CDAG it computes the vector cost
  static int computeCostVectorOp(int VL, Node *VOps[], int VOpsSched[],
                                 Node *VLoadA[], int VLoadSchedA[],
                                 Node *VLoadB[], int VLoadSchedB[]);
};

} // namespace macveth
#endif
