/**
 * File					 : src/CDAG.cpp
 * Author				 : Marcos Horro
 * Date					 : Wed 08 Apr 2020 01:00 +02:00
 *
 * Last Modified : Thu 11 Jun 2020 11:13 +02:00
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

#include "include/CDAG.h"
#include "include/PlcmntAlgo.h"
#include "include/Utils.h"
#include "include/Vectorization/SIMD/SIMDGeneratorFactory.h"
#include "include/Vectorization/VectorIR.h"
#include "clang/AST/Expr.h"
#include <llvm-10/llvm/Support/ErrorHandling.h>
#include <llvm-10/llvm/Support/raw_ostream.h>
#include <stdexcept>

using namespace macveth;

//---------------------------------------------
std::list<VectorIR::VectorOP> greedyOpsConsumer(Node::NodeListType NL,
                                                SIMDGenerator *SG) {
  std::list<VectorIR::VectorOP> VList;
  Node *VLoadA[64];
  Node *VLoadB[64];
  Node *VOps[64];
  int Cursor = 0;
repeat:
  bool IsUnary = false;
  // Consume nodes
  int VL = 4;
  // This is where magic should happen
  while (!NL.empty()) {
    if ((Cursor > 0) &&
        (NL.front()->getScop()[0] != VOps[Cursor - 1]->getScop()[0])) {
      break;
    }
    // Consume the first one
    VOps[Cursor] = NL.front();
    // Get vector length
    VL = SG->getMaxVectorSize(VOps[Cursor]->getDataType());
    // NOTE: how do you solve this? I mean, for reductions, for instance,
    // you will have different Plcmnts, something like: 1,2,3,4; but this
    // algorithm should be able to select them. So maybe when selecting
    // operations you should only look at the type of operations, but not at
    // the Plcmnt or schedule.
    // 06/03/2020: I think this is solved by tackling reductions as a non
    // standard case and, then, detecting them before going onto the general
    // case.
    // 06/25/2020: not sure if reductions should be handled that way...
    if ((Cursor > 0) &&
        (VOps[Cursor]->getValue().compare(VOps[Cursor - 1]->getValue()))) {
      Utils::printDebug("CDAG", "Full OPS of same type and placement = " +
                                    VOps[Cursor - 1]->getValue() + "; (" +
                                    VOps[Cursor]->getValue() + ")");
      break;
    }
    NL.erase(NL.begin());
    if (++Cursor == VL) {
      break;
    }
  }

  // Compute memory operands for the operations fetched above
  int i = 0;
  while ((i < Cursor) && (VOps[i] != nullptr)) {
    auto Aux = VOps[i]->getInputs();
    VLoadA[i] = Aux[0];
    if (Aux[1] != nullptr) {
      VLoadB[i] = Aux[1];
      Aux.erase(Aux.begin());
      IsUnary = false;
    } else {
      IsUnary = true;
    }
    i++;
  }

  // Debugging
  for (int n = 0; n < Cursor; ++n) {
    std::string B = IsUnary ? "" : VLoadB[n]->getRegisterValue() + "; ";
    Utils::printDebug("CDAG", VOps[n]->getRegisterValue() + " = " +
                                  VLoadA[n]->getRegisterValue() + " " +
                                  VOps[n]->getValue() + " " + B +
                                  VOps[n]->getSchedInfoStr());
  }

  if (Cursor != 0) {
    // Compute the vector cost
    auto NewVectInst =
        VectorIR::VectorOP(Cursor, VOps, VLoadA, (IsUnary) ? nullptr : VLoadB);
    // if (NewVectInst.isSequential()) {
    //   // TODO: Emit TAC "as it is"
    //   Utils::printDebug("CDAG", "is sequential");
    // }
    VList.push_back(NewVectInst);
  }

  // Repeat process if list not empty
  if (!NL.empty()) {
    Cursor = 0;
    for (int i = 0; i < VL; ++i) {
      VOps[i] = VLoadA[i] = VLoadB[i] = nullptr;
    }
    goto repeat;
  }
  return VList;
}

//---------------------------------------------
std::list<VectorIR::VectorOP> getVectorOpFromCDAG(Node::NodeListType NList,
                                                  SIMDGenerator *SG) {
  // Returning list
  std::list<VectorIR::VectorOP> VList;

  // Working with a copy
  Node::NodeListType NL(NList);

  //
  Utils::printDebug("CDAG", "Detecting reductions");
  Node::NodeListType NRedux = PlcmntAlgo::detectReductions(&NL);
  if (NRedux.size() == 0) {
    Utils::printDebug("CDAG", "No reductions detected");
  }

  Utils::printDebug("CDAG", "General case");
  VList.splice(VList.end(), greedyOpsConsumer(NL, SG));
  Utils::printDebug("CDAG", "Reductions case");
  VList.splice(VList.end(), greedyOpsConsumer(NRedux, SG));

  return VList;
}

// ---------------------------------------------
SIMDGenerator::SIMDInfo CDAG::computeCostModel(CDAG *G, SIMDGenerator *SG) {
  // Be clean
  // VectorIR::clearMappigs();

  // Set placement according to the desired algorithm
  Node::NodeListType NL = PlcmntAlgo::sortGraph(G->getNodeListOps());

  // Debugging
  for (auto N : NL) {
    Utils::printDebug("CDAG", N->toString());
  }

  // Execute greedy algorithm
  std::list<VectorIR::VectorOP> VList = getVectorOpFromCDAG(NL, SG);

  // Order Vector Operations by TAC ID
  VList.sort([](VectorIR::VectorOP V1, VectorIR::VectorOP V2) {
    return V1.Order < V2.Order;
  });

  // If debug enabled, it will print the VectorOP obtained
  for (auto V : VList) {
    Utils::printDebug("VectorIR", V.toString());
  }

  // Generate the SIMD list
  SIMDGenerator::SIMDInstListType S = SG->getSIMDfromVectorOP(VList);

  return SG->computeSIMDCost(S);
}

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
      Utils::printDebug("CDAG/NODE",
                        "no output found = " + T.getC()->getExprStr());
      goto no_output;
    }
    Utils::printDebug("CDAG/NODE", "output found = " + T.getA()->getExprStr());
    N->setOutputExpr(T.getA());
    N->setNodeType(Node::NODE_STORE);
    N->setTacID(T.getTacID());
    auto WarNode = findWARDataRace(N, this->NLOps);
    auto RawNode = findRAWDataRace(N, this->NLOps);
    if (WarNode != nullptr) {
      Utils::printDebug("CDAG/NODE",
                        "WAR found = " + WarNode->getRegisterValue() + "; " +
                            WarNode->getSchedInfoStr());
      N->setFreeSchedInfo(WarNode->getSchedInfo().FreeSched + 1);
    }
    return nullptr;
  }
no_output:
  auto NewNode = new Node(T, L);
  auto WarNode = findWARDataRace(NewNode, this->NLOps);
  auto RawNode = findRAWDataRace(NewNode, this->NLOps);

  PlcmntAlgo::computeFreeSchedule(NewNode);
  NewNode->setFreeSchedInfo(
      std::max(NewNode->getSchedInfo().FreeSched,
               (RawNode == nullptr) ? 0 : RawNode->getSchedInfo().FreeSched));
  Utils::printDebug("CDAG/NODE", "new node = " + T.toString() + "; " +
                                     NewNode->getSchedInfoStr());
  this->NLOps.push_back(NewNode);
  return NewNode;
}

// ---------------------------------------------
CDAG *CDAG::createCDAGfromTAC(TacListType TL) {
  // Literally create a new CDAG
  CDAG *G = new CDAG();
  // Restarting numbering of nodes
  Node::restart();
  for (TAC T : TL) {
    // TACs are of the form a = b op c, so if we create a Node for each TAC
    // we are basically creating NODE_OP Nodes. This way, when we connect
    // this new node to the rest of the CDAG, we are basically looking for
    // connections between the inputs and outputs. Actually we are looking
    // for outputs of of already connected Nodes that match the input of this
    // new NODE_OP. Looking for inputs
    Node *PrevNode = G->insertTac(T, G->getNodeListOps());
  }

  for (auto R : G->MapRAW) {
    Utils::printDebug("CDAG", "RAW for TAC = " + std::to_string(R.first));
    for (auto RAW : R.second) {
      Utils::printDebug("CDAG", "\t TAC = " + std::to_string(RAW));
    }
  }

  for (auto R : G->MapWAR) {
    Utils::printDebug("CDAG", "WAR for TAC = " + std::to_string(R.first));
    for (auto RAW : R.second) {
      Utils::printDebug("CDAG", "\t TAC = " + std::to_string(RAW));
    }
  }

  return G;
}
