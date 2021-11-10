// MACVETH - MVCostModel.cpp
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

#include "include/CostModel/MVCostModel.h"
#include "include/CostModel/SIMDCostInfo.h"
#include "include/Debug.h"
#include "include/PlcmntAlgo.h"
#include "include/Vectorization/SIMD/SIMDBackEndFactory.h"

// ---------------------------------------------
InstCostInfo getOperationCost(const MVOp &Op, const std::string &T) {
  if (Op.isAssignment()) {
    return InstCostInfo(CostTable::getMVOPRow("ST_" + Utils::toUppercase(T)),
                        InstType::MVOP);
  }
  return InstCostInfo(CostTable::getMVOPRow(Op.getTableMVOPstr(T)),
                      InstType::MVOP);
}

// ---------------------------------------------
InstCostInfo getOperandCost(MVExpr *A, std::string T) {
  InstCostInfo C;
  if (A->needsToBeLoaded()) {
    return InstCostInfo(CostTable::getMVOPRow("LD_" + Utils::toUppercase(T)),
                        InstType::MVOP);
  }
  return C;
}

// ---------------------------------------------
InstCostInfo MVCostModel::computeCostForNodeOp(Node *N) {
  return getOperationCost(N->getOutputInfo().MVOP, N->getDataType());
}

// ---------------------------------------------
InstCostInfo
MVCostModel::computeCostForNodeOpsList(int VL, const Node::NodeListType &NL) {
  InstCostInfo TotalCost;
  for (int i = 0; i < VL; ++i) {
    TotalCost += computeCostForNodeOp(NL[i]);
    // MACVETH_DEBUG("MVCostModel",
    //                   "Node = " + NL[i]->toStringShort() +
    //                       "; cost = " +
    //                       computeCostForNodeOp(NL[i]).toString());
  }
  return TotalCost;
}

