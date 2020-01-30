/**
 * File              : CDAG.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:35 MST
 * Last Modified Date: Mér 15 Xan 2020 11:28:22 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/CDAG.h"
#include "include/Vectorization/SIMD/AVX2Gen.h"
#include "include/Vectorization/VectorIR.h"
#include "clang/AST/Expr.h"
#include <llvm-10/llvm/Support/ErrorHandling.h>
#include <llvm-10/llvm/Support/raw_ostream.h>
#include <stdexcept>

namespace macveth {

// ---------------------------------------------
int computeMaxDepth(Node *N) {
  /// This algorithm is not efficient if the CDAG is big. At some point more
  /// intelligent strategies should be used instead.
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

// FIXME ---------------------------------------------
std::list<VectorIR::VectorOP> getVectorOpFromCDAG(CDAG *G) {
  std::list<VectorIR::VectorOP> VList;
  Node *VLoadA[64];
  Node *VLoadB[64];
  Node *VOps[64];
  int Cursor = 0;

  /// Working with a copy
  Node::NodeListType NL(G->getNodeListOps());
  // The sorting of the list is done by comparing the FreeSched value and the
  // StmtID, which is sequential when creating each node. This way we have a
  // chronological order of the nodes.
  NL.sort([](Node *lhs, Node *rhs) { return *lhs < *rhs; });

  Utils::printDebug("CDAG", "Printing nodes");
  for (Node *N : NL) {
    Utils::printDebug("NODE", N->toString());
  }

  // Until the list is over
repeat:
  // int VL = AVX->getMaxWidth();
  // FIXME
  int VL = 4;
  while (!NL.empty()) {

    VOps[Cursor] = NL.front();
    if ((Cursor > 0) &&
        (VOps[Cursor]->getValue().compare(VOps[Cursor - 1]->getValue()))) {
      Utils::printDebug("CDAG", "Full OPS of same type = " +
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
void setPlcmtFromFile(CDAG *G) {
  std::ifstream CF;
  CF.open(Utils::getExePath() + MVOptions::InCDAGFile, std::ios_base::in);
  if (CF.is_open()) {
    std::string L;
    for (auto N : G->getNodeListOps()) {
      while (getline(CF, L)) {
        if (L.rfind("//", 0) == 0) {
          // This is a comment, skip it
          continue;
        } else {
          if (!(L.find_first_not_of("0123456789") == std::string::npos)) {
            llvm::outs() << "CDAG input file is not correct!\n";
            llvm::llvm_unreachable_internal();
          }
          // Clang does not allow to perform any type of exception handling in
          // order to minimize the size of the executable (same reasoning for
          // not using RTTI), that is why we perform the previous check. On any
          // case, user should be aware of the format of the file: comments
          // starting with // are allowed, but each non-comment row must only
          // contain an integer value
          N->setPlcmt(std::stoi(L));
          break;
        }
      }
    }
  } else {
    llvm::llvm_unreachable_internal();
  }
  CF.close();
}

// ---------------------------------------------
SIMDGenerator::SIMDInfo CDAG::computeCostModel(CDAG *G, SIMDGenerator *SG) {
  VectorIR::clearMappigs();
  if (MVOptions::InCDAGFile != "") {
    setPlcmtFromFile(G);
  }

  // Execute greedy algorithm
  std::list<VectorIR::VectorOP> VList = getVectorOpFromCDAG(G);

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
// Node *findOutputNode(std::string Out, Node::NodeListType NL) {
//  for (auto N : NL) {
//    if (N->getOutputInfoName() == Out) {
//      return N;
//    }
//  }
//  return nullptr;
//}

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
