// MACVETH - MVFrontend.h
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

#ifndef MACVETH_FRONTEND_H
#define MACVETH_FRONTEND_H

#include "include/CDAG.h"
#include "include/MVPragmaHandler.h"
#include "include/MVRewriter.h"
#include "include/StmtWrapper.h"
#include "include/Vectorization/SIMD/SIMDBackEnd.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

namespace macveth {

class MVFuncVisitor : public RecursiveASTVisitor<MVFuncVisitor> {
public:
  explicit MVFuncVisitor(ASTContext *Context, Rewriter &R, ScopHandler *L)
      : Context(Context), Rewrite(R), SL(L), MVR(MVRewriter(R)) {}
  virtual bool VisitFunctionDecl(FunctionDecl *F);

private:
  /// Check scop options regarding unrolling and apply them to the statements
  /// within
  void performUnrolling(std::list<StmtWrapper *> SL);
  /// Main function in charge of scanning scops on each function with scops
  void scanScops(FunctionDecl *fd);
  /// Context
  ASTContext *Context;
  /// For rewriting code in the output program
  Rewriter &Rewrite;
  /// Holds information regarding the ROI
  ScopHandler *SL;
  /// Custom rewriter for rendering in custom order
  MVRewriter MVR;
};

/// Implementation of the ASTConsumer interface for reading an AST produced
/// by the Clang parser. It registers a couple of matchers and runs them on
/// the AST.
class MVConsumer : public ASTConsumer {
public:
  MVConsumer(Rewriter &R, ASTContext *C, ScopHandler *L) : Visitor(C, R, L) {}

  /// Parse from the very beggining of the file and traverse all declarations
  virtual void HandleTranslationUnit(clang::ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  MVFuncVisitor Visitor;
};

/// MACVETH Frontend which is in charge of creating the AST consumer and to
/// write the changes to the output file
class MVFrontendAction : public ASTFrontendAction {
public:
  /// Empty constructor
  MVFrontendAction() {}

  /// This routine is called in BeginSourceFile(), from
  /// CreateWrapperASTConsumer.
  /// * CompilerInstance CI: got from getCompilerInstance()
  /// * StringRef file: input file, provided by getCurrentFile()
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override;

  /// This function is called after parsing the whole file
  void EndSourceFileAction() override;

private:
  /// Main interfacer to the rewrite buffers: dispatches high-level
  /// requests to the low-level RewriteBuffers involved.
  Rewriter TheRewriter;
};
} // namespace macveth
#endif /* !MACVETH_FRONTEND_H */
