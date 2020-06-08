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
#include "include/MVOptions.h"
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
void MVFuncVisitor::unrollOptions(std::list<StmtWrapper *> SL) {
  bool CouldFullyUnroll = false;
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
      Utils::printDebug("MVConsumer", "unroll and jam...");
      CouldFullyUnroll = S->unrollAndJam(LI, Scop);
      assert(CouldFullyUnroll &&
             "Need to be able to full unroll when having leftovers");
    } else if (Scop->PA.Unroll) {
      Utils::printDebug("MVConsumer", "unroll and jam...");
      CouldFullyUnroll = S->unrollByDim(LI, Scop);
    }
  }
}

// ---------------------------------------------
void rewriteLoops(std::list<StmtWrapper *> SList, Rewriter *Rewrite) {
  std::list<std::string> DimsDeclared = {};
  for (auto SWrap : SList) {
    // Rewrite loop header
    if (SWrap->isLeftOver()) {
      continue;
    }
    auto Loop = SWrap->getLoopInfo();
    Utils::printDebug("MVConsumers", "Rewriting loop = " + Loop.Dim);
    // Rewrite headers
    Rewrite->ReplaceText(Loop.SRVarInit,
                         Loop.Dim + " = " + std::to_string(Loop.InitVal));
    // IMPORTANT: need to put ";" at the end since the range is calculated
    // as the -1 offset of the location of the increment. This is done like
    // this because the SourceLocation of the UpperBound could be a macro
    // variable located in another place. This happens, for instance, with
    // the loop bounds in PolyBench suite
    Rewrite->ReplaceText(Loop.SRVarCond, "(" + Loop.Dim + " + " +
                                             std::to_string(Loop.StepUnrolled) +
                                             ") <= " + Loop.StrUpperBound +
                                             ";");
    Rewrite->ReplaceText(Loop.SRVarInc,
                         Loop.Dim + " += " + std::to_string(Loop.StepUnrolled));
    if (Loop.Declared) {
      std::list<std::string> L = StmtWrapper::LoopInfo::DimDeclared;
      if (!(std::find(L.begin(), L.end(), Loop.Dim) != L.end())) {
        StmtWrapper::LoopInfo::DimDeclared.push_back(Loop.Dim);
        DimsDeclared.push_back(Loop.Dim);
      }
    }
    std::string Epilog = StmtWrapper::LoopInfo::getEpilogs(SWrap);
    Rewrite->InsertTextAfterToken(Loop.EndLoc, Epilog + "\n");
    rewriteLoops(SWrap->getListStmt(), Rewrite);
  }
  // Declare variables
  SourceLocation Loc = SList.front()->getClangStmt()->getBeginLoc();
  Rewrite->InsertTextBefore(
      Loc, StmtWrapper::LoopInfo::getDimDeclarations(DimsDeclared));
}

// ---------------------------------------------
void MVFuncVisitor::commentReplacedStmts(std::list<StmtWrapper *> SList) {
  for (auto S : SList) {
    if (S->isLoop()) {
      commentReplacedStmts(S->getListStmt());
      continue;
    }
    Rewrite.InsertText(S->getClangStmt()->getBeginLoc(),
                       "// statement replaced: ", true, true);
  }
}

StmtWrapper *loopContainsSIMD(SIMDGenerator::SIMDInst SI,
                              std::list<StmtWrapper *> SL) {
  for (auto S : SL) {
    if (S->isLoop()) {
      for (auto B : S->getListStmt()) {
        if (B->isLoop()) {
          auto L = loopContainsSIMD(SI, B->getListStmt());
          if (L != NULL) {
            return B;
          }
        }
        for (auto T : B->getTacList()) {
          if (SI.TacID == T.getTacID()) {
            return S;
          }
        }
      }
    }
  }
  return NULL;
}

bool MVFuncVisitor::renderSIMDInstAfterPlace(SIMDGenerator::SIMDInst SI,
                                             std::list<StmtWrapper *> SL) {
  for (auto S : SL) {
    if (S->isLoop()) {
      auto L = renderSIMDInstAfterPlace(SI, S->getListStmt());
      // auto Loop = loopContainsSIMD(SI, S->getListStmt());
      if (L) {
        // Utils::printDebug("MVFuncVisitor",
        //                   "Printing reduce after loop = " +
        //                       Utils::getStringFromStmt(S->getClangStmt()));
        Rewrite.InsertTextAfterToken(
            S->getClangStmt()->getEndLoc(),
            SI.render() + ";\t// cost = " + std::to_string(SI.Cost) + "\n");
      }
      return false;
    } else {
      for (auto T : S->getTacList()) {
        if (SI.TacID == T.getTacID()) {
          return true;
        }
      }
    }
  }
  return false;
}

