/**
 * File              : MVExprArray.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:26:12 MST
 * Last Modified Date: Xov 12 Dec 2019 10:27:27 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#include "include/MVExpr/MVExprArray.h"

//------------------------------------------------
MVExpr *MVExprArray::unrollExpr(int UF, std::string LL) {
  MVExprArray *NewExpr = new MVExprArray(this);
  NewExpr->updateIndex(UF, LL);
  return NewExpr;
}

/// Given a ArraySubscriptExpr, recursively gets the base name and the indexes
/// from the outermost to the innermost
const Expr *MVExprArray::getArrayBaseExprAndIdxs(const ArraySubscriptExpr *ASE,
                                                 IdxVector &Idxs) {
  const Expr *BaseE = NULL;
  while (ASE) {
    const Expr *IdxE = ASE->getIdx();
    std::string Idx = Utils::getStringFromExpr(IdxE);
    BaseE = ASE->getBase()->IgnoreParenCasts();
    ASE = dyn_cast<ArraySubscriptExpr>(BaseE);
    Idxs.push_back(Idx);
  }
  /// To sort from outermost to innermost
  Idxs.reverse();
  return BaseE;
}

void MVExprArray::updateIndex(int UF, std::string LL) {
  auto it = find(this->Idx.begin(), this->Idx.end(), LL);
  if (it != this->Idx.end()) {
    std::string &S(*it);
    S = S + " + " + std::to_string(UF);
  }
  std::string NewStrExpr = this->BaseName;
  for (std::string I : this->Idx) {
    NewStrExpr += "[" + I + "]";
  }
  this->setExprStr(NewStrExpr);
}
