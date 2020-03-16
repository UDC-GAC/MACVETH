/**
 * File              : MVFrontend.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 13 Mar 2020 12:28:45 CET
 * Last Modified Date: Lun 16 Mar 2020 18:00:58 CET
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

#include "include/MVFrontend.h"
#include "include/MVHandlers.h"
#include "include/MVOptions.h"
#include "include/MVPragmaHandler.h"
#include "include/Utils.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include <llvm-10/llvm/Support/ErrorHandling.h>

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
bool MVHandler::checkIfWithinScop(StmtWrapper *S) {
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
void MVHandler::unrollOptions(StmtWrapper *S) {
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
    Utils::printDebug("MVHandlers", "Rewriting loop = " + Loop.Dim);
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
void MVHandler::run(const MatchFinder::MatchResult &Result) {
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

// ---------------------------------------------
void scanScops(FunctionDecl *fd) {
  CompoundStmt *CS = dyn_cast<clang::CompoundStmt>(fd);
  if (!CS) {
    llvm::llvm_unreachable_internal();
  }

  // For each scop in the function
  for (auto Scop : ScopHandler::funcGetScops(fd)) {
    // Check if ST is within the ROI or not
    for (auto ST : CS->body()) {
    }
  }

  return;
}

// ---------------------------------------------
bool MVConsumer::HandleTopLevelDecl(DeclGroupRef dg) {
  DeclGroupRef::iterator it;

  for (it = dg.begin(); it != dg.end(); ++it) {
    FunctionDecl *fd = dyn_cast<clang::FunctionDecl>(*it);
    if (!fd)
      continue;
    if (!fd->hasBody())
      continue;
    if (!ScopHandler::funcHasROI(fd))
      continue;

    scanScops(fd);
  }

  return true;
}

// ---------------------------------------------
std::unique_ptr<ASTConsumer>
MVFrontendAction::CreateASTConsumer(CompilerInstance &CI, StringRef file) {
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
  return std::make_unique<MVConsumer>(TheRewriter, &CI.getASTContext(), scops);
#else
  return llvm::make_unique<MVConsumer>(TheRewriter, &CI.getASTContext(), scops);
#endif
}

// ---------------------------------------------
void MVFrontendAction::EndSourceFileAction() {
  // 1.- Get RewriteBuffer from FileID
  // 2.- Write to Stream (in this case llvm::outs(), which is
  // std::out) the result of applying all changes to the original
  // buffer. Original buffer is modified before calling this function,
  // from the ASTConsumer
  SourceManager &SM = TheRewriter.getSourceMgr();
  std::error_code ErrorCode;
  std::string OutFileName = Utils::getExePath() + MVOptions::OutFile;
  llvm::raw_fd_ostream outFile(OutFileName, ErrorCode, llvm::sys::fs::F_None);
  TheRewriter.getEditBuffer(SM.getMainFileID()).write(outFile);
}
