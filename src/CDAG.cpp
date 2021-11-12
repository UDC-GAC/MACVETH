// MACVETH - CDAG.cpp
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

#include "include/CDAG.h"
#include "include/Debug.h"
#include "include/PlcmntAlgo.h"
#include "include/Utils.h"
#include "include/Vectorization/VectorIR.h"
#include "clang/AST/Expr.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include <stdexcept>

using namespace macveth;

// ---------------------------------------------
Node *CDAG::findWARDataRace(Node *N, Node::NodeListType NL) {
  Node *WarNode = nullptr;
  for (auto Op : NL) {
    for (auto In : Op->getInputs()) {
      if ((N->getOutputInfoName() == In->getRegisterValue()) &&
          ((WarNode == nullptr) ||
           (((WarNode != nullptr) && (WarNode->getTacID() < In->getTacID()) &&
             (In->getTacID() < N->getTacID()))))) {
        WarNode = In;
        this->MapWAR[N->getTacID()].insert(WarNode->getTacID());
      }
    }
  }
  return WarNode;
}

// ---------------------------------------------
Node *CDAG::findRAWDataRace(Node *N, Node::NodeListType NL) {
  Node *RawNode = nullptr;
  for (auto Op : NL) {
    if (!Op->isStoreNodeOp()) {
      continue;
    }
    auto OutName = Op->getOutputInfoName();
    for (auto In : N->getInputs()) {
      if (((OutName == In->getRegisterValue()) &&
           (Op->getTacID() < N->getTacID()))) {
        RawNode = Op;
      }
    }
  }
  if (RawNode != nullptr) {
    this->MapRAW[RawNode->getTacID()].insert(N->getTacID());
  }
  return RawNode;
}

// ---------------------------------------------
Node *CDAG::insertTac(TAC T, Node::NodeListType L) {
  // Special cases are those Nodes which hold an assignment after the
  // operation
  if (T.getMVOP().isAssignment()) {
    // Assumption: a = b op c, c is always the "connector"
    auto N = Node::findOutputNode(T.getC()->getExprStr(), L);
    if (N == nullptr) {
      MACVETH_DEBUG("CDAG/NODE", "no output found = " + T.getC()->getExprStr());
      goto no_output;
    }
    MACVETH_DEBUG("CDAG/NODE", "output found = " + T.getA()->getExprStr());
    N->setOutputExpr(T.getA());
    N->setNodeType(Node::NODE_STORE);
    N->setTacID(T.getTacID());
    auto WarNode = findWARDataRace(N, this->NLOps);
    // auto RawNode = findRAWDataRace(N, this->NLOps);
    if (WarNode != nullptr) {
      MACVETH_DEBUG("CDAG/NODE", "WAR found = " + WarNode->getRegisterValue() +
                                     "; " + WarNode->getSchedInfoStr());
      N->setFreeSchedInfo(WarNode->getSchedInfo().FreeSched + 1);
    }
    // if (RawNode != nullptr) {
    //   MACVETH_DEBUG("CDAG/NODE", "RAW found = " + RawNode->getRegisterValue()
    //   +
    //                                  "; " + RawNode->getSchedInfoStr());
    //   N->setFreeSchedInfo(RawNode->getSchedInfo().FreeSched + 1);
    // }
    return nullptr;
  }
no_output:
  auto NewNode = new Node(T, L);
  // auto WarNode = findWARDataRace(NewNode, this->NLOps);
  auto RawNode = findRAWDataRace(NewNode, this->NLOps);

  PlcmntAlgo::computeFreeSchedule(NewNode);
  NewNode->setFreeSchedInfo(
      std::max(NewNode->getSchedInfo().FreeSched,
               (RawNode == nullptr) ? 0 : RawNode->getSchedInfo().FreeSched));
  MACVETH_DEBUG("CDAG/NODE", "new node = " + T.toString() + "; " +
                                 NewNode->getSchedInfoStr());
  this->NLOps.push_back(NewNode);
  return NewNode;
}

// ---------------------------------------------
CDAG CDAG::createCDAGfromTAC(const TacListType &TL) {
  // Literally create a new CDAG
  CDAG G;
  // Restarting numbering of nodes
  Node::restart();
  for (TAC T : TL) {
    // TACs are of the form a = b op c, so if we create a Node for each TAC
    // we are basically creating NODE_OP Nodes. This way, when we connect
    // this new node to the rest of the CDAG, we are basically looking for
    // connections between the inputs and outputs. Actually we are looking
    // for outputs of of already connected Nodes that match the input of this
    // new NODE_OP. Looking for inputs
    // Node *PrevNode = G->insertTac(T, G->getNodeListOps());
    G.insertTac(T, G.getNodeListOps());
  }

  if (MVOptions::Debug) {
    for (auto R : G.MapRAW) {
      MACVETH_DEBUG("CDAG", "RAW for TAC = " + std::to_string(R.first));
      for (auto RAW : R.second) {
        MACVETH_DEBUG("CDAG", "\t TAC = " + std::to_string(RAW));
      }
    }

    for (auto R : G.MapWAR) {
      MACVETH_DEBUG("CDAG", "WAR for TAC = " + std::to_string(R.first));
      for (auto RAW : R.second) {
        MACVETH_DEBUG("CDAG", "\t TAC = " + std::to_string(RAW));
      }
    }
  }
  return G;
}
