/**
 * File              : StmtWrapper.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 25 Nov 2019 13:48:24 MST
 * Last Modified Date: Mar 26 Nov 2019 13:59:26 MST
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
#include "clang/AST/Expr.h"

using namespace clang;
using namespace macveth;

StmtWrapper::StmtType StmtWrapper::getStmtType(const BinaryOperator *S) {
  return StmtWrapper::StmtType::VECTORIZABLE;
}

void StmtWrapper::unroll(int UnrollFactor, int UpperBound) {
  if (this->getStmtType() == StmtWrapper::StmtType::VECTORIZABLE) {
    this->setTacList(
        TAC::unrollTacList(this->getTacList(), UnrollFactor, UpperBound));
  } else if (this->getStmtType() == StmtWrapper::StmtType::REDUCTION) {
  } else {
    std::cout << "STMT not supported yet!!!" << std::endl;
  }
  for (TAC Tac : this->getTacList()) {
    Tac.printTAC();
  }
}
