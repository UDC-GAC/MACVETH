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
#include "include/Vectorization/SIMD/SIMDGenerator.h"
#include <map>

using namespace macveth;

namespace macveth {

class MVCostModel {
public:
  /// Result of computation
  enum VectorizationType {
    /// Do not vectorize anything
    NONE,
    /// Perform partial vectorization
    PARTIAL,
    /// Perform full vectorization
    FULL
  };

  /// Return value when generating new code
  struct SIMDInfo {
    /// List of SIMD instructions generated
    SIMDGenerator::SIMDInstListType SIMDList;
    /// Cost of operations
    std::map<std::string, SIMDCostInfo> CostOp;
    /// Number of operations of each type
    std::map<std::string, long> NumOp;
    /// Total cost
    long TotCost;
    /// Vectorization type
    VectorizationType Vectorize;

    /// Constructor
    SIMDInfo(SIMDGenerator::SIMDInstListType S,
             std::map<std::string, SIMDCostInfo> CostOp,
             std::map<std::string, long> NumOp, long TotCost)
        : SIMDList(S), CostOp(CostOp), NumOp(NumOp), TotCost(TotCost) {}

    /// Do we have to vectorize
    bool isThereAnyVectorization() {
      return Vectorize != VectorizationType::NONE;
    }

    /// Printing the total cost of the operations
    void printCost() {
      std::cout << "---------- SIMD REPORT ----------\n";
      for (auto It = CostOp.begin(); It != CostOp.end(); ++It) {
        std::cout << It->first + "\t=\t" +
                         std::to_string(NumOp[It->first] * It->second.Latency) +
                         "\t(" + std::to_string(NumOp[It->first]) + ")"
                  << std::endl;
      }
      std::cout << " TOTAL = " + std::to_string(TotCost) << std::endl;
      std::cout << "-------- END SIMD REPORT --------\n";
    }
  };

  using MVCost = std::map<std::string, long>;

  static long computeSequentialCostStmtWrapper(std::list<StmtWrapper *> SL);

  /// Compute the cost model according to the latency-uops-
  static SIMDInfo computeCostModel(CDAG *G, SIMDGenerator *SG);

  static SIMDInfo computeSIMDCost(SIMDGenerator::SIMDInstListType S);

private:
  MVCostModel *_instance;
};

} // namespace macveth

#endif /* !MACVETH_COSTMODEL_H */