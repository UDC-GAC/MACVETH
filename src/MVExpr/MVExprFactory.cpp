/**
 * File					 : /MVExprFactory.cpp
 * Author				 : Marcos Horro
 * Date					 : Wed 26 Feb 2020 06:05 +01:00
 *
 * Last Modified : Wed 10 Jun 2020 10:00 +02:00
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

#include "include/MVExpr/MVExprFactory.h"
#include "include/MVExpr/MVExprArray.h"
#include "include/MVExpr/MVExprLiteral.h"
#include "include/MVExpr/MVExprVar.h"
#include "clang/AST/Expr.h"

//------------------------------------------------
MVExprFactory::MVExprType MVExprFactory::getTempTypeFromExpr(Expr *E) {
  if (E == nullptr) {
    return MVExprType::VARIABLE;
  }

  if (ArraySubscriptExpr *ASE =
          dyn_cast<clang::ArraySubscriptExpr>(E->IgnoreImpCasts())) {
    return MVExprType::ARRAY;
  }

  if (CXXOperatorCallExpr *CXX =
          dyn_cast<clang::CXXOperatorCallExpr>(E->IgnoreImpCasts())) {
    return MVExprType::ARRAY;
  }

  if (Utils::isNumericValue(E)) {
    return MVExprType::LITERAL;
  }

  return MVExprType::VARIABLE;
}

//------------------------------------------------
MVExpr *MVExprFactory::createMVExpr(Expr *E) {
  switch (MVExprFactory::getTempTypeFromExpr(E)) {
  case MVExprFactory::MVExprType::ARRAY:
    return new MVExprArray(E);
  case MVExprFactory::MVExprType::LITERAL:
    return new MVExprLiteral(E);
  case MVExprFactory::MVExprType::VARIABLE:
  default:
    return new MVExprVar(E);
  }
}

//------------------------------------------------
MVExpr *MVExprFactory::createMVExpr(std::string E, bool Temp,
                                    std::string Type) {
  MVExprVar *Ex = new MVExprVar(E, Temp);
  Ex->setTypeStr(Type);
  return Ex;
}
