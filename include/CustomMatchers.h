/**
 * File              : CustomMatchers.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 15 Nov 2019 09:15:23 MST
 * Last Modified Date: Dom 29 Dec 2019 10:20:16 MST
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
#ifndef MACVETH_CUSTOM_MATCHERS_H
#define MACVETH_CUSTOM_MATCHERS_H

#include "include/Intrinsics/IntrinsicsGenerator.h"
#include "include/MVExpr/MVExpr.h"
#include "include/StmtWrapper.h"
#include "include/TAC.h"
#include "include/Utils.h"
#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include <string>

// Do not use namespace std
// using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;

namespace macveth {
namespace matchers_utils {

namespace varnames {
const std::string NameVarInc = "incVar";
const std::string NameVarIncPos = "incVarPos";
const std::string NameVarInit = "initVar";
const std::string NameVarCond = "condVar";

const std::string UpperBound = "upperBound";
} // namespace varnames

// IteartionHandler is called every time we find an assignment like:
// BinaryOperator(lhs, rhs, "=") inside a loop
class IterationHandler : public MatchFinder::MatchCallback {
public:
  /// Basic constructor
  IterationHandler(Rewriter &R) : Rewrite(R) {}
  /// Basic constructor with context
  IterationHandler(Rewriter &R, ASTContext *C) : Rewrite(R), Ctx(C) {}

  /// Override run method from MatchFinder
  virtual void run(const MatchFinder::MatchResult &Result);

  /// Get the ASTContext
  const ASTContext *getCtx() { return Ctx; }

private:
  /// For rewriting code in the output program
  Rewriter &Rewrite;
  /// A copy of the ASTContext for rewriting purposes
  const ASTContext *Ctx;
};

/// Matching any type of statement no matter the assignment operator or
/// expression type
StatementMatcher anyStmt(std::string Name, std::string Lhs, std::string Rhs);
/// Matching reduction statements
StatementMatcher reductionStmt(std::string Name, std::string Lhs,
                               std::string Rhs);
/// Matching any type of binary operations
StatementMatcher assignArrayBinOp(std::string Name, std::string Lhs,
                                  std::string Rhs);
/// Matcher for any for loop
StatementMatcher forLoopMatcher(std::string Name, StatementMatcher InnerStmt);
/// Matcher for NumLevels nested for loops with InnerStmt type
StatementMatcher forLoopNested(int NumLevels, StatementMatcher InnerStmt);
} // namespace matchers_utils
} // namespace macveth
#endif
