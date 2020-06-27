/**
 * File              : MVExprLiteral.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:05:57 MST
 * Last Modified Date: Xov 09 Xan 2020 21:34:48 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#ifndef MACVETH_MVEXPRLITERAL_H
#define MACVETH_MVEXPRLITERAL_H

#include "include/MVExpr/MVExpr.h"

using namespace macveth;

namespace macveth {

/// Abstraction of variables for simplicity
class MVExprLiteral : public MVExpr {
public:
  /// Destructor
  virtual ~MVExprLiteral(){};

  /// Clone
  MVExprLiteral(MVExprLiteral *E) : MVExpr(MVK_Literal, E->getClangExpr()){};

  /// Basic constructor
  MVExprLiteral(Expr *E) : MVExpr(MVK_Literal, E) {}

  /// Unroll expression
  virtual MVExpr *unrollExpr(int UF, std::string LL);

  /// In order to be able to use RTTI
  static bool classof(const MVExpr *S) { return S->getKind() == MVK_Literal; }
};

} // namespace macveth
#endif /* !MACVETH_MVEXPRLITERAL_H */
