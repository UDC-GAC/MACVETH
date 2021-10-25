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
//     Louis-Noël Pouchet <pouchet@colostate.edu>

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
    for (Node *T : N->getInputs()) {
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
void PlcmntAlgo::computeFreeSchedule(Node::NodeListType NL) {
  for (Node *N : NL) {
    N->setFreeSchedInfo(computeMaxDepth(N));
  }
}

// ---------------------------------------------
Node::NodeListType PlcmntAlgo::detectReductions(Node::NodeListType *NL) {
  Node::NodeListType NCopy(*NL);
  Node::NodeListType LRedux;
  Node::NodeListType Visited;
  Node::NodeListType Reduction;
  auto ReductionFound = false;

  std::reverse(std::begin(NCopy), std::end(NCopy));
  NL->clear();

  for (auto R : NCopy) {
    ReductionFound = false;
    if (std::find(Visited.begin(), Visited.end(), R) != Visited.end()) {
      continue;
    }
    Visited.push_back(R);
    Reduction.insert(Reduction.begin(), R);
    auto S = R->getInputs();
  loop:
    for (auto In : S) {
      if (In == nullptr) {
        MACVETH_DEBUG("PlcmntAlgo", "Skipping");
        continue;
      }
      // Since we are checking the inputs of a node, we are do not have to check
      // if there are any RAW dependencies, because there are. Some conditions
      // we are checking:
      // 1 .- Check if same type (getValue())
      // 2 .- Check if sequential (FreeSched)
      // 3a.- Check if they belong to the same TAC
      // 3b.- Check if same scop
      if ((R->getValue() == In->getValue()) &&
          (R->getSchedInfo().FreeSched > (In->getSchedInfo().FreeSched)) &&
          ((R->getSchedInfo().TacID == (In->getSchedInfo().TacID)) ||
           R->getSchedInfo().Scop[0] == (In->getSchedInfo().Scop[0]))) {

        // FIXME: should we consider this or not...
        // if ((R->getSchedInfo().TacID != In->getSchedInfo().TacID) &&
        //     (R->getSchedInfo().Scop[0] == In->getSchedInfo().Scop[0])) {

        //   // Corner case, e.g.:
        //   // [0,1,2,3] and [2,3,4,5]
        //   // There is no gain in doing 2,3 as reductions, better a gather or
        //   // something
        //   // if ((R->getSchedInfo().UnrollFactor) !=
        //   //     (In->getSchedInfo().UnrollFactor)) {
        //   //   continue;
        //   // }
        //   // Corner case, e.g.:
        //   // [0,1,2,3] and [-1,1,4,5]
        // }

        MACVETH_DEBUG("PlcmntAlgo",
                      "Reduction found for " + In->getRegisterValue());
        ReductionFound = true;
        Reduction.push_back(In);
        Visited.push_back(In);
        for (auto Sin : S) {
          if (Sin != nullptr) {
            Sin->setNodeAsReduction();
          }
        }
        S = In->getInputs();
        R->setNodeAsReduction();
        In->setNodeAsReduction();
        for (auto Sin : S) {
          if (Sin != nullptr) {
            Sin->setNodeAsReduction();
          }
        }
        goto loop;
      }
    }
    if (ReductionFound) {
      for (auto RNode : Reduction) {
        LRedux.insert(LRedux.begin(), RNode);
      }
    } else {
      NL->push_back(R);
    }
    Reduction.clear();
  }
  std::reverse(std::begin(*NL), std::end(*NL));
  // if (LRedux.size() <= 2) {
  //   for (auto R : LRedux) {
  //     R->setNodeAsNonReduction();
  //     for (auto RIn : R->getInputs()) {
  //       if (RIn != nullptr) {
  //         RIn->setNodeAsNonReduction();
  //       }
  //     }
  //   }
  //   NL->insert(NL->end(), LRedux.begin(), LRedux.end());
  //   (*NL) = sortGraph(*NL);
  //   // Undo reductions
  //   LRedux.clear();
  // }
  return LRedux;
}

// ---------------------------------------------
Node::NodeListType PlcmntAlgo::sortGraph(Node::NodeListType NL) {
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
  return NL;
}

// ---------------------------------------------
void PlcmntAlgo::setPlcmtFromFile(Node::NodeListType NL) {
  std::ifstream CF;
  CF.open(Utils::getExePath() + MVOptions::InCDAGFile, std::ios_base::in);
  if (CF.is_open()) {
    std::string L;
    // For each node, read until lines skipping those with comments. Fail if
    // line with no numbers
    for (auto N : NL) {
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
  } else {
    MACVETH_DEBUG("PlcmntAlgo", "CDAG input file can not be opened");
    llvm::llvm_unreachable_internal();
  }
  CF.close();
}
