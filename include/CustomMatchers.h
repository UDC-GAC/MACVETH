/**
 * File              : CustomMatchers.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 15 Nov 2019 09:15:23 MST
 * Last Modified Date: Dom 01 Dec 2019 10:39:53 MST
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
#include "include/StmtWrapper.h"
#include "include/TAC.h"
#include "include/Utils.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ParentMap.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Analysis/CFG.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
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
  IterationHandler(Rewriter &R) : Rewrite(R) {}

  virtual void run(const MatchFinder::MatchResult &Result);

private:
  Rewriter &Rewrite;
};

// Matcher related functions for simplifying
StatementMatcher reductionStmt(std::string Name, std::string Lhs,
                               std::string Rhs);
StatementMatcher assignArrayBinOp(std::string Name, std::string Lhs,
                                  std::string Rhs);
StatementMatcher forLoopMatcher(std::string Name, StatementMatcher InnerStmt);
StatementMatcher forLoopNested(int NumLevels, StatementMatcher InnerStmt);
} // namespace matchers_utils
} // namespace macveth
#endif
