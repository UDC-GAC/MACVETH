/**
 * File              : MVExprVar.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:05:01 MST
 * Last Modified Date: Xov 09 Xan 2020 21:35:49 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#ifndef MACVETH_MVEXPRVAR_H
#define MACVETH_MVEXPRVAR_H
#include "include/MVExpr/MVExpr.h"
#include <unordered_map>

using namespace macveth;

namespace macveth {

/// Abstraction of variables for simplicity
class MVExprVar : public MVExpr {
public:
  virtual ~MVExprVar(){};
  MVExprVar(MVExprVar *E) : MVExpr(E){};
  MVExprVar(Expr *E) : MVExpr(MVK_Var, E) {}
  MVExprVar(std::string E) : MVExpr(MVK_Var, E) {}
  MVExprVar(std::string E, bool Temp) : MVExpr(MVK_Var, E) {
    if (Temp) {
      this->setTempInfo(MVExprInfo::TMP_RES);
    } else {
      this->setTempInfo(MVExprInfo::TAC_EXPR);
    }
  }
  /// Unroll expression
  virtual MVExpr *unrollExpr(int UF, std::string LL);
  static bool hasBeenUnrolled(std::string Reg, std::string Dim);
  static void regUnrollDim(MVExpr *MVE, std::string Dim, int UF);
  static void regUndoUnrollDim(MVExpr *MVE, std::string Dim, int UF);
  /// In order to be able to use RTTI
  static bool classof(const MVExpr *S) { return S->getKind() == MVK_Var; }
  static inline std::map<std::string, std::list<std::string>> RegDimUnrolled;
};

} // namespace macveth
#endif
