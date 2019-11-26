/**
 * File              : StmtWrapper.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 09:05:09 MST
 * Last Modified Date: Mar 26 Nov 2019 10:53:27 MST
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
#ifndef MACVETH_STMTWRAPPER_H
#define MACVETH_STMTWRAPPER_H

#include "include/Intrinsics/IntrinsicsGenerator.h"
#include "include/TAC.h"
#include "include/Utils.h"
#include "clang/AST/AST.h"

using namespace clang;
using namespace macveth;

namespace macveth {

typedef std::list<TAC> TacListType;

/// StmtWrapper, as its name suggests, wraps statements, its TAC representation
/// and the desired intrinsic translation
class StmtWrapper {
public:
  enum StmtType { REDUCTION, VECTORIZABLE };
  /// Constructors
  StmtWrapper(){};
  StmtWrapper(const BinaryOperator *S) {
    TAC::binaryOperator2TAC(S, &this->TacList, -1);
    this->TacList.reverse();
    for (TAC Tac : TacList) {
      Tac.printTAC();
    }
  };

  static StmtType getStmtType(const BinaryOperator *S);
  StmtType getStmtType() { return this->ST; }
  void setStmtType(StmtType ST) { this->ST = ST; }

  /// Translator
  void translateTacToIntrinsics() {
    this->InstList = IntrinsicsInsGen::translateTAC(this->getTacList());
  }

  /// Unroll
  void unroll(int UnrollFactor, int UpperBound);

  /// Getters and setters
  std::list<InstListType> getInstList() { return this->InstList; };
  void setInstList(std::list<InstListType> InstList) {
    this->InstList = InstList;
  };
  Stmt *getStmt() { return this->S; };
  void setStmt(Stmt *S) { this->S = S; };
  TacListType getTacList() { return this->TacList; };
  void setTacList(TacListType TacList) { this->TacList = TacList; };

private:
  Stmt *S;
  StmtType ST;
  TacListType TacList;
  std::list<InstListType> InstList;
}; // namespace macveth
} // namespace macveth
#endif // MACVETH_STMTWRAPPER_H
