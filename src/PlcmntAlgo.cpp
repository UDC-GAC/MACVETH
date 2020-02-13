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
      Max = std::max(Max, computeMaxDepth(T));
    }
    return 1 + Max;
  }
}

// ---------------------------------------------
void computeFreeSchedule(Node::NodeListType NL) {
  for (Node *N : NL) {
    N->setFreeSchedInfo(computeMaxDepth(N));
  }
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
          // not using RTTI), that is why we perform the previous check. On any
          // case, user should be aware of the format of the file: comments
          // starting with // are allowed, but each non-comment row must only
          // contain an integer value
          N->setPlcmt(std::stoi(L));
          break;
        }
      }
    }
  } else {
    llvm::llvm_unreachable_internal();
  }
  CF.close();
}
