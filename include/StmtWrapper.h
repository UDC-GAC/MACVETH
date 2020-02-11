/**
 * File              : StmtWrapper.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 09:05:09 MST
 * Last Modified Date: Lun 13 Xan 2020 19:16:12 MST
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

using namespace clang;
using namespace clang::ast_matchers;
using namespace macveth;

namespace macveth {

/// StmtWrapper, as its name suggests, wraps statements, its TAC representation
/// and the desired intrinsic translation
class StmtWrapper {
public:
  /// Default value
  static constexpr int UBFallback = 4;

  friend clang::Stmt;

  /// Helper structure to handle the loop information for this statement
  struct LoopInfo {
    /// Name of dimension
    std::string Dim;
    /// Initial value (-1 if not known)
    long InitVal = 0;
    /// Upperbound value (-1 if not known)
    long UpperBound = 0;
    /// Stride or step
    long Step = UBFallback;
    /// TODO Leftover values: if Upperbound is -1, then LeftOver as well
    long LeftOver = -1;

    /// For debugging purposes
    std::string toString() {
      std::string Str = Dim + "; init val = " + std::to_string(InitVal) +
                        ", upperbound = " + std::to_string(UpperBound);
      return Str;
    }
  };

  /// LoopInfo list type
  typedef std::list<LoopInfo> LoopList;

  /// From the result of a matcher it gets the loop hierarchy
  static LoopList getLoopList(const MatchFinder::MatchResult &Result);

  /// Empty constructor
  StmtWrapper(){};

  /// Be clean
  ~StmtWrapper() {
    for (Stmt *V : this->S) {
      // delete V;
    }
  };

  /// Full constructor that parses the loop hierarchy
  StmtWrapper(const MatchFinder::MatchResult &Result) {
    auto E = Result.Nodes.getNodeAs<clang::CompoundStmt>("ROI");
    /// Get loop information
    this->LoopL = getLoopList(Result);
    /// Conver the expression to a set of TACs
    TAC::exprToTAC(const_cast<CompoundStmt *>(E), &this->S, &this->TacList);
  }

  /// Perform unrolling for a given statement given its unroll factor and the
  /// upperbound of the loop
  void unroll(long UnrollFactor, long UpperBound, std::string LoopLevel);
  /// Unrolls the TAC list in all the possible dimensions
  void unrollAndJam(long UnrollFactor, long UpperBoundFallback = UBFallback);

  /// Get LoopInfo
  LoopList getLoopInfo() { return this->LoopL; }

  /// Get Clang Stmt
  std::vector<Stmt *> getStmt() { return this->S; };
  /// Set Clang Stmt
  void setStmt(std::vector<Stmt *> S) { this->S = S; };
  /// Get TAC list
  TacListType getTacList() { return this->TacList; };
  /// Set TAC lsit
  void setTacList(TacListType TacList) { this->TacList = TacList; };

private:
  /// Statement holded
  std::vector<Stmt *> S;
  /// Loop list
  LoopList LoopL;
  /// TAC list with regard to the Statement S
  TacListType TacList;
};
} // namespace macveth
#endif // MACVETH_STMTWRAPPER_H
