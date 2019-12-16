/**
 * File              : CDAG.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:35 MST
 * Last Modified Date: Xov 12 Dec 2019 15:15:38 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/CDAG.h"

namespace macveth {

// ---------------------------------------------
void Node::connectInput(Node *N) {
  this->In.push_back(*N);
  N->Out.push_back(*this);
}

// ---------------------------------------------
void Node::connectOutput(Node *N) {
  this->Out.push_back(*N);
  N->In.push_back(*this);
}

// ---------------------------------------------
bool Node::hasOut() { return !(this->Out.size() == 0); }

// ---------------------------------------------
bool Node::hasInputs() { return !(this->In.size() == 0); }

// ---------------------------------------------
bool Node::isTerminal() { return !this->hasOut(); }

// ---------------------------------------------
bool Node::hasOutNode(Node *N) {
  for (Node C : this->Out) {
    if (&C == N) {
      return true;
    }
  }
  return false;
}

// ---------------------------------------------
bool Node::hasInNode(Node *N) {
  for (Node C : this->In) {
    if (&C == N) {
      return true;
    }
  }
  return false;
}

// ---------------------------------------------
void Node::mergeIfFound(Node NIn) {
  if (!this->hasInputs()) {
    return;
  }
  for (Node C : this->getInputs()) {
    if (C == NIn) {
      std::cout << "Merging node " + C.getValue() << "\n" << std::flush;
      this->getInputs().remove(C);
      this->getInputs().push_back(C);
      return;
    }
  }
}

// ---------------------------------------------
Node CDAG::insertTac(TAC T) {
  Node NewNode(T);
  // this->NL.push_back(NewNode);
  return NewNode;
}

// ---------------------------------------------
bool CDAG::connectNode(Node NewNode) {
  std::cout << "Connecting node " + NewNode.getValue() << "\n" << std::flush;
  for (Node PN : this->getNodeList()) {
    for (Node OutN : PN.getOuput()) {
      NewNode.mergeIfFound(OutN);
    }
  }
  this->NL.push_back(NewNode);
  return true;
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
    Node ActualNode = G->insertTac(T);
    G->connectNode(ActualNode);
  }
  return G;
}
} // namespace macveth
