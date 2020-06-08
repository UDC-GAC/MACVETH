/**
 * File              : PlcmntAlgo.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Sáb 29 Feb 2020 16:55:54 CET
 * Last Modified Date: Lun 02 Mar 2020 17:53:38 CET
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#include "include/PlcmntAlgo.h"
#include "include/MVOptions.h"

// ---------------------------------------------
int computeMaxDepth(Node *N) {
  // This algorithm is not efficient if the CDAG is big. At some point more
  // intelligent strategies should be used instead.
  if (N->getInputNum() == 0) {
    return 0;
  } else {
    int Max = 0;
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
  // Compute RAW
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
  bool ReductionFound = false;

  NCopy.reverse();
  NL->clear();

  for (auto R : NCopy) {
    ReductionFound = false;
    if (std::find(Visited.begin(), Visited.end(), R) != Visited.end()) {
      Utils::printDebug("PlcmntAlgo", "Visited " + R->getRegisterValue());
      continue;
    }
    Visited.push_back(R);

    Reduction.push_front(R);
    auto S = R->getInputs();
  loop:
    for (auto In : S) {
      if (In == nullptr) {
        Utils::printDebug("PlcmntAlgo", "Skipping");
        continue;
      }
      // Since we are checking the inputs of a node, we are do not have to check
      // if there are any RAW dependencies, because there are. Some conditions
      // we are checking:
      // 1.- Check if same type (getValue())
      // 2.- Check if sequential (FreeSched)
      // 3.- Check if they belong to the same TAC (after unrolling)
      if ((R->getValue() == In->getValue()) &&
          (R->getSchedInfo().FreeSched > (In->getSchedInfo().FreeSched)) &&
          (R->getSchedInfo().TacID == (In->getSchedInfo().TacID))) {
        Utils::printDebug("PlcmntAlgo", "Reduction found!");
        ReductionFound = true;
        Reduction.push_front(In);
        Visited.push_back(In);
        S = In->getInputs();
        goto loop;
      }
    }
    if (ReductionFound) {
      for (auto RNode : Reduction) {
        LRedux.push_back(RNode);
      }
    } else {
      NL->push_back(R);
    }
    Reduction.clear();
  }
  NL->reverse();
  return LRedux;
}

// ---------------------------------------------
Node::NodeListType PlcmntAlgo::sortGraph(Node::NodeListType NL) {
  computeFreeSchedule(NL);
  if (MVOptions::InCDAGFile != "") {
    setPlcmtFromFile(NL);
    NL.sort([](Node *Lhs, Node *Rhs) {
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
            llvm::outs() << "CDAG input file is not correct!\n";
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
    assert(false && "");
    llvm::llvm_unreachable_internal();
  }
  CF.close();
}
