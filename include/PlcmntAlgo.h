#ifndef MACVETH_PLCMNTALGO_H
#define MACVETH_PLCMNTALGO_H

#include "include/CDAG.h"
#include "include/Node.h"

using namespace macveth;

namespace macveth {

class PlcmntAlgo {
public:
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
