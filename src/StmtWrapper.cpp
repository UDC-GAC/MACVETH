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
    LoopInfo(Loop);

    Utils::printDebug("LoopInfo", "parsing vardecl...");
    // Get name of variable
    const VarDecl *V = Result.Nodes.getNodeAs<clang::VarDecl>(
        varnames::NameVarInit + std::to_string(n));
    Loop.Declared = (V != nullptr);
    Utils::printDebug("LoopInfo", std::to_string(Loop.Declared) + " and " +
                                      std::to_string((V == nullptr)));
    if (Loop.Declared) {
      Utils::printDebug("LoopInfo", V->getNameAsString());
      Loop.Dim = V->getNameAsString();
    } else {
      const DeclRefExpr *VN = Result.Nodes.getNodeAs<DeclRefExpr>(
          varnames::NameVarInitNotDeclared + std::to_string(n));
      Loop.Dim = Utils::getStringFromExpr(VN);
      Utils::printDebug("LoopInfo", "value got from DeclRefExpr = " + Loop.Dim);
    }

    // Get init val
    const DeclRefExpr *NameValInit = Result.Nodes.getNodeAs<DeclRefExpr>(
        varnames::NameValInit + std::to_string(n));
    if (NameValInit != nullptr) {
      Utils::printDebug("LoopInfo", "NameValInt not NULL");
      clang::Expr::EvalResult R;
      if (NameValInit->EvaluateAsInt(R, *Utils::getCtx())) {
        Loop.InitVal = (long)R.Val.getInt().getExtValue();
      }
    } else {
      Utils::printDebug("LoopInfo", "NameValInt NULL");
      const Expr *ValInit =
          Result.Nodes.getNodeAs<Expr>(varnames::ValInit + std::to_string(n));
      if (ValInit != nullptr) {
        Loop.InitVal = Utils::getIntFromExpr(ValInit, Utils::getCtx());
      }
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
    Utils::printDebug("StmtWrapper", Loop.toString());
    // Check if next loop
    ForLoop =
        Result.Nodes.getNodeAs<clang::ForStmt>("forLoop" + std::to_string(++n));
  }
  return L;
}

// ---------------------------------------------
void StmtWrapper::unrollAndJam(long UnrollFactor, long UpperBoundFallback) {
  // IMPORTANT
  this->LoopL.reverse();
  for (LoopInfo L : this->LoopL) {
    long UB = L.UpperBound == -1 ? UpperBoundFallback : L.UpperBound;
    this->unroll(UnrollFactor, UB, L.Dim);
  }
}

// ---------------------------------------------
void StmtWrapper::unroll(long UnrollFactor, long UpperBound,
                         std::string LoopLevel) {
  TacListType TL;
  int i = 0;
  for (auto TempTL : this->ListOfTAC) {
    TacListType StmtTL;
    for (auto T :
         TAC::unrollTacList(TempTL, UnrollFactor, UpperBound, LoopLevel)) {
      TL.push_back(T);
      StmtTL.push_back(T);
    }
    this->ListOfTAC[i++] = StmtTL;
  }
  this->setTacList(TL);
}
