/**
 * File              : TAC.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 18 Nov 2019 14:51:25 MST
 * Last Modified Date: Lun 25 Nov 2019 17:44:21 MST
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

#include "include/TempExpr.h"
#include "include/Utils.h"
#include "clang/AST/AST.h"
#include <iostream>
#include <list>
#include <string>

using namespace clang;
using namespace macveth;

namespace macveth {
// Class TAC: three-address-code
// This class is no more than a wrapper for holding three Expr and a Opcode, in
// a way that:
class TAC {
  // This class is meant to hold structures such as:
  //                  a = b op c
  // Where a, b and c are Expr and op is an Opcode
  // Consturctor
public:
  TAC(){}; // empty constructor
  TAC(TempExpr *A, TempExpr *B, TempExpr *C, clang::BinaryOperator::Opcode OP)
      : A(A), B(B), C(C), OP(OP) {}

  TempExpr *getA() { return this->A; };
  TempExpr *getB() { return this->B; };
  TempExpr *getC() { return this->C; };
  clang::BinaryOperator::Opcode getOP() { return this->OP; };

  // Just for debugging purposes
  void printTAC() {
    std::string Op = BinaryOperator::getOpcodeStr(this->getOP()).str();
    std::cout << "t: " << this->getA()->getExprStr() << ", "
              << this->getB()->getExprStr() << ", "
              << this->getC()->getExprStr() << ", " << Op << std::endl;
  }

  // Return list of 3AC from a starting Binary Operator
  static void binaryOperator2TAC(const clang::BinaryOperator *S,
                                 std::list<TAC> *TacList, int Val);

  /// Modifies given list adding at the end the unrolled TACs
  static void unrollTacList(std::list<TAC> *Tac, int UnrollFactor,
                            int UpperBound);

  /// Unrolls TacList given onto a new list
  static std::list<TAC> unrollTacList(std::list<TAC> Tac, int UnrollFactor,
                                      int UpperBound);
  static TAC *unroll(TAC *Tac, int UnrollFactor);
  TAC *unroll(int UnrollFactor);

private:
  TempExpr *A;
  TempExpr *B;
  TempExpr *C;
  clang::BinaryOperator::Opcode OP;
};

} // namespace macveth
#endif
