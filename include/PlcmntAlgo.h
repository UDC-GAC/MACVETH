/**
 * File              : PlcmntAlgo.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 02 Mar 2020 15:29:56 CET
 * Last Modified Date: Lun 02 Mar 2020 16:56:51 CET
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#ifndef MACVETH_PLCMNTALGO_H
#define MACVETH_PLCMNTALGO_H

#include "include/CDAG.h"
#include "include/Node.h"

using namespace macveth;

namespace macveth {

class PlcmntAlgo {
public:
  /// Calculate free scheduling for a node (in-order)
  static void computeFreeSchedule(Node *N);

  /// Calculate free scheduling for a list of nodes (in-order)
  static void computeFreeSchedule(Node::NodeListType NL);

  /// Detect reductions in the list of input nodes
  static Node::NodeListType detectReductions(Node::NodeListType *NL);

  /// Fuse reductions. Input **MUST** contain reductions in order to operate
  /// properly, in any other way it will not generate anything at all.
  static Node::NodeListType fuseReductions(Node::NodeListType NL);

  /// Set the placement from the input file
  static void setPlcmtFromFile(Node::NodeListType NL);

  /// Perform sorting of the list of nodes given
  static Node::NodeListType sortGraph(Node::NodeListType NL);

  /// Topological sort, very naive implementation
  static Node::NodeListType topSort(Node::NodeListType NL) {
    NL.sort([](Node *Lhs, Node *Rhs) { return *Lhs < *Rhs; });
    return NL;
  }

  /// Breadth first search (BFS): this should perform badly in most cases
  static Node::NodeListType breadthFirstSearch(Node::NodeListType NL) {
    return NL;
  }
};

} // namespace macveth

#endif
