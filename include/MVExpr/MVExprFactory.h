/**
 * File              : MVExprFactory.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:11:49 MST
 * Last Modified Date: Xov 09 Xan 2020 20:34:12 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#ifndef MACVETH_MVEXPRFACTORY_H
#define MACVETH_MVEXPRFACTORY_H
#include "include/MVExpr/MVExpr.h"
#include "include/MVExpr/MVExprArray.h"
#include "include/MVExpr/MVExprFunction.h"
#include "include/MVExpr/MVExprLiteral.h"
#include "include/MVExpr/MVExprVar.h"

using namespace macveth;

namespace macveth {

/// Create MVExpr depending on the dynamic cast clang does
class MVExprFactory {
public:
  /// Types available:
  /// * ARRAY, e.g. a[i], b[i][j]
  /// * LITERAL, e.g. 5.0, 42
  /// * VARIABLE, otherwise
  enum MVExprType { ARRAY, LITERAL, VARIABLE, FUNC };

  /// Detect the type of MVExpr
  static MVExprType getTempTypeFromExpr(Expr *E);
  /// Create MVExpr from Clang expression
  static MVExpr *createMVExpr(Expr *E);
  /// Create temporal MVExpr (as MVExprVar)
  static MVExpr *createMVExpr(std::string E, bool Temp, std::string Type);
};
} // namespace macveth
#endif
