// MACVETH - MVExprFactory.cpp
//
// Copyright (c) Colorado State University. 2019-2021
// Copyright (c) Universidade da Coruña. 2020-2021
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Authors:
//     Marcos Horro <marcos.horro@udc.es>
//     Louis-Noël Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Marcos Horro <marcos.horro@udc.es>

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

  if (dyn_cast<clang::ArraySubscriptExpr>(E->IgnoreImpCasts())) {
    return MVExprType::ARRAY;
  }

  if (dyn_cast<clang::CXXOperatorCallExpr>(E->IgnoreImpCasts())) {
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
