/**
 * File              : TAC.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 14:18:48 MST
 * Last Modified Date: Xov 09 Xan 2020 22:05:00 MST
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
#include "clang/Lex/Lexer.h"

using namespace clang;
using namespace macveth;

// ---------------------------------------------
MVOp TAC::getMVOPfromExpr(MVExpr *E) {
  if (MVExprFunc *F = dyn_cast<MVExprFunc>(E)) {
    return MVOp(F->getExprStr());
  }
  return MVOp("");
}

// ---------------------------------------------
void TAC::exprToTAC(clang::Expr *S, std::list<TAC> *TacList, int Val) {
  clang::BinaryOperator *SBin = NULL;
  bool STypeBin = (SBin = dyn_cast<clang::BinaryOperator>(S->IgnoreImpCasts()));
  if (STypeBin) {
    binaryOperator2TAC(SBin, TacList, Val);
    return;
  }
  MVExpr *TmpA =
      MVExprFactory::createMVExpr(Utils::getNameTempReg(Val + 1), true);
  MVExpr *TmpB = MVExprFactory::createMVExpr(S);
  MVOp OP = TAC::getMVOPfromExpr(TmpB);
  TAC NewTac = TAC(TmpA, TmpB, NULL, OP);
  TacList->push_back(NewTac);
}

// ---------------------------------------------
void TAC::binaryOperator2TAC(const clang::BinaryOperator *S,
                             std::list<TAC> *TacList, int Val) {
  S->dumpPretty(*Utils::getCtx());
  std::cout << std::endl;
  Expr *Lhs = S->getLHS();
  Expr *Rhs = S->getRHS();
  clang::BinaryOperator *LhsBin = NULL;
  clang::BinaryOperator *RhsBin = NULL;
  bool LhsTypeBin =
      (LhsBin = dyn_cast<clang::BinaryOperator>(Lhs->IgnoreImpCasts()));
  bool RhsTypeBin =
      (RhsBin = dyn_cast<clang::BinaryOperator>(Rhs->IgnoreImpCasts()));

  /// Since this is a recursive function, we have to test the first case
  MVExpr *TmpA =
      (Val == -1)
          ? MVExprFactory::createMVExpr(Lhs)
          : MVExprFactory::createMVExpr(Utils::getNameTempReg(Val), true);

  // recursive part, to delve deeper into BinaryOperators
  if (LhsTypeBin && RhsTypeBin) {
    // both true
    MVExpr *TmpB =
        MVExprFactory::createMVExpr(Utils::getNameTempReg(Val + 1), true);
    MVExpr *TmpC = MVExprFactory::createMVExpr(Utils::getNameTempReg(Val + 2));
    TAC NewTac = TAC(TmpA, TmpB, TmpC, MVOp(S->getOpcode()));

    TacList->push_back(NewTac);
    binaryOperator2TAC(RhsBin, TacList, Val + 1);
    binaryOperator2TAC(LhsBin, TacList, Val + 2);
  } else if (!LhsTypeBin && RhsTypeBin) {
    MVExpr *TmpB = MVExprFactory::createMVExpr(Lhs);
    MVExpr *TmpC =
        MVExprFactory::createMVExpr(Utils::getNameTempReg(Val + 1), true);
    TAC NewTac = TAC(TmpA, TmpB, TmpC, MVOp(S->getOpcode()));
    TacList->push_back(NewTac);
    binaryOperator2TAC(RhsBin, TacList, Val + 1);
  } else if (LhsTypeBin && !RhsTypeBin) {
    MVExpr *TmpB =
        MVExprFactory::createMVExpr(Utils::getNameTempReg(Val + 1), true);
    MVExpr *TmpC = MVExprFactory::createMVExpr(Rhs);
    TAC NewTac = TAC(TmpA, TmpB, TmpC, MVOp(S->getOpcode()));
    TacList->push_back(NewTac);
    binaryOperator2TAC(LhsBin, TacList, Val + 1);
  } else {
    MVExpr *TmpB = MVExprFactory::createMVExpr(Lhs);
    MVExpr *TmpC = MVExprFactory::createMVExpr(Rhs);
    TAC NewTac = TAC(TmpA, TmpB, TmpC, MVOp(S->getOpcode()));
    TacList->push_back(NewTac);
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
  // std::cout << "A = " << std::to_string(UnrollA)
  //          << "; B = " << std::to_string(UnrollB)
  //          << "; C = " << std::to_string(UnrollC) << std::endl;

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
  for (int S = 0; S < Steps; ++S) {
    // FIXME
    int Mask = 0;
    for (TAC Tac : TacList) {
      // FIXME Skip stores until the end
      // if ((BOtoValue[Tac.getOP()] == "store") && (S < Steps - 1))
      //  continue;
      TacListOrg.push_back(
          *(TAC::unroll(&Tac, UnrollFactor, S, 0x000000, LoopLevel)));
      //*(TAC::unroll(&Tac, UnrollFactor, S, MaskList[Mask++], LoopLevel)));
    }
  }
  return TacListOrg;
}
