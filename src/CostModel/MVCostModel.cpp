/*
 * File					 : src/CostModel/MVCostModel.cpp
 * Author				 : Marcos Horro
 * Date					 : Tue 20 Oct 2020 10:49 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 04:26 +02:00
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

#include "include/CostModel/MVCostModel.h"
#include "include/CostModel/SIMDCostInfo.h"
#include "include/PlcmntAlgo.h"
#include "include/Vectorization/SIMD/SIMDBackEndFactory.h"

// ---------------------------------------------
InstCostInfo getOperationCost(MVOp Op, std::string T) {
  InstCostInfo C(CostTable::getMVOPRow(Op.getTableMVOPstr(T)));
  return C;
}

// ---------------------------------------------
InstCostInfo getOperandCost(MVExpr *A, std::string T) {
  InstCostInfo C;
  if (A->needsToBeLoaded()) {
    C = InstCostInfo(CostTable::getMVOPRow("LD_" + Utils::toUppercase(T)));
  }
  return C;
}

// ---------------------------------------------
InstCostInfo MVCostModel::computeCostForNode(Node *N) {
  return getOperationCost(N->getOutputInfo().MVOP, N->getDataType());
}

// ---------------------------------------------
InstCostInfo MVCostModel::computeCostForNodeList(int VL,
                                                 Node::NodeListType &NL) {
  InstCostInfo TotalCost;
  // FIXME: Dunno how to iterate vectors in C++...
  // for (const auto &N : NL) {
  // TotalCost += computeCostForNode(N);
  for (int i = 0; i < VL; ++i) {
    TotalCost += computeCostForNode(NL[i]);
  }
  return TotalCost;
}

// ---------------------------------------------
InstCostInfo MVCostModel::computeCostForStmtWrapper(StmtWrapper *S) {
  InstCostInfo Tot;
  if (S->isLoop()) {
    for (auto S : S->getListStmt()) {
      Tot += computeCostForStmtWrapper(S);
    }
  } else {
    for (auto T : S->getTacList()) {
      Tot += getOperationCost(T.getMVOP(), T.getA()->getTypeStr());
      Tot += getOperandCost(T.getA(), T.getA()->getTypeStr());
      Tot += getOperandCost(T.getB(), T.getB()->getTypeStr());
      if (T.getC() != nullptr) {
        Tot += getOperandCost(T.getC(), T.getC()->getTypeStr());
      }
    }
  }
  return Tot;
}

// ---------------------------------------------
InstCostInfo
MVCostModel::computeCostForStmtWrapperList(std::list<StmtWrapper *> &SL) {
  InstCostInfo TotalCost;
  for (auto S : SL) {
    TotalCost += computeCostForStmtWrapper(S);
  }
  return TotalCost;
}

//---------------------------------------------
InstCostInfo MVCostModel::computeVectorOPCost(VectorIR::VectorOP V,
                                              SIMDBackEnd *SG) {
  InstCostInfo C;
  auto S = SG->getSIMDfromVectorOP(V);
  for (auto I : S) {
    if (I.isSequential()) {
      C.Latency += 10;
      continue;
    }
    C += InstCostInfo(CostTable::getIntrinRow(I.FuncName));
  }
  return C;
}

// ---------------------------------------------
SIMDInfo MVCostModel::generateSIMDInfoReport(SIMDBackEnd::SIMDInstListType S) {
  std::map<std::string, InstCostInfo> CostOp;
  std::map<std::string, long> NumOp;
  std::list<std::string> L;
  long TotCost = 0;
  for (auto I : S) {
    CostOp[I.FuncName] = InstCostInfo(CostTable::getIntrinRow(I.FuncName));
    NumOp[I.FuncName]++;
    TotCost += CostOp[I.FuncName].Latency;
  }
  SIMDInfo SI(S, CostOp, NumOp, TotCost);
  if (TotCost > 0) {
    SI.Vectorize = VectorizationType::PARTIAL;
  }
  return SI;
}

//---------------------------------------------
std::list<VectorIR::VectorOP>
MVCostModel::greedyOpsConsumer(Node::NodeListType &NL, SIMDBackEnd *SG) {
  std::list<VectorIR::VectorOP> VList;
  auto CopyNL = NL;
  Node::NodeListType VLoadA;
  Node::NodeListType VLoadB;
  Node::NodeListType VOps;
  VLoadA.reserve(8);
  VLoadB.reserve(8);
  VOps.reserve(8);
  int Cursor = 0;
repeat:
  bool IsUnary = false;
  // Consume nodes
  int VL = 4;
  // This is where magic should happen
  while (!CopyNL.empty()) {
    if ((Cursor > 0) &&
        ((CopyNL.front()->getScop()[0] != VOps[Cursor - 1]->getScop()[0]) ||
         (CopyNL.front()->belongsToAReduction() !=
          VOps[Cursor - 1]->belongsToAReduction()))) {
      break;
    }

    // Consume the first one
    VOps[Cursor] = CopyNL.front();

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
        (VOps[Cursor]->getValue() != VOps[Cursor - 1]->getValue())) {
      Utils::printDebug("MVCostModel",
                        "Full OPS of same type and placement = " +
                            VOps[Cursor - 1]->getValue() + "; (" +
                            VOps[Cursor]->getValue() +
                            "); cursor = " + std::to_string(Cursor));
      break;
    }
    CopyNL.erase(CopyNL.begin());
    if (++Cursor == VL) {
      Utils::printDebug("MVCostModel", "Full vector");
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
      VLoadB[0] = nullptr;
    }
    i++;
  }

  // Debugging
  for (int n = 0; n < Cursor; ++n) {
    auto B = IsUnary ? "" : VLoadB[n]->getRegisterValue() + "; ";
    Utils::printDebug("MVCostModel", VOps[n]->getRegisterValue() + " = " +
                                         VLoadA[n]->getRegisterValue() + " " +
                                         VOps[n]->getValue() + " " + B +
                                         VOps[n]->getSchedInfoStr());
  }

  if (Cursor != 0) {
    // Compute the vector cost
    auto NewVectInst = VectorIR::VectorOP(Cursor, VOps, VLoadA, VLoadB);
    auto CostVect = computeVectorOPCost(NewVectInst, SG);
    auto CostNodes = computeCostForNodeList(Cursor, VOps);
    CostNodes += computeCostForNodeList(Cursor, VLoadA);
    if (!IsUnary) {
      CostNodes += computeCostForNodeList(Cursor, VLoadB);
    }
    Utils::printDebug("MVCostModel", "Cost Vect = " + CostVect.toString());
    Utils::printDebug("MVCostModel", "Cost Nodes = " + CostNodes.toString());
    auto DoVectorize = (CostVect <= CostNodes);
    if ((DoVectorize) ||
        (MVOptions::SIMDCostModel == MVSIMDCostModel::UNLIMITED)) {
      VList.push_back(NewVectInst);
    } else {
      // FIXME: Undo newvectorinst?
    }
  }

  // Repeat process if list not empty
  if (!CopyNL.empty()) {
    Cursor = 0;
    for (int i = 0; i < VL; ++i) {
      VOps[i] = VLoadA[i] = VLoadB[i] = nullptr;
    }
    goto repeat;
  }
  return VList;
}

//---------------------------------------------
std::list<VectorIR::VectorOP>
MVCostModel::getVectorOpFromCDAG(Node::NodeListType &NList, SIMDBackEnd *SG) {
  // Returning list
  std::list<VectorIR::VectorOP> VList;

  // Working with a copy
  Node::NodeListType NL(NList);

  // Detecting reductions
  Utils::printDebug("MVCostModel", "Detecting reductions");
  Node::NodeListType NRedux = PlcmntAlgo::detectReductions(&NL);
  if (NRedux.size() == 0) {
    Utils::printDebug("MVCostModel", "No reductions detected");
  }

  Utils::printDebug("MVCostModel", "General case");
  VList.splice(VList.end(), greedyOpsConsumer(NL, SG));
  Utils::printDebug("MVCostModel", "Reductions case");
  VList.splice(VList.end(), greedyOpsConsumer(NRedux, SG));

  // Order Vector Operations by TAC ID
  VList.sort([](VectorIR::VectorOP V1, VectorIR::VectorOP V2) {
    return V1.Order < V2.Order;
  });

  return VList;
}

// ---------------------------------------------
SIMDInfo MVCostModel::computeCostModel(std::list<StmtWrapper *> SL,
                                       SIMDBackEnd *SG) {

  // Get all the TAC from all the Stmts
  TacListType TL;
  for (auto SWrap : SL) {
    for (auto S : SWrap->getTacList()) {
      S.printTAC();
      TL.push_back(S);
    }
  }

  // Creating the CDAG
  auto G = CDAG::createCDAGfromTAC(TL);

  // Set placement according to the desired algorithm
  auto NL = PlcmntAlgo::sortGraph(G->getNodeListOps());

  // Debugging
  for (auto N : NL) {
    Utils::printDebug("MVCostModel", N->toString());
  }

  // Setting CDAG
  SG->setCDAG(G);

  // Execute greedy algorithm
  auto VList = getVectorOpFromCDAG(NL, SG);

  // If debug enabled, it will print the VectorOP obtained
  for (auto V : VList) {
    Utils::printDebug("VectorIR", V.toString());
  }

  // SIMD cost model used backend to generate intrinsics so registers are
  // polluted, need to clean them
  SG->clearMappings();

  // Generate the SIMD list
  auto SIMD = SG->getSIMDfromVectorOP(VList);

  return MVCostModel::generateSIMDInfoReport(SIMD);
}
