/**
 * File              : TAC.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 14:18:48 MST
 * Last Modified Date: Lun 25 Nov 2019 10:57:01 MST
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

#include "include/TAC.h"

#include "clang/Lex/Lexer.h"

using namespace clang;
using namespace macveth;

void TAC::binaryOperator2TAC(const clang::BinaryOperator *S,
                             std::list<TAC> *TacList, int Val) {
  Expr *Lhs = S->getLHS();
  Expr *Rhs = S->getRHS();
  clang::BinaryOperator *LhsBin = NULL;
  clang::BinaryOperator *RhsBin = NULL;
  bool LhsTypeBin = (LhsBin = dyn_cast<clang::BinaryOperator>(Lhs));
  bool RhsTypeBin = (RhsBin = dyn_cast<clang::BinaryOperator>(Rhs));

  /// Since this is a recursive function, we have to test the first case
  TempExpr *TmpA = NULL;
  if (Val == -1) {
    TmpA = new TempExpr(Lhs);
  } else {
    TmpA = new TempExpr(TempExpr::getNameTempReg(Val));
  }
  // recursive part, to delve deeper into BinaryOperators
  if (LhsTypeBin && RhsTypeBin) {
    // both true
    TempExpr *TmpB = new TempExpr(TempExpr::getNameTempReg(Val + 1));
    TempExpr *TmpC = new TempExpr(TempExpr::getNameTempReg(Val + 2));
    TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
    TacList->push_back(NewTac);
    binaryOperator2TAC(RhsBin, TacList, Val + 1);
    binaryOperator2TAC(LhsBin, TacList, Val + 2);
  } else if (!LhsTypeBin && RhsTypeBin) {
    TempExpr *TmpB = new TempExpr(Lhs);
    TempExpr *TmpC = new TempExpr(TempExpr::getNameTempReg(Val + 1));
    TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
    TacList->push_back(NewTac);
    binaryOperator2TAC(RhsBin, TacList, Val + 1);
  } else if (LhsTypeBin && !RhsTypeBin) {
    TempExpr *TmpB = new TempExpr(TempExpr::getNameTempReg(Val + 1));
    TempExpr *TmpC = new TempExpr(Rhs);
    TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
    TacList->push_back(NewTac);
    binaryOperator2TAC(LhsBin, TacList, Val + 1);
  } else {
    TempExpr *TmpB = new TempExpr(Lhs);
    TempExpr *TmpC = new TempExpr(Rhs);
    TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
    TacList->push_back(NewTac);
  }
  return;
}
