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
  Str += "name\t\t= " + this->getValue() +
         " (reg val = " + this->getRegisterValue() + ")\n";
  Str +=
      "\tFreeSched\t= " + std::to_string(this->getSchedInfo().FreeSched) + "\n";
  Str += "\tTacID\t\t= " + std::to_string(this->getSchedInfo().TacID) + "\n";
  Str += "\tPlcmnt\t\t= " + std::to_string(this->getSchedInfo().Plcmnt) + "\n";
  Str += "\tNodeID\t\t= " + std::to_string(this->getSchedInfo().NodeID) + "\n";
  Str += "\tOutput\t\t= " + this->getOutputInfoName() + "\n";
  for (int i = 0; i < this->getInputs().size(); ++i) {
    if (this->getInputs()[i] != nullptr) {
      Str += "\tInput " + std::to_string(i) +
             "\t\t= " + this->getInputs()[i]->getValue() + " (ID = " +
             std::to_string(this->getInputs()[i]->getSchedInfo().NodeID) +
             "); NODE_MEM = " +
             std::to_string(this->getInputs()[i]->T == NODE_MEM) + "\n";
    }
  }
  Str += "\tLoopName\t= " + this->getLoopName() + "\n";
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
  std::reverse(std::begin(CL), std::end(CL));
  for (auto NL : CL) {
    if (NodeName == NL->getOutputInfoName()) {
      return NL;
    }
  }
  return nullptr;
}
