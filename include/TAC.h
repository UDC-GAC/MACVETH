/**
 * File              : TAC.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 18 Nov 2019 14:51:25 MST
 * Last Modified Date: Lun 13 Xan 2020 19:39:54 MST
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

#include "include/MVExpr/MVExpr.h"
#include "include/MVOps.h"
#include "include/Utils.h"
#include "clang/AST/AST.h"
#include <iostream>
#include <list>
#include <string>

using namespace clang;
using namespace macveth;

namespace macveth {
/// Class TAC: three-address-code. This abstraction is a way of representing the
/// Single-Statement Assignment (SSA)
class TAC {
private:
  constexpr static unsigned int MASK_OP_A = {0xFF0000};
  constexpr static unsigned int BITS_OP_A = 16;
  constexpr static unsigned int MASK_OP_B = {0x00FF00};
  constexpr static unsigned int BITS_OP_B = 8;
  constexpr static unsigned int MASK_OP_C = {0x0000FF};
  constexpr static unsigned int BITS_OP_C = 0;
  /// This class is meant to hold structures such as:
  ///                  a = b op c
  /// Where a, b and c are Expr and op is an Opcode
public:
  inline static int TacUUID = 0;
  /// Empty constructor
  TAC(){};

  /// Constructor when using MVOp for the operator
  TAC(MVExpr *A, MVExpr *B, MVExpr *C, MVOp MVOP)
      : A(A), B(B), C(C), MVOP(MVOP), TacOrder(TAC::TacUUID++) {}

  /// Get first (result) operand of the TAC expression
  MVExpr *getA() { return this->A; };
  /// Set first (result) operand of the TAC expression
  void setA(MVExpr *A) { this->A = A; };
  /// Get second operand of the TAC expression
  MVExpr *getB() { return this->B; };
  /// Set second operand of the TAC expression
  void setB(MVExpr *B) { this->B = B; };
  /// Get third operand of the TAC expression
  MVExpr *getC() { return this->C; };
  /// Set third operand of the TAC expression
  void setC(MVExpr *C) { this->C = C; };

  /// Get macveth operation type
  MVOp getMVOP() { return this->MVOP; };

  /// Just for debugging purposes
  void printTAC() {
    std::string Op = this->MVOP.toString();
    std::cout << "t: " << this->getA()->getExprStr() << ", "
              << this->getB()->getExprStr() << ", "
              << this->getC()->getExprStr() << ", " << Op << std::endl;
  }

  /// Discover the equivalent MVOp given a clang::Expr
  static MVOp getMVOPfromExpr(MVExpr *E);

  /// Return list of 3AC from a starting Binary Operator
  static void binaryOperator2TAC(const clang::BinaryOperator *S,
                                 std::list<TAC> *TacList, int Val);

  /// Inserts TACs in the input TacList
  // static void exprToTAC(clang::Expr *S, std::list<TAC> *TacList, int Val);
  static void exprToTAC(clang::CompoundStmt *S, std::list<TAC> *TacList);

  /// Unrolls TacList given onto a new list
  static std::list<TAC> unrollTacList(std::list<TAC> Tac, int UnrollFactor,
                                      int UpperBound, unsigned int MaskList[],
                                      std::string LoopLevel);

  /// Unroll a TAC given a LoopLevel, besides its mask, unroll factor, and
  /// the S value which holds the iteration of the unrolling basically
  static TAC *unroll(TAC *Tac, int UnrollFactor, int S, unsigned int mask,
                     std::string LoopLevel);

  /// For generating TACs
  static inline long RegVal = 0;

private:
  /// TAC result
  MVExpr *A;
  /// First operand of TAC
  MVExpr *B;
  /// Second operand of TAC
  MVExpr *C;
  /// Type of operation
  MVOp MVOP;
  /// Tac order
  int TacOrder = -1;
};

/// List of TACs
typedef std::list<TAC> TacListType;

} // namespace macveth
#endif
