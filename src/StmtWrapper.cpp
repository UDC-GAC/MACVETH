/**
 * File              : StmtWrapper.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 25 Nov 2019 13:48:24 MST
 * Last Modified Date: Lun 13 Xan 2020 15:09:03 MST
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

#include "include/StmtWrapper.h"
#include "include/CustomMatchers.h"
#include "include/TAC.h"
#include "clang/AST/Expr.h"
#include <string>

using namespace clang;
using namespace macveth;
using namespace macveth::matchers_utils;

// ---------------------------------------------
StmtWrapper::LoopList
StmtWrapper::getLoopList(const MatchFinder::MatchResult &Result) {
  LoopList L;
  int n = 1;
  const clang::ForStmt *ForLoop =
      Result.Nodes.getNodeAs<clang::ForStmt>("forLoop" + std::to_string(n));
  while (ForLoop != nullptr) {
    LoopInfo Loop;

    // Get name of variable
    const VarDecl *V = Result.Nodes.getNodeAs<clang::VarDecl>(
        varnames::NameVarInit + std::to_string(n));
    Loop.Dim = V->getNameAsString();

    // Get init val
    const clang::Expr *initializerExpr = V->getInit();
    clang::Expr::EvalResult R;
    if (initializerExpr->EvaluateAsInt(R, *Utils::getCtx())) {
      Loop.InitVal = (long)R.Val.getInt().getExtValue();
    }

    // Get UpperBound
    const Expr *UpperBoundExpr = Result.Nodes.getNodeAs<clang::Expr>(
        varnames::UpperBound + std::to_string(n));
    if (UpperBoundExpr != nullptr) {
      Loop.UpperBound = Utils::getIntFromExpr(UpperBoundExpr, Utils::getCtx());
    }

    // Get step or stride and leftover
    if (Loop.UpperBound != -1) {
      Loop.Step = Loop.UpperBound;
      Loop.LeftOver = 0;
    }

    L.push_back(Loop);
    // Check if next loop
    ForLoop =
        Result.Nodes.getNodeAs<clang::ForStmt>("forLoop" + std::to_string(++n));
  }
  return L;
}

// ---------------------------------------------
void StmtWrapper::unrollAndJam(long UnrollFactor, long UpperBoundFallback) {
  // this->LoopL.reverse();
  for (LoopInfo L : this->LoopL) {
    long UB = L.UpperBound == -1 ? UpperBoundFallback : L.UpperBound;
    this->unroll(UnrollFactor, UB, L.Dim);
  }
}

// ---------------------------------------------
void StmtWrapper::unroll(long UnrollFactor, long UpperBound,
                         std::string LoopLevel) {
  // FIXME
  unsigned int MaskList[] = {0x000000, 0x000000, 0x000000, 0x000000, 0x000000};
  TacListType T = TAC::unrollTacList(this->getTacList(), UnrollFactor,
                                     UpperBound, MaskList, LoopLevel);
  this->setTacList(T);
}
