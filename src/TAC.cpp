/**
 * File              : TAC.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 14:18:48 MST
 * Last Modified Date: Lun 13 Xan 2020 19:56:09 MST
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
#include "include/CDAG.h"
#include "include/MVExpr/MVExprFactory.h"
#include "clang/AST/Expr.h"
#include "clang/Lex/Lexer.h"
#include <llvm-10/llvm/Support/ErrorHandling.h>

using namespace clang;
using namespace macveth;

// ---------------------------------------------
std::string getNameTempReg() { return "r" + std::to_string(TAC::RegVal); }

// ---------------------------------------------
std::string getCurrentNameTempReg() {
  return "r" + std::to_string(TAC::RegVal++);
}

// ---------------------------------------------
MVOp TAC::getMVOPfromExpr(MVExpr *E) {
  if (MVExprFunc *F = dyn_cast<MVExprFunc>(E)) {
    return MVOp(F->getExprStr());
  }
  return MVOp("");
}

// ---------------------------------------------
clang::BinaryOperator *getBinOp(clang::Expr *E) {
  clang::BinaryOperator *B =
      dyn_cast<clang::BinaryOperator>(E->IgnoreImpCasts());
  clang::ParenExpr *P = dyn_cast<clang::ParenExpr>(E->IgnoreImpCasts());
  while ((P = dyn_cast<clang::ParenExpr>(E->IgnoreImpCasts()))) {
    if ((B = dyn_cast<clang::BinaryOperator>(
             P->getSubExpr()->IgnoreImpCasts()))) {
      break;
    }
    P = dyn_cast<clang::ParenExpr>(P->getSubExpr()->IgnoreImpCasts());
  }

  return B;
}

// ---------------------------------------------
void TAC::exprToTAC(clang::CompoundStmt *CS, std::vector<Stmt *> *SList,
                    std::list<TAC> *TacList) {
  TAC::RegVal = 0;
  for (auto ST : CS->body()) {
    clang::Expr *S = dyn_cast<clang::Expr>(ST);
    // If the Statement can not be converted onto an Expr, then we are not
    // interested on in: maybe it is a loop that will be parsed using the AST
    // matchers
    if (S == NULL) {
      continue;
    }

    // If the Statement can be parsed and replaced with TACs, then we add it
    SList->push_back(ST);

    clang::BinaryOperator *SBin = NULL;
    bool STypeBin = (SBin = getBinOp(S->IgnoreImpCasts()));

    // Check if binary expression
    std::list<TAC> TL;
    if (STypeBin) {
      binaryOperator2TAC(SBin, &TL, -1);
      for (auto T : TL) {
        TacList->push_back(T);
      }
      continue;
    }
    // New TAC will look like: TmpA = MVExpr(S) OP NULL; (not binary)
    MVExpr *TmpA = MVExprFactory::createMVExpr(getNameTempReg(), true);
    MVExpr *TmpB = MVExprFactory::createMVExpr(S);
    MVOp OP = TAC::getMVOPfromExpr(TmpB);
    TAC NewTac = TAC(TmpA, TmpB, NULL, OP);
    TacList->push_front(NewTac);
  }
}

// ---------------------------------------------
void TAC::binaryOperator2TAC(const clang::BinaryOperator *S,
                             std::list<TAC> *TacList, int Val) {
  Expr *Lhs = S->getLHS();
  Expr *Rhs = S->getRHS();
  clang::BinaryOperator *LhsBin = NULL;
  clang::BinaryOperator *RhsBin = NULL;
  bool LhsTypeBin = (LhsBin = getBinOp(Lhs->IgnoreImpCasts()));
  bool RhsTypeBin = (RhsBin = getBinOp(Rhs->IgnoreImpCasts()));

  /// Since this is a recursive function, we have to test the first case
  MVExpr *TmpA =
      (Val == -1) ? MVExprFactory::createMVExpr(Lhs)
                  : MVExprFactory::createMVExpr(getCurrentNameTempReg(), true);

  // recursive part, to delve deeper into BinaryOperators
  if (LhsTypeBin && RhsTypeBin) {
    // both true
    MVExpr *TmpB = MVExprFactory::createMVExpr(getNameTempReg(), true);
    MVExpr *TmpC = MVExprFactory::createMVExpr(getNameTempReg());
    TAC NewTac = TAC(TmpA, TmpB, TmpC, MVOp(S->getOpcode()));

    TacList->push_front(NewTac);
    binaryOperator2TAC(RhsBin, TacList, Val + 1);
    binaryOperator2TAC(LhsBin, TacList, Val + 2);
  } else if (!LhsTypeBin && RhsTypeBin) {
    MVExpr *TmpB = MVExprFactory::createMVExpr(Lhs);
    MVExpr *TmpC = MVExprFactory::createMVExpr(getNameTempReg(), true);
    TAC NewTac = TAC(TmpA, TmpB, TmpC, MVOp(S->getOpcode()));
    TacList->push_front(NewTac);
    binaryOperator2TAC(RhsBin, TacList, Val + 1);
  } else if (LhsTypeBin && !RhsTypeBin) {
    MVExpr *TmpB = MVExprFactory::createMVExpr(getNameTempReg(), true);
    MVExpr *TmpC = MVExprFactory::createMVExpr(Rhs);
    TAC NewTac = TAC(TmpA, TmpB, TmpC, MVOp(S->getOpcode()));
    TacList->push_front(NewTac);
    binaryOperator2TAC(LhsBin, TacList, Val + 1);
  } else {
    MVExpr *TmpB = MVExprFactory::createMVExpr(Lhs);
    MVExpr *TmpC = MVExprFactory::createMVExpr(Rhs);
    TAC NewTac = TAC(TmpA, TmpB, TmpC, MVOp(S->getOpcode()));
    TacList->push_front(NewTac);
  }
  return;
}

// ---------------------------------------------
TAC *TAC::unroll(TAC *Tac, int UnrollFactor, int S, unsigned int mask,
                 std::string LoopLevel) {
  int UnrollA = S * UnrollFactor +
                UnrollFactor * ((mask & TAC::MASK_OP_A) >> TAC::BITS_OP_A);
  int UnrollB = S * UnrollFactor +
                UnrollFactor * ((mask & TAC::MASK_OP_B) >> TAC::BITS_OP_B);
  int UnrollC = S * UnrollFactor +
                UnrollFactor * ((mask & TAC::MASK_OP_C) >> TAC::BITS_OP_C);

  TAC *UnrolledTac = new TAC(
      Tac->getA()->unrollExpr(UnrollA, LoopLevel),
      Tac->getB()->unrollExpr(UnrollB, LoopLevel),
      Tac->getC() != NULL ? Tac->getC()->unrollExpr(UnrollC, LoopLevel) : NULL,
      Tac->getMVOP());
  return UnrolledTac;
}

// ---------------------------------------------
std::list<TAC> TAC::unrollTacList(std::list<TAC> TacList, int UnrollFactor,
                                  int UpperBound, unsigned int MaskList[],
                                  std::string LoopLevel) {
  std::list<TAC> TacListOrg;
  int Steps = UpperBound / UnrollFactor;
  for (TAC Tac : TacList) {
    for (int S = 0; S < Steps; ++S) {
      // FIXME
      int Mask = 0;
      TacListOrg.push_back(
          *(TAC::unroll(&Tac, UnrollFactor, S, 0x000000, LoopLevel)));
    }
  }
  return TacListOrg;
}
