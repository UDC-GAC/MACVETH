#ifndef MACVETH_FRONTEND_H
#define MACVETH_FRONTEND_H

#include "include/MVHandlers.h"
#include "clang/ASTMatchers/ASTMatchers.h"

#ifndef LLVM_VERSION
#define LLVM_VERSION 10
#endif

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;
using namespace macveth;
using namespace macveth::matchers_utils;

/// Implementation of the ASTConsumer interface for reading an AST produced
/// by the Clang parser. It registers a couple of matchers and runs them on
/// the AST.
class MACVETHConsumer : public ASTConsumer {
public:
  MACVETHConsumer(Rewriter &R, ASTContext *C, ScopHandler *L)
      : Handler(R, C, L), Context(C) {}

  /// Apply matchers to the AST
  void HandleTranslationUnit(ASTContext &Context) override;

private:
  /// Context
  ASTContext *Context;
  /// Handler for matching results in the code
  MVHandler Handler;
  MatchFinder MatcherVec;
};

/// MACVETH Frontend which is in charge of creating the AST consumer and to
/// write the changes to the output file
class MACVETHFrontendAction : public ASTFrontendAction {
public:
  /// Empty constructor
  MACVETHFrontendAction() {}

  /// This routine is called in BeginSourceFile(), from
  /// CreateWrapperASTConsumer.
  /// * CompilterInstance CI: got from getCompilerInstance()
  /// * StringRef file: input file, provided by getCurrentFile()
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override;

  /// This is called after parsing the whole file
  void EndSourceFileAction() override;

private:
  /// Main interfacer to the rewrite buffers: dispatches high-level
  /// requests to the low-level RewriteBuffers involved.
  Rewriter TheRewriter;
};

#endif
