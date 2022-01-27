// MACVETH - PlcmntAlgo.cpp
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

#include "include/PlcmntAlgo.h"
#include "include/Debug.h"
#include "include/MVOptions.h"

// ---------------------------------------------
int computeMaxDepth(Node *N) {
  // This algorithm is not efficient if the CDAG is big. At some point more
  // intelligent strategies should be used instead.
  if (N->getInputNum() == 0) {
    return 0;
  } else {
    int Max = N->getSchedInfo().FreeSched;
    for (auto *T : N->getInputs()) {
      Max = std::max(Max, 1 + computeMaxDepth(T));
    }
    return Max;
  }
}

// ---------------------------------------------
void PlcmntAlgo::computeFreeSchedule(Node *N) {
  N->setFreeSchedInfo(computeMaxDepth(N));
}

// ---------------------------------------------
void PlcmntAlgo::computeFreeSchedule(NodeVectorT &NL) {
  std::for_each(NL.begin(), NL.end(),
                [&NL](auto *N) { N->setFreeSchedInfo(computeMaxDepth(N)); });
}

// ---------------------------------------------
NodeVectorT PlcmntAlgo::detectReductions(NodeVectorT *NL) {
  NodeVectorT NCopy(*NL);
  NodeVectorT LRedux;
  NodeVectorT Visited;
  NodeVectorT Reduction;
  auto ReductionFound = false;
  std::reverse(std::begin(NCopy), std::end(NCopy));
  NL->clear();
  for (auto TmpNode : NCopy) {
    ReductionFound = false;
    if (std::find(Visited.begin(), Visited.end(), TmpNode) != Visited.end())
      continue;
    Visited.push_back(TmpNode);
    Reduction.insert(Reduction.begin(), TmpNode);
    auto Inputs = TmpNode->getInputs();
  loop:
    for (auto In : Inputs) {
      if (In == nullptr) {
        MACVETH_DEBUG("PlcmntAlgo", "Skipping");
        continue;
      }

      if ((TmpNode->getValue() == In->getValue()) &&
          (TmpNode->getSchedInfo().FreeSched >
           (In->getSchedInfo().FreeSched))) {

        MACVETH_DEBUG("PlcmntAlgo", "Reduction found for " +
                                        In->getRegisterValue() + ", " +
                                        In->getSchedInfoStr());
        ReductionFound = true;
        Reduction.push_back(In);
        Visited.push_back(In);
        for (auto NodeIn : Inputs) {
          if (NodeIn != nullptr)
            NodeIn->setNodeAsReduction();
        }
        Inputs = In->getInputs();
        TmpNode->setNodeAsReduction();
        In->setNodeAsReduction();
        for (auto NodeIn : In->getInputs()) {
          if (NodeIn != nullptr)
            NodeIn->setNodeAsReduction();
        }
        // Now we want to iterate over the inputs
        goto loop;
      }
    }
    if (ReductionFound) {
      LRedux.insert(LRedux.end(), Reduction.begin(), Reduction.end());
    } else {
      if (!TmpNode->belongsToAReduction())
        NL->push_back(TmpNode);
    }
    Reduction.clear();
  }
  std::reverse(std::begin(*NL), std::end(*NL));
  std::reverse(std::begin(LRedux), std::end(LRedux));
  return LRedux;
}

// ---------------------------------------------
NodeVectorT PlcmntAlgo::sortGraph(NodeVectorT NL) {
  computeFreeSchedule(NL);
  if (MVOptions::InCDAGFile != "") {
    setPlcmtFromFile(NL);
    std::sort(NL.begin(), NL.end(), [](Node *Lhs, Node *Rhs) {
      return Lhs->getSchedInfo().Plcmnt < Rhs->getSchedInfo().Plcmnt;
    });
    return NL;
  } else {
    return topSort(NL);
  }
  // Debugging
  if (MVOptions::Debug) {
    for (auto N : NL) {
      MACVETH_DEBUG("MVCostModel", N->toString());
    }
  }

  return NL;
}

// ---------------------------------------------
void PlcmntAlgo::setPlcmtFromFile(NodeVectorT &NL) {
  std::ifstream CF(Utils::getExePath() + MVOptions::InCDAGFile,
                   std::ios_base::in);
  assert(!CF.fail() && "File does not exist for PlcmntAlgo!");
  if (CF.is_open()) {
    // For each node, read until lines skipping those with comments. Fail if
    // line with no numbers
    for (auto N : NL) {

      std::string L;
      while (getline(CF, L)) {
        if (L.rfind("//", 0) == 0) {
          // This is a comment, skip it
          continue;
        } else {
          if (!(L.find_first_not_of("0123456789") == std::string::npos)) {
            MACVETH_DEBUG("PlcmntAlgo", "CDAG input file is not correct!");
            llvm::llvm_unreachable_internal();
          }
          // Clang does not allow to perform any type of exception handling in
          // order to minimize the size of the executable (same reasoning for
          // not using RTTI), that is why we perform the previous check. On
          // any case, user should be aware of the format of the file:
          // comments starting with // are allowed, but each non-comment row
          // must only contain an integer value
          N->setPlcmnt(std::stoi(L));
          break;
        }
      }
    }
  }
  CF.close();
}
