/**
 * File              : MVExprFactory.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:11:49 MST
 * Last Modified Date: Xov 12 Dec 2019 10:39:42 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#ifndef MACVETH_MVEXPRFACTORY_H
#define MACVETH_MVEXPRFACTORY_H
#include "include/MVExpr/MVExpr.h"
#include "include/MVExpr/MVExprArray.h"
#include "include/MVExpr/MVExprLiteral.h"
#include "include/MVExpr/MVExprVar.h"

using namespace macveth;

namespace macveth {

class MVExprFactory {
public:
  /// Types available:
  /// * ARRAY, e.g. a[i], b[i][j]
  /// * LITERAL, e.g. 5.0, 42
  /// * VARIABLE, otherwise
  enum MVExprType { ARRAY, LITERAL, VARIABLE };

  static MVExprType getTempTypeFromExpr(Expr *E);

  static MVExpr *createMVExpr(Expr *E);
  static MVExpr *createMVExpr(std::string E);
};
} // namespace macveth
#endif
