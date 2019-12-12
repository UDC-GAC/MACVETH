/**
 * File              : MVExprVar.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:27:43 MST
 * Last Modified Date: Xov 12 Dec 2019 10:27:43 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/MVExpr/MVExprVar.h"

//------------------------------------------------
MVExpr *MVExprVar::unrollExpr(int UF, std::string LL) {
  MVExprVar *NewExpr = new MVExprVar(this);
  return NewExpr;
}
