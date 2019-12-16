/**
 * File              : CDAG.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:35 MST
 * Last Modified Date: Lun 16 Dec 2019 16:21:12 MST
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
  int Class = 0;
  int VL = 4;
  std::string VLoad[VL];
  std::string VOps[VL];
  int Cursor = 0;
  for (Node *N : G->getNodeListOps()) {
  }
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
    /// TACs are of the form a = b op c, so if we create a Node for each TAC we
    /// are basically creating NODE_OP Nodes. This way, when we connect this new
    /// node to the rest of the CDAG, we are basically looking for connections
    /// between the inputs and outputs. Actually we are looking for outputs of
    /// of already connected Nodes that match the input of this new NODE_OP.
    /// Looking for inputs
    Node *ActualNode = G->insertTac(T, G->getNodeListOps());
  }
  return G;
}
} // namespace macveth
