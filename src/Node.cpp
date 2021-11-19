// MACVETH - Node.cpp
//
// Copyright (c) Colorado State University. 2019-2021
// Copyright (c) Universidade da Coruña. 2020-2021
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Authors:
//     Marcos Horro <marcos.horro@udc.es>
//     Louis-Noël Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Marcos Horro <marcos.horro@udc.es>

#include "include/Node.h"

// ---------------------------------------------
std::string Node::toStringShort() {
  std::string Str;
  Str += this->getValue() + " (" + this->getRegisterValue() + ");";
  if (this->getNodeType() != Node::NODE_MEM) {
    Str += " O = " + this->getOutputInfoName() + "; ";
  }
  for (size_t i = 0; i < this->getInputs().size(); ++i) {
    if (this->getInputs()[i] != nullptr) {
      Str +=
          " I(" + std::to_string(i) + ") = " + this->getInputs()[i]->getValue();
      Str += (this->getInputs()[i + 1] == nullptr) ? "" : ";";
    }
  }
  return Str;
}

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
  if (this->getNodeType() != Node::NODE_MEM) {
    Str += "\tOutput\t\t= " + this->getOutputInfoName() + "\n";
  }
  for (size_t i = 0; i < this->getInputs().size(); ++i) {
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
void Node::connectInput(Node *N) { this->I.push_back(N); }

// ---------------------------------------------
bool Node::hasInputs() { return !(this->I.size() == 0); }

// ---------------------------------------------
Node *Node::findOutputNode(const std::string &NodeName, NodeListType L) {
  NodeListType CL(L);
  std::reverse(std::begin(CL), std::end(CL));
  for (auto NL : CL) {
    if (NodeName == NL->getOutputInfoName()) {
      return NL;
    }
  }
  return nullptr;
}
