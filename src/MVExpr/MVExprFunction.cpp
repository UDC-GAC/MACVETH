/**
 * File              : MVExprFunc.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 01 Xan 2020 17:56:19 MST
 * Last Modified Date: Mér 01 Xan 2020 17:56:19 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/MVExpr/MVExprFunction.h"

//------------------------------------------------
MVExpr *MVExprFunc::unrollExpr(int UF, std::string LL) {
  MVExprFunc *NewExpr = new MVExprFunc(this);
  NewExpr->OPS.clear();
  for (MVExpr E : this->OPS) {
    NewExpr->OPS.push_back(E.unrollExpr(UF, LL));
  }
  return NewExpr;
}
