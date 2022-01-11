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
//     Louis-Noël Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Marcos Horro <marcos.horro@udc.es>

#ifndef MACVETH_COSTMODEL_H
#define MACVETH_COSTMODEL_H

#include "include/CostModel/SIMDCostInfo.h"
#include "include/MVOptions.h"
#include "include/StmtWrapper.h"
#include "include/Vectorization/SIMD/SIMDBackEnd.h"

using namespace macveth;

namespace macveth {
class MVCostModel {
public:
  /// Compute cost of a concrete statement
  InstCostInfo computeCostForStmtWrapper(StmtWrapper *Stmt);

  /// Compute cost of a region or set of statements
  InstCostInfo computeCostForStmtWrapperList(StmtWrapperVectorT &Stmts);

  /// Compute cost of a concrete node
  InstCostInfo computeCostForNodeOp(Node *N);

  /// Compute cost of a region or set of operation nodes
  InstCostInfo computeCostForNodeOpsList(int VL, const NodeVectorT &Nodes);

  /// Compute cost of set of operand nodes
  InstCostInfo computeCostForNodeOperandsList(int VL, const NodeVectorT &Nodes);

  /// Compute cost of a vector operation generated in the Vector IR
  InstCostInfo computeVectorOPCost(VectorOP VOP);

  /// Compute the cost model according to the latency-uops
  SIMDInfo computeCostModel(StmtWrapperVectorT &Stmts);

  /// Generate SIMD report, with all costs for different operations
  SIMDInfo generateSIMDInfoReport(SIMDBackEnd::SIMDInstListType &SIMD);

  /// Get the vector operations from the CDAG
  VectorOPListT getVectorOpFromCDAG(NodeVectorT &Nodes);

  /// Greedy algorithm for consuming nodes in the CDAG
  VectorOPListT greedyOpsConsumer(NodeVectorT &Nodes);
  /// Smart algorithm for grouping together reductions
  VectorOPListT bottomUpConsumer(NodeVectorT &Nodes);

  VectorOPListT searchMapNodes(NodeVectorT &Nodes);

  MVCostModel(SIMDBackEnd *Backend) : Backend(Backend) {
    MaxVectorLength = Backend->getMaxVectorSize("float");
  }

private:
  VectorOPListT consumeNodes(NodeVectorT &Nodes);

  SIMDBackEnd *Backend = nullptr;
  unsigned long MaxVectorLength = 8;
};

} // namespace macveth
#endif /* !MACVETH_COSTMODEL_H */