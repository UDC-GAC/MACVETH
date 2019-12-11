/**
 * File              : TempExpr.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 18 Nov 2019 14:51:25 MST
 * Last Modified Date: Mér 11 Dec 2019 15:09:22 MST
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

#ifndef MACVETH_TEMPEXPR_H
#define MACVETH_TEMPEXPR_H

#include "include/Utils.h"
#include "clang/AST/AST.h"
#include "clang/AST/Expr.h"
#include <iostream>
#include <string>

using namespace clang;

namespace macveth {

typedef std::list<std::string> IdxVector;

/// This class holds a double identity for expressions: a string name (for
/// debugging purposes and for clarity) and the actual Clang Expr, for
/// transformations. Besides, it also "simplifies" the typing of the expression
/// to some defined types (TempExprType). This will help to calculate offsets
/// and to simplify the Clang AST typing.
class TempExpr {
public:
  /// Types available:
  /// * ARRAY, e.g. a[i], b[i][j]
  /// * TEMP_RES, for those temporal auxiliary
  enum TempExprType { ARRAY, LITERAL, VARIABLE, POINTER, TBD };
  enum TempExprInfo { EXPR_CLANG, TAC_EXPR, TMP_RES, TMP_VAL };

  /// This only holds information in case of the expression being of ARRAY type
  struct ArrayInfo {
    std::string BaseName = "";
    IdxVector Idx;
  };
  void updateIndex(int UF, std::string LL);
  const Expr *getArrayBaseExprAndIdxs(const ArraySubscriptExpr *ASE,
                                      IdxVector &Idxs);

  /// Constructors
  TempExpr(){};
  TempExpr(TempExpr *TE) {
    this->setExprStr(TE->getExprStr());
    this->setClangExpr(TE->getClangExpr());
    this->setTypeStr(TE->getTypeStr());
    this->setTempType(TE->getTempType());
    this->setTempInfo(TE->getTempInfo());
  }

  TempExpr(std::string E) : ExprStr(E) { this->TempInfo = TAC_EXPR; }
  TempExpr(std::string E, TempExprInfo TI) : ExprStr(E), TempInfo(TI) {}
  TempExpr(std::string E, TempExprType TE) : ExprStr(E), TempType(TE) {}
  TempExpr(std::string E, TempExprType TE, TempExprInfo TI)
      : ExprStr(E), TempType(TE), TempInfo(TI) {}
  TempExpr(Expr *E) : ClangExpr(E) {
    this->TempType = getTempTypeFromExpr(E);
    this->TempInfo = TempExprInfo::EXPR_CLANG;
    this->ExprStr = Utils::getStringFromExpr(E);
    this->setTypeStr(this->getClangExpr()->getType().getAsString());
  }

  /// Given a Clang expression we get back our custom interpretation
  TempExprType getTempTypeFromExpr(Expr *E);

  /// Getters and setters
  void setTempType(TempExprType TempType) { this->TempType = TempType; }
  TempExprType getTempType() { return this->TempType; }

  void setTempInfo(TempExprInfo TempInfo) { this->TempInfo = TempInfo; }
  TempExprInfo getTempInfo() { return this->TempInfo; }

  void setClangExpr(clang::Expr *ClangExpr) { this->ClangExpr = ClangExpr; }
  clang::Expr *getClangExpr() { return this->ClangExpr; }

  void setTypeStr(std::string TypeStr) { this->TypeStr = TypeStr; }
  std::string getTypeStr() { return this->TypeStr; }

  void setExprStr(std::string ExprStr) { this->ExprStr = ExprStr; }
  std::string getExprStr() { return this->ExprStr; }

  /// Given a TempExpr it will return its unrolled version
  static TempExpr *unrollTemp(const TempExpr &TE, int UF, std::string LL);

  /// FIXME
  /// THIS SHOULD BE REMOVED AT SOME POINT...
  /// This method allows to check whether the expression belongs or not to the
  /// original AST created by Clang
  bool isNotClang() { return (this->getTempInfo() != EXPR_CLANG); }

private:
  ArrayInfo ArrInfo;
  TempExprInfo TempInfo = TMP_VAL;
  TempExprType TempType = POINTER;
  std::string TypeStr = "double";
  std::string ExprStr = "";
  clang::Expr *ClangExpr = NULL;
};

/// Operators
TempExpr *operator+(const TempExpr &Lhs, int Rhs);

} // namespace macveth
#endif