// ---------------------------------------------
void MVFuncVisitor::renderSIMDInstInPlace(SIMDGenerator::SIMDInst SI,
                                          std::list<StmtWrapper *> SL) {
  // Reduce op
  if (SI.SType == SIMDGenerator::SIMDType::VREDUC) {
    renderSIMDInstAfterPlace(SI, SL);
  } else {
    for (auto S : SL) {
      if (S->isLoop()) {
        renderSIMDInstInPlace(SI, S->getListStmt());
      } else {
        for (auto T : S->getTacList()) {
          if (SI.TacID == T.getTacID()) {
            Rewrite.InsertText(
                S->getClangStmt()->getBeginLoc(),
                SI.render() + ";\t// cost = " + std::to_string(SI.Cost) + "\n",
                true, true);
            return;
          }
        }
      }
    }
  }
}

// ---------------------------------------------
void MVFuncVisitor::renderTACInPlace(std::list<StmtWrapper *> SL) {
  for (auto S : SL) {
    if (S->isLoop()) {
      renderTACInPlace(S->getListStmt());
      continue;
    }
    Rewrite.InsertText(S->getClangStmt()->getBeginLoc(), S->renderTacAsStmt(),
                       true, true);
  }
}

// ---------------------------------------------
void MVFuncVisitor::scanScops(FunctionDecl *fd) {
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

    Utils::printDebug("MVConsumer", "list of stmt wrappers parsed = " +
                                        std::to_string(SL.size()));

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

    if (!Scop->PA.SIMDCode) {
      Utils::printDebug("MVConsumer",
                        "No SIMD code to generate, just printing the code");
      renderTACInPlace(SL);
      // Rewriting loops
      rewriteLoops(SL, &Rewrite);
    } else {
      // Creating the CDAG
      CDAG *G = CDAG::createCDAGfromTAC(TL);
      // Get SIMD generator according to the option chosen
      SIMDGenerator *SIMDGen = SIMDGeneratorFactory::getBackend(MVOptions::ISA);
      // Computing the cost model of the CDAG created
      auto SInfo = CDAG::computeCostModel(G, SIMDGen);
      // Rewriting loops
      rewriteLoops(SL, &Rewrite);
      //// Printing the registers we are going to use
      for (auto InsSIMD : SIMDGen->renderSIMDRegister(SInfo.SIMDList)) {
        Rewrite.InsertText(SL.front()->getClangStmt()->getBeginLoc(),
                           InsSIMD + "\n", true, true);
      }
      for (auto InsSIMD : SInfo.SIMDList) {
        renderSIMDInstInPlace(InsSIMD, SL);
      }
      delete G;
    }
    // Comment statements
    commentReplacedStmts(SL);

    // Be clean
    for (auto SWrap : SL) {
      delete SWrap;
    }
  }

  return;
}

// ---------------------------------------------
bool areAllScopsScanned() {
  bool Scanned = true;
  for (auto S : ScopHandler::List) {
    if (!S->ScopHasBeenScanned) {
      return false;
    }
  }
  return Scanned;
}

// ---------------------------------------------
bool MVFuncVisitor::VisitFunctionDecl(FunctionDecl *F) {
  // Continue if empty function
  if (!F->hasBody())
    return true;
  // Check if pragmas to parse
  if (!ScopHandler::funcHasROI(F))
    return true;

  // If the function has scops to parse, then scan them
  this->scanScops(F);
  return true;
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

  SourceManager &SM = TheRewriter.getSourceMgr();
  std::error_code ErrorCode;
  std::string OutFileName = Utils::getExePath() + MVOptions::OutFile;
  llvm::raw_fd_ostream outFile(OutFileName, ErrorCode, llvm::sys::fs::F_None);
  TheRewriter.getEditBuffer(SM.getMainFileID()).write(outFile);
}
