/**
 * File              : MVExpr.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 14:18:48 MST
 * Last Modified Date: Mér 11 Dec 2019 18:20:09 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 *
 * Copyright (c) 2019 Marcos Horro <marcos.horro@udc.gal>
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

#include "include/MVExpr/MVExpr.h"
#include <string>

using namespace clang;
using namespace macveth;

namespace macveth {

/// Given a ArraySubscriptExpr, recursively gets the base name and the indexes
/// from the outermost to the innermost
const Expr *MVExprArray::getArrayBaseExprAndIdxs(const ArraySubscriptExpr *ASE,
                                                 IdxVector &Idxs) {
  const Expr *BaseE = NULL;
  while (ASE) {
    const Expr *IdxE = ASE->getIdx();
    std::string Idx = Utils::getStringFromExpr(IdxE);
    BaseE = ASE->getBase()->IgnoreParenCasts();
    ASE = dyn_cast<ArraySubscriptExpr>(BaseE);
    Idxs.push_back(Idx);
  }
  /// To sort from outermost to innermost
  Idxs.reverse();
  return BaseE;
}

void MVExprArray::updateIndex(int UF, std::string LL) {
  auto it = find(this->Idx.begin(), this->Idx.end(), LL);
  if (it != this->Idx.end()) {
    std::string &S(*it);
    S = S + " + " + std::to_string(UF);
  }
  std::string NewStrExpr = this->BaseName;
  for (std::string I : this->Idx) {
    NewStrExpr += "[" + I + "]";
  }
  this->setExprStr(NewStrExpr);
}

//------------------------------------------------
//------------------------------------------------
MVExpr *MVExprLiteral::unrollExpr(int UF, std::string LL) {
  MVExprLiteral *NewExpr = new MVExprLiteral(this);
  return NewExpr;
}
//------------------------------------------------
MVExpr *MVExprVar::unrollExpr(int UF, std::string LL) {
  MVExprVar *NewExpr = new MVExprVar(this);
  return NewExpr;
}
//------------------------------------------------
MVExpr *MVExprArray::unrollExpr(int UF, std::string LL) {
  MVExprArray *NewExpr = new MVExprArray(this);
  NewExpr->updateIndex(UF, LL);
  return NewExpr;
}

/// \deprecated
/// New definition of operator+ in order to ease the unrolling
MVExpr *operator+(const MVExpr &Lhs, int Rhs) {
  MVExpr *NewExpr = new MVExpr(Lhs);
  MVExpr::MVExprInfo TI = NewExpr->getTempInfo();
  switch (TI) {
  case MVExpr::MVExprInfo::TMP_RES:
  case MVExpr::MVExprInfo::TMP_VAL:
    NewExpr->setExprStr("unroll" + std::to_string(Rhs));
    break;
  case MVExpr::MVExprInfo::EXPR_CLANG:
  case MVExpr::MVExprInfo::TAC_EXPR:
  default:
    /// FIXME
    NewExpr->setExprStr(NewExpr->getExprStr() + " + " + std::to_string(Rhs) +
                        " + offset");
    break;
  }

  return NewExpr;
}
} // namespace macveth
