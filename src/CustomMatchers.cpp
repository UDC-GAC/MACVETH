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
#include "clang/Basic/AttrKinds.h"
#include "clang/Basic/SourceLocation.h"
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
  bool NewStmt = false;
  if ((Scop != NULL)) {
    for (auto L : S->getLoopInfo()) {
      if (std::find(Scop->DimVisited.begin(), Scop->DimVisited.end(), L.Dim) ==
          Scop->DimVisited.end()) {
        if (Scop->DimVisited.size() == 0) {
          // Clearing mapping
          StmtWrapper::LoopInfo::DimDeclared.clear();
        }
        Scop->DimVisited.push_back(L.Dim);
        // New dimension
        NewStmt = true;
      }
    }
    // The scop is not recognized
    return NewStmt;
  }

  return NewStmt;
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
void rewriteLoops(StmtWrapper *SWrap, Rewriter *Rewrite) {
  int Inc = 0;
  std::list<StmtWrapper::LoopInfo> Dims = {};
  std::list<std::string> DimsDeclared = {};
  // Rewrite loop header
  for (auto Loop : SWrap->getLoopInfo()) {
    Utils::printDebug("CustomMatchers", "Rewriting loop = " + Loop.Dim);
    // Rewrite headers
    Rewrite->ReplaceText(Loop.SRVarInit,
                         Loop.Dim + " = " + std::to_string(Loop.InitVal));
    // IMPORTANT: need to put ";" at the end since the range is calculated as
    // the -1 offset of the location of the increment. This is done like this
    // because the SourceLocation of the UpperBound could be a macro variable
    // located in another place. This happens, for instance, with the loop
    // bounds in PolyBench suite
    Rewrite->ReplaceText(Loop.SRVarCond,
                         "(" + Loop.Dim + " + " + std::to_string(Loop.Step) +
                             ") <= " + Loop.StrUpperBound + ";");
    Rewrite->ReplaceText(Loop.SRVarInc,
                         Loop.Dim + " += " + std::to_string(Loop.Step));
    if (Loop.Declared) {
      std::list<std::string> L = StmtWrapper::LoopInfo::DimDeclared;
      if (!(std::find(L.begin(), L.end(), Loop.Dim) != L.end())) {
        StmtWrapper::LoopInfo::DimDeclared.push_back(Loop.Dim);
        DimsDeclared.push_back(Loop.Dim);
      }
    }
    Dims.push_front(Loop);
    std::string Epilog =
        StmtWrapper::LoopInfo::getEpilogs(Dims, SWrap->getStmt());
    Rewrite->InsertTextAfterToken(Loop.EndLoc, Epilog + "\n");
  }
  // Declare variables
  SourceLocation Loc = SWrap->getLoopInfo().back().BegLoc;
  Rewrite->InsertTextBefore(
      Loc, StmtWrapper::LoopInfo::getDimDeclarations(DimsDeclared));
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

  // Get SIMD generator according to the option chosen
  SIMDGenerator *SIMDGen = SIMDGeneratorFactory::getBackend(MVOptions::ISA);

  // Computing the cost model of the CDAG created
  auto SInfo = CDAG::computeCostModel(G, SIMDGen);

  // FIXME: create epilog as well
  // Unroll factor applied to the for header

  rewriteLoops(SWrap, &Rewrite);

  // Printing the registers we are going to use
  for (auto InsSIMD : SIMDGen->renderSIMDRegister(SInfo.SIMDList)) {
    Rewrite.InsertText(SWrap->getStmt()[0]->getBeginLoc(), InsSIMD + "\n", true,
                       true);
  }
  for (auto InsSIMD : SInfo.SIMDList) {
    Rewrite.InsertText(
        SWrap->getStmt()[SWrap->getTacToStmt()[InsSIMD.TacID]]->getBeginLoc(),
        InsSIMD.render() + ";\t// cost = " + std::to_string(InsSIMD.Cost) +
            "\n",
        true, true);
  }

  // Comment statements
  for (auto S : SWrap->getStmt()) {
    Rewrite.InsertText(S->getBeginLoc(), "// statement replaced: ", true, true);
  }

  // Be clean
  delete SWrap;
  delete G;
}

typedef clang::ast_matchers::internal::Matcher<clang::ForStmt> MatcherForStmt;

// ---------------------------------------------
MatcherForStmt customLoopInit(std::string Name) {
  // This matches either int i = 0 or int i = val
  return hasLoopInit(anyOf(
      binaryOperator(
          hasOperatorName("="),
          hasLHS(declRefExpr(hasType(isInteger()))
                     .bind(matchers_utils::varnames::NameVarInitNotDeclared +
                           Name)),
          hasRHS(
              integerLiteral().bind(matchers_utils::varnames::ValInit + Name))),
      declStmt(hasSingleDecl(
          varDecl(hasInitializer(anyOf(
                      integerLiteral().bind(matchers_utils::varnames::ValInit +
                                            Name),
                      ignoringImpCasts(
                          declRefExpr(hasType(isInteger()))
                              .bind(matchers_utils::varnames::NameValInit +
                                    Name)))))
              .bind(matchers_utils::varnames::NameVarInit + Name)))));
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
  return hasCondition(
      binaryOperator(
          hasOperatorName("<"),
          hasLHS(ignoringParenImpCasts(declRefExpr(
              to(varDecl(hasType(isInteger()))
                     .bind(matchers_utils::varnames::NameVarCond + Name))))),
          hasRHS(expr().bind(matchers_utils::varnames::UpperBound + Name)))
          .bind(matchers_utils::varnames::ExprCond + Name));
}

// ---------------------------------------------
MatcherForStmt customBody(StatementMatcher InnerStmt) {
  // Body of the loops we are looking for. In this case could be something
  // like:
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
