/**
 * File              : MVExprVar.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:27:43 MST
 * Last Modified Date: Dom 05 Xan 2020 13:24:44 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/MVExpr/MVExprVar.h"
#include <string>

//------------------------------------------------
void MVExprVar::regUnrollDim(MVExpr *MVE, std::string Dim, int UF) {
  auto M = dyn_cast<MVExprVar>(MVE);
  if (!M) {
    return;
  }
  Utils::printDebug("MVExprVar", MVE->getExprStr() + "; " + Dim);
  MVExprVar::RegDimUnrolled[MVE->getExprStr()].push_back(Dim);
}

//------------------------------------------------
void MVExprVar::regUndoUnrollDim(MVExpr *MVE, std::string Dim, int UF) {
  auto M = dyn_cast<MVExprVar>(MVE);
  if (!M) {
    return;
  }
  auto L = MVExprVar::RegDimUnrolled.find(MVE->getExprStr());
  if (L != MVExprVar::RegDimUnrolled.end()) {
    auto it = L->second;
    auto E = std::find(it.begin(), it.end(), Dim);
    if (E != it.end()) {
      Utils::printDebug("MVExprVar",
                        "undoing " + MVE->getExprStr() + ", " + Dim);
      it.erase(E);
    }
  }
}

//------------------------------------------------
bool MVExprVar::hasBeenUnrolled(std::string Reg, std::string Dim) {
  auto it = MVExprVar::RegDimUnrolled.find(Reg);
  if (it != RegDimUnrolled.end()) {
    for (auto D : it->second) {
      Utils::printDebug("MVExprVar", "hasBeenUnrolled = " + Dim);
      if (D == Dim) {
        return true;
      }
    }
  }
  return false;
}

//------------------------------------------------
MVExpr *MVExprVar::unrollExpr(int UF, std::string LL) {
  if (this->getTempInfo() == MVExpr::MVExprInfo::TMP_RES) {
    //    FIXME
    //    if (!MVExprVar::hasBeenUnrolled(this->getExprStr(), LL)) {
    //      return this;
    //    }
    MVExprVar *NewExpr = new MVExprVar(this);
    NewExpr->setExprStr(NewExpr->getExprStr() + LL + std::to_string(UF));
    //    Utils::printDebug("MVExprVar",
    //                      "new Expr = " + NewExpr->getExprStr() + "; " + LL);
    return NewExpr;
  } else if (this->getExprStr() == LL) {
    // Special case, when unrolling loop variable
    MVExprVar *NewExpr = new MVExprVar(this);
    NewExpr->setExprStr("(" + (NewExpr->getExprStr()) + " + " +
                        std::to_string(UF) + ")");
    NewExpr->setDoNotLoadFromMem();
    return NewExpr;
  }
  return this;
}

//------------------------------------------------
MVExpr *unrollExprString(MVExprVar *V, std::string LL) {
  if (V->getTempInfo() == MVExpr::MVExprInfo::TMP_RES) {
    MVExprVar *NewExpr = new MVExprVar(V);
    NewExpr->setExprStr(NewExpr->getExprStr() + LL);
    return NewExpr;
  }
  return V;
}
