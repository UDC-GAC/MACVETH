// MACVETH - StmtWrapper.h
//
// Copyright (c) Colorado State University. 2019-2021
// Copyright (c) Universidade da Coruña. 2020-2021
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Authors:
//     Marcos Horro <marcos.horro@udc.es>
//     Louis-Noël Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Marcos Horro <marcos.horro@udc.es>

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
    clang::ForStmt *LoopStmt = nullptr;
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

    /// Check if bounds are known, i.e. if values are known at compiler time
    bool knownBounds() { return (UpperBound != -1) && (InitVal != -1); }

    /// Get epilogs given a StmtWrapper, which may contain loops within
    static std::string getEpilogs(StmtWrapper *SWrap);

    /// Get a list of the dimensions declared in the loop
    static std::string getDimDeclarations(std::list<std::string> DimsDeclared);

    std::string getDim() { return this->Dim; }

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

  /// Main constructor, based on clang's Stmt
  StmtWrapper(clang::Stmt *S);

  /// Perform unrolling for a given statement given its unroll factor and the
  /// upperbound of the loop
  TacListType unroll(LoopInfo L);

  /// Unrolls the TAC list in all the possible dimensions
  bool unrollAndJam(std::list<LoopInfo> LI, ScopLoc *Scop);

  /// Unrolls the TAC list in the specified dimensions
  bool unrollByDim(std::list<LoopInfo> LI, ScopLoc *Scop);

  /// Unroll a list of given loops and return if full unrolling was performed
  bool unrollLoopList(std::list<LoopInfo> LI);

  /// Get list of statements
  std::list<StmtWrapper *> getListStmt() { return this->ListStmt; }

  /// Get LoopInfo structure
  LoopInfo getLoopInfo() { return this->LoopInfoStmt; }

  /// Get Clang Stmt
  Stmt *getClangStmt() { return this->ClangStmt; };

  /// Get TAC list
  TacListType getTacList() { return TacList; };

  /// Set TAC list in the StmtWrapper
  void setTacList(TacListType TacList) { this->TacList = TacList; };

  /// StmtWrapper may me a "regular" statement or a for loop. This is useful
  /// for identifying properly regions
  bool isLoop() { return dyn_cast<ForStmt>(this->ClangStmt); }

  /// Check if StmtWrapper holds a leftover (i.e. does not hold a loop)
  bool isLeftOver() { return !this->isLoop(); }

  /// Set the name of the loop surrounding this Stmt
  void setInnerLoopInfo(LoopInfo InnerLoopInfo) {
    this->InnerLoopInfo = InnerLoopInfo;
    TacListType NewTac;
    for (auto T : this->getTacList()) {
      auto NT = T;
      NT.setLoopName(InnerLoopInfo.getDim());
      NewTac.push_back(NT);
    }
    this->setTacList(NewTac);
  }

  /// Get the name of the loop surrounding this clang::Stmt
  std::string getInnerLoopName() { return this->InnerLoopInfo.getDim(); }

  bool isInLoop() { return this->InnerLoopInfo.getDim() != ""; }

  void setNotVectorized() { this->Vectorized = false; }

  bool isVectorized() { return this->Vectorized; }

  long getScop() { return this->TacList.front().getScop(); }

private:
  /// Statements hold if loop
  std::list<StmtWrapper *> ListStmt;
  /// Statement if not loop
  Stmt *ClangStmt;
  /// Loop information if needed
  LoopInfo LoopInfoStmt;
  /// TAC list with regard to the Statement S
  TacListType TacList;
  /// Loop within the statement is. This only makes sense if Stmt is not a loop
  LoopInfo InnerLoopInfo;
  /// Has been vectorized or not
  bool Vectorized = true;
};
} // namespace macveth
#endif /* !MACVETH_STMTWRAPPER_H */
