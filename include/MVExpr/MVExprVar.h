/*
 * File					 : include/MVExpr/MVExprVar.h
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:35 +02:00
 * Modified By	 : Marcos Horro (marcos.horro@udc.gal>)
 *
 * MIT License
 *
 * Copyright (c) 2020 Colorado State University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
  /// Destructor
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

  /// To string
  virtual std::string toString() const { return this->getExprStrRaw(); }

  /// Unroll expression
  virtual MVExpr *unrollExpr(int UF, std::string LL,
                             bool SubstituteVal = false);
  /// Check if this has been unrolled
  static bool hasBeenUnrolled(std::string Reg, std::string Dim);
  /// Unroll register
  static void regUnrollDim(MVExpr *MVE, std::string Dim, int UF);
  /// Undo unrolled register
  static void regUndoUnrollDim(MVExpr *MVE, std::string Dim, int UF);
  /// In order to be able to use RTTI
  static bool classof(const MVExpr *S) { return S->getKind() == MVK_Var; }
  /// Map to register the dimensions unrolled
  static inline std::map<std::string, std::list<std::string>> RegDimUnrolled;
};

} // namespace macveth
#endif /* !MACVETH_MVEXPRVAR_H */
