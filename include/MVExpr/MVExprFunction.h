/**
 * File              : MVExprFunction.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 01 Xan 2020 16:56:22 MST
 * Last Modified Date: Mér 01 Xan 2020 18:10:35 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_MVEXPRFUNC_H
#define MACVETH_MVEXPRFUNC_H
#include "include/MVExpr/MVExpr.h"

using namespace macveth;

namespace macveth {

/// Abstraction of functions for simplicity
class MVExprFunc : public MVExpr {
public:
  virtual ~MVExprFunc(){};
  MVExprFunc(Expr *E) : MVExpr(E) {}

  MVExprFunc(MVExprFunc *E) : MVExpr(E->getClangExpr()) {
    this->FuncName = E->FuncName;
    this->OPS = E->OPS;
  }

  /// Implementation of unrolling for functions. In this case we will need to
  /// create a new MVExpr
  virtual MVExpr *unrollExpr(int UF, std::string LL);

private:
  /// Name of the function
  std::string FuncName;
  /// Parameters of the funcion
  std::list<MVExpr> OPS;
};

} // namespace macveth
#endif
