/**
 * File              : macveth_translator.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 06 Nov 2019 12:29:24 MST
 * Last Modified Date: Dom 01 Dec 2019 16:04:56 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 * Original Code     : Eli Bendersky <eliben@gmail.com>
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

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <tuple>

#include "include/CustomMatchers.h"
#include "include/Intrinsics/IntrinsicsGenerator.h"
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
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/raw_ostream.h"

/// Do not use namespace std
/// using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;
using namespace macveth;

/// SOME NOTES:
/// * Stmt - statements, could be a for loop, while, a single statement
/// * Decl - declarations (or defenitions) of variables, typedef, function...
/// * Type - types, CanonicalType, Builtin-type
/// * Expr - expressions, they inherit from Stmt tho; this is quite weird for
/// me...

/// Implementation of the ASTConsumer interface for reading an AST produced
/// by the Clang parser. It registers a couple of matchers and runs them on
/// the AST.
class MACVETHConsumer : public ASTConsumer {
public:
  MACVETHConsumer(Rewriter &R, ASTContext *C)
      : HandlerReduction(R), HandlerVector(R), Context(C) {
    /// For reduction statements
    StatementMatcher ForLoopNestedMatcherRed = matchers_utils::forLoopMatcher(
        "1", matchers_utils::reductionStmt("reduction", "lhs", "rhs"));
    Matcher.addMatcher(ForLoopNestedMatcherRed, &HandlerReduction);

    /// For vectorizable statements
    StatementMatcher ForLoopNestedMatcherVec = matchers_utils::forLoopMatcher(
        "1",
        matchers_utils::assignArrayBinOp("assignArrayBinOp", "lhs", "rhs"));
    Matcher.addMatcher(ForLoopNestedMatcherVec, &HandlerVector);
  }

  void HandleTranslationUnit(ASTContext &Context) override {
    /// Run the matchers when we have the whole TU parsed.
    Matcher.matchAST(Context);
  }

private:
  ASTContext *Context;
  matchers_utils::IterationHandler HandlerReduction;
  matchers_utils::IterationHandler HandlerVector;
  MatchFinder Matcher;
};

/// SECOND STEP
/// For each source file provided to the tool, a new ASTFrontendAction is
/// created, which inherits from FrontendAction (abstract class)
class MACVETHFrontendAction : public ASTFrontendAction {
public:
  /// empty constructor
  MACVETHFrontendAction() {}

  /// This routine is called in BeginSourceFile(), from
  /// CreateWrapperASTConsumer.
  /// * CompilterInstance CI: got from getCompilerInstance()
  /// * StringRef file: input file, provided by getCurrentFile()
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override {
    /// setSourceMgr: setter for the Rewriter
    /// * SourceManager: handles loading and caching of source files into
    /// memory. It can be queried for information such as SourceLocation
    /// of objects.
    /// * LangOptions: controls the dialect of C/C++ accepted
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    Utils::setOpts(&CI.getSourceManager(), &CI.getLangOpts());
    /// std::make_unique is C++14, while LLVM is written in C++11, this
    /// is the reason of this custom implementation
    return llvm::make_unique<MACVETHConsumer>(TheRewriter, &CI.getASTContext());
  }

  /// this is called only following a successful call to
  /// BeginSourceFileAction (and BeginSourceFile)
  void EndSourceFileAction() override {
    /// 1.- Get RewriteBuffer from FileID
    /// 2.- Write to Stream (in this case llvm::outs(), which is
    /// std::out) the result of applying all changes to the original
    /// buffer. Original buffer is modified before calling this function,
    /// from the ASTConsumer
    TheRewriter.getEditBuffer(TheRewriter.getSourceMgr().getMainFileID())
        .write(llvm::outs());
  }

private:
  /// Main interfacer to the rewrite buffers: dispatches high-level
  /// requests to the low-level RewriteBuffers involved.
  Rewriter TheRewriter;
};

/// Set up the command line options
static llvm::cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static llvm::cl::OptionCategory MacvethCategory("Macveth Options");

/// Main program
int main(int argc, const char **argv) {
  llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);

  /// FIRST STEP
  /// Parser for options common to all cmd-line Clang tools
  CommonOptionsParser Op(argc, argv, MacvethCategory);
  /// Utility to run a FrontendAction over a set of files
  /// * getCompilations(): contains compile cmd lines for the given source
  /// paths
  /// * getSourcePathList(): source files to run over
  ClangTool Tool(Op.getCompilations(), Op.getSourcePathList());

  /// Runs ToolAction over all files specified in the cmd line
  /// newFrontendActionFactory returns a new FrontendActionFactory for
  /// a given type, in this case our FrontendAction, declared above
  return Tool.run(newFrontendActionFactory<MACVETHFrontendAction>().get());
}
