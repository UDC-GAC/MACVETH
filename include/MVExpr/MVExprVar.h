/**
 * File              : MVExprVar.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:05:01 MST
 * Last Modified Date: Mér 18 Dec 2019 14:22:58 MST
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
  MVExprVar(Expr *E) : MVExpr(E) {}
  MVExprVar(std::string E) : MVExpr(E) {}
  MVExprVar(std::string E, bool Temp) : MVExpr(E) {
    if (Temp) {
      this->setTempInfo(MVExprInfo::TMP_RES);
    } else {
      this->setTempInfo(MVExprInfo::TAC_EXPR);
    }
  }
  virtual MVExpr *unrollExpr(int UF, std::string LL);
  virtual MVExpr *unrollExpr(std::unordered_map<int, std::string> LList);
};

} // namespace macveth
#endif
