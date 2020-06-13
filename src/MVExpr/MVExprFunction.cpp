/**
 * File              : MVExprFunc.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 01 Xan 2020 17:56:19 MST
 * Last Modified Date: Mér 01 Xan 2020 17:56:19 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#include "include/MVExpr/MVExprFunction.h"
#include "include/MVExpr/MVExprFactory.h"

//------------------------------------------------
MVExprFunc::MVExprFunc(Expr *E) : MVExpr(MVK_Function, E) {
  auto F = dyn_cast<clang::CallExpr>(E->IgnoreImpCasts());
  assert((F != NULL) && "Something went wrong");

  auto Args = F->getArgs();
  for (int i = 0; i < F->getNumArgs(); ++i) {
    Utils::printDebug("MVExprFunc", Utils::getStringFromExpr(Args[i]));
    this->OPS.push_back(MVExprFactory::createMVExpr(Args[i]));
  }
}

//------------------------------------------------
MVExpr *MVExprFunc::unrollExpr(int UF, std::string LL) {
  MVExprFunc *NewExpr = new MVExprFunc(this);
  NewExpr->OPS.clear();
  for (auto E : this->OPS) {
    NewExpr->OPS.push_back(E->unrollExpr(UF, LL));
  }
  return NewExpr;
}
