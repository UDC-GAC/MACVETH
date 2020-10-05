/**
 * File              : CDAG.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:51 MST
 * Last Modified Date: Lun 13 Xan 2020 15:04:08 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_CDAG_H
#define MACVETH_CDAG_H

#include "include/Node.h"
#include "include/TAC.h"
//#include "include/Vectorization/SIMD/SIMDGenerator.h"

using namespace macveth;

namespace macveth {

/// The CDAG only holds a list of Nodes which are of type NODE_OP
class CDAG {
public:
  using DepMap = std::map<int, std::set<int>>;

  /// Given a list of TACs, create its correspondent CDAG
  static CDAG *createCDAGfromTAC(TacListType TL);

  /// Compute cost model for a set of Nodes
  static int computeCostModel(Node::NodeListType NL);

  /// Computer the free schedule, basically the topology order
  static void computeFreeSchedule(CDAG *C);

  /// Computer the free schedule, basically the topology order for a list or set
  /// of Nodes
  static void computeFreeSchedule(Node::NodeListType NL);

  /// Compute WAR anti-dependencies for a given node
  Node *findWARDataRace(Node *N, Node::NodeListType NL);

  /// Compute RAW dependencies for a given node
  Node *findRAWDataRace(Node *N, Node::NodeListType NL);

  /// Get the node of list registered for this CDAG
  Node::NodeListType getNodeListOps() { return this->NLOps; }

  DepMap getRAWs() { return this->MapRAW; }
  DepMap getWARs() { return this->MapWAR; }

private:
  /// Insert TAC instruction to the CDAG
  Node *insertTac(TAC T, Node::NodeListType L);
  /// List of OP nodes
  Node::NodeListType NLOps;
  /// List of memory nodes
  Node::NodeListType NLMem;
  /// RAW dependencies
  DepMap MapRAW;
  /// WAR dependencies
  DepMap MapWAR;
};

} // namespace macveth
#endif /* !MACVETH_CDAG_H */
