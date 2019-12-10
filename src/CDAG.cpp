/**
 * File              : CDAG.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:35 MST
 * Last Modified Date: Mar 10 Dec 2019 12:08:00 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/CDAG.h"

namespace macveth {

// ---------------------------------------------
void Node::connectInput(Node *N) {
  this->In.push_back(*N);
  N->Out = this;
}

// ---------------------------------------------
void Node::connectOutput(Node *N) {
  this->Out = N;
  N->In.push_back(*this);
}

// ---------------------------------------------
bool Node::hasOut() { return this->Out != NULL; }
bool Node::isTerminal() { return !this->hasOut(); }

// ---------------------------------------------
bool Node::hasOutNode(Node *N) { return (this->Out == N); }

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
int Node::replaceInputWithNode(Node *NewIn) {
  int count = 0;
  for (Node OldIn : this->getInputs()) {
    if (OldIn == *NewIn) {
      this->In.remove(OldIn);
      this->In.push_back(*NewIn);
      count++;
    }
  }
  return count;
}

// ---------------------------------------------
Node *CDAG::insertTac(TAC T) {
  Node *NewNode = new Node(T);
  this->NL.push_back(*NewNode);
  return NewNode;
}

// ---------------------------------------------
bool CDAG::connectNode(Node *NewNode) {
  for (Node PN : this->getNodeList()) {
    if (NewNode->hasInNode(&PN)) {
      NewNode->replaceInputWithNode(&PN);
    }
  }
  return true;
}
// ---------------------------------------------
CDAG *CDAG::createCDAGfromTAC(TacListType TL) {
  CDAG *G;
  for (TAC T : TL) {
    Node *ActualNode = G->insertTac(T);
    G->connectNode(ActualNode);
  }
  return G;
}
} // namespace macveth
