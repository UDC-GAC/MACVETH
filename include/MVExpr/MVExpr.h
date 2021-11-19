// MACVETH - MVExpr.h
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

#ifndef MACVETH_MVEXPR_H
#define MACVETH_MVEXPR_H

#include "include/Debug.h"
#include "include/MVAssert.h"
#include "include/MVExpr/MVDataType.h"
#include "include/Utils.h"
#include "clang/AST/AST.h"
#include "clang/AST/Expr.h"
#include "llvm/Support/Casting.h"
#include <iostream>
#include <memory>
#include <string>

using namespace clang;
using namespace macveth;

namespace macveth {

/// Expressions in MACVETH inherit from MVExpr
///
/// This class holds a double identity for expressions: a string name (for
/// debugging purposes and for clarity) and the actual Clang Expr, for
/// transformations. Besides, it also "simplifies" the typing of the expression
/// to some defined types (MVExprType). This will help to calculate offsets
/// and to simplify the Clang AST typing.
class MVExpr {
public:
  /// Types of MVExpr
  enum MVExprKind {
    /// Defined in MVExprArray
    MVK_Array,
    /// Defined in MVExprLiteral
    MVK_Literal,
    /// Defined in MVExprVar
    MVK_Var,
    /// Other
    MVK_Other
  };

  /// A MVExpr can be created from a clang::Expr or from a string, it can be an
  /// array a literal or even a variable, but when it comes to unroll, it is not
  /// the same if it is a temporal expression created ad-hoc (e.g. tempX
  /// register which holds a temporal value in the TAC representation). This
  /// way, we can distinguish those cases.
  enum MVExprInfo { EXPR_CLANG, TAC_EXPR, TMP_RES };

  /// Destructor
  virtual ~MVExpr(){};

  /// Clone constructor
  MVExpr(MVExpr *TE) {
    this->setExprStr(TE->getExprStr());
    this->setClangExpr(TE->getClangExpr());
    this->setTypeStr(TE->getTypeStr());
    this->setTempInfo(TE->getTempInfo());
    this->setKind(TE->getKind());
  }

  /// Create MVExpr from string. In this case we will assume we are treating
  /// with a TAC_EXPR
  MVExpr(MVExprKind K, std::string E) : MK(K), ExprStr(E) {
    this->TempInfo = TAC_EXPR;
  }

  /// Create MVExpr from clang::Expr, which sets its inner type/info to
  /// EXPR_CLANG
  MVExpr(MVExprKind K, Expr *E) : MK(K), ClangExpr(E) {
    this->TempInfo = MVExprInfo::EXPR_CLANG;
    this->ExprStr = Utils::getStringFromExpr(E);
    if (isa<clang::TypedefType>(E->getType())) {
      auto T = dyn_cast<clang::TypedefType>(E->getType());
      if (!T->isSugared()) {
        MACVETH_DEBUG(
            "MVExpr",
            "This is not 'sugared'... That is an error... Unreachable");
        llvm::llvm_unreachable_internal();
      }
      auto ET = dyn_cast<ElaboratedType>(T->desugar());
      auto TT = dyn_cast<TypedefType>(ET->desugar());
      auto Subs = dyn_cast<SubstTemplateTypeParmType>(TT->desugar());
      this->setTypeStr(Subs->desugar().getAsString());
    } else {
      this->setTypeStr(E->getType().getAsString());
    }
    this->MVDT = MVDataType::CTypeToVDataType[this->getTypeStr()];
  }

  /// Set info
  void setTempInfo(MVExprInfo TempInfo) { this->TempInfo = TempInfo; }

  /// Get MVExpr info
  MVExprInfo getTempInfo() { return this->TempInfo; }

  /// Set clang::Expr attribute
  void setClangExpr(clang::Expr *ClangExpr) { this->ClangExpr = ClangExpr; }

  /// Get clang::Expr attribute
  clang::Expr *getClangExpr() const { return this->ClangExpr; }

  /// Set type as a string
  void setTypeStr(std::string TypeStr) {
    // This is a fucking hack: treat const as if they are not const...
    // Thus, typing is easier. Do we really need to know if a variable is
    // constant or not? I think we do not.
    auto LastNotNullptrToken = TypeStr;
    char *dup = strdup(TypeStr.c_str());
    auto Tok = strtok(dup, " ");
    while (Tok != nullptr) {
      LastNotNullptrToken = Tok;
      Tok = strtok(nullptr, " ");
    }
    this->TypeStr = LastNotNullptrToken;
    // At least be clean
    free(dup);
  }

  /// Get type as a string
  std::string getTypeStr() { return this->TypeStr; }

  /// Set expression as an string
  void setExprStr(std::string ExprStr) { this->ExprStr = ExprStr; }

  /// To string function
  virtual std::string toString() const = 0;

  /// Get expression as an string
  std::string getExprStr() const { return this->toString(); }

  /// Get the expression as a "raw" string. Should avoid this if the object is
  /// volatile meaning for that it _can_ change, e.g. a[i] when parsed can be
  /// converted as a string "a[i]". However, if we unroll the value then we
  /// will still have "a[i]" instead of "a[i+unroll]". That is why toString()
  /// is way more powerful
  std::string getExprStrRaw() const { return this->ExprStr; }

  /// Set kind of the expression
  void setKind(MVExprKind Kind) { this->MK = Kind; }

  /// Get kind of the expression
  MVExprKind getKind() const { return this->MK; }

  /// This method allows to check whether the expression belongs or not to the
  /// original AST created by Clang
  bool isNotClang() { return (this->getTempInfo() != EXPR_CLANG); }

  /// Given a MVExpr it will return its unrolled version
  virtual MVExpr *unrollExpr(int UF, std::string LL,
                             bool SubstituteVal = false) = 0;

  /// Given a MVExpr and the map of unrolled,loop_level, it will return the
  /// unrolled version
  virtual MVExpr *unrollExpr(std::unordered_map<int, std::string> LList) {
    return this;
  }

  /// If this function is called, then the expression is in memory
  void setLoadFromMem() { this->NeedsMemLoad = true; }

  /// If this function is called, then the expression is not in memory
  void setDoNotLoadFromMem() { this->NeedsMemLoad = false; }

  /// Check if expression is in memory or not
  bool needsToBeLoaded() { return this->NeedsMemLoad; }

  /// Two expressions are equal if and only if their name or original code
  /// expression is equal, regardless where they appear in code.
  bool operator==(const MVExpr &MVE) const {
    return !getExprStr().compare(MVE.getExprStr());
  }

  /// Two expressions are not equal if and only if their name or original code
  /// expression is not equal.
  bool operator!=(MVExpr &MVE) { return !operator==(MVE); }

  /// Operators
  virtual int operator-(const MVExpr &MVE) { return -1; }

  virtual bool operator<(const MVExpr &MVE) { return false; }

private:
  /// MVExpr kind
  MVExprKind MK;
  /// Type of MVExpr
  MVExprInfo TempInfo = TMP_RES;
  /// Type of data in string
  std::string TypeStr = "";
  /// MACVETH's datatype
  MVDataType::VDataType MVDT = MVDataType::VDataType::UNDEF;
  /// Expresion as Clang's
  clang::Expr *ClangExpr = nullptr;
  /// Expression as string
  std::string ExprStr = "";
  /// Need to be loaded from mem
  bool NeedsMemLoad = true;
  /// Dimensions of the expressions
  std::list<std::string> Dims;
};

} // namespace macveth
#endif /* !MACVETH_MVEXPR_H */
