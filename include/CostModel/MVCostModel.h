/*
 * File					 : include/CostModel/MVCostModel.h
 * Author        : Marcos Horro
 * Date				   : Sat 10 Oct 2020 11:08 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:32 +02:00
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