/**
 * File              : MVFrontend.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 13 Mar 2020 12:27:21 CET
 * Last Modified Date: Lun 16 Mar 2020 17:58:22 CET
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
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
class MVConsumer : public ASTConsumer {
public:
  MVConsumer(Rewriter &R, ASTContext *C, ScopHandler *L)
      : Handler(R, C, L), Context(C) {}

  virtual bool HandleTopLevelDecl(DeclGroupRef dg) override;
  /// Apply matchers to the AST
  // void HandleTranslationUnit(ASTContext &Context) override;

private:
  /// Context
  ASTContext *Context;
  /// Handler for matching results in the code
  MVHandler Handler;
};

/// MACVETH Frontend which is in charge of creating the AST consumer and to
/// write the changes to the output file
class MVFrontendAction : public ASTFrontendAction {
public:
  /// Empty constructor
  MVFrontendAction() {}

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
