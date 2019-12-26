/**
 * File              : CDAG.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:35 MST
 * Last Modified Date: Xov 26 Dec 2019 16:00:06 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/CDAG.h"

namespace macveth {

// ---------------------------------------------
void printDebug(std::string M, std::string S) {
  std::cout << "[" << M << " DEBUG] " << S << std::endl;
}

// ---------------------------------------------
int computeMaxDepth(Node *N) {
  /// This algorithm is not efficient if the CDAG is big. At some point more
  /// intelligent strategies should be used instead.
  if (N->getInputNum() == 0) {
    return 0;
  } else {
    int max = 0;
    for (Node *T : N->getInputs()) {
      max = std::max(max, computeMaxDepth(T));
    }
    return 1 + max;
  }
}

// ---------------------------------------------
int CDAG::computeCostModel(CDAG *G) {
  int VL = VectorIR::VL;
  Node *VLoadA[VL];
  Node *VLoadB[VL];
  Node *VOps[VL];
  int Cursor = 0;
  int TotalCost = -1;

  /// Working with a copy
  Node::NodeListType NL(G->getNodeListOps());
  // The sorting of the list is done by comparing the FreeSched value and the
  // StmtID, which is sequential when creating each node. This way we have a
  // chronological order of the nodes.
  NL.sort([](Node *lhs, Node *rhs) { return *lhs < *rhs; });

  printDebug("CDAG", "Printing nodes");
  for (Node *N : NL) {
    N->printNode();
  }

  printDebug("CDAG", "Entering loop");

  // Until the list is over
repeat:
  while (!NL.empty()) {
    VOps[Cursor] = NL.front();
    if ((Cursor > 0) &&
        (VOps[Cursor]->getValue().compare(VOps[Cursor - 1]->getValue()))) {
      printDebug("CDAG",
                 "Full OPS of same type = " + VOps[Cursor - 1]->getValue());
      break;
    }
    NL.pop_front();
    if (++Cursor == VL) {
      printDebug("CDAG", "VL OPS achieved");
      break;
    }
  }
  bool IsPartial = (Cursor < VL);
  VL = IsPartial ? Cursor : VectorIR::VL;
  if (IsPartial) {
    printDebug("CDAG", "isPartial: " + std::to_string(VL));
  }
  // Compute memory operands for the operations fetched above
  int i = 0;
  while ((i < VL) && (VOps[i] != nullptr)) {
    Node::NodeListType Aux = VOps[i]->getInputs();
    VLoadA[i] = Aux.front();
    Aux.pop_front();
    VLoadB[i] = Aux.front();
    Aux.pop_front();
    i++;
  }

  // Compute the vector cost
  int CostVectorOperation =
      VectorIR::computeCostVectorOp(VL, VOps, VLoadA, VLoadB);

  // Repeat process if list not empty
  if (!NL.empty()) {
    Cursor = 0;
    VL = VectorIR::VL;
    for (int i = 0; i < VL; ++i) {
      VOps[i] = VLoadA[i] = VLoadB[i] = nullptr;
    }
    goto repeat;
  }

  return TotalCost;
}

// ---------------------------------------------
void CDAG::computeFreeSchedule(Node::NodeListType NL) {
  for (Node *N : NL) {
    N->setFreeSchedInfo(computeMaxDepth(N));
  }
}

// ---------------------------------------------
void CDAG::computeFreeSchedule(CDAG *C) {
  CDAG::computeFreeSchedule(C->getNodeListOps());
}

// ---------------------------------------------
Node *CDAG::insertTac(TAC T, Node::NodeListType L) {
  Node *NewNode = new Node(T, L);
  this->NLOps.push_back(NewNode);
  return NewNode;
}

// ---------------------------------------------
CDAG *CDAG::createCDAGfromTAC(TacListType TL) {
  CDAG *G = new CDAG();
  for (TAC T : TL) {
    /// TACs are of the form a = b op c, so if we create a Node for each TAC
    /// we are basically creating NODE_OP Nodes. This way, when we connect
    /// this new node to the rest of the CDAG, we are basically looking for
    /// connections between the inputs and outputs. Actually we are looking
    /// for outputs of of already connected Nodes that match the input of this
    /// new NODE_OP. Looking for inputs
    Node *ActualNode = G->insertTac(T, G->getNodeListOps());
  }
  return G;
}
} // namespace macveth
