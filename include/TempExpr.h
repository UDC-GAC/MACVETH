/**
 * File              : TempExpr.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 18 Nov 2019 14:51:25 MST
 * Last Modified Date: Mar 26 Nov 2019 10:25:50 MST
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
#include <iostream>
#include <string>

using namespace clang;

namespace macveth {
// This class holds a double identity for expressions: a string name (for
// debugging purposes and for clarity) and the actual Clang Expr, for
// transformations
class TempExpr {
public:
  enum TempExprType { ARRAY, TEMP_RES, LITERAL, VARIABLE, POINTER };

  /// Constructors
  TempExpr(){};
  TempExpr(TempExpr *TE) {
    this->setExprStr(TE->getExprStr());
    this->setClangExpr(TE->getClangExpr());
    this->setTypeStr(TE->getTypeStr());
    this->setTempType(TE->getTempType());
  }
  TempExpr(std::string E) : ExprStr(E) { this->TempType = TEMP_RES; }
  TempExpr(Expr *E) : ClangExpr(E) {
    this->ExprStr = Utils::getStringFromExpr(E);
    this->setTypeStr(this->getClangExpr()->getType().getAsString());
  }

  /// Getters and setters
  void setTempType(TempExprType TempType) { this->TempType = TempType; }
  TempExprType getTempType() { return this->TempType; }

  void setClangExpr(clang::Expr *ClangExpr) { this->ClangExpr = ClangExpr; }
  clang::Expr *getClangExpr() { return this->ClangExpr; }

  void setTypeStr(std::string TypeStr) { this->TypeStr = TypeStr; }
  std::string getTypeStr() { return this->TypeStr; }

  void setExprStr(std::string ExprStr) { this->ExprStr = ExprStr; }
  std::string getExprStr() { return this->ExprStr; }

  /// This method allows to check whether the expression belongs or not to the
  /// original AST created by Clang
  bool isNotClang() { return (this->ClangExpr == NULL); }

  /// Get name of TempReg given a value
  static std::string getNameTempReg(int Val) {
    return "t" + std::to_string(Val);
  }

private:
  TempExprType TempType = POINTER;
  std::string TypeStr = "double";
  std::string ExprStr;
  clang::Expr *ClangExpr = NULL;
};

} // namespace macveth
#endif
