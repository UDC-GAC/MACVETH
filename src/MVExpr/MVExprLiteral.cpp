/**
 * File              : MVExprLiteral.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:26:48 MST
 * Last Modified Date: Xov 12 Dec 2019 10:26:48 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/MVExpr/MVExprLiteral.h"

//------------------------------------------------
MVExpr *MVExprLiteral::unrollExpr(int UF, std::string LL, bool SubstituteVal) {
  MVExprLiteral *NewExpr = new MVExprLiteral(this);
  return NewExpr;
}
