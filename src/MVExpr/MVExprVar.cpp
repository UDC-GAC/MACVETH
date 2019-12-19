/**
 * File              : MVExprVar.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:27:43 MST
 * Last Modified Date: Mér 18 Dec 2019 14:35:42 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/MVExpr/MVExprVar.h"
#include "MVExpr/MVExpr.h"
#include <string>

//------------------------------------------------
MVExpr *MVExprVar::unrollExpr(int UF, std::string LL) {
  if (this->getTempInfo() == MVExpr::MVExprInfo::TMP_RES) {
    MVExprVar *NewExpr = new MVExprVar(this);
    NewExpr->setExprStr(NewExpr->getExprStr() + LL + std::to_string(UF));
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

//------------------------------------------------
MVExpr *MVExprVar::unrollExpr(std::unordered_map<int, std::string> LList) {
  std::string Unroll = "";
  for (auto X : LList) {
    Unroll += X.second + std::to_string(X.first);
  }
  return unrollExprString(this, Unroll);
}
