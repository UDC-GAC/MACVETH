/**
 * File              : MVFrontend.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 13 Mar 2020 12:27:21 CET
 * Last Modified Date: Mér 18 Mar 2020 15:22:20 CET
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 *
 * Copyright (c) 2020 Marcos Horro <marcos.horro@udc.gal>
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

#ifndef MACVETH_FRONTEND_H
#define MACVETH_FRONTEND_H

#include "include/CDAG.h"
#include "include/MVPragmaHandler.h"
#include "include/StmtWrapper.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"

#ifndef LLVM_VERSION
#define LLVM_VERSION 10
#endif

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

namespace macveth {

class MVFuncVisitor : public RecursiveASTVisitor<MVFuncVisitor> {
public:
  explicit MVFuncVisitor(ASTContext *Context, Rewriter &R, ScopHandler *L)
      : Context(Context), Rewrite(R), SL(L) {}
  virtual bool VisitFunctionDecl(FunctionDecl *F);

private:
  /// Check scop options regarding unrolling and apply them to the statements
  /// within
  void unrollOptions(std::list<StmtWrapper *> SL);
  /// Main function in charge of scanning scops on each function with scops
  void scanScops(FunctionDecl *fd);
  /// Comment those stmts which are replaced
  void commentReplacedStmts(std::list<StmtWrapper *> SList);
  //// Add includes to files if needed
  void addHeaders(std::list<std::string> S, FileID FID);
  /// Rewrite TAC as regular statements
  void renderTACInPlace(std::list<StmtWrapper *> SL, long TacID);
  /// Render SIMD instructions where they should be (regular map operations)
  void renderSIMDInstInPlace(SIMDGenerator::SIMDInst SI,
                             std::list<StmtWrapper *> SL);
  /// Render SIMD after a statement (for reductions, for instance)
  bool renderSIMDInstAfterPlace(SIMDGenerator::SIMDInst SI,
                                std::list<StmtWrapper *> SL);
  /// Render SIMD before a statement (for initializing reductions, for instance)
  bool renderSIMDInstBeforePlace(SIMDGenerator::SIMDInst SI,
                                 std::list<StmtWrapper *> SL);
  /// Holds information regarding the ROI
  ScopHandler *SL;
  /// Context
  ASTContext *Context;
  /// For rewriting code in the output program
  Rewriter &Rewrite;
};

/// Implementation of the ASTConsumer interface for reading an AST produced
/// by the Clang parser. It registers a couple of matchers and runs them on
/// the AST.
class MVConsumer : public ASTConsumer {
public:
  MVConsumer(Rewriter &R, ASTContext *C, ScopHandler *L) : Visitor(C, R, L) {}

  /// Parse from the very beggining
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

  /// This is called after parsing the whole file
  void EndSourceFileAction() override;

private:
  /// Main interfacer to the rewrite buffers: dispatches high-level
  /// requests to the low-level RewriteBuffers involved.
  Rewriter TheRewriter;
};
} // namespace macveth
#endif /* !MACVETH_FRONTEND_H */
