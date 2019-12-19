/**
 * File              : Node.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 18 Dec 2019 17:01:36 MST
 * Last Modified Date: Mér 18 Dec 2019 17:07:25 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Node.h"

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
    if (!NodeName.compare(NL->getOuputInfo().Name)) {
      return NL;
    }
  }
  return NULL;
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
