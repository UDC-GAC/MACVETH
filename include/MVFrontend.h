#ifndef MACVETH_FRONTEND_H
#define MACVETH_FRONTEND_H

#include "include/CustomMatchers.h"
#include "include/MVOptions.h"
#include "include/MVPragmaHandler.h"
#include "include/Utils.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"

#ifndef LLVM_VERSION
#define LLVM_VERSION 10
#endif

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;
using namespace macveth;

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser. It registers a couple of matchers and runs them on
// the AST.
class MACVETHConsumer : public ASTConsumer {
  // SOME NOTES:
  // * Stmt - statements, could be a for loop, while, a single statement
  // * Decl - declarations (or defenitions) of variables, typedef, function...
  // * Type - types, CanonicalType, Builtin-type
  // * Expr - expressions, they inherit from Stmt tho; this is quite weird for
  // me...
public:
  MACVETHConsumer(Rewriter &R, ASTContext *C, ScopHandler *L)
      : Handler(R, C, L), Context(C) {}

  void HandleTranslationUnit(ASTContext &Context) override {
    // For vectorizable statements
    for (int n = 0; n < 6; ++n) {
      StatementMatcher ForLoopNestedMatcherVec =
          matchers_utils::ROI(n, compoundStmt(has(expr())).bind("ROI"));
      MatcherVec.addMatcher(ForLoopNestedMatcherVec, &Handler);
    }
    // Run the matchers when we have the whole TU parsed.
    MatcherVec.matchAST(Context);
  }

private:
  ASTContext *Context;
  matchers_utils::IterationHandler Handler;
  MatchFinder MatcherVec;
};

// SECOND STEP
// For each source file provided to the tool, a new ASTFrontendAction is
// created, which inherits from FrontendAction (abstract class)
class MACVETHFrontendAction : public ASTFrontendAction {
public:
  // empty constructor
  MACVETHFrontendAction() {}

  // This routine is called in BeginSourceFile(), from
  // CreateWrapperASTConsumer.
  // * CompilterInstance CI: got from getCompilerInstance()
  // * StringRef file: input file, provided by getCurrentFile()
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override {
    // setSourceMgr: setter for the Rewriter
    // * SourceManager: handles loading and caching of source files into
    // memory. It can be queried for information such as SourceLocation
    // of objects.
    // * LangOptions: controls the dialect of C/C++ accepted
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    Utils::setOpts(&CI.getSourceManager(), &CI.getLangOpts(),
                   &CI.getASTContext());
    Preprocessor &PP = CI.getPreprocessor();
    ScopHandler *scops = new ScopHandler();
    PP.AddPragmaHandler(new PragmaScopHandler(scops));
    PP.AddPragmaHandler(new PragmaEndScopHandler(scops));

    // std::make_unique is C++14, while LLVM 9 is written in C++11, this
    // is the reason of this custom implementation
#if LLVM_VERSION > 9
    return std::make_unique<MACVETHConsumer>(TheRewriter, &CI.getASTContext(),
                                             scops);
#else
    return llvm::make_unique<MACVETHConsumer>(TheRewriter, &CI.getASTContext(),
                                              scops);
#endif
  }

  // this is called only following a successful call to
  // BeginSourceFileAction (and BeginSourceFile)
  void EndSourceFileAction() override {
    // 1.- Get RewriteBuffer from FileID
    // 2.- Write to Stream (in this case llvm::outs(), which is
    // std::out) the result of applying all changes to the original
    // buffer. Original buffer is modified before calling this function,
    // from the ASTConsumer
    SourceManager &SM = TheRewriter.getSourceMgr();
    std::error_code ErrorCode;
    std::string OutFile =
        (MVOptions::OutFile == "") ? "macveth_output.c" : MVOptions::OutFile;
    OutFile = Utils::getExePath() + OutFile;
    llvm::raw_fd_ostream outFile(OutFile, ErrorCode, llvm::sys::fs::F_None);
    TheRewriter.getEditBuffer(SM.getMainFileID()).write(outFile);
  }

private:
  // Main interfacer to the rewrite buffers: dispatches high-level
  // requests to the low-level RewriteBuffers involved.
  Rewriter TheRewriter;
};

#endif
