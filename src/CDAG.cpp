/**
 * File              : CDAG.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:35 MST
 * Last Modified Date: Mér 18 Dec 2019 17:16:36 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/CDAG.h"

namespace macveth {

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
bool opsAreSequential(int VL, Node *VOps[], int VOpsSched[]) {
  for (int i = 1; i < VL; ++i) {
    if (VOps[i - 1]->getSchedInfo().FreeSched <
        VOps[i]->getSchedInfo().FreeSched)
      return true;
  }
  return false;
}

// ---------------------------------------------
void printDebug(std::string S) {
  std::cout << "[CDAG DEBUG] " << S << std::endl;
}

// ---------------------------------------------
void Node::printNode() {
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "[NODE] name\t\t= " << this->getValue() << std::endl;
  std::cout << "\tFreeSched\t= " << this->getSchedInfo().FreeSched << std::endl;
  std::cout << "\tStmtID\t\t= " << this->getSchedInfo().StmtID << std::endl;
  std::cout << "----------------------------------------" << std::endl;
}

// ---------------------------------------------
int CDAG::computeCostModel(CDAG *G) {
  int VL = 4;
  Node *VLoad[VL][2];
  Node *VOps[VL];
  int VLoadSched[VL][2];
  int VOpsSched[VL];
  int Cursor = 0;
  int TotalCost = -1;

  /// Working with a copy
  Node::NodeListType NL(G->getNodeListOps());
  /// The sorting of the list is done by comparing the FreeSched value and the
  /// StmtID, which is sequential when creating each node. This way we have a
  /// chronological order of the nodes.
  NL.sort([](Node *lhs, Node *rhs) { return *lhs < *rhs; });

  for (Node *N : NL) {
    N->printNode();
  }

  printDebug("Entering loop");

  /// Until the list is over
repeat:
  while (!NL.empty()) {
    VOps[Cursor] = NL.front();
    VOpsSched[Cursor] = NL.front()->getSchedInfo().FreeSched;
    if ((Cursor > 0) &&
        (VOps[Cursor]->getValue().compare(VOps[Cursor - 1]->getValue()))) {
      printDebug("Full OPS of same type = " + VOps[Cursor - 1]->getValue());
      break;
    }
    NL.pop_front();
    if (++Cursor == VL) {
      printDebug("VL OPS achieved");
      break;
    }
  }
  bool IsPartial = (Cursor < VL);
  VL = IsPartial ? Cursor : 4;
  if (IsPartial) {
    printDebug("isPartial: " + std::to_string(VL));
  }
  /// Compute memory operands for the operations fetched above
  int i = 0;
  while ((i < VL) && (VOps[i] != nullptr)) {
    Node::NodeListType Aux = VOps[i]->getInputs();
    for (int j = 0; j < 2; ++j) {
      printDebug(" i " + std::to_string(i) + ", j " + std::to_string(j));
      VLoad[i][j] = Aux.front();
      VLoadSched[i][j] = Aux.front()->getSchedInfo().FreeSched;
      Aux.pop_front();
    }
    i++;
  }

  /// Premises of our algorithm
  if (bool Seq = opsAreSequential(VL, VOps, VOpsSched)) {
    printDebug("Ops are sequential");
  }
  bool RAW;
  bool Atomicity;

  /// Compute the cost
  /// Loads
  // int LoadCost = loadCost(VL, VLoad, VLoadSched);
  /// Computations
  // int OpsCost = opsCost(VL, VOps, VOpsSched);
  /// Stores
  // int StoreCost = loadCost(VLoad, VLoadSched);

  /// Repeat process...
  if (!NL.empty()) {
    Cursor = 0;
    VL = 4;
    for (int i = 0; i < VL; ++i) {
      VOps[i] = nullptr;
      VOpsSched[i] = 0;
      for (int j = 0; j < 2; ++j) {
        VLoad[i][j] = nullptr;
        VLoadSched[i][j] = 0;
      }
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
