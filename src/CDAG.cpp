/**
 * File              : CDAG.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:35 MST
 * Last Modified Date: Mar 17 Dec 2019 14:22:51 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/CDAG.h"

namespace macveth {

// ---------------------------------------------
void Node::connectInput(Node *N) {
  this->I.push_back(N);
  N->OutNodes.push_back(this);
}

// ---------------------------------------------
void Node::connectOutput(Node *N) {}

// ---------------------------------------------
bool Node::hasOutNodes() { return (this->OutNodes.size() == 0); }

// ---------------------------------------------
bool Node::hasInputs() { return !(this->I.size() == 0); }

// ---------------------------------------------
bool Node::isTerminal() { return !(this->hasOutNodes()); }

// ---------------------------------------------
Node *Node::findOutputNode(std::string NodeName, NodeListType L) {
  for (Node *NL : L) {
    if (NodeName.compare(NL->getOuputInfo().OutName)) {
      return NL;
    }
  }
  return NULL;
}

// ---------------------------------------------
bool Node::hasOutNode(Node *N) {
  for (Node *C : this->OutNodes) {
    if (C == N) {
      return true;
    }
  }
  return false;
}

// ---------------------------------------------
bool Node::hasInNode(Node *N) {
  for (Node *C : this->I) {
    if (C == N) {
      return true;
    }
  }
  return false;
}

// ---------------------------------------------
void Node::setFreeSchedInfo(int Value) { this->SI.FreeSched = Value; }

// ---------------------------------------------
int computeMaxDepth(Node *N) {
  /// This algorithm is not efficient if the CDAG is big... At some point more
  /// intelligent strategies should be used instead
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
  NL.sort();

  /// Until the list is over
repeat:
  while (!NL.empty()) {
    VOps[Cursor] = NL.front();
    VOpsSched[Cursor++] = NL.front()->getSchedInfo().FreeSched;
    NL.pop_front();
    if (Cursor == VL) {
      break;
    }
  }
  bool IsPartial = (Cursor < VL);

  /// Compute memory operands for the operations fetched above
  int i = 0;
  while (VOps[i] != nullptr) {
    Node::NodeListType Aux = VOps[i]->getInputs();
    for (int j = 0; j < 2; ++j) {
      VLoad[i][j] = Aux.front();
      VLoadSched[i][j] = Aux.front()->getSchedInfo().FreeSched;
      Aux.pop_front();
    }
    i++;
  }

  /// Compute the cost

  /// Repeat process...
  if (!IsPartial) {
    Cursor = 0;
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
