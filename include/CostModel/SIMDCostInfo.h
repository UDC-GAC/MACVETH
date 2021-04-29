// MACVETH - SIMDCostInfo.h
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

#ifndef MACVETH_SIMDCOSTINFO_H
#define MACVETH_SIMDCOSTINFO_H

#include "include/CostModel/CostTable.h"
#include "include/Vectorization/SIMD/SIMDBackEnd.h"
#include <map>

using namespace macveth;

namespace macveth {

/// Result of computation
enum VectorizationType {
  /// Do not vectorize anything
  NONE,
  /// Perform partial vectorization
  PARTIAL,
  /// Perform full vectorization
  FULL
};

/// Types of instruction
enum InstType {
  /// SIMD type operation
  SIMDOp,
  /// MVOP is the same as sequential
  MVOP
};

/// Wrapper for calculating the cost of instructions
struct InstCostInfo {
  /// Default empty constructor
  InstCostInfo() {}

  /// Constructor with all parameters
  InstCostInfo(int L, float T, int U) : Latency(L), Throughput(T), UOps(U) {}

  /// Default constructor given a row from the CostTable and the type of
  /// instruction
  InstCostInfo(CostTable::Row R, InstType I) {
    UOps = R.NUops;
    if (I == InstType::SIMDOp) {
      Latency = CostTable::getLatencyIntrin(R.Intrinsics);
    } else if (I == InstType::MVOP) {
      Latency = CostTable::getLatencyMVOP(R.MVOP);
    }
    Throughput = R.Throughput;
  }

  /// Number of cycles wasted by the instruction
  int Latency = 0;
  /// TODO: Issue latency: the lower the better. On Intel architectures it
  /// represents the velocity of issuing another uop
  double Throughput = 1.0;
  /// Number of micro-instructions issued by the SIMD instruction
  int UOps = 1;
  /// TODO: Number of ports used
  std::map<int, int> NoPorts;

  std::string toString() { return std::to_string(Latency); }

  long operator-(const InstCostInfo &R) { return this->Latency - R.Latency; }

  InstCostInfo operator+=(const InstCostInfo &R) {
    this->Latency += R.Latency;
    return *this;
  }

  InstCostInfo operator+(const InstCostInfo &R) {
    this->Latency += R.Latency;
    return *this;
  }

  bool operator<(const InstCostInfo &R) { return this->Latency < R.Latency; }

  bool operator<=(const InstCostInfo &R) { return this->Latency <= R.Latency; }
};

/// Return value when generating new code
struct SIMDInfo {
  /// List of SIMD instructions generated
  SIMDBackEnd::SIMDInstListType SIMDList;
  /// Cost of operations
  std::map<std::string, InstCostInfo> CostOp;
  /// Number of operations of each type
  std::map<std::string, long> NumOp;
  /// Total cost
  unsigned long TotCost = 0;
  /// Vectorization type
  VectorizationType Vectorize = VectorizationType::NONE;

  /// Constructor
  SIMDInfo(SIMDBackEnd::SIMDInstListType S,
           std::map<std::string, InstCostInfo> CostOp,
           std::map<std::string, long> NumOp, long TotCost)
      : SIMDList(S), CostOp(CostOp), NumOp(NumOp), TotCost(TotCost) {}

  /// Empty constructor
  SIMDInfo() {}

  /// Do we have to vectorize
  bool isThereAnyVectorization() {
    return Vectorize != VectorizationType::NONE;
  }

  /// Generate cost report
  void generateCostReport() {
    if (MVOptions::SIMDReportFile == "") {
      return;
    }
    std::ofstream F;
    F.open(MVOptions::SIMDReportFile);
    if (!F.is_open()) {
      MVWarn("Cost report file could not be opened!");
      return;
    }
    F << " === MACVETH: SIMD report" << std::endl;
    for (auto It = CostOp.begin(); It != CostOp.end(); ++It) {
      F << It->first + " \t = \t" +
               std::to_string(NumOp[It->first] * It->second.Latency) + "\t(" +
               std::to_string(NumOp[It->first]) + ")"
        << std::endl;
    }
    F << " TOTAL COST = " + std::to_string(TotCost) << std::endl;
    F << " === ENDMACVETH" << std::endl;
    F.close();
  }
};

} // namespace macveth
#endif /* !MACVETH_SIMDCOSTINFO_H */