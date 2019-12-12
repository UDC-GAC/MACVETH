/**
 * File              : MVExpr.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 18 Nov 2019 14:51:25 MST
 * Last Modified Date: Mér 11 Dec 2019 18:30:24 MST
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
#include <iostream>
#include <memory>
#include <string>

using namespace clang;

namespace macveth {

typedef std::list<std::string> IdxVector;

/// This class holds a double identity for expressions: a string name (for
/// debugging purposes and for clarity) and the actual Clang Expr, for
/// transformations. Besides, it also "simplifies" the typing of the expression
/// to some defined types (MVExprType). This will help to calculate offsets
/// and to simplify the Clang AST typing.
class MVExpr {
public:
  /// FIXME: I do not see the use of this
  enum MVExprInfo { EXPR_CLANG, TAC_EXPR, TMP_RES, TMP_VAL };
  virtual ~MVExpr(){};

  /// Constructors
  MVExpr(){};
  MVExpr(MVExpr *TE) {
    this->setExprStr(TE->getExprStr());
    this->setClangExpr(TE->getClangExpr());
    this->setTypeStr(TE->getTypeStr());
    this->setTempInfo(TE->getTempInfo());
  }

  MVExpr(std::string E) : ExprStr(E) { this->TempInfo = TAC_EXPR; }
  MVExpr(std::string E, MVExprInfo TI) : ExprStr(E), TempInfo(TI) {}
  MVExpr(Expr *E) : ClangExpr(E) {
    this->TempInfo = MVExprInfo::EXPR_CLANG;
    this->ExprStr = Utils::getStringFromExpr(E);
    this->setTypeStr(this->getClangExpr()->getType().getAsString());
  }

  void setTempInfo(MVExprInfo TempInfo) { this->TempInfo = TempInfo; }
  MVExprInfo getTempInfo() { return this->TempInfo; }

  void setClangExpr(clang::Expr *ClangExpr) { this->ClangExpr = ClangExpr; }
  clang::Expr *getClangExpr() { return this->ClangExpr; }

  void setTypeStr(std::string TypeStr) { this->TypeStr = TypeStr; }
  std::string getTypeStr() { return this->TypeStr; }

  void setExprStr(std::string ExprStr) { this->ExprStr = ExprStr; }
  std::string getExprStr() { return this->ExprStr; }

  /// This method allows to check whether the expression belongs or not to the
  /// original AST created by Clang
  bool isNotClang() { return (this->getTempInfo() != EXPR_CLANG); }

  /// Given a MVExpr it will return its unrolled version
  virtual MVExpr *unrollExpr(int UF, std::string LL) { return this; }

private:
  MVExprInfo TempInfo = TMP_VAL;
  std::string TypeStr = "double";
  std::string ExprStr = "";
  clang::Expr *ClangExpr = NULL;
};

/// Abstraction of variables for simplicity
class MVExprLiteral : public MVExpr {
public:
  virtual ~MVExprLiteral(){};
  MVExprLiteral(MVExprLiteral *E) : MVExpr(E->getClangExpr()){};
  MVExprLiteral(Expr *E) : MVExpr(E) {}
  virtual MVExpr *unrollExpr(int UF, std::string LL);
};

/// Abstraction of variables for simplicity
class MVExprVar : public MVExpr {
public:
  virtual ~MVExprVar(){};
  MVExprVar(MVExprVar *E) : MVExpr(E->getClangExpr()){};
  MVExprVar(Expr *E) : MVExpr(E) {}
  virtual MVExpr *unrollExpr(int UF, std::string LL);
};

/// Abstraction of arrays for simplicity
class MVExprArray : public MVExpr {
public:
  virtual ~MVExprArray(){};
  MVExprArray(Expr *E) : MVExpr(E) {
    if (ArraySubscriptExpr *ASE =
            dyn_cast<clang::ArraySubscriptExpr>(E->IgnoreImpCasts())) {
      const Expr *TmpExpr = getArrayBaseExprAndIdxs(ASE, this->Idx);
      this->BaseName = Utils::getStringFromExpr(TmpExpr);
    }
  }
  MVExprArray(MVExprArray *E) : MVExpr(E->getClangExpr()) {
    this->BaseName = E->BaseName;
    this->Idx = E->Idx;
  }

  virtual MVExpr *unrollExpr(int UF, std::string LL);

private:
  void updateIndex(int UF, std::string LL);
  /// Get base name and indexes given a ArraySubscriptExpr
  const Expr *getArrayBaseExprAndIdxs(const ArraySubscriptExpr *ASE,
                                      IdxVector &Idxs);

private:
  std::string BaseName;
  IdxVector Idx;
};

class MVExprFactory {
public:
  /// Types available:
  /// * ARRAY, e.g. a[i], b[i][j]
  /// * LITERAL, e.g. 5.0, 42
  /// * VARIABLE, otherwise
  enum MVExprType { ARRAY, LITERAL, VARIABLE };

  static MVExprType getTempTypeFromExpr(Expr *E) {
    if (E == NULL)
      return MVExprType::VARIABLE;
    if (ArraySubscriptExpr *ASE =
            dyn_cast<clang::ArraySubscriptExpr>(E->IgnoreImpCasts())) {
      return MVExprType::ARRAY;
    }

    if (Utils::isNumericValue(E)) {
      return MVExprType::LITERAL;
    }

    return MVExprType::VARIABLE;
  }

  static MVExpr *createMVExpr(Expr *E) {
    MVExprType T = MVExprFactory::getTempTypeFromExpr(E);
    switch (T) {
    case ARRAY:
      return new MVExprArray(E);
    case LITERAL:
      return new MVExprLiteral(E);
    case VARIABLE:
    default:
      return new MVExprVar(E);
    }
  }
};

/// Operators
MVExpr *operator+(const MVExpr &Lhs, int Rhs);

} // namespace macveth
#endif