// ---------------------------------------------
InstCostInfo
MVCostModel::computeCostForNodeOperandsList(int VL,
                                            const Node::NodeListType &NL) {
  InstCostInfo TotalCost;
  for (int i = 0; i < VL; ++i) {
    auto M = NL[i]->getMVExpr();
    if ((M == nullptr) || (M->isNotClang())) {
      continue;
    }
    auto T = M->getTypeStr();
    TotalCost += getOperandCost(M, T);
    // MACVETH_DEBUG("MVCostModel",
    //                   "Node = " + NL[i]->toStringShort() +
    //                       "; cost = " + getOperandCost(M, T).toString());
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
                                              SIMDBackEnd *Backend) {
  InstCostInfo C;
  auto SIMDList = Backend->getSIMDfromVectorOP(V);
  for (auto Ins : SIMDList) {
    if ((Ins.isSequential()) || (Ins.isTemplate())) {
      C.Latency += Ins.getCost();
      continue;
    }
    C += InstCostInfo(CostTable::getIntrinRow(Ins.FuncName), InstType::SIMDOp);
    MACVETH_DEBUG("MVCostModel",
                  "SIMD inst = " + Ins.FuncName + " ( " + Ins.render() +
                      "); cost = " +
                      InstCostInfo(CostTable::getIntrinRow(Ins.FuncName),
                                   InstType::SIMDOp)
                          .toString());
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
    CostOp[I.FuncName] =
        InstCostInfo(CostTable::getIntrinRow(I.FuncName), InstType::SIMDOp);
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
MVCostModel::greedyOpsConsumer(Node::NodeListType &NL, SIMDBackEnd *Backend) {
  std::list<VectorIR::VectorOP> VList;
  auto CopyNL = NL;
  Node::NodeListType VLoadA;
  Node::NodeListType VLoadB;
  Node::NodeListType VOps;
  VLoadA.reserve(8);
  VLoadB.reserve(8);
  VOps.reserve(8);
  int Cursor = 0;
  std::set<int> SetTAC;
  std::set<int> SetFS;
repeat:
  bool IsUnary = false;
  // Consume nodes
  int VL = 4;
  // This is where magic should happen
  while (!CopyNL.empty()) {
    auto NextNode = CopyNL.front();
    SetTAC.insert(NextNode->getTacID());
    SetFS.insert(NextNode->getSchedInfo().FreeSched);

    // FIXME:
    // if ((SetTAC.size() > 1) && (SetFS.size() > 1) &&
    //     (!NextNode->belongsToAReduction())) {
    //   MVSkip("Skipping region... Not handable!!!", GotoEndScop);
    // }

    // if ((SetTAC.size() == 1) && (SetFS.size() == 1) &&
    //     ((Cursor > 0) && (NextNode->belongsToAReduction()) &&
    //      (!VOps[Cursor - 1]->belongsToAReduction()))) {
    //   MVSkip("Skipping region... PARTIAL REDUCTION? DUNNO", GotoEndScop);
    // }

    // if ((Cursor > 0) &&
    //     ((NextNode->getScop()[0] != VOps[Cursor - 1]->getScop()[0]) ||
    //      (NextNode->belongsToAReduction() !=
    //       VOps[Cursor - 1]->belongsToAReduction()))) {
    //   break;
    // }

    // Corner case...
    // if ((NextNode->getSchedInfo().UnrollFactor % 4) != (Cursor % 4)) {
    //   break;
    // }

    // Consume the first one
    VOps[Cursor] = NextNode;

    // Get vector length
    VL = Backend->getMaxVectorSize(VOps[Cursor]->getDataType());
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
      MACVETH_DEBUG("MVCostModel", "Full OPS of same type and placement = " +
                                       VOps[Cursor - 1]->getValue() + "; (" +
                                       VOps[Cursor]->getValue() +
                                       "); cursor = " + std::to_string(Cursor));
      break;
    }
    // Remove node from list
    CopyNL.erase(CopyNL.begin());
    if (++Cursor == VL) {
      MACVETH_DEBUG("MVCostModel", "Full vector");
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
    MACVETH_DEBUG("MVCostModel", VOps[n]->getRegisterValue() + " = " +
                                     VLoadA[n]->getRegisterValue() + " " +
                                     VOps[n]->getValue() + " " + B +
                                     VOps[n]->getSchedInfoStr());
  }

  // FIXME: to implement cost model here
  if (Cursor != 0) {
    // Compute the vector cost
    auto NewVectInst = VectorIR::VectorOP(Cursor, VOps, VLoadA, VLoadB);
    auto CostVect = computeVectorOPCost(NewVectInst, Backend);
    auto CostNodes = computeCostForNodeOpsList(Cursor, VOps);
    CostNodes += computeCostForNodeOperandsList(Cursor, VLoadA);
    if (!IsUnary) {
      CostNodes += computeCostForNodeOperandsList(Cursor, VLoadB);
    }
    // FIXME:
    auto DoVectorize = true;
    if ((DoVectorize) ||
        (MVOptions::SIMDCostModel == MVSIMDCostModel::UNLIMITED)) {
      VList.push_back(NewVectInst);
    } else {
      // FIXME: undo vectorized version or directly emit sequential code for
      // rest of the tree
      MVWarn("Region not vectorized");
    }
  }

  // Repeat process if list not empty
  if (!CopyNL.empty()) {
    Cursor = 0;
    for (int i = 0; i < VL; ++i) {
      VOps[i] = VLoadA[i] = VLoadB[i] = nullptr;
    }
    SetFS.clear();
    SetTAC.clear();
    goto repeat;
  }
  return VList;
}

//---------------------------------------------
std::list<VectorIR::VectorOP>
MVCostModel::getVectorOpFromCDAG(Node::NodeListType &NList,
                                 SIMDBackEnd *Backend) {
  // Returning list
  std::list<VectorIR::VectorOP> VList;

  // Working with a copy
  // Node::NodeListType NL(NList);

  // Detecting reductions
  MACVETH_DEBUG("MVCostModel", "Detecting reductions");
  // Node::NodeListType NRedux = PlcmntAlgo::detectReductions(&NL);
  // if (NRedux.size() == 0) {
  //   MACVETH_DEBUG("MVCostModel", "No reductions detected");
  // }

  // MACVETH_DEBUG("MVCostModel", "General case");
  // VList.splice(VList.end(), greedyOpsConsumer(NL, Backend));
  // MACVETH_DEBUG("MVCostModel", "Reductions case");
  // VList.splice(VList.end(), greedyOpsConsumer(NRedux, Backend));
  PlcmntAlgo::markReductions(&NList);
  VList.splice(VList.end(), greedyOpsConsumer(NList, Backend));

  // Order Vector Operations by TAC ID
  VList.sort([](VectorIR::VectorOP V1, VectorIR::VectorOP V2) {
    return V1.Order < V2.Order;
  });

  return VList;
}

// ---------------------------------------------
SIMDInfo MVCostModel::computeCostModel(std::list<StmtWrapper *> SL,
                                       SIMDBackEnd *Backend) {

  // Get all the TAC from all the Stmts
  TacListType TL;
  for (auto SWrap : SL) {
    TL.splice(TL.end(), SWrap->getTacList());
  }

  // Creating the CDAG
  auto G = CDAG::createCDAGfromTAC(TL);

  // Set placement according to the desired algorithm
  auto NL = PlcmntAlgo::sortGraph(G.getNodeListOps());

  // Setting CDAG
  Backend->setCDAG(G);

  // Execute greedy algorithm
  auto VList = getVectorOpFromCDAG(NL, Backend);

  // Debugging
  if (MVOptions::Debug) {
    for (auto N : NL) {
      MACVETH_DEBUG("MVCostModel", N->toString());
    }
    for (auto V : VList) {
      MACVETH_DEBUG("VectorIR", V.toString());
    }
  }

  // FIXME: SIMD cost model used backend to generate intrinsics so registers are
  // polluted, need to clean them
  // Backend->clearMappings();

  // Generate the SIMD list
  auto SIMD = Backend->getSIMDfromVectorOP(VList);

  return MVCostModel::generateSIMDInfoReport(SIMD);
}
