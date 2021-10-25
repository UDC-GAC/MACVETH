// MACVETH - MVExprFactory.h
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

#ifndef MACVETH_MVEXPRFACTORY_H
#define MACVETH_MVEXPRFACTORY_H

#include "include/MVExpr/MVExpr.h"

using namespace macveth;

namespace macveth {

/// Create MVExpr depending on the dynamic cast clang does
class MVExprFactory {
public:
  /// Types available:
  /// * ARRAY, e.g. a[i], b[i][j]
  /// * LITERAL, e.g. 5.0, 42
  /// * VARIABLE, otherwise
  enum MVExprType { ARRAY, LITERAL, VARIABLE };

  /// Detect the type of MVExpr
  static MVExprType getTempTypeFromExpr(Expr *E);
  /// Create MVExpr from Clang expression
  static MVExpr *createMVExpr(Expr *E);
  /// Create temporal MVExpr (as MVExprVar)
  static MVExpr *createMVExpr(std::string E, bool Temp, std::string Type);
};
} // namespace macveth
#endif /* !MACVETH_MVEXPRFACTORY_H */
