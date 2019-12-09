/**
 * File              : TAC.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 14:18:48 MST
 * Last Modified Date: Lun 09 Dec 2019 11:06:13 MST
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
  // std::cout << Utils::getStringFromExpr(Lhs) << std::endl;
  // std::cout << Utils::getStringFromExpr(Rhs) << std::endl;
  clang::BinaryOperator *LhsBin = NULL;
  clang::BinaryOperator *RhsBin = NULL;
  bool LhsTypeBin =
      (LhsBin = dyn_cast<clang::BinaryOperator>(Lhs->IgnoreImpCasts()));
  bool RhsTypeBin =
      (RhsBin = dyn_cast<clang::BinaryOperator>(Rhs->IgnoreImpCasts()));

  /// Since this is a recursive function, we have to test the first case
  TempExpr *TmpA = NULL;
  if (Val == -1) {
    TmpA = new TempExpr(Lhs);
  } else {
    TmpA = new TempExpr(Utils::getNameTempReg(Val),
                        TempExpr::TempExprInfo::TMP_RES);
  }
  // recursive part, to delve deeper into BinaryOperators
  if (LhsTypeBin && RhsTypeBin) {
    // both true
    TempExpr *TmpB = new TempExpr(Utils::getNameTempReg(Val + 1),
                                  TempExpr::TempExprInfo::TMP_VAL);
    TempExpr *TmpC = new TempExpr(Utils::getNameTempReg(Val + 2),
                                  TempExpr::TempExprInfo::TMP_VAL);
    TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());

    TacList->push_back(NewTac);
    binaryOperator2TAC(RhsBin, TacList, Val + 1);
    binaryOperator2TAC(LhsBin, TacList, Val + 2);
  } else if (!LhsTypeBin && RhsTypeBin) {
    TempExpr *TmpB = new TempExpr(Lhs);
    TempExpr *TmpC = new TempExpr(Utils::getNameTempReg(Val + 1),
                                  TempExpr::TempExprInfo::TMP_VAL);
    TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
    TacList->push_back(NewTac);
    binaryOperator2TAC(RhsBin, TacList, Val + 1);
  } else if (LhsTypeBin && !RhsTypeBin) {
    TempExpr *TmpB = new TempExpr(Utils::getNameTempReg(Val + 1),
                                  TempExpr::TempExprInfo::TMP_VAL);
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

// Static version of unrolling
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

// Object version of unrolling
TAC *TAC::unroll(int UnrollFactor, unsigned int mask) {
  int UnrollA =
      UnrollFactor + UnrollFactor * ((mask & TAC::MASK_OP_A) >> TAC::BITS_OP_A);
  int UnrollB =
      UnrollFactor + UnrollFactor * ((mask & TAC::MASK_OP_B) >> TAC::BITS_OP_B);
  int UnrollC =
      UnrollFactor + UnrollFactor * ((mask & TAC::MASK_OP_C) >> TAC::BITS_OP_C);
  TAC *UnrolledTac =
      new TAC((*(this->getA()) + UnrollA), (*(this->getB()) + UnrollB),
              (*(this->getC()) + UnrollC), this->getOP());
  return UnrolledTac;
}

TAC *TAC::unroll(int UnrollFactor, int S, unsigned int mask) {
  int UnrollA = S * UnrollFactor +
                UnrollFactor * ((mask & TAC::MASK_OP_A) >> TAC::BITS_OP_A);
  int UnrollB = S * UnrollFactor +
                UnrollFactor * ((mask & TAC::MASK_OP_B) >> TAC::BITS_OP_B);
  int UnrollC = S * UnrollFactor +
                UnrollFactor * ((mask & TAC::MASK_OP_C) >> TAC::BITS_OP_C);
  TAC *UnrolledTac =
      new TAC((*(this->getA()) + UnrollA), (*(this->getB()) + UnrollB),
              (*(this->getC()) + UnrollC), this->getOP());
  return UnrolledTac;
}

void TAC::unrollTacList(std::list<TAC> *TacList, int UnrollFactor,
                        int UpperBound, unsigned int Mask) {
  std::list<TAC> TacListOrg = *TacList;
  int steps = UpperBound / UnrollFactor;
  for (int s = 0; s < steps - 1; ++s) {
    for (TAC Tac : TacListOrg) {
      TacList->push_back(*Tac.unroll(UnrollFactor * s, Mask));
    }
  }
}

/// Same as before but without mask
void TAC::unrollTacList(std::list<TAC> *TacList, int UnrollFactor,
                        int UpperBound) {
  TAC::unrollTacList(TacList, UnrollFactor, UpperBound, 0x000000);
}

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

/// Same as before but without mask
std::list<TAC> TAC::unrollTacList(std::list<TAC> TacList, int UnrollFactor,
                                  int UpperBound) {
  return TAC::unrollTacList(TacList, UnrollFactor, UpperBound,
                            (unsigned int)0x00);
}
