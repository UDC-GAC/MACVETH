/**
 * File              : StmtWrapper.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 22 Nov 2019 09:05:09 MST
 * Last Modified Date: Lun 23 Mar 2020 18:33:25 CET
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
    long InitVal = -1;
    /// Initial value if it can not be evaluated
    std::string StrInitVal = "";
    /// Upperbound value (-1 if not known)
    long UpperBound = 0;
    /// Comparator upperbound
    BinaryOperator::Opcode UpperBoundComp = BinaryOperator::Opcode::BO_LT;
    /// Upperbound as string
    std::string StrUpperBound = "";
    /// Stride or step
    long Step = UBFallback;
    /// Stride or step after unrolling
    long StepUnrolled = UBFallback;
    /// TODO Leftover values: if Upperbound is -1, then LeftOver as well
    long LeftOver = -1;
    /// Variable declared in the loop creation
    bool Declared = false;
    /// Unrolling factor
    int UnrollFactor = UBFallback;
    /// Full unroll
    bool FullyUnrolled = false;
    /// Location of the initialization in the loop
    clang::CharSourceRange SRVarInit;
    /// Location of the condition of the loop
    clang::CharSourceRange SRVarCond;
    /// Location of the increment in the loop
    clang::CharSourceRange SRVarInc;

    /// Empty constructor
    LoopInfo() {}

    /// Known bounds
    bool knownBounds() { return (UpperBound != -1) && (InitVal != -1); }

    /// Get epilogs of given dimensions and
    static std::string getEpilogs(StmtWrapper *SWrap);

    static std::string getDimDeclarations(std::list<std::string> DimsDeclared);

    /// For debugging purposes
    std::string toString() {
      return Dim + "; init val = " + std::to_string(InitVal) +
             ", upperbound = " + std::to_string(UpperBound) +
             ", step = " + std::to_string(Step) +
             "; declared = " + std::to_string(Declared) +
             "; stepunrolled = " + std::to_string(StepUnrolled) +
             "; knownbounds = " + std::to_string(knownBounds());
    }

    static void clearDims() { LoopInfo::DimDeclared.clear(); }
  };

  /// LoopInfo list type
  using LoopList = std::list<LoopInfo>;

  /// Generate a list of StmtWrapper
  static std::list<StmtWrapper *> genStmtWraps(CompoundStmt *CS, ScopLoc *Scop);

  /// Get all the loops given one
  static LoopInfo getLoop(clang::ForStmt *ForLoop);

  /// Default destructor
  ~StmtWrapper(){};

  /// Empty constructor
  StmtWrapper(){};

  /// Constructor
  StmtWrapper(clang::Stmt *S);

  /// Compute the sequential cost
  static long computeSequentialCostStmtWrapper(std::list<StmtWrapper *> SL);

  /// Compute the cost of the statments within
  long getCost();

  /// Perform unrolling for a given statement given its unroll factor and the
  /// upperbound of the loop
  TacListType unroll(LoopInfo L);

  /// Unrolls the TAC list in all the possible dimensions
  bool unrollAndJam(std::list<LoopInfo> LI, ScopLoc *Scop);

  /// Unrolls the TAC list in the specified dimensions
  bool unrollByDim(std::list<LoopInfo> LI, ScopLoc *Scop);

  /// Get list of stmts
  std::list<StmtWrapper *> getListStmt() { return this->ListStmt; }

  /// Get LoopInfo
  LoopInfo getLoopInfo() { return this->LoopL; }

  /// Get Clang Stmt
  Stmt *getClangStmt() { return this->ClangStmt; };

  /// Get TAC list
  TacListType getTacList() { return TacList; };

  /// Set TAC list
  void setTacList(TacListType TacList) { this->TacList = TacList; };

  /// Check if it is a loop
  bool isLoop() { return dyn_cast<ForStmt>(this->ClangStmt); }

  /// Check if StmtWrapper holds a leftover (i.e. does not hold a loop)
  bool isLeftOver() { return !this->isLoop(); }

  /// Set the name of the loop surrounding this Stmt
  void setInnerLoopName(std::string InnerLoopName) {
    this->InnerLoopName = InnerLoopName;
    TacListType NewTac;
    for (auto T : this->getTacList()) {
      auto NT = T;
      NT.setLoopName(InnerLoopName);
      NewTac.push_back(NT);
    }
    this->setTacList(NewTac);
  }
  /// Get the name of the loop surrounding this Stmt
  std::string getInnerLoopName() { return this->InnerLoopName; }

    void setNotVectorized() { this->Vectorized = false; }
  bool isVectorized() { return this->Vectorized; }

  /// Get scop
  long getScop() { return this->TacList.front().getScop(); }

private:
  /// Statements holded if loop
  std::list<StmtWrapper *> ListStmt;
  /// Statement if not loop
  Stmt *ClangStmt;
  /// Loop information if needed
  LoopInfo LoopL;
  /// TAC list with regard to the Statement S
  TacListType TacList;
  /// Loop within
  std::string InnerLoopName = "";
  /// Has been vectorized or not
  bool Vectorized = true;
};
} // namespace macveth
#endif /* !MACVETH_STMTWRAPPER_H */
