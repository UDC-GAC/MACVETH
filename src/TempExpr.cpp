/**
 * File              : TempExpr.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 14:18:48 MST
 * Last Modified Date: Dom 08 Dec 2019 18:52:35 MST
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
    NewExpr->setExprStr(NewExpr->getExprStr() + " + " + std::to_string(Rhs));
    break;
  }

  return NewExpr;
}
} // namespace macveth
