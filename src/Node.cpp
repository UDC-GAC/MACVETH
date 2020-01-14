/**
 * File              : Node.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 18 Dec 2019 17:01:36 MST
 * Last Modified Date: Lun 13 Xan 2020 23:33:27 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Node.h"

#define NODE_DEBUG 1

// ---------------------------------------------
void Node::printNode() {
  if (!NODE_DEBUG)
    return;
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "[NODE] name\t\t= " << this->getValue() << std::endl;
  std::cout << "\tFreeSched\t= " << this->getSchedInfo().FreeSched << std::endl;
  std::cout << "\tPlcmnt\t\t= " << this->getSchedInfo().Plcmnt << std::endl;
  std::cout << "\tStmtID\t\t= " << this->getSchedInfo().StmtID << std::endl;
  std::cout << "\tOutput\t\t= " << this->getOutputInfo().Name << std::endl;
  std::cout << "\tInput1\t\t= " << this->getInputs().front()->getValue()
            << " (ID = " << this->getInputs().front()->getSchedInfo().StmtID
            << "); NODE_MEM = " << (this->getInputs().front()->T == NODE_MEM)
            << std::endl;
  std::cout << "\tInput2\t\t= " << this->getInputs().back()->getValue()
            << " (ID = " << this->getInputs().back()->getSchedInfo().StmtID
            << "); NODE_MEM = " << (this->getInputs().back()->T == NODE_MEM)
            << std::endl;
  std::cout << "----------------------------------------" << std::endl;
}

// ---------------------------------------------
void Node::connectInput(Node *N) {
  this->I.push_back(N);
  N->OutNodes.push_back(this);
}

// ---------------------------------------------
bool Node::hasOutNodes() { return (this->OutNodes.size() == 0); }

// ---------------------------------------------
bool Node::hasInputs() { return !(this->I.size() == 0); }

// ---------------------------------------------
bool Node::isTerminal() { return !(this->hasOutNodes()); }

// ---------------------------------------------
Node *Node::findOutputNode(std::string NodeName, NodeListType L) {
  NodeListType CL(L);
  CL.reverse();
  for (Node *NL : CL) {
    if (NodeName == NL->getOutputInfoName()) {
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
