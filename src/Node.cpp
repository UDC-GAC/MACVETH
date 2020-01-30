/**
 * File              : Node.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 18 Dec 2019 17:01:36 MST
 * Last Modified Date: Lun 13 Xan 2020 23:33:27 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Node.h"

// ---------------------------------------------
std::string Node::toString() {
  std::string Str;
  Str += "name\t\t= " + this->getValue() + "\n";
  Str +=
      "\tFreeSched\t= " + std::to_string(this->getSchedInfo().FreeSched) + "\n";
  Str +=
      "\tTacOrder\t= " + std::to_string(this->getSchedInfo().TacOrder) + "\n";
  Str += "\tPlcmnt\t\t= " + std::to_string(this->getSchedInfo().Plcmnt) + "\n";
  Str += "\tStmtID\t\t= " + std::to_string(this->getSchedInfo().StmtID) + "\n";
  Str += "\tOutput\t\t= " + this->getOutputInfo().Name + "\n";
  Str += "\tInput1\t\t= " + this->getInputs().front()->getValue() + " (ID = " +
         std::to_string(this->getInputs().front()->getSchedInfo().StmtID) +
         "); NODE_MEM = " +
         std::to_string(this->getInputs().front()->T == NODE_MEM) + "\n";
  Str += "\tInput2\t\t= " + this->getInputs().back()->getValue() + " (ID = " +
         std::to_string(this->getInputs().back()->getSchedInfo().StmtID) +
         "); NODE_MEM = " +
         std::to_string(this->getInputs().back()->T == NODE_MEM) + "\n";
  Str += "----------------------------------------";
  return Str;
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
