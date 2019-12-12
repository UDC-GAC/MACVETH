/**
 * File              : MVExpr.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 14:18:48 MST
 * Last Modified Date: Xov 12 Dec 2019 10:28:14 MST
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
