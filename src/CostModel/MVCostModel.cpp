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
InstCostInfo MVCostModel::computeCostForNodeOpsList(int VL,
                                                    const NodeVectorT &NL) {
  InstCostInfo TotalCost;
  for (int i = 0; i < VL; ++i) {
    TotalCost += computeCostForNodeOp(NL[i]);
  }
  return TotalCost;
}

// ---------------------------------------------
InstCostInfo
MVCostModel::computeCostForNodeOperandsList(int VL, const NodeVectorT &NL) {
  InstCostInfo TotalCost;
  for (int i = 0; i < VL; ++i) {
    auto M = NL[i]->getMVExpr();
    if ((M == nullptr) || (M->isNotClang())) {
      continue;
    }
    auto T = M->getTypeStr();
    TotalCost += getOperandCost(M, T);
  }
  return TotalCost;
}

// ---------------------------------------------
InstCostInfo MVCostModel::computeCostForStmtWrapper(StmtWrapper *S) {
  InstCostInfo Tot;
  if (S->isLoop()) {
    for (auto S : S->getStmtVector()) {
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
MVCostModel::computeCostForStmtWrapperList(StmtWrapperVectorT &SL) {
  InstCostInfo TotalCost;
  for (auto S : SL) {
    TotalCost += computeCostForStmtWrapper(S);
  }
  return TotalCost;
}

//---------------------------------------------
InstCostInfo MVCostModel::computeVectorOPCost(VectorOP V) {
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
                          .toString() +
                      "; Order = " + std::to_string(Ins.MVSL.getOrder()));
  }
  return C;
}

// ---------------------------------------------
SIMDInfo
MVCostModel::generateSIMDInfoReport(SIMDBackEnd::SIMDInstListType &SIMD) {
  std::map<std::string, InstCostInfo> CostOp;
  std::map<std::string, long> NumOp;
  std::vector<std::string> L;
  long TotCost = 0;
  for (auto I : SIMD) {
    CostOp[I.FuncName] =
        InstCostInfo(CostTable::getIntrinRow(I.FuncName), InstType::SIMDOp);
    NumOp[I.FuncName]++;
    TotCost += CostOp[I.FuncName].Latency;
  }
  SIMDInfo SIMDInst(SIMD, CostOp, NumOp, TotCost);
  if (TotCost > 0) {
    SIMDInst.Vectorize = VectorizationType::PARTIAL;
  }
  return SIMDInst;
}

//---------------------------------------------
VectorOPListT MVCostModel::greedyOpsConsumer(NodeVectorT &NL) {
  VectorOPListT VList;
  auto CopyNL = NL;
  int Cursor = 0;
  NodeVectorT VLoadA(MaxVectorLength);
  NodeVectorT VLoadB(MaxVectorLength);
  NodeVectorT VOps(MaxVectorLength);
  do {
    // Consume nodes
    int VL = MaxVectorLength;
    // This is where magic should happen
    while (!CopyNL.empty()) {
      auto NextNode = CopyNL.front();
      if (Cursor == 0) {
        VL = Backend->getMaxVectorSize(NextNode->getDataType());
      }
      // Consume the first one
      VOps[Cursor] = NextNode;
      if ((Cursor > 0) &&
          (VOps[Cursor]->getValue() != VOps[Cursor - 1]->getValue())) {
        MACVETH_DEBUG("MVCostModel",
                      "Full OPS of same type and placement = " +
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
      } else {
        VLoadB[0] = nullptr;
      }
      i++;
    }

    // Debugging
    if (MVOptions::Debug) {
      for (int n = 0; n < Cursor; ++n) {
        MACVETH_DEBUG("MVCostModel", VOps[n]->getRegisterValue() + " = " +
                                         VLoadA[n]->getRegisterValue() + " " +
                                         VOps[n]->getValue() + " " +
                                         VLoadB[n]->getRegisterValue() + "; " +
                                         VOps[n]->getSchedInfoStr());
      }
    }

    // Generate new vector instruction
    if (Cursor != 0) {
      VList.push_back(VectorOP(Cursor, VOps, VLoadA, VLoadB));
    }

    // Repeat process if list not empty
    Cursor = 0;
    for (int i = 0; i < VL; ++i) {
      VOps[i] = VLoadA[i] = VLoadB[i] = nullptr;
    }
  } while (!CopyNL.empty());

  return VList;
}

//---------------------------------------------
using MapReductions = std::map<std::string, NodeVectorT>;
using VectorMapReductions = std::vector<std::pair<std::string, NodeVectorT>>;

//---------------------------------------------
VectorMapReductions mapReductions(const NodeVectorT &Reductions) {
  MapReductions Map;
  std::for_each(Reductions.begin(), Reductions.end(), [&Map](auto Node) {
    Map[Node->getRegisterValue()].push_back(Node);
  });
  VectorMapReductions VMap{std::make_move_iterator(Map.begin()),
                           std::make_move_iterator(Map.end())};
  std::sort(VMap.begin(), VMap.end(),
            [](auto P0, auto P1) { return P0.second[0] < P1.second[0]; });
  return VMap;
}

//---------------------------------------------
VectorOPListT MVCostModel::consumeReduction(NodeVectorT &Nodes) {
  VectorOPListT VList;
  NodeVectorT PrevOps;
  for (auto Node : Nodes) {
    for (auto NodeIn : Node->getInputs()) {
      if (NodeIn->getNodeType() == Node::NodeType::NODE_OP) {
        PrevOps.push_back(NodeIn);
      }
    }
  }
  PrevOps.insert(PrevOps.end(), Nodes.begin(), Nodes.end());
  return greedyOpsConsumer(PrevOps);
}

//---------------------------------------------
VectorOPListT MVCostModel::bottomUpConsumer(NodeVectorT &Nodes) {
  VectorOPListT VList;
  auto FullMapReductions = mapReductions(Nodes);
  MACVETH_DEBUG("MVCostModel", "=== BOTTOM UP CONSUMER ===");
  int WindowsSize = MVOptions::ReduxWinSize;
  if (WindowsSize <= 0) {
    WindowsSize = FullMapReductions.size();
  }
  while (FullMapReductions.size() > 0) {
    unsigned long PackingSize = MaxVectorLength;
    MACVETH_DEBUG(
        "MVCostModel",
        "===  Window size = " + std::to_string(WindowsSize) +
            "; FullMap = " + std::to_string(FullMapReductions.size()));
    int End = std::min((int)FullMapReductions.size(), WindowsSize);
    VectorMapReductions MapReductions(&FullMapReductions[0],
                                      &FullMapReductions[End]);
    FullMapReductions.erase(FullMapReductions.begin(),
                            FullMapReductions.begin() + End);
    for (const auto &[Key, List] : MapReductions) {
      MACVETH_DEBUG("MVCostModel", Key + " = " + std::to_string(List.size()));
    }
    NodeVectorT NewPacking;
    while (PackingSize > 1) {
      for (auto &[Key, List] : MapReductions) {
        MACVETH_DEBUG("MVCostModel",
                      "PackingSize = " + std::to_string(PackingSize) +
                          "; Key = " + Key +
                          "; Size = " + std::to_string(List.size()));
        while ((List.size() >= PackingSize) &&
               (NewPacking.size() < MaxVectorLength)) {
          MACVETH_DEBUG(
              "MVCostModel",
              "PackingSize = " + std::to_string(PackingSize) +
                  "; Key = " + Key + "; Size = " + std::to_string(List.size()) +
                  "; NewPacking = " + std::to_string(NewPacking.size()));
          NodeVectorT NewRedux(&List[0], &List[PackingSize]);
          NewPacking.insert(NewPacking.end(), NewRedux.begin(), NewRedux.end());
          List.erase(List.begin(), List.begin() + PackingSize);
        }
        if (NewPacking.size() == MaxVectorLength) {
          MACVETH_DEBUG("MVCostModel", "MaxVectorLength = consuming reduction");
          std::sort(NewPacking.begin(), NewPacking.end(),
                    [](Node *P0, Node *P1) { return *P0 < *P1; });
          VList.splice(VList.end(), consumeReduction(NewPacking));
          NewPacking.clear();
        }
      }
      PackingSize /= 2;
    }
    if (NewPacking.size() > 0) {
      MACVETH_DEBUG("MVCostModel", "Consuming leftover reduction");
      std::sort(NewPacking.begin(), NewPacking.end(),
                [](Node *P0, Node *P1) { return *P0 < *P1; });
      VList.splice(VList.end(), consumeReduction(NewPacking));
    }
    // Compute orphan nodes
    NodeVectorT OrphanNodes;
    for (auto &[LSize, List] : MapReductions) {
      while (List.size() >= 4) {
        int PS = std::min((int)List.size(), 8);
        NodeVectorT NewRedux(&List[0], &List[PS]);
        std::sort(NewRedux.begin(), NewRedux.end(),
                  [](Node *P0, Node *P1) { return *P0 < *P1; });
        VList.splice(VList.end(), consumeReduction(NewRedux));
        List.erase(List.begin(), List.begin() + PS);
      }
      if (List.size() > 0) {
        OrphanNodes.insert(OrphanNodes.end(), List.begin(), List.end());
      }
    }
    if (OrphanNodes.size() > 0) {
      std::sort(OrphanNodes.begin(), OrphanNodes.end(),
                [](Node *P0, Node *P1) { return *P0 < *P1; });
      VList.splice(VList.end(), consumeReduction(OrphanNodes));
    }
  }
  return VList;
}

//---------------------------------------------
VectorOPListT MVCostModel::getVectorOpFromCDAG(NodeVectorT &NList) {
  // Returning list
  VectorOPListT VList;
  MACVETH_DEBUG("MVCostModel", "Detecting reductions");
  // Working with a copy
  NodeVectorT NL(NList);

  // Divide-and-conquer approach: get reductions first, then deal with the rest
  NodeVectorT NRedux = PlcmntAlgo::detectReductions(&NL);

  MACVETH_DEBUG("MVCostModel", "=== REDUCTIONS CASE ===");
  std::sort(NRedux.begin(), NRedux.end(), [](const Node *N0, const Node *N1) {
    return N0->getSchedInfo().NodeID < N1->getSchedInfo().NodeID;
  });
  VList.splice(VList.end(), bottomUpConsumer(NRedux));
  MACVETH_DEBUG("MVCostModel", "=== END REDUCTIONS CASE ===");

  MACVETH_DEBUG("MVCostModel", "=== GENERAL CASE ===");
  VList.splice(VList.end(), greedyOpsConsumer(NL));
  MACVETH_DEBUG("MVCostModel", "=== END GENERAL CASE ===");

  // Order Vector Operations by TAC ID
  VList.sort([](VectorOP V1, VectorOP V2) { return V1.Order < V2.Order; });

  return VList;
}

// ---------------------------------------------
SIMDInfo MVCostModel::computeCostModel(StmtWrapperVectorT &Stmts) {

  // Get all the TAC from all the Stmts
  TacListT TL;
  std::for_each(Stmts.begin(), Stmts.end(), [&TL](auto SWrap) {
    TL.splice(TL.end(), SWrap->getTacList());
  });
  // Creating the CDAG
  auto DAG = CDAG::createCDAGfromTAC(TL);
  // Set placement according to the desired algorithm
  auto NodeList = PlcmntAlgo::sortGraph(DAG.getNodeListOps());
  // Execute greedy algorithm
  auto VectorList = getVectorOpFromCDAG(NodeList);
  // Generate the SIMD list
  auto SIMD = Backend->getSIMDfromVectorOP(VectorList);
  return generateSIMDInfoReport(SIMD);
}
