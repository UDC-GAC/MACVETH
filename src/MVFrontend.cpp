/**
 * File              : MVFrontend.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 13 Mar 2020 12:28:45 CET
 * Last Modified Date: Lun 23 Mar 2020 18:33:47 CET
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
#include "include/CDAG.h"
#include "include/MVOptions.h"
//#include "include/StmtWrapper.h"
#include "include/TAC.h"
#include "include/Utils.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include <llvm-10/llvm/Support/ErrorHandling.h>

// ---------------------------------------------
ScopLoc *getScopLoc(StmtWrapper *S) {
  auto SLoc = S->getClangStmt()->getBeginLoc();
  for (int n = 0; n < ScopHandler::List.size(); ++n) {
    if ((SLoc >= ScopHandler::List[n]->Scop) &&
        (SLoc <= ScopHandler::List[n]->EndScop)) {
      return ScopHandler::List[n];
    }
  }
  return NULL;
}

// ---------------------------------------------
void MVConsumer::unrollOptions(std::list<StmtWrapper *> SL) {
  bool CouldFullyUnroll = true;
  std::list<StmtWrapper::LoopInfo> LI;
  for (auto S : SL) {
    if (S->isLeftOver()) {
      continue;
    }
    auto SLoc = S->getClangStmt()->getBeginLoc();
    auto Scop = getScopLoc(S);
    assert(Scop != NULL && "Scop not found for these statements");
    if (Scop->PA.UnrollAndJam) {
      // CouldFullyUnroll = S->unrollAndJam(Scop->PA.UnrollFactor);
      Utils::printDebug("MVConsumer", "unrolling...");
      CouldFullyUnroll = S->unrollAndJam(LI);
    }
  }
  assert(CouldFullyUnroll &&
         "Need to be able to full unroll when having leftovers");
}

// ---------------------------------------------
void rewriteLoops(StmtWrapper *SWrap, Rewriter *Rewrite) {
  int Inc = 0;
  std::list<StmtWrapper::LoopInfo> Dims = {};
  std::list<std::string> DimsDeclared = {};
  // Rewrite loop header
  // for (auto Loop : SWrap->getLoopInfo()) {
  //  Utils::printDebug("MVConsumers", "Rewriting loop = " + Loop.Dim);
  //  // Rewrite headers
  //  Rewrite->ReplaceText(Loop.SRVarInit,
  //                       Loop.Dim + " = " + std::to_string(Loop.InitVal));
  //  // IMPORTANT: need to put ";" at the end since the range is calculated
  //  // as the -1 offset of the location of the increment. This is done like
  //  // this because the SourceLocation of the UpperBound could be a macro
  //  // variable located in another place. This happens, for instance, with
  //  // the loop bounds in PolyBench suite
  //  Rewrite->ReplaceText(Loop.SRVarCond,
  //                       "(" + Loop.Dim + " + " + std::to_string(Loop.Step) +
  //                           ") <= " + Loop.StrUpperBound + ";");
  //  Rewrite->ReplaceText(Loop.SRVarInc,
  //                       Loop.Dim + " += " + std::to_string(Loop.Step));
  //  if (Loop.Declared) {
  //    std::list<std::string> L = StmtWrapper::LoopInfo::DimDeclared;
  //    if (!(std::find(L.begin(), L.end(), Loop.Dim) != L.end())) {
  //      StmtWrapper::LoopInfo::DimDeclared.push_back(Loop.Dim);
  //      DimsDeclared.push_back(Loop.Dim);
  //    }
  //  }
  //  Dims.push_front(Loop);
  //  std::string Epilog = "";
  //  // StmtWrapper::LoopInfo::getEpilogs(Dims, SWrap->getClangStmt());
  //  Rewrite->InsertTextAfterToken(Loop.EndLoc, Epilog + "\n");
  //  //}
  //}
  //// FIXME
  //// Declare variables
  SourceLocation Loc = SWrap->getLoopInfo().BegLoc;
  Rewrite->InsertTextBefore(
      Loc, StmtWrapper::LoopInfo::getDimDeclarations(DimsDeclared));
}

// ---------------------------------------------
// void MVConsumer::run(const MatchFinder::MatchResult &Result) {
//  // Get the info about the loops surrounding this statement
//  StmtWrapper *SWrap = new StmtWrapper(Result);
//
//  // If this pragma has been already visited, then we skip the analysis. This
//  // may happen because of the implementatio of the AST finders and matchers
//  if (!checkIfWithinScop(SWrap)) {
//    return;
//  }
//
//  // Perform unrolling according to the pragmas
//  unrollOptions(SWrap);
//
//  // Debugging purposes
//  for (auto S : SWrap->getTacList()) {
//    S.printTAC();
//  }
//
//  // Creating the CDAG
//  CDAG *G = CDAG::createCDAGfromTAC(SWrap->getTacList());
//
//  // Get SIMD generator according to the option chosen
//  SIMDGenerator *SIMDGen = SIMDGeneratorFactory::getBackend(MVOptions::ISA);
//
//  // Computing the cost model of the CDAG created
//  auto SInfo = CDAG::computeCostModel(G, SIMDGen);
//
//  // FIXME: create epilog as well
//  // Unroll factor applied to the for header
//
//  rewriteLoops(SWrap, &Rewrite);
//
//  // Printing the registers we are going to use
//  for (auto InsSIMD : SIMDGen->renderSIMDRegister(SInfo.SIMDList)) {
//    Rewrite.InsertText(SWrap->getStmt()[0]->getBeginLoc(), InsSIMD + "\n",
//    true,
//                       true);
//  }
//  for (auto InsSIMD : SInfo.SIMDList) {
//    Rewrite.InsertText(
//        SWrap->getStmt()[SWrap->getTacToStmt()[InsSIMD.TacID]]->getBeginLoc(),
//        InsSIMD.render() + ";\t// cost = " + std::to_string(InsSIMD.Cost) +
//            "\n",
//        true, true);
//  }
//
//  // Comment statements
//  for (auto S : SWrap->getStmt()) {
//    Rewrite.InsertText(S->getBeginLoc(), "// statement replaced: ", true,
//    true);
//  }
//
//  // Be clean
//  delete SWrap;
//  delete G;
//}

// ---------------------------------------------
void MVConsumer::scanScops(FunctionDecl *fd) {
  CompoundStmt *CS = dyn_cast<clang::CompoundStmt>(fd->getBody());
  if (!CS) {
    llvm::llvm_unreachable_internal();
  }

  Utils::printDebug("MVConsumer", "scanScops");

  // For each scop in the function
  for (auto Scop : ScopHandler::funcGetScops(fd)) {
    // Check if ST is within the ROI or not
    Utils::printDebug("MVConsumer",
                      "scop = " + std::to_string(Scop->StartLine));

    // Get the info about the loops surrounding this statement
    std::list<StmtWrapper *> SL = StmtWrapper::genStmtWraps(CS, Scop);

    Utils::printDebug("MVConsumer", "list of stmt wrappers parsed!");

    // Perform unrolling according to the pragmas
    unrollOptions(SL);

    // Get all the TAC from all the Stmts
    TacListType TL;
    for (auto SWrap : SL) {
      Utils::printDebug("MVConsumer",
                        "number of tacs = " +
                            std::to_string(SWrap->getTacList().size()));
      for (auto S : SWrap->getTacList()) {
        S.printTAC();
        TL.push_back(S);
      }
    }

    // Creating the CDAG
    CDAG *G = CDAG::createCDAGfromTAC(TL);

    // Get SIMD generator according to the option chosen
    SIMDGenerator *SIMDGen = SIMDGeneratorFactory::getBackend(MVOptions::ISA);

    // Computing the cost model of the CDAG created
    auto SInfo = CDAG::computeCostModel(G, SIMDGen);

    Utils::printDebug("MVConsumer", "computed cost model");
    // FIXME: create epilog as well
    // Unroll factor applied to the for header

    // rewriteLoops(SWrap, &Rewrite);

    //// Printing the registers we are going to use
    // for (auto InsSIMD : SIMDGen->renderSIMDRegister(SInfo.SIMDList)) {
    //  Rewrite.InsertText(SWrap->getStmt()[0]->getBeginLoc(), InsSIMD + "\n",
    //                     true, true);
    //}
    // for (auto InsSIMD : SInfo.SIMDList) {
    //  Rewrite.InsertText(
    //      SWrap->getStmt()[SWrap->getTacToStmt()[InsSIMD.TacID]]->getBeginLoc(),
    //      InsSIMD.render() + ";\t// cost = " + std::to_string(InsSIMD.Cost) +
    //          "\n",
    //      true, true);
    //}

    //// Comment statements
    // for (auto S : SL) {
    //  Rewrite.InsertText(S->getStmt()->getBeginLoc(),
    //                     "// statement replaced: ", true, true);
    //}

    //// Be clean
    // for (auto SWrap : SL) {
    //  delete SWrap;
    //}
    // delete G;
    ////}
    Utils::printDebug("MVConsumer", "scop finished!!");
  }

  return;
}

// ---------------------------------------------
bool areAllScopsScaned() {
  bool Scanned = false;
  for (auto S : ScopHandler::List) {
    Utils::printDebug("MVConsumer", std::to_string(S->StartLine) + " = " +
                                        std::to_string(S->ScopHasBeenScanned));
    if (!S->ScopHasBeenScanned) {
      return false;
    } else {
      Scanned = true;
    }
  }
  return Scanned;
}

// ---------------------------------------------
bool MVConsumer::HandleTopLevelDecl(DeclGroupRef dg) {
  DeclGroupRef::iterator it;

  // Iterate over all the functions in the file
  for (it = dg.begin(); it != dg.end(); ++it) {
    // Get function body
    FunctionDecl *fd = dyn_cast<clang::FunctionDecl>(*it);
    // Continue if found is not a function
    if (!fd)
      continue;
    // Continue if empty function
    if (!fd->hasBody())
      continue;
    // Check if pragmas to parse
    if (!ScopHandler::funcHasROI(fd))
      continue;
    Utils::printDebug("MVConsumer",
                      "func = " + fd->getNameInfo().getAsString());

    // If the function has scops to parse, then scan them
    this->scanScops(fd);
  }

  // While HandleTopLevelDecl returns true, ClangTool will keep parsing the
  // file, thus, we want to stop when all scops we are interested in (parsed
  // with the preprocessor) are visited
  return (!areAllScopsScaned());
}

// ---------------------------------------------
std::unique_ptr<ASTConsumer>
MVFrontendAction::CreateASTConsumer(CompilerInstance &CI, StringRef file) {
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
  Utils::setOpts(&CI.getSourceManager(), &CI.getLangOpts(),
                 &CI.getASTContext());
  // Parsing pragmas
  Preprocessor &PP = CI.getPreprocessor();
  ScopHandler *Scops = new ScopHandler();
  PP.AddPragmaHandler(new PragmaScopHandler(Scops));
  PP.AddPragmaHandler(new PragmaEndScopHandler(Scops));

  // std::make_unique is C++14, while LLVM 9 is written in C++11, this
  // is the reason of this custom implementation
#if LLVM_VERSION > 9
  return std::make_unique<MVConsumer>(TheRewriter, &CI.getASTContext(), Scops);
#else
  return llvm::make_unique<MVConsumer>(TheRewriter, &CI.getASTContext(), Scops);
#endif
}

// ---------------------------------------------
void MVFrontendAction::EndSourceFileAction() {
  // 1.- Get RewriteBuffer from FileID
  // 2.- Write to Stream (in this case llvm::outs(), which is
  // std::out) the result of applying all changes to the original
  // buffer. Original buffer is modified before calling this function,
  // from the ASTConsumer

  Utils::printDebug("MVConsumer", "endSourceFile");
  SourceManager &SM = TheRewriter.getSourceMgr();
  std::error_code ErrorCode;
  std::string OutFileName = Utils::getExePath() + MVOptions::OutFile;
  llvm::raw_fd_ostream outFile(OutFileName, ErrorCode, llvm::sys::fs::F_None);
  TheRewriter.getEditBuffer(SM.getMainFileID()).write(outFile);
  Utils::printDebug("MVConsumer", "endSourceFile passed");
}
