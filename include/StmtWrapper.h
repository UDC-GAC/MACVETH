/**
 * File              : StmtWrapper.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 09:05:09 MST
 * Last Modified Date: Sáb 11 Xan 2020 12:17:16 MST
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

#include "include/MVExpr/MVExpr.h"
#include "include/TAC.h"
#include "include/Utils.h"
#include "clang/AST/AST.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <unordered_map>

using namespace clang;
using namespace clang::ast_matchers;
using namespace macveth;

namespace macveth {

/// StmtWrapper, as its name suggests, wraps statements, its TAC representation
/// and the desired intrinsic translation
class StmtWrapper {
public:
  /// Helper structure to handle the loop information for this statement
  struct LoopInfo {
    /// Name of dimension
    std::string Dim;
    /// Initial value (-1 if not known)
    long InitVal = 0;
    /// Upperbound value (-1 if not known)
    long UpperBound = 0;

    /// For debugging purposes
    void print() {
      std::cout << "[LOOP] " << Dim
                << "; init val = " << std::to_string(InitVal)
                << ", upperbound = " << std::to_string(UpperBound) << std::endl;
    }
  };

  /// LoopInfo list type
  typedef std::list<LoopInfo> LoopList;

  /// From the result of a matcher it gets the loop hierarchy
  static LoopList getLoopList(const MatchFinder::MatchResult &Result);

  /// Types of statements we differentiate when creating them
  enum StmtType { REDUCTION, VECTORIZABLE };
  /// Empty constructor
  StmtWrapper(){};
  /// Full constructor that parses the loop hierarchy
  StmtWrapper(const MatchFinder::MatchResult &Result) {
    const Expr *E = Result.Nodes.getNodeAs<clang::Expr>("ROI");

    /// Get loop information
    this->LoopL = getLoopList(Result);
    this->S = (Stmt *)E;
    // this->setStmtType(StmtWrapper::getStmtType(BinOp));
    TAC::exprToTAC(const_cast<Expr *>(E), &this->TacList, -1);
    this->TacList.reverse();
  }

  /// Given a statement, it is able to determine wherever it is or not a
  /// reduction
  static StmtType getStmtType(const BinaryOperator *S);
  StmtType getStmtType() { return this->ST; }
  void setStmtType(StmtType ST) { this->ST = ST; }

  /// Perform unrolling for a given statement given its unroll factor and the
  /// upperbound of the loop
  // void unroll(int UnrollFactor, int UpperBound);
  void unroll(long UnrollFactor, long UpperBound, std::string LoopLevel);
  void unrollAndJam(long UnrollFactor, long UpperBoundFallback = 4);

  /// Get LoopInfo
  LoopList getLoopInfo() { return this->LoopL; }

  /// Get Clang Stmt
  Stmt *getStmt() { return this->S; };
  /// Set Clang Stmt
  void setStmt(Stmt *S) { this->S = S; };
  /// Get TAC list
  TacListType getTacList() { return this->TacList; };
  /// Set TAC lsit
  void setTacList(TacListType TacList) { this->TacList = TacList; };

private:
  /// Statement holded
  Stmt *S;
  /// Loop list
  LoopList LoopL;
  /// Type of statement; we only address those which are of our interest
  StmtType ST;
  /// TAC list with regard to the Statement S
  TacListType TacList;
};
} // namespace macveth
#endif // MACVETH_STMTWRAPPER_H
