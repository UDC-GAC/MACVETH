// MACVETH - MVExprLiteral.h
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

#ifndef MACVETH_MVEXPRLITERAL_H
#define MACVETH_MVEXPRLITERAL_H

#include "include/MVExpr/MVExpr.h"

using namespace macveth;

namespace macveth {

/// Abstraction of variables for simplicity
class MVExprLiteral : public MVExpr {
public:
  /// Destructor
  virtual ~MVExprLiteral(){};

  /// Clone
  MVExprLiteral(MVExprLiteral *E) : MVExpr(MVK_Literal, E->getClangExpr()){};

  /// Basic constructor
  MVExprLiteral(Expr *E) : MVExpr(MVK_Literal, E) {}

  /// Unroll expression
  virtual MVExpr *unrollExpr(int UF, std::string LL,
                             bool SubstituteVal = false);

  /// To string
  virtual std::string toString() const {
    return Utils::getStringFromExpr(this->getClangExpr());
  }

  /// In order to be able to use RTTI
  static bool classof(const MVExpr *S) { return S->getKind() == MVK_Literal; }
};

} // namespace macveth
#endif /* !MACVETH_MVEXPRLITERAL_H */
