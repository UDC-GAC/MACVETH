/**
 * File              : Node.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 18 Dec 2019 17:01:36 MST
 * Last Modified Date: Mar 31 Dec 2019 18:12:26 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Node.h"

// ---------------------------------------------
void Node::printNode() {
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "[NODE] name\t\t= " << this->getValue() << std::endl;
  std::cout << "\tFreeSched\t= " << this->getSchedInfo().FreeSched << std::endl;
  std::cout << "\tPlcmnt\t= " << this->getSchedInfo().Plcmnt << std::endl;
  std::cout << "\tStmtID\t\t= " << this->getSchedInfo().StmtID << std::endl;
  std::cout << "\tInput1\t\t= " << this->getInputs().front()->getValue()
            << std::endl;
  std::cout << "\tInput2\t\t= " << this->getInputs().back()->getValue()
            << std::endl;
  std::cout << "----------------------------------------" << std::endl;
}

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
