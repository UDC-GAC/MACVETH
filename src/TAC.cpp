/**
 * File              : TAC.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 14:18:48 MST
 * Last Modified Date: Mér 18 Dec 2019 13:32:46 MST
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
#include "CDAG.h"
#include "include/MVExpr/MVExprFactory.h"
#include "clang/Lex/Lexer.h"

using namespace clang;
using namespace macveth;

// ---------------------------------------------
void TAC::binaryOperator2TAC(const clang::BinaryOperator *S,
                             std::list<TAC> *TacList, int Val) {
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
    TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());

    TacList->push_back(NewTac);
    binaryOperator2TAC(RhsBin, TacList, Val + 1);
    binaryOperator2TAC(LhsBin, TacList, Val + 2);
  } else if (!LhsTypeBin && RhsTypeBin) {
    MVExpr *TmpB = MVExprFactory::createMVExpr(Lhs);
    MVExpr *TmpC =
        MVExprFactory::createMVExpr(Utils::getNameTempReg(Val + 1), true);
    TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
    TacList->push_back(NewTac);
    binaryOperator2TAC(RhsBin, TacList, Val + 1);
  } else if (LhsTypeBin && !RhsTypeBin) {
    MVExpr *TmpB =
        MVExprFactory::createMVExpr(Utils::getNameTempReg(Val + 1), true);
    MVExpr *TmpC = MVExprFactory::createMVExpr(Rhs);
    TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
    TacList->push_back(NewTac);
    binaryOperator2TAC(LhsBin, TacList, Val + 1);
  } else {
    MVExpr *TmpB = MVExprFactory::createMVExpr(Lhs);
    MVExpr *TmpC = MVExprFactory::createMVExpr(Rhs);
    TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
    TacList->push_back(NewTac);
  }
  return;
}

// ---------------------------------------------
TAC *TAC::unroll(TAC *Tac, int UnrollFactor, unsigned int mask) {
  int UnrollA =
      UnrollFactor + UnrollFactor * ((mask & TAC::MASK_OP_A) >> TAC::BITS_OP_A);
  int UnrollB =
      UnrollFactor + UnrollFactor * ((mask & TAC::MASK_OP_B) >> TAC::BITS_OP_B);
  int UnrollC =
      UnrollFactor + UnrollFactor * ((mask & TAC::MASK_OP_C) >> TAC::BITS_OP_C);
  TAC *UnrolledTac =
      new TAC((*(Tac->getA()) + UnrollA), (*(Tac->getB()) + UnrollB),
              (*(Tac->getC()) + UnrollC), Tac->getOP());
  return UnrolledTac;
}

// ---------------------------------------------
TAC *TAC::unroll(TAC *Tac, int UnrollFactor, int S, unsigned int mask) {
  int UnrollA = S * UnrollFactor +
                UnrollFactor * ((mask & TAC::MASK_OP_A) >> TAC::BITS_OP_A);
  int UnrollB = S * UnrollFactor +
                UnrollFactor * ((mask & TAC::MASK_OP_B) >> TAC::BITS_OP_B);
  int UnrollC = S * UnrollFactor +
                UnrollFactor * ((mask & TAC::MASK_OP_C) >> TAC::BITS_OP_C);
  TAC *UnrolledTac =
      new TAC((*(Tac->getA()) + UnrollA), (*(Tac->getB()) + UnrollB),
              (*(Tac->getC()) + UnrollC), Tac->getOP());
  return UnrolledTac;
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
  TAC *UnrolledTac =
      new TAC(Tac->getA()->unrollExpr(UnrollA, LoopLevel),
              Tac->getB()->unrollExpr(UnrollB, LoopLevel),
              Tac->getC()->unrollExpr(UnrollC, LoopLevel), Tac->getOP());
  return UnrolledTac;
}

// ---------------------------------------------
std::list<TAC> TAC::unrollTacList(std::list<TAC> TacList, int UnrollFactor,
                                  int UpperBound, unsigned int MaskList[],
                                  std::string LoopLevel) {
  std::list<TAC> TacListOrg;
  int steps = UpperBound / UnrollFactor;
  for (int s = 0; s < steps; ++s) {
    // FIXME
    int Mask = 0;
    for (TAC Tac : TacList) {
      // Skip stores until the end
      if ((!BOtoValue[Tac.getOP()].compare("store")) && (s < steps - 1))
        continue;
      TacListOrg.push_back(
          *(TAC::unroll(&Tac, UnrollFactor, s, MaskList[Mask++], LoopLevel)));
    }
  }
  return TacListOrg;
}

// ---------------------------------------------
std::list<TAC> TAC::unrollTacList(std::list<TAC> TacList, int UnrollFactor,
                                  int UpperBound, unsigned int MaskList[]) {
  std::list<TAC> TacListOrg = TacList;
  int steps = UpperBound / UnrollFactor;
  for (int s = 0; s < steps - 1; ++s) {
    int Mask = 0;
    for (TAC Tac : TacList) {
      TacListOrg.push_back(
          *(TAC::unroll(&Tac, UnrollFactor, s, MaskList[Mask++])));
    }
  }
  return TacListOrg;
}

// ---------------------------------------------
std::list<TAC> TAC::unrollTacList(std::list<TAC> TacList, int UnrollFactor,
                                  int UpperBound, unsigned int Mask) {
  std::list<TAC> TacListOrg = TacList;
  int steps = UpperBound / UnrollFactor;
  for (int s = 0; s < steps - 1; ++s) {
    for (TAC Tac : TacList) {
      TacListOrg.push_back(*(TAC::unroll(&Tac, UnrollFactor * s, Mask)));
    }
  }
  return TacListOrg;
}

// ---------------------------------------------
std::list<TAC> TAC::unrollTacList(std::list<TAC> TacList, int UnrollFactor,
                                  int UpperBound) {
  return TAC::unrollTacList(TacList, UnrollFactor, UpperBound,
                            (unsigned int)0x00);
}
