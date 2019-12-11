/**
 * File              : TempExpr.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 14:18:48 MST
 * Last Modified Date: Mér 11 Dec 2019 11:07:28 MST
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

#include "include/TempExpr.h"
#include <string>

using namespace clang;
using namespace macveth;

namespace macveth {

TempExpr::TempExprType TempExpr::getTempTypeFromExpr(Expr *E) {
  if (ArraySubscriptExpr *ASE =
          dyn_cast<clang::ArraySubscriptExpr>(E->IgnoreImpCasts())) {
    const Expr *TmpExpr = getArrayBaseExprAndIdxs(ASE, this->ArrInfo.Idx);
    this->ArrInfo.BaseName = Utils::getStringFromExpr(TmpExpr);
    return TempExprType::ARRAY;
  }

  return TBD;
}

/// Given a ArraySubscriptExpr, recursively gets the base name and the indexes
/// from the outermost to the innermost
const Expr *TempExpr::getArrayBaseExprAndIdxs(const ArraySubscriptExpr *ASE,
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

void TempExpr::updateIndex(int UF, std::string LL) {
  if (this->getTempType() == TempExpr::TempExprType::ARRAY) {
    auto it = find(this->ArrInfo.Idx.begin(), this->ArrInfo.Idx.end(), LL);
    if (it != this->ArrInfo.Idx.end()) {
      std::string &S(*it);
      S = S + " + " + std::to_string(UF);
    }
    std::string NewStrExpr = this->ArrInfo.BaseName;
    for (std::string I : this->ArrInfo.Idx) {
      NewStrExpr += "[" + I + "]";
    }
    this->setExprStr(NewStrExpr);
  }
}

TempExpr *TempExpr::unrollTemp(const TempExpr &TE, int UF, std::string LL) {
  TempExpr *NewExpr = new TempExpr(TE);
  if (NewExpr->getTempType() == TempExpr::TempExprType::ARRAY) {
    NewExpr->updateIndex(UF, LL);
  }
  return NewExpr;
}

/// \deprecated
/// New definition of operator+ in order to ease the unrolling
TempExpr *operator+(const TempExpr &Lhs, int Rhs) {
  TempExpr *NewExpr = new TempExpr(Lhs);
  TempExpr::TempExprInfo TI = NewExpr->getTempInfo();
  switch (TI) {
  case TempExpr::TempExprInfo::TMP_RES:
  case TempExpr::TempExprInfo::TMP_VAL:
    NewExpr->setExprStr("unroll" + std::to_string(Rhs));
    break;
  case TempExpr::TempExprInfo::EXPR_CLANG:
  case TempExpr::TempExprInfo::TAC_EXPR:
  default:
    /// FIXME
    NewExpr->setExprStr(NewExpr->getExprStr() + " + " + std::to_string(Rhs) +
                        " + offset");
    break;
  }

  return NewExpr;
}
} // namespace macveth
