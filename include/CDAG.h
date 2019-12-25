/**
 * File              : CDAG.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:51 MST
 * Last Modified Date: Mar 24 Dec 2019 14:43:55 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_CDAG_H
#define MACVETH_CDAG_H
#include <list>
#include <stdio.h>

#include "include/Node.h"
#include "include/Utils.h"
#include "include/Vectorization/VectorIR.h"

using namespace macveth;

namespace macveth {

/// The CDAG only holds a list of Nodes which are of type NODE_OP
class CDAG {
public:
  /// Given a list of TACs, create its correspondent CDAG
  static CDAG *createCDAGfromTAC(TacListType TL);
  /// Compute cost model
  static int computeCostModel(CDAG *C);
  /// Compute cost model for a set of Nodes
  static int computeCostModel(Node::NodeListType NL);

  /// Computer the free schedule, basically the topology order
  static void computeFreeSchedule(CDAG *C);
  /// Computer the free schedule, basically the topology order for a list or set
  /// of Nodes
  static void computeFreeSchedule(Node::NodeListType NL);

  /// Generate intrinsics
  static int generateIntrinsics(CDAG *C);
  /// Get the node of list registered for this CDAG
  Node::NodeListType getNodeListOps() { return this->NLOps; }
  /// Get the node of list registered for this CDAG
  Node::NodeListType getNodeListMem() { return this->NLMem; }

private:
  /// Insert TAC instruction to the CDAG
  Node *insertTac(TAC T, Node::NodeListType L);
  /// List of OP nodes
  Node::NodeListType NLOps;
  /// List of memory nodes
  Node::NodeListType NLMem;
};

} // namespace macveth
#endif
