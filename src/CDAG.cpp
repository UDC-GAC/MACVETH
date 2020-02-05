/**
 * File              : CDAG.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:35 MST
 * Last Modified Date: Mér 15 Xan 2020 11:28:22 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/CDAG.h"
#include "include/PlcmntAlgo.h"
#include "include/Vectorization/SIMD/AVX2Gen.h"
#include "include/Vectorization/VectorIR.h"
#include "clang/AST/Expr.h"
#include <llvm-10/llvm/Support/ErrorHandling.h>
#include <llvm-10/llvm/Support/raw_ostream.h>
#include <stdexcept>

namespace macveth {

// ---------------------------------------------
int computeMaxDepth(Node *N) {
  // This algorithm is not efficient if the CDAG is big. At some point more
  // intelligent strategies should be used instead.
  if (N->getInputNum() == 0) {
    return 0;
  } else {
    int Max = 0;
    for (Node *T : N->getInputs()) {
      Max = std::max(Max, computeMaxDepth(T));
    }
    return 1 + Max;
  }
}

//---------------------------------------------
std::list<VectorIR::VectorOP> getVectorOpFromCDAG(Node::NodeListType NList,
                                                  SIMDGenerator *SG) {
  // Returning list
  std::list<VectorIR::VectorOP> VList;
  Node *VLoadA[64];
  Node *VLoadB[64];
  Node *VOps[64];
  int Cursor = 0;

  // Working with a copy
  Node::NodeListType NL(NList);

  // Debugging options
  for (Node *N : NL) {
    Utils::printDebug("CDAG", N->toString());
  }

  // Until the list is empty
repeat:
  // Get vector length
  int VL = SG->getMaxVectorSize();
  // Consume nodes
  while (!NL.empty()) {
    // Consume the first one
    VOps[Cursor] = NL.front();
    if ((Cursor > 0) &&
        (VOps[Cursor]->getSchedInfo().Plcmnt !=
         VOps[Cursor - 1]->getSchedInfo().Plcmnt) &&
        (VOps[Cursor]->getValue().compare(VOps[Cursor - 1]->getValue()))) {
      Utils::printDebug("CDAG", "Full OPS of same type and placement = " +
                                    VOps[Cursor - 1]->getValue());
      break;
    }
    NL.pop_front();
    if (++Cursor == VL) {
      Utils::printDebug("CDAG", "VL OPS achieved");
      break;
    }
  }
  bool IsPartial = (Cursor < VL);
  if (IsPartial) {
    Utils::printDebug("CDAG", "isPartial: " + std::to_string(Cursor));
  }

  // Compute memory operands for the operations fetched above
  int i = 0;
  while ((i < Cursor) && (VOps[i] != nullptr)) {
    Node::NodeListType Aux = VOps[i]->getInputs();
    VLoadA[i] = Aux.front();
    Aux.pop_front();
    VLoadB[i++] = Aux.front();
    Aux.pop_front();
  }

  // Debugging
  for (int n = 0; n < Cursor; ++n) {
    Utils::printDebug("CDAG", VOps[n]->getRegisterValue() + " = " +
                                  VLoadA[n]->getRegisterValue() + " " +
                                  VOps[n]->getValue() + " " +
                                  VLoadB[n]->getRegisterValue());
  }

  // Compute the vector cost
  VList.push_back(VectorIR::VectorOP(Cursor, VOps, VLoadA, VLoadB));

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

// ---------------------------------------------
SIMDGenerator::SIMDInfo CDAG::computeCostModel(CDAG *G, SIMDGenerator *SG) {
  // Be clean
  VectorIR::clearMappigs();

  // Set placement according to the desired algorithm
  Node::NodeListType NL = PlcmntAlgo::sortGraph(G->getNodeListOps());

  // Execute greedy algorithm
  std::list<VectorIR::VectorOP> VList = getVectorOpFromCDAG(NL, SG);

  // If debug enabled, it will print the VectorOP obtained
  for (auto V : VList) {
    Utils::printDebug("VectorIR", V.toString());
  }

  // Generate the SIMD list
  SIMDGenerator::SIMDInstListType S = SG->getSIMDfromVectorOP(VList);

  return SG->computeSIMDCost(S);
}

// ---------------------------------------------
void CDAG::computeFreeSchedule(Node::NodeListType NL) {
  for (Node *N : NL) {
    N->setFreeSchedInfo(computeMaxDepth(N));
  }
}

// ---------------------------------------------
void CDAG::computeFreeSchedule(CDAG *C) {
  CDAG::computeFreeSchedule(C->getNodeListOps());
}

// ---------------------------------------------
void replaceOutput(TAC T, Node *N) {
  N->setOutputName(T.getA()->getExprStr());
  N->setNodeType(Node::NODE_STORE);
}

// ---------------------------------------------
Node *CDAG::insertTac(TAC T, Node *PrevNode, Node::NodeListType L) {
  // if ((T.getMVOP().isAssignment()) && (PrevNode != nullptr)) {
  if (T.getMVOP().isAssignment()) {
    // Assumption: a = b op c, c is always the "connector"
    if (Node::findOutputNode(T.getC()->getExprStr(), L) == NULL) {
      goto no_output;
    }
    replaceOutput(T, Node::findOutputNode(T.getC()->getExprStr(), L));
    return nullptr;
  }
no_output:
  Node *NewNode = new Node(T, L);
  this->NLOps.push_back(NewNode);
  return NewNode;
}

// ---------------------------------------------
CDAG *CDAG::createCDAGfromTAC(TacListType TL) {
  CDAG *G = new CDAG();
  Node::restart();
  Node *PrevNode = nullptr;
  for (TAC T : TL) {
    // TACs are of the form a = b op c, so if we create a Node for each TAC
    // we are basically creating NODE_OP Nodes. This way, when we connect
    // this new node to the rest of the CDAG, we are basically looking for
    // connections between the inputs and outputs. Actually we are looking
    // for outputs of of already connected Nodes that match the input of this
    // new NODE_OP. Looking for inputs
    PrevNode = G->insertTac(T, PrevNode, G->getNodeListOps());
  }
  return G;
}
} // namespace macveth
