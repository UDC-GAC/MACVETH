/**
 * File              : MVExprFactory.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:36:04 MST
 * Last Modified Date: Mér 18 Dec 2019 12:04:58 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/MVExpr/MVExprFactory.h"

MVExprFactory::MVExprType MVExprFactory::getTempTypeFromExpr(Expr *E) {
  if (E == NULL)
    return MVExprType::VARIABLE;
  if (ArraySubscriptExpr *ASE =
          dyn_cast<clang::ArraySubscriptExpr>(E->IgnoreImpCasts())) {
    return MVExprType::ARRAY;
  }

  if (Utils::isNumericValue(E)) {
    return MVExprType::LITERAL;
  }

  return MVExprType::VARIABLE;
}

MVExpr *MVExprFactory::createMVExpr(Expr *E) {
  MVExprFactory::MVExprType T = MVExprFactory::getTempTypeFromExpr(E);
  switch (T) {
  case MVExprFactory::MVExprType::ARRAY:
    return new MVExprArray(E);
  case MVExprFactory::MVExprType::LITERAL:
    return new MVExprLiteral(E);
  case MVExprFactory::MVExprType::VARIABLE:
  default:
    return new MVExprVar(E);
  }
}

MVExpr *MVExprFactory::createMVExpr(std::string E) { return new MVExprVar(E); }
MVExpr *MVExprFactory::createMVExpr(std::string E, bool Temp) {
  return new MVExprVar(E, Temp);
}
