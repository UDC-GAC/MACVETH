// MACVETH - MVCostModel.h
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
//     Louis-Nöel Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Louis-Nöel Pouchet <pouchet@colostate.edu>

#ifndef MACVETH_COSTMODEL_H
#define MACVETH_COSTMODEL_H

#include "include/CostModel/SIMDCostInfo.h"
#include "include/MVOptions.h"
#include "include/StmtWrapper.h"
#include "include/Vectorization/SIMD/SIMDBackEnd.h"
#include <map>

using namespace macveth;

namespace macveth {

class MVCostModel {
public:
  /// Compute cost of a concrete statement
  static InstCostInfo computeCostForStmtWrapper(StmtWrapper *S);

  /// Compute cost of a region or set of statements
  static InstCostInfo
  computeCostForStmtWrapperList(std::list<StmtWrapper *> &SL);

  /// Compute cost of a concrete node
  static InstCostInfo computeCostForNodeOp(Node *N);

  /// Compute cost of a region or set of operation nodes
  static InstCostInfo computeCostForNodeOpsList(int VL, Node::NodeListType &NL);

  /// Compute cost of set of operand nodes
  static InstCostInfo computeCostForNodeOperandsList(int VL,
                                                     Node::NodeListType &NL);

  /// Compute cost of a vector operation generated in the Vector IR
  static InstCostInfo computeVectorOPCost(VectorIR::VectorOP V,
                                          SIMDBackEnd *SG);

  /// Compute the cost model according to the latency-uops
  static SIMDInfo computeCostModel(std::list<StmtWrapper *> SL,
                                   SIMDBackEnd *SG);

  /// Generate SIMD report, with all costs for different operations
  static SIMDInfo generateSIMDInfoReport(SIMDBackEnd::SIMDInstListType S);

  /// Get the vector operations from the CDAG
  static std::list<VectorIR::VectorOP>
  getVectorOpFromCDAG(Node::NodeListType &NList, SIMDBackEnd *SG);

  /// Greedy algorithm for consuming nodes in the CDAG
  static std::list<VectorIR::VectorOP> greedyOpsConsumer(Node::NodeListType &NL,
                                                         SIMDBackEnd *SG);
};

} // namespace macveth
#endif /* !MACVETH_COSTMODEL_H */