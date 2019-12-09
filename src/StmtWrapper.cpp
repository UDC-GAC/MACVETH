/**
 * File              : StmtWrapper.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 25 Nov 2019 13:48:24 MST
 * Last Modified Date: Dom 08 Dec 2019 18:44:36 MST
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
#include "TAC.h"
#include "clang/AST/Expr.h"
#include <string>

using namespace clang;
using namespace macveth;

/// FIXME
/// Given a statement, it is able to determine wherever it is or not a
/// reduction
StmtWrapper::StmtType StmtWrapper::getStmtType(const BinaryOperator *S) {
  if (!isa<ArraySubscriptExpr>(S->getLHS())) {
    return StmtWrapper::StmtType::REDUCTION;
  }
  return StmtWrapper::StmtType::VECTORIZABLE;
}

void printDebug(std::string Name, TacListType TempTacList) {
  std::cout << "========" + Name + " UNROLLING=========" << std::endl;
  for (TAC Tac : TempTacList) {
    Tac.printTAC();
  }
  std::cout << "=================================" << std::endl;
}

/// Limitations:
/// * Type of statements: VECTORIZABLE | REDUCTION
/// * Regarding reductions:
///    - Works for reduction statements such as:
///            sum = sum + [whatever]
/// * Code is not beautiful, I know
void StmtWrapper::unroll(int UnrollFactor, int UpperBound) {
  if (this->getStmtType() == StmtWrapper::StmtType::VECTORIZABLE) {
    this->setTacList(
        TAC::unrollTacList(this->getTacList(), UnrollFactor, UpperBound));
  } else if (this->getStmtType() == StmtWrapper::StmtType::REDUCTION) {
    /// Get the last element, which is the final reduction
    TAC RedTac = this->getTacList().back();
    /// Making a copy of the TAC list
    std::list<TAC> TempTacList = this->getTacList();
    /// Remove the last element, the final reduction
    printDebug("BEFORE", TempTacList);
    TempTacList.pop_back();
    TAC AddTac = TempTacList.back();
    TempTacList.pop_back();
    printDebug("BEFORE", TempTacList);
    /// Get the name of the last operand which holds basically the result of the
    /// reduction. Thus, create a new TAC which basically will be the core for
    /// unrolling.
    std::string LastTempReg = TempTacList.back().getA()->getExprStr();
    TAC *TempTac = new TAC(
        new TempExpr("unroll0", TempExpr::TempExprInfo::TMP_RES),
        new TempExpr("unroll0", TempExpr::TempExprInfo::TMP_RES),
        new TempExpr("temp1", TempExpr::TempExprInfo::TMP_RES), AddTac.getOP());
    TempTacList.push_back(*TempTac);
    /// Unroll TempTacList (which is the original without the last statement)
    unsigned int MaskList[] = {0x010101, 0x010100};
    TempTacList =
        TAC::unrollTacList(TempTacList, UnrollFactor, UpperBound, MaskList);
    /// Now we are going to attach to the front of the list a new TAC which is
    /// basically the "base case"
    TAC TempInit = TempTacList.front();
    TempInit.setA(new TempExpr("unroll0"));
    TempTacList.pop_front();
    TempTacList.pop_front();
    TempTacList.push_front(TempInit);
    /// Setting thiw new TAC list for this statement
    this->setTacList(TempTacList);
    printDebug("AFTER", TempTacList);
  } else {
    std::cout << "STMT not supported yet!!!" << std::endl;
  }
}
