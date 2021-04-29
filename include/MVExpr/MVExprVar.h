// MACVETH - MVExprVar.h
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
//     Louis-Nöel Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Louis-Nöel Pouchet <pouchet@colostate.edu>

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
