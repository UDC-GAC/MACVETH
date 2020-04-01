/**
 * File              : MVExpr.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 18 Nov 2019 14:51:25 MST
 * Last Modified Date: Mér 15 Xan 2020 12:07:57 MST
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

#ifndef MACVETH_MVEXPR_H
#define MACVETH_MVEXPR_H

#include "include/Utils.h"
#include "clang/AST/AST.h"
#include "clang/AST/Expr.h"
#include "llvm/Support/Casting.h"
#include <iostream>
#include <memory>
#include <string>

using namespace clang;

namespace macveth {

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
    /// Defined in MVExprFunc
    MVK_Function,
    /// Defined in MVExprVar
    MVK_Var
  };

  /// A MVExpr can be created from a clang::Expr or from a string, it can be an
  /// array a literal or even a variable, but when it comes to unroll, it is not
  /// the same if it is a temporal expression created ad-hoc (e.g. tempX
  /// register which holds a temporal value in the TAC representation). This
  /// way, we can distinguish those cases.
  enum MVExprInfo { EXPR_CLANG, TAC_EXPR, TMP_RES };

  /// Destructor
  virtual ~MVExpr(){};

  /// Empty constructor
  MVExpr(){};
  MVExpr(MVExprKind MK) : MK(MK){};
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
  /// Create MVExpr from string and set ad-hoc MVExprInfo
  MVExpr(std::string E, MVExprInfo TI) : ExprStr(E), TempInfo(TI) {}
  /// Create MVExpr from clang::Expr, which sets its inner type/info to
  /// EXPR_CLANG
  MVExpr(MVExprKind K, Expr *E) : MK(K), ClangExpr(E) {
    this->TempInfo = MVExprInfo::EXPR_CLANG;
    this->ExprStr = Utils::getStringFromExpr(E);
    this->setTypeStr(this->getClangExpr()->getType().getAsString());
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
  void setTypeStr(std::string TypeStr) { this->TypeStr = TypeStr; }
  /// Get type as a string
  std::string getTypeStr() { return this->TypeStr; }

  /// Set expression as an string
  void setExprStr(std::string ExprStr) { this->ExprStr = ExprStr; }
  /// Get expression as an string
  std::string getExprStr() const { return this->ExprStr; }

  /// Set kind of the expression
  void setKind(MVExprKind Kind) { this->MK = Kind; }
  /// Get kind of the expression
  MVExprKind getKind() const { return this->MK; }

  /// This method allows to check whether the expression belongs or not to the
  /// original AST created by Clang
  bool isNotClang() { return (this->getTempInfo() != EXPR_CLANG); }

  /// Given a MVExpr it will return its unrolled version
  virtual MVExpr *unrollExpr(int UF, std::string LL) { return this; }
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

private:
  /// MVExpr kind
  MVExprKind MK;
  /// Type of MVExpr
  MVExprInfo TempInfo = TMP_RES;
  /// Type of data in string
  std::string TypeStr = "double";
  /// Expression as string
  std::string ExprStr = "";
  /// Expresion as Clang's
  clang::Expr *ClangExpr = NULL;
  /// Need to be loaded from mem
  bool NeedsMemLoad = true;
};

/// Operators
MVExpr *operator+(const MVExpr &Lhs, int Rhs);

} // namespace macveth
#endif
