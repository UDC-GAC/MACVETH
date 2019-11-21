/**
 * File              : ThreeAddressCode.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 18 Nov 2019 14:51:25 MST
 * Last Modified Date: Xov 21 Nov 2019 15:33:53 MST
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

#ifndef TAC_H
#define TAC_H
#include <iostream>
#include <string>

#include "Utils.h"
#include "clang/AST/AST.h"

using namespace clang;

namespace macveth {
// This class holds a double identity for expressions: a string name (for
// debugging purposes and for clarity) and the actual Clang Expr, for
// transformations
class TempExpr {
   public:
    TempExpr(std::string E) : ExprStr(E) {}

    void setClangExpr(clang::Expr* ClangExpr) { this->ClangExpr = ClangExpr; }
    clang::Expr* getClangExpr() { return this->ClangExpr; }

    void setTypeStr(std::string TypeStr) { this->TypeStr = TypeStr; }
    std::string getTypeStr() { return this->TypeStr; }

    void setExprStr(std::string ExprStr) { this->ExprStr = ExprStr; }
    std::string getExprStr() { return this->ExprStr; }

    bool isNotClang() { return (this->ClangExpr == NULL); }

    // dumb method
    static std::string getNameTempReg(int Val) {
        return "t" + std::to_string(Val);
    }

    // A hack for translating Expr to string
    static TempExpr* getTempExprFromExpr(Expr* E);

   private:
    std::string TypeStr = "double";
    std::string ExprStr;
    clang::Expr* ClangExpr = NULL;
};

// Class TAC: three-address-code
// This class is no more than a wrapper for holding three Expr and a Opcode, in
// a way that:
class TAC {
    // This class is meant to hold structures such as:
    //                  a = b op c
    // Where a, b and c are Expr and op is an Opcode
    // Consturctor
   public:
    TAC(){};  // empty constructor
    TAC(TempExpr* A, TempExpr* B, TempExpr* C, clang::BinaryOperator::Opcode OP)
        : A(A), B(B), C(C), OP(OP) {}

    TempExpr* getA() { return this->A; };
    TempExpr* getB() { return this->B; };
    TempExpr* getC() { return this->C; };
    clang::BinaryOperator::Opcode getOP() { return this->OP; };

    // Just for debugging purposes
    void printTAC() {
        std::string Op = BinaryOperator::getOpcodeStr(this->getOP()).str();
        std::cout << "t: " << this->getA()->getExprStr() << ", "
                  << this->getB()->getExprStr() << ", "
                  << this->getC()->getExprStr() << ", " << Op << std::endl;
    }

    static Expr* createExprFromTAC(TAC* TacExpr) { return NULL; }

    Expr* getExprFromTAC() { return TAC::createExprFromTAC(this); }

    // Return list of 3AC from a starting Binary Operator
    static void binaryOperator2TAC(const clang::BinaryOperator* S,
                                   std::list<TAC>* TacList, int Val) {
        // cout << "[DEBUG]: bo2TAC" << endl;
        Expr* Lhs = S->getLHS();
        Expr* Rhs = S->getRHS();
        bool LhsTypeBin = false;
        bool RhsTypeBin = false;
        clang::BinaryOperator* LhsBin = NULL;
        clang::BinaryOperator* RhsBin = NULL;
        if (LhsBin = dyn_cast<clang::BinaryOperator>(Lhs)) {
            LhsTypeBin = true;
        }
        if (RhsBin = dyn_cast<clang::BinaryOperator>(Rhs)) {
            RhsTypeBin = true;
        }

        // we are in the the first case
        TempExpr* TmpA = NULL;
        if (Val == -1) {
            TmpA = TempExpr::getTempExprFromExpr(Lhs);
            TmpA->setClangExpr(Lhs);
        } else {
            TmpA = new TempExpr(TempExpr::getNameTempReg(Val));
        }
        // recursive part, to delve deeper into BinaryOperators
        if ((LhsTypeBin == true) && (RhsTypeBin == true)) {
            // both true
            TempExpr* TmpB = new TempExpr(TempExpr::getNameTempReg(Val + 1));
            TempExpr* TmpC = new TempExpr(TempExpr::getNameTempReg(Val + 2));
            TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
            TacList->push_back(NewTac);
            binaryOperator2TAC(RhsBin, TacList, Val + 1);
            binaryOperator2TAC(LhsBin, TacList, Val + 2);
        } else if ((LhsTypeBin == false) && (RhsTypeBin == true)) {
            TempExpr* TmpB = TempExpr::getTempExprFromExpr(Lhs);
            TmpB->setClangExpr(Lhs);
            TempExpr* TmpC = new TempExpr(TempExpr::getNameTempReg(Val + 1));
            TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
            TacList->push_back(NewTac);
            binaryOperator2TAC(RhsBin, TacList, Val + 1);
        } else if ((LhsTypeBin == true) && (RhsTypeBin == false)) {
            TempExpr* TmpB = new TempExpr(TempExpr::getNameTempReg(Val + 1));
            TempExpr* TmpC = TempExpr::getTempExprFromExpr(Rhs);
            TmpC->setClangExpr(Rhs);
            TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
            TacList->push_back(NewTac);
            binaryOperator2TAC(LhsBin, TacList, Val + 1);
        } else {
            TempExpr* TmpB = TempExpr::getTempExprFromExpr(Lhs);
            TmpB->setClangExpr(Lhs);
            TempExpr* TmpC = TempExpr::getTempExprFromExpr(Rhs);
            TmpC->setClangExpr(Rhs);
            TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
            TacList->push_back(NewTac);
        }
        return;
    }

   private:
    TempExpr* A;
    TempExpr* B;
    TempExpr* C;
    clang::BinaryOperator::Opcode OP;
};

TempExpr* TempExpr::getTempExprFromExpr(Expr* E) {
    SourceManager* SM = Utils::getSourceMgr();
    LangOptions* LO = Utils::getLangOpts();
    clang::CharSourceRange CharRangeExpr =
        clang::CharSourceRange::getTokenRange(E->getSourceRange());
    const std::string Text = Lexer::getSourceText(CharRangeExpr, *SM, *LO);
    // const std::string Text = "";
    TempExpr* Temp = new TempExpr(Text);
    //    Temp->setClangExpr(E);
    return Temp;
}

}  // namespace macveth
#endif
