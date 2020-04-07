/**
 * File              : CDAG.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:35 MST
 * Last Modified Date: Mér 04 Mar 2020 16:30:00 CET
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

  // Detect reductions
  // Node::NodeListType NRedux = PlcmntAlgo::detectReductions(&NL);
  // Utils::printDebug("CDAG", "Nodes of reductions found:");
  // for (Node *N : NRedux) {
  //  Utils::printDebug("CDAG", N->toString());
  //}

  // Until the list is empty
  Utils::printDebug("CDAG", "Greedy algorithm: ");
repeat:
  // Get vector length
  int VL = SG->getMaxVectorSize();
  // Consume nodes
  while (!NL.empty()) {
    // Consume the first one
    VOps[Cursor] = NL.front();
    // Utils::printDebug("CDAG", "Node selected =>\n" +
    // VOps[Cursor]->toString());
    // FIXME: how do you solve this? I mean, for reductions, for instance,
    // you will have different Plcmnts, something like: 1,2,3,4; but this
    // algorithm should be able to select them. So maybe when selecting
    // operations you should only look at the type of operations, but not at
    // the Plcmnt or schedule.
    //(VOps[Cursor]->getSchedInfo().Plcmnt !=
    // VOps[Cursor - 1]->getSchedInfo().Plcmnt) &&
    if ((Cursor > 0) &&
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
Node *CDAG::insertTac(TAC T, Node::NodeListType L) {
  // Special cases are those Nodes which hold an assignment after the operation
  if (T.getMVOP().isAssignment()) {
    // Assumption: a = b op c, c is always the "connector"
    Node *N = Node::findOutputNode(T.getC()->getExprStr(), L);
    if (N == NULL) {
      Utils::printDebug("CDAG/NODE",
                        "no output found = " + T.getC()->getExprStr());
      goto no_output;
    }
    Utils::printDebug("CDAG/NODE", "output found = " + T.getC()->getExprStr());
    N->setOutputName(T.getA()->getExprStr());
    N->setNodeType(Node::NODE_STORE);
    N->setTacID(T.getTacID());
    return nullptr;
  }
no_output:
  Node *NewNode = new Node(T, L);
  Utils::printDebug("CDAG/NODE", "new node = " + T.toString());
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
  return G;
}
} // namespace macveth
