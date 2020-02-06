/**
 * File              : CustomMatchers.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 15 Nov 2019 09:23:38 MST
 * Last Modified Date: Mar 14 Xan 2020 12:10:46 MST
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

#include "include/CustomMatchers.h"
#include "include/CDAG.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include <iostream>
#include <llvm-10/llvm/ADT/StringRef.h>
#include <llvm-10/llvm/Support/ErrorHandling.h>
#include <string>

using namespace macveth::matchers_utils;

// ---------------------------------------------
ScopLoc *getScopLoc(StmtWrapper *S, ScopHandler *SL) {
  auto SLoc = S->getStmt()[0]->getBeginLoc();
  for (int n = 0; n < SL->List.size(); ++n) {
    if ((SLoc >= SL->List[n].Scop) && (SLoc <= SL->List[n].EndScop)) {
      return &SL->List[n];
    }
  }
  return NULL;
}

// ---------------------------------------------
bool IterationHandler::checkIfWithinScop(StmtWrapper *S) {
  auto Scop = getScopLoc(S, this->SL);
  if ((Scop != NULL)) {
    bool NewStmt = false;
    for (auto L : S->getLoopInfo()) {
      if (std::find(Scop->DimVisited.begin(), Scop->DimVisited.end(), L.Dim) ==
          Scop->DimVisited.end()) {
        Scop->DimVisited.push_back(L.Dim);
        NewStmt = true;
      }
    }
    return NewStmt;
  }
  return false;
}

// ---------------------------------------------
void IterationHandler::unrollOptions(StmtWrapper *S) {
  auto SLoc = S->getStmt()[0]->getBeginLoc();
  auto Scop = getScopLoc(S, SL);
  assert(Scop != NULL && "Scop not found for these statements");
  if (Scop->PA.UnrollAndJam) {
    S->unrollAndJam(Scop->PA.UnrollFactor);
  }
}

// ---------------------------------------------
void IterationHandler::run(const MatchFinder::MatchResult &Result) {
  // Get the info about the loops surrounding this statement
  StmtWrapper *SWrap = new StmtWrapper(Result);

  // If this pragma has been already visited, then we skip the analysis. This
  // may happen because of the implementatio of the AST finders and matchers
  if (!checkIfWithinScop(SWrap)) {
    return;
  }

  // Perform unrolling according to the pragmas
  unrollOptions(SWrap);

  // Debugging purposes
  for (auto S : SWrap->getTacList()) {
    S.printTAC();
  }

  // Creating the CDAG
  CDAG *G = CDAG::createCDAGfromTAC(SWrap->getTacList());

  // Computing the free schedule of the CDAG created
  CDAG::computeFreeSchedule(G);

  SIMDGenerator *SIMDGen = SIMDGeneratorFactory::getBackend(MVOptions::ISA);

  // Computing the cost model of the CDAG created
  auto SInfo = CDAG::computeCostModel(G, SIMDGen);

  // Unroll factor applied to the for header
  int Inc = 1;
  for (auto Loop : SWrap->getLoopInfo()) {
    int UpperBound = Loop.Step;
    const UnaryOperator *IncVarPos =
        Result.Nodes.getNodeAs<clang::UnaryOperator>(varnames::NameVarIncPos +
                                                     std::to_string(Inc));
    const DeclRefExpr *IncVarName = Result.Nodes.getNodeAs<DeclRefExpr>(
        varnames::NameVarInc + std::to_string(Inc++));
    clang::CharSourceRange charRange = clang::CharSourceRange::getTokenRange(
        IncVarPos->getBeginLoc(), IncVarPos->getEndLoc());
    Rewrite.ReplaceText(charRange, IncVarName->getNameInfo().getAsString() +
                                       " += " + std::to_string(UpperBound));
  }

  // Print new lines
  for (auto InsSIMD : SIMDGen->renderSIMDasString(SInfo.SIMDList)) {
    Rewrite.InsertText(SWrap->getStmt()[0]->getBeginLoc(), InsSIMD + "\n", true,
                       true);
  }

  // Comment statements
  for (auto S : SWrap->getStmt()) {
    Rewrite.InsertText(S->getBeginLoc(), "// statement replaced: ", true, true);
  }
}

typedef clang::ast_matchers::internal::Matcher<clang::ForStmt> MatcherForStmt;

// ---------------------------------------------
MatcherForStmt customLoopInit(std::string Name) {
  // This matches either int i = 0 or int i = val
  return hasLoopInit(declStmt(hasSingleDecl(
      varDecl(hasInitializer(
                  anyOf(integerLiteral(equals(0)),
                        ignoringImpCasts(declRefExpr(hasType(isInteger()))))))
          .bind(matchers_utils::varnames::NameVarInit + Name))));
}

// ---------------------------------------------
MatcherForStmt customIncrement(std::string Name) {
  return hasIncrement(
      unaryOperator(hasOperatorName("++"),
                    hasUnaryOperand(
                        declRefExpr(to(varDecl(hasType(isInteger()))))
                            .bind(matchers_utils::varnames::NameVarInc + Name)))
          .bind(matchers_utils::varnames::NameVarIncPos + Name));
}

// ---------------------------------------------
MatcherForStmt customCondition(std::string Name) {
  // This condition allows for loops such as:
  // * (int)var < (int)val
  return hasCondition(binaryOperator(
      hasOperatorName("<"),
      hasLHS(ignoringParenImpCasts(declRefExpr(
          to(varDecl(hasType(isInteger()))
                 .bind(matchers_utils::varnames::NameVarCond + Name))))),
      hasRHS(expr(hasType(isInteger()))
                 .bind(matchers_utils::varnames::UpperBound + Name))));
}

// ---------------------------------------------
MatcherForStmt customBody(StatementMatcher InnerStmt) {
  // Body of the loops we are looking for. In this case could be something like:
  // * for() { InnerStmt; }
  // * for() { [Stmt]* [InnerStmt]+ [InnerStmt|Stmt]*}
  return hasBody(anyOf(InnerStmt, compoundStmt(forEach(InnerStmt))));
}

// ---------------------------------------------
StatementMatcher matchers_utils::forLoopMatcher(std::string Name,
                                                StatementMatcher InnerStmt) {
  // Function wrapper for matching expressions such as:
  // for (int var = 0; var < upper_bound; [++]var[++])
  return forStmt(customLoopInit(Name), customIncrement(Name),
                 customCondition(Name), customBody(InnerStmt))
      .bind("forLoop" + Name);
}

// ---------------------------------------------
StatementMatcher matchers_utils::forLoopNested(int NumLevels,
                                               StatementMatcher InnerStmt) {
  // Function wrapper for loop nests with the forLoopMatcher form
  StatementMatcher NestedMatcher = InnerStmt;
  for (int N = NumLevels; N > 0; --N) {
    NestedMatcher =
        matchers_utils::forLoopMatcher(std::to_string(N), NestedMatcher);
  }
  return NestedMatcher;
}

// ---------------------------------------------
StatementMatcher matchers_utils::ROI(int NumLevels,
                                     StatementMatcher InnerStmt) {
  // Function wrapper for loop nests with the forLoopMatcher form
  StatementMatcher NestedMatcher =
      matchers_utils::forLoopNested(NumLevels, InnerStmt);
  StatementMatcher Annot = compoundStmt(forEach(NestedMatcher));
  return Annot;
}
