/**
 * File              : MVExprLiteral.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:05:57 MST
 * Last Modified Date: Xov 12 Dec 2019 10:15:37 MST
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
  virtual ~MVExprLiteral(){};
  MVExprLiteral(MVExprLiteral *E) : MVExpr(E->getClangExpr()){};
  MVExprLiteral(Expr *E) : MVExpr(E) {}
  virtual MVExpr *unrollExpr(int UF, std::string LL);
};

} // namespace macveth
#endif
