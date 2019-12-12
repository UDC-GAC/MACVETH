/**
 * File              : MVExprVar.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:05:01 MST
 * Last Modified Date: Xov 12 Dec 2019 10:42:54 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#ifndef MACVETH_MVEXPRVAR_H
#define MACVETH_MVEXPRVAR_H
#include "include/MVExpr/MVExpr.h"

using namespace macveth;

namespace macveth {

/// Abstraction of variables for simplicity
class MVExprVar : public MVExpr {
public:
  virtual ~MVExprVar(){};
  MVExprVar(MVExprVar *E) : MVExpr(E->getClangExpr()){};
  MVExprVar(Expr *E) : MVExpr(E) {}
  MVExprVar(std::string E) : MVExpr(E) {}
  virtual MVExpr *unrollExpr(int UF, std::string LL);
};

} // namespace macveth
#endif
