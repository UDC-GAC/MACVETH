/**
 * File              : CDAG.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:35 MST
 * Last Modified Date: Xov 09 Xan 2020 22:56:03 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/CDAG.h"
#include "include/Vectorization/SIMD/AVX2Gen.h"
#include "include/Vectorization/SIMD/SIMDGenerator.h"
#include "include/Vectorization/SIMD/SIMDGeneratorFactory.h"
#include "include/Vectorization/VectorIR.h"
#include "clang/AST/Expr.h"

namespace macveth {

// ---------------------------------------------
void printDebug(std::string M, std::string S) {
  std::cout << "[" << M << " DEBUG] " << S << std::endl;
}

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

// ---------------------------------------------
int CDAG::computeCostModel(CDAG *G) {
  /// FIXME at some point the compiler should be able to recognize the
  /// architecture and ISA where it is compiling
  SIMDGenerator *AVX =
      SIMDGeneratorFactory::getBackend(SIMDGeneratorFactory::Arch::AVX2);
  std::list<VectorIR::VectorOP> VList;
  printDebug("CDAG", "Creating backend");

  Node *VLoadA[64];
  Node *VLoadB[64];
  Node *VOps[64];
  int Cursor = 0;
  int TotalCost = -1;

  /// Working with a copy
  Node::NodeListType NL(G->getNodeListOps());
  // The sorting of the list is done by comparing the FreeSched value and the
  // StmtID, which is sequential when creating each node. This way we have a
  // chronological order of the nodes.
  NL.sort([](Node *lhs, Node *rhs) { return *lhs < *rhs; });

  printDebug("CDAG", "Printing nodes");
  for (Node *N : NL) {
    N->printNode();
  }

  printDebug("CDAG", "Entering loop");

  // Until the list is over
repeat:
  // int VL = AVX->getMaxWidth();
  int VL = 4;
  printDebug("CDAG", std::to_string(VL));
  while (!NL.empty()) {
    VOps[Cursor] = NL.front();
    // printDebug("CDAG", VOps[Cursor]->getDataType());
    // VL /= (SIMDGenerator::SizeOf[VOps[Cursor]->getDataType()] << 3);
    if ((Cursor > 0) &&
        (VOps[Cursor]->getValue().compare(VOps[Cursor - 1]->getValue()))) {
      printDebug("CDAG",
                 "Full OPS of same type = " + VOps[Cursor - 1]->getValue());
      break;
    }
    NL.pop_front();
    if (++Cursor == VL) {
      printDebug("CDAG", "VL OPS achieved");
      break;
    }
  }
  bool IsPartial = (Cursor < VL);
  if (IsPartial) {
    printDebug("CDAG", "isPartial: " + std::to_string(Cursor));
  }
  // Compute memory operands for the operations fetched above
  int i = 0;
  while ((i < Cursor) && (VOps[i] != nullptr)) {
    Node::NodeListType Aux = VOps[i]->getInputs();
    VLoadA[i] = Aux.front();
    Aux.pop_front();
    VLoadB[i] = Aux.front();
    Aux.pop_front();
    i++;
  }

  for (int n = 0; n < Cursor; ++n) {
    printDebug("CDAG", VOps[n]->getRegisterValue() + " = " +
                           VLoadA[n]->getRegisterValue() + " " +
                           VOps[n]->getValue() + " " +
                           VLoadB[n]->getRegisterValue());
  }

  // Compute the vector cost
  // int CostVectorOperation =
  //    VectorIR::computeCostVectorOp(VL, VOps, VLoadA, VLoadB);
  VList.push_back(VectorIR::VectorOP(Cursor, VOps, VLoadA, VLoadB));

  // Repeat process if list not empty
  if (!NL.empty()) {
    Cursor = 0;
    for (int i = 0; i < VL; ++i) {
      VOps[i] = VLoadA[i] = VLoadB[i] = nullptr;
    }
    goto repeat;
  }

  SIMDGenerator::SIMDInstListType S = AVX->getSIMDfromVectorOP(VList);
  // std::list<std::string> Ins = AVX->renderSIMDasString(S);
  std::list<std::string> Ins = AVX->computeSIMDCost(S);
  for (auto I : Ins) {
    std::cout << I << std::endl;
  }

  return TotalCost;
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
void replaceOutput(TAC T, Node *N) { N->setOutputName(T.getA()->getExprStr()); }

// ---------------------------------------------
Node *CDAG::insertTac(TAC T, Node *PrevNode, Node::NodeListType L) {
  if ((T.getMVOP().isAssignment()) && (PrevNode != nullptr)) {
    replaceOutput(T, PrevNode);
    return nullptr;
  }
  Node *NewNode = new Node(T, L);
  this->NLOps.push_back(NewNode);
  return NewNode;
}

// ---------------------------------------------
CDAG *CDAG::createCDAGfromTAC(TacListType TL) {
  CDAG *G = new CDAG();
  Node *PrevNode = nullptr;
  for (TAC T : TL) {
    /// TACs are of the form a = b op c, so if we create a Node for each TAC
    /// we are basically creating NODE_OP Nodes. This way, when we connect
    /// this new node to the rest of the CDAG, we are basically looking for
    /// connections between the inputs and outputs. Actually we are looking
    /// for outputs of of already connected Nodes that match the input of this
    /// new NODE_OP. Looking for inputs
    PrevNode = G->insertTac(T, PrevNode, G->getNodeListOps());
  }
  return G;
}
} // namespace macveth
