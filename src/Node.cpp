/*
 * File					 : src/Node.cpp
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Mon 02 Nov 2020 05:52 +01:00
 * Modified By	 : Marcos Horro (marcos.horro@udc.gal>)
 *
 * MIT License
 *
 * Copyright (c) 2020 Colorado State University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "include/Node.h"

// ---------------------------------------------
std::string Node::toStringShort() {
  std::string Str;
  Str += this->getValue() + " (" + this->getRegisterValue() + ");";
  if (this->getNodeType() != Node::NODE_MEM) {
    Str += " O = " + this->getOutputInfoName() + "; ";
  }
  for (int i = 0; i < this->getInputs().size(); ++i) {
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
