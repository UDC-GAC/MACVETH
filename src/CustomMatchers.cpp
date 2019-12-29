/**
 * File              : CustomMatchers.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 15 Nov 2019 09:23:38 MST
 * Last Modified Date: Dom 29 Dec 2019 13:07:25 MST
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
#include "include/StmtWrapper.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include <iostream>
#include <string>

// using namespace macveth;
typedef clang::ast_matchers::internal::Matcher<clang::ForStmt> MatcherForStmt;

/// FIXME please, need to pass arguments to the Matcher, in order to determine
///       the 1) unrolling factor and 2) the stride
void matchers_utils::IterationHandler::run(
    const MatchFinder::MatchResult &Result) {
  std::list<const DeclRefExpr *> IncVarList;

  std::cout << "LLEGOOOOOOOOOO\n";

  /// FIXME
  /// This is a hack of bad taste, really
  const BinaryOperator *TacBinOp =
      Result.Nodes.getNodeAs<clang::BinaryOperator>("assignArrayBinOp") ==
              nullptr
          ? Result.Nodes.getNodeAs<clang::BinaryOperator>("reduction")
          : Result.Nodes.getNodeAs<clang::BinaryOperator>("assignArrayBinOp");

  /// FIXME
  if (Result.Nodes.getNodeAs<clang::BinaryOperator>("reduction") == nullptr)
    return;

  StmtWrapper *SWrap = new StmtWrapper(TacBinOp);

  /// FIXME: please you can not do this not even in 101
  const Expr *UpperBoundExpr =
      Result.Nodes.getNodeAs<clang::Expr>(varnames::UpperBound + "1");
  int UpperBound = 32;
  if (UpperBoundExpr != nullptr) {
    int res = Utils::getIntFromExpr(UpperBoundExpr, this->getCtx());
    UpperBound = res == -1 ? UpperBound : res;
  }
  int UnrollFactor = 1;
  const VarDecl *V = Result.Nodes.getNodeAs<clang::VarDecl>(
      matchers_utils::varnames::NameVarInit + std::to_string(1));
  SWrap->unroll(UnrollFactor, 6, V->getNameAsString());
  for (TAC t : SWrap->getTacList())
    t.printTAC();
  std::cout << "Creating CDAG" << std::endl;
  CDAG *G = CDAG::createCDAGfromTAC(SWrap->getTacList());
  std::cout << "CDAG done" << std::endl;
  CDAG::computeFreeSchedule(G);
  CDAG::computeCostModel(G);
  SWrap->translateTacToIntrinsics();
  int NLevel = 1;
  /// Unroll factor applied to the for header
  for (int Inc = NLevel; Inc > 0; --Inc) {
    const UnaryOperator *IncVarPos =
        Result.Nodes.getNodeAs<clang::UnaryOperator>(varnames::NameVarIncPos +
                                                     std::to_string(Inc));
    const DeclRefExpr *IncVarName = Result.Nodes.getNodeAs<DeclRefExpr>(
        varnames::NameVarInc + std::to_string(Inc));
    clang::CharSourceRange charRange = clang::CharSourceRange::getTokenRange(
        IncVarPos->getBeginLoc(), IncVarPos->getEndLoc());
    Rewrite.ReplaceText(charRange, IncVarName->getNameInfo().getAsString() +
                                       "+=" + std::to_string(UpperBound));
  }

  clang::CharSourceRange CharRangeStr =
      clang::CharSourceRange::getTokenRange(TacBinOp->getSourceRange());

  for (InstListType IList : SWrap->getInstList()) {
    for (std::string S : IList) {
      Rewrite.InsertText(TacBinOp->getBeginLoc(), S + ";\n", true, true);
    }
  }
  Rewrite.InsertText(TacBinOp->getBeginLoc(), "//", true, true);
}

/// Possible RHS
/// var[][]..[]
/// var
StatementMatcher possibleRhs(std::string BindName) {
  // return anyOf(implicitCastExpr().bind(BindName),
  //             binaryOperator().bind(BindName));
  return (binaryOperator().bind(BindName));
}

// Function wrapper for matching expressions such as:
// var      = expr op expr
// StatementMatcher matchers_utils::anyStmt(std::string Name, std::string Lhs,
//                                         std::string Rhs) {
//  return expr(anyOf(hasOperatorName("="), hasOperatorName("+="),
//                    hasOperatorName("*="), hasOperatorName("-=")),
//              hasLHS(implicitCastExpr().bind(Lhs)),
//              hasRHS(implicitCastExpr().bind(Rhs)))
//      // hasRHS(possibleRhs(Rhs)))
//      .bind(Name);
//}

/// Function wrapper for matching expressions such as:
/// var      = expr op expr
StatementMatcher matchers_utils::reductionStmt(std::string Name,
                                               std::string Lhs,
                                               std::string Rhs) {
  return binaryOperator(anyOf(hasOperatorName("="), hasOperatorName("+=")),
                        hasLHS(declRefExpr().bind(Lhs)),
                        hasRHS(ignoringImpCasts(possibleRhs(Rhs))))
      // hasRHS(possibleRhs(Rhs)))
      .bind(Name);
}

/// Function wrapper for matching expressions such as:
/// var[idx] = expr op expr
StatementMatcher matchers_utils::assignArrayBinOp(std::string Name,
                                                  std::string Lhs,
                                                  std::string Rhs) {
  return binaryOperator(hasOperatorName("="),
                        hasLHS(arraySubscriptExpr().bind(Lhs)),
                        hasRHS(possibleRhs(Rhs)))
      .bind(Name);
}

/// This matches either int i = 0 or int i = val
MatcherForStmt customLoopInit(std::string Name) {
  return hasLoopInit(declStmt(hasSingleDecl(
      varDecl(hasInitializer(
                  anyOf(integerLiteral(equals(0)),
                        ignoringImpCasts(declRefExpr(hasType(isInteger()))))))
          .bind(matchers_utils::varnames::NameVarInit + Name))));
}

MatcherForStmt customIncrement(std::string Name) {
  return hasIncrement(
      unaryOperator(hasOperatorName("++"),
                    hasUnaryOperand(
                        declRefExpr(to(varDecl(hasType(isInteger()))))
                            .bind(matchers_utils::varnames::NameVarInc + Name)))
          .bind(matchers_utils::varnames::NameVarIncPos + Name));
}

/// This condition allows for loops such as:
/// * (int)var < (int)val
MatcherForStmt customCondition(std::string Name) {
  return hasCondition(binaryOperator(
      hasOperatorName("<"),
      hasLHS(ignoringParenImpCasts(declRefExpr(
          to(varDecl(hasType(isInteger()))
                 .bind(matchers_utils::varnames::NameVarCond + Name))))),
      hasRHS(expr(hasType(isInteger()))
                 .bind(matchers_utils::varnames::UpperBound + Name))));
}

/// Body of the loops we are looking for. In this case could be something like:
/// * for() { InnerStmt; }
/// * for() { [Stmt]* [InnerStmt]+ [InnerStmt|Stmt]*}
MatcherForStmt customBody(StatementMatcher InnerStmt) {
  return hasBody(has(InnerStmt));
  // return hasBody(anyOf(InnerStmt, compoundStmt(forEach(InnerStmt))));
}

/// Function wrapper for matching expressions such as:
/// for (int var = 0; var < upper_bound; [++]var[++])
StatementMatcher matchers_utils::forLoopMatcher(std::string Name,
                                                StatementMatcher InnerStmt) {
  return forStmt(customLoopInit(Name), customIncrement(Name),
                 customCondition(Name), customBody(InnerStmt))
      .bind("forLoop" + Name);
}

/// Function wrapper for loop nests with the forLoopMatcher form
StatementMatcher matchers_utils::forLoopNested(int NumLevels,
                                               StatementMatcher InnerStmt) {
  StatementMatcher NestedMatcher = InnerStmt;
  for (int N = NumLevels; N > 0; --N) {
    NestedMatcher =
        matchers_utils::forLoopMatcher(std::to_string(N), NestedMatcher);
  }
  return NestedMatcher;
}

/// Function wrapper for loop nests with the forLoopMatcher form
StatementMatcher matchers_utils::ROI(int NumLevels,
                                     StatementMatcher InnerStmt) {
  StatementMatcher NestedMatcher =
      matchers_utils::forLoopNested(NumLevels, InnerStmt);
  StatementMatcher Annot =
      compoundStmt(has(declStmt(hasSingleDecl(
                       varDecl(hasInitializer(integerLiteral(equals(42))),
                               hasAnyName("begin_roi"))))),
                   has(NestedMatcher));
  return Annot;
}
