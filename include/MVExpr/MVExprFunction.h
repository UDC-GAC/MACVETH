/**
 * File              : MVExprFunction.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 01 Xan 2020 16:56:22 MST
 * Last Modified Date: Xov 09 Xan 2020 21:33:34 MST
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
  MVExprFunc(Expr *E) : MVExpr(MVK_Function, E) {}

  MVExprFunc(MVExprFunc *E) : MVExpr(MVK_Function, E->getClangExpr()) {
    this->FuncName = E->FuncName;
    this->OPS = E->OPS;
  }

  /// Implementation of unrolling for functions. In this case we will need to
  /// create a new MVExpr
  virtual MVExpr *unrollExpr(int UF, std::string LL);

  /// In order to be able to use RTTI
  static bool classof(const MVExpr *S) { return S->getKind() == MVK_Function; }

private:
  /// Name of the function
  std::string FuncName;
  /// Parameters of the funcion
  std::list<MVExpr> OPS;
};

} // namespace macveth
#endif
