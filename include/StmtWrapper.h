/**
 * File              : StmtWrapper.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 09:05:09 MST
 * Last Modified Date: Mar 17 Mar 2020 19:55:43 CET
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
#include "include/MVPragmaHandler.h"
#include "include/TAC.h"
#include "include/Utils.h"
#include "clang/AST/AST.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceLocation.h"

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
    /// List of dimensions declared
    static inline std::list<std::string> DimDeclared = {};
    /// Location of the loop itself
    clang::SourceLocation BegLoc;
    /// Location after the loop itself
    clang::SourceLocation EndLoc;
    /// Name of dimension
    std::string Dim;
    /// Initial value (-1 if not known)
    long InitVal = 0;
    /// Upperbound value (-1 if not known)
    long UpperBound = 0;
    /// Upperbound as string
    std::string StrUpperBound = "";
    /// Stride or step
    long Step = UBFallback;
    /// TODO Leftover values: if Upperbound is -1, then LeftOver as well
    long LeftOver = -1;
    /// Variable declared in the loop creation
    bool Declared = false;
    /// Location of the initialization in the loop
    clang::CharSourceRange SRVarInit;
    /// Location of the condition of the loop
    clang::CharSourceRange SRVarCond;
    /// Location of the increment in the loop
    clang::CharSourceRange SRVarInc;

    /// Empty constructor
    LoopInfo() {}

    /// Get epilogs of given dimensions and
    static std::string getEpilogs(std::list<LoopInfo> Dims,
                                  std::vector<Stmt *> SVec);

    static std::string getDimDeclarations(std::list<std::string> DimsDeclared);

    /// For debugging purposes
    std::string toString() {
      std::string Str = Dim + "; init val = " + std::to_string(InitVal) +
                        ", upperbound = " + std::to_string(UpperBound) +
                        "; declared within loop = " + std::to_string(Declared);
      return Str;
    }
  };

  /// LoopInfo list type
  typedef std::list<LoopInfo> LoopList;

  /// From the result of a matcher it gets the loop hierarchy
  static LoopList getLoopList(const MatchFinder::MatchResult &Result);

  static LoopList getLoopList(clang::ForStmt *ForLoop);

  /// Empty constructor
  StmtWrapper(){};

  /// Be clean
  ~StmtWrapper() {
    for (Stmt *V : this->ListStmt) {
      // delete V;
    }
  };

  /// Constructor
  StmtWrapper(clang::CompoundStmt *CS, ScopLoc *SL) {
    /// Get loop information
    if (!CS) {
      Utils::printDebug("StmtWrapper", "UNREACHABLE");
    }
    for (auto ST : CS->body()) {
      Utils::printDebug("StmtWrapper", ST->getStmtClassName());
      unsigned int Start =
          Utils::getSourceMgr()->getExpansionLineNumber(ST->getBeginLoc());
      unsigned int End =
          Utils::getSourceMgr()->getExpansionLineNumber(ST->getEndLoc());
      Utils::printDebug("StmtWrapper", "start = " + std::to_string(Start));
      Utils::printDebug("StmtWrapper", "end = " + std::to_string(End));
      if (auto Loop = dyn_cast<ForStmt>(ST)) {
        if ((SL->StartLine <= Start) && (SL->EndLine >= End)) {
          this->LoopL.push_back(getLoopList(Loop));
        }
      }
    }
    /// Convert the expression to a set of TACs
    this->TacToStmt =
        TAC::exprToTAC(CS, &this->ListOfTAC, &this->ListStmt, &this->TacList);
  }

  /// Perform unrolling for a given statement given its unroll factor and the
  /// upperbound of the loop
  void unroll(long UnrollFactor, long UpperBound, std::string LoopLevel);
  /// Unrolls the TAC list in all the possible dimensions
  void unrollAndJam(long UnrollFactor, long UpperBoundFallback = UBFallback);
  /// Get LoopInfo
  std::vector<LoopList> getLoopInfo() { return this->LoopL; }
  /// Return the mapping of TAC to Stmt
  std::map<int, int> getTacToStmt() { return this->TacToStmt; }
  /// Get Clang Stmt
  std::vector<Stmt *> getStmt() { return this->ListStmt; };
  /// Set Clang Stmt
  void setStmt(std::vector<Stmt *> LS) { this->ListStmt = LS; };
  /// Get TAC list
  TacListType getTacList() { return this->TacList; };
  /// Set TAC lsit
  void setTacList(TacListType TacList) { this->TacList = TacList; };

private:
  /// Statement holded
  std::vector<Stmt *> ListStmt;
  /// Sorted list of TACs
  std::vector<TacListType> ListOfTAC;
  /// Loop list
  std::vector<LoopList> LoopL;
  /// TAC list with regard to the Statement S
  TacListType TacList;
  /// Map from TacID to Stmt number
  std::map<int, int> TacToStmt;
};
} // namespace macveth
#endif // MACVETH_STMTWRAPPER_H
