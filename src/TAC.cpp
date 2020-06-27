/**
 * File					 : src/TAC.cpp
 * Author				 : Marcos Horro
 * Date					 : Wed 03 Jun 2020 04:24 +02:00
 *
 * Last Modified : Wed 10 Jun 2020 10:21 +02:00
 * Modified By	 : Marcos Horro (marcos.horro@udc.gal>)
 *
 * MIT License
 *
 * Copyright (c) 2020 Colorado State University
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
#include "include/MVExpr/MVExprArray.h"
#include "include/MVExpr/MVExprFactory.h"
#include "include/MVExpr/MVExprLiteral.h"
#include "include/MVExpr/MVExprVar.h"
#include "clang/AST/Expr.h"
#include "clang/Lex/Lexer.h"
#include <llvm-10/llvm/Support/ErrorHandling.h>

using namespace clang;
using namespace macveth;

// ---------------------------------------------
std::string getNameTempReg() { return "r" + std::to_string(TAC::RegVal); }

// ---------------------------------------------
std::string getNameTempReg(int Val) { return "r" + std::to_string(Val); }

// ---------------------------------------------
std::string getCurrentNameTempReg() {
  return "r" + std::to_string(TAC::RegVal++);
}

// ---------------------------------------------
clang::BinaryOperator *getBinOp(clang::Expr *E) {
  auto B = dyn_cast<clang::BinaryOperator>(E->IgnoreImpCasts());
  auto P = dyn_cast<clang::ParenExpr>(E->IgnoreImpCasts());
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
bool isTerminal(const clang::Stmt *ST) {
  auto S = dyn_cast<BinaryOperator>(ST);
  auto F = dyn_cast<CallExpr>(ST);
  auto Op = dyn_cast<CXXOperatorCallExpr>(ST);

  return !(S || (F && !Op));
}

// ---------------------------------------------
void stmtToTACRecursive(const clang::Stmt *ST, std::list<TAC> *TacList,
                        int Val) {
  Expr *S1 = nullptr;
  Expr *S2 = nullptr;
  bool IsTerminalS1 = false;
  bool IsTerminalS2 = false;
  bool IsUnary = false;
  MVOp Op;
  auto S = dyn_cast<BinaryOperator>(ST);
  auto F = dyn_cast<CallExpr>(ST);
  if (S) {
    S1 = S->getLHS()->IgnoreImpCasts();
    S2 = S->getRHS()->IgnoreImpCasts();
    clang::BinaryOperator *LhsBin = NULL;
    clang::BinaryOperator *RhsBin = NULL;
    if ((LhsBin = getBinOp(S1))) {
      S1 = LhsBin;
    }
    if ((RhsBin = getBinOp(S2))) {
      S2 = RhsBin;
    }
    IsTerminalS1 = isTerminal(S1);
    IsTerminalS2 = isTerminal(S2);
    Op = MVOp(S->getOpcode());
  } else if (F) {
    auto Args = F->getArgs();
    assert((F->getNumArgs() <= 2) && "Function not binary!! Not supported yet");
    auto MT = dyn_cast<MaterializeTemporaryExpr>(Args[0]);
    if (MT) {
      S1 = const_cast<Expr *>(MT->getSubExpr()->IgnoreImpCasts());
    } else {
      S1 = const_cast<Expr *>(Args[0]);
    }
    IsTerminalS1 = isTerminal(S1->IgnoreImpCasts());
    if (F->getNumArgs() > 1) {
      MT = dyn_cast<MaterializeTemporaryExpr>(Args[1]);
      if (MT) {
        S2 = const_cast<Expr *>(MT->getSubExpr()->IgnoreImpCasts());
      } else {
        S2 = const_cast<Expr *>(Args[1]);
      }
      IsTerminalS2 = isTerminal(S2->IgnoreImpCasts());
    } else {
      IsTerminalS2 = true;
      IsUnary = true;
    }
    Op = MVOp(Utils::getStringFromExpr(F->getCallee()));
  }

  assert(((S1 != nullptr) && ((IsUnary) || (S2 != nullptr))) &&
         "Something went wrong...");

  // FIXME: this part is very sensible and is done for parsing properly
  // classes such as std::vector
  auto T = S1->getType().getAsString();
  if (isa<clang::TypedefType>(S1->getType())) {
    auto aT = dyn_cast<clang::TypedefType>(S1->getType());
    if (!aT->isSugared()) {
      llvm::llvm_unreachable_internal();
    }
    auto ET = dyn_cast<ElaboratedType>(aT->desugar());
    auto TT = dyn_cast<TypedefType>(ET->desugar());
    auto Subs = dyn_cast<SubstTemplateTypeParmType>(TT->desugar());
    T = Subs->desugar().getAsString();
  }

  /// Since this is a recursive function, we have to test the first case
  auto TmpA = (Val == -1) ? MVExprFactory::createMVExpr(S1)
                          : MVExprFactory::createMVExpr(getCurrentNameTempReg(),
                                                        true, T);

  auto TmpB = (!IsTerminalS1) ? MVExprFactory::createMVExpr(
                                    getNameTempReg(), true, TmpA->getExprStr())
                              : MVExprFactory::createMVExpr(S1);
  MVExpr *TmpC = nullptr;

  int Inc = 1;
  if (!IsTerminalS1) {
    stmtToTACRecursive(S1, TacList, Val + Inc);
  }

  if (!IsUnary) {
    TmpC = (!IsTerminalS2) ? MVExprFactory::createMVExpr(getNameTempReg(), true,
                                                         TmpA->getExprStr())
                           : MVExprFactory::createMVExpr(S2);
  }

  if (!IsTerminalS2) {
    stmtToTACRecursive(S2, TacList, Val + Inc + 1);
  }

  TAC NewTac(TmpA, TmpB, TmpC, Op);
  TacList->push_back(NewTac);
}

// ---------------------------------------------
TacListType TAC::stmtToTAC(clang::Stmt *ST) {
  // Reset the RegVal in TAC class
  auto S = dyn_cast<clang::Expr>(ST);
  TacListType TacList;

  // Check if the expression is binary
  clang::BinaryOperator *SBin = NULL;
  bool STypeBin = (SBin = getBinOp(S->IgnoreImpCasts()));
  if (STypeBin) {
    TacListType TL;
    stmtToTACRecursive(SBin, &TL, -1);
    TacList.splice(TacList.end(), TL);
    return TacList;
  }

  assert(false && "This type of statement is not permitted!!!");
  return TacList;
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

  auto NewA = Tac->getA()->unrollExpr(UnrollA, LoopLevel);
  auto NewB = Tac->getB()->unrollExpr(UnrollB, LoopLevel);
  auto NewC =
      Tac->getC() != NULL ? Tac->getC()->unrollExpr(UnrollC, LoopLevel) : NULL;
  auto UnrolledTac = new TAC(NewA, NewB, NewC, Tac->getMVOP(), Tac->getTacID());
  UnrolledTac->setLoopName(Tac->getLoopName());
  return UnrolledTac;
}

// ---------------------------------------------
TacListType TAC::unrollTacList(TacListType TacList, int UnrollFactor,
                               int UpperBound, std::string LoopLevel) {
  TacListType TacListOrg;
  int Steps = UpperBound / UnrollFactor;
  for (int S = 0; S < Steps; S++) {
    for (auto T : TacList) {
      auto NT = (TAC::unroll(&T, UnrollFactor, S, 0x000000, LoopLevel));
      if (NT != NULL) {
        TacListOrg.push_back(*NT);
      }
      if ((NT == NULL) && (S == 0)) {
        TacListOrg.push_back(T);
      }
    }
  }
  return TacListOrg;
}
