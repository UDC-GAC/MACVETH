/**
 * File              : TempExpr.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 18 Nov 2019 14:51:25 MST
 * Last Modified Date: Lun 25 Nov 2019 11:01:09 MST
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

#ifndef MACVETH_TAC_H
#define MACVETH_TAC_H

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
  TempExpr(std::string E) : ExprStr(E) {}
  TempExpr(Expr *E) : ClangExpr(E) {
    this->ExprStr = Utils::getStringFromExpr(E);
  }

  void setClangExpr(clang::Expr *ClangExpr) { this->ClangExpr = ClangExpr; }
  clang::Expr *getClangExpr() { return this->ClangExpr; }

  void setTypeStr(std::string TypeStr) { this->TypeStr = TypeStr; }
  std::string getTypeStr() { return this->TypeStr; }

  void setExprStr(std::string ExprStr) { this->ExprStr = ExprStr; }
  std::string getExprStr() { return this->ExprStr; }

  bool isNotClang() { return (this->ClangExpr == NULL); }

  // dumb method
  static std::string getNameTempReg(int Val) {
    return "t" + std::to_string(Val);
  }

private:
  std::string TypeStr = "double";
  std::string ExprStr;
  clang::Expr *ClangExpr = NULL;
};

} // namespace macveth
#endif
