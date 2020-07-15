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
#include "include/Vectorization/SIMD/SIMDGeneratorFactory.h"
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
      Utils::printDebug("MVConsumer", "unroll and jam...");
      CouldFullyUnroll = S->unrollAndJam(LI, Scop);
      assert(CouldFullyUnroll &&
             "Need to be able to full unroll when having leftovers");
    } else if (Scop->PA.Unroll) {
      Utils::printDebug("MVConsumer", "unroll by dim...");
      CouldFullyUnroll = S->unrollByDim(LI, Scop);
    }
  }
}

// ---------------------------------------------
std::list<std::string> rewriteLoops(std::list<StmtWrapper *> SList,
                                    Rewriter *Rewrite,
                                    std::list<std::string> DimAlreadyDecl) {
  std::list<std::string> DimsDeclared = {};
  for (auto SWrap : SList) {
    // Rewrite loop header
    if (SWrap->isLeftOver()) {
      continue;
    }
    auto Loop = SWrap->getLoopInfo();
    Utils::printDebug("MVConsumers", "Rewriting loop = " + Loop.Dim);

    // Rewrite headers
    Rewrite->ReplaceText(Loop.SRVarInit, Loop.Dim + " = " +
                                             ((Loop.InitVal != -1)
                                                  ? std::to_string(Loop.InitVal)
                                                  : Loop.StrInitVal));

    // IMPORTANT: need to put ";" at the end since the range is calculated
    // as the -1 offset of the location of the increment. This is done like
    // this because the SourceLocation of the UpperBound could be a macro
    // variable located in another place. This happens, for instance, with
    // the loop bounds in PolyBench suite
    if (!Loop.FullyUnrolled) {
      Rewrite->ReplaceText(Loop.SRVarCond,
                           "(" + Loop.Dim + " + " +
                               std::to_string(Loop.StepUnrolled) +
                               ") <= " + Loop.StrUpperBound + ";");
    }
    Rewrite->ReplaceText(Loop.SRVarInc,
                         Loop.Dim + " += " + std::to_string(Loop.StepUnrolled));
    if (Loop.Declared) {
      std::list<std::string> L = StmtWrapper::LoopInfo::DimDeclared;
      if ((!(std::find(L.begin(), L.end(), Loop.Dim) != L.end())) &&
          (!(std::find(DimsDeclared.begin(), DimsDeclared.end(), Loop.Dim) !=
             DimsDeclared.end())) &&
          ((!(std::find(DimAlreadyDecl.begin(), DimAlreadyDecl.end(),
                        Loop.Dim) != DimAlreadyDecl.end())))) {
        StmtWrapper::LoopInfo::DimDeclared.push_back(Loop.Dim);
        DimsDeclared.push_back(Loop.Dim);
      }
    }
    // If it has been fully unrolled
    std::string Epilog = StmtWrapper::LoopInfo::getEpilogs(SWrap);
    Rewrite->InsertTextAfterToken(Loop.EndLoc, Epilog + "\n");
    rewriteLoops(SWrap->getListStmt(), Rewrite, DimAlreadyDecl);
  }
  // Declare variables
  SourceLocation Loc = SList.front()->getClangStmt()->getBeginLoc();
  Rewrite->InsertTextBefore(
      Loc, StmtWrapper::LoopInfo::getDimDeclarations(DimsDeclared));
  StmtWrapper::LoopInfo::clearDims();
  return DimsDeclared;
}

// ---------------------------------------------
void MVFuncVisitor::commentReplacedStmts(std::list<StmtWrapper *> SList) {
  for (auto S : SList) {
    if (S->isLoop()) {
      commentReplacedStmts(S->getListStmt());
      continue;
    }
    if (S->isVectorized()) {
      Rewrite.InsertText(S->getClangStmt()->getBeginLoc(),
                         "// vectorized: ", true, true);
    } else {
      Rewrite.InsertText(S->getClangStmt()->getBeginLoc(),
                         "// replaced: ", true, true);
    }
  }
}

// // --------------------------------------------
// StmtWrapper *loopContainsSIMD(SIMDGenerator::SIMDInst SI,
//                               std::list<StmtWrapper *> SL) {
//   for (auto S : SL) {
//     if (S->isLoop()) {
//       for (auto B : S->getListStmt()) {
//         if (B->isLoop()) {
//           auto L = loopContainsSIMD(SI, B->getListStmt());
//           if (L != NULL) {
//             return B;
//           }
//         }
//         for (auto T : B->getTacList()) {
//           if (SI.TacID == T.getTacID()) {
//             return S;
//           }
//         }
//       }
//     }
//   }
//   return NULL;
// }

// ---------------------------------------------
bool MVFuncVisitor::renderSIMDInstBeforePlace(SIMDGenerator::SIMDInst SI,
                                              std::list<StmtWrapper *> SL) {
  for (auto S : SL) {
    if (S->isLoop()) {
      auto L = renderSIMDInstBeforePlace(SI, S->getListStmt());
      if (L) {
        Rewrite.InsertText(S->getClangStmt()->getBeginLoc(),
                           SI.render() + ";\t// latency = " +
                               std::to_string(SI.Cost.Latency) + "\n");
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
bool MVFuncVisitor::renderSIMDInstAfterPlace(SIMDGenerator::SIMDInst SI,
                                             std::list<StmtWrapper *> SL) {
  for (auto S : SL) {
    if (S->isLoop()) {
      auto L = renderSIMDInstAfterPlace(SI, S->getListStmt());
      if (L) {
        Rewrite.InsertTextAfterToken(
            S->getClangStmt()->getEndLoc(),
            SI.render() + ";\t// latency = " + std::to_string(SI.Cost.Latency) +
                "\n");
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

  if (SI.SType == SIMDGenerator::SIMDType::VSEQ) {
    for (auto S : SL) {
      for (auto T : S->getTacList()) {
        if (SI.TacID == T.getTacID()) {
          renderTACInPlace({S}, SI.TacID);
          return;
        }
      }
    }
  }
  if ((SI.SType == SIMDGenerator::SIMDType::VREDUC) ||
      (SI.SType == SIMDGenerator::SIMDType::VSTORER) ||
      (SI.SType == SIMDGenerator::SIMDType::VSEQR)) {
    renderSIMDInstAfterPlace(SI, SL);
  } else {
    // Rest of SIMD operations
    for (auto S : SL) {
      if (S->isLoop()) {
        renderSIMDInstInPlace(SI, S->getListStmt());
      } else {
        for (auto T : S->getTacList()) {
          if (SI.TacID == T.getTacID()) {
            Rewrite.InsertText(S->getClangStmt()->getBeginLoc(),
                               SI.render() + ";\t// latency = " +
                                   std::to_string(SI.Cost.Latency) + "\n",
                               true, true);
            return;
          }
        }
      }
    }
  }
}

// ---------------------------------------------
void MVFuncVisitor::renderTACInPlace(std::list<StmtWrapper *> SL, long TacID) {
  for (auto S : SL) {
    if (S->isLoop()) {
      renderTACInPlace(S->getListStmt(), TacID);
      continue;
    }
    if ((TacID == S->getTacList().back().getTacID()) || (TacID == -1)) {
      S->setNotVectorized();
      Utils::printDebug("MVFuncVisitor", TAC::renderTacAsStmt(S->getTacList()));
      Rewrite.InsertText(S->getClangStmt()->getBeginLoc(),
                         TAC::renderTacAsStmt(S->getTacList()), true, true);
    }
  }
}

// ---------------------------------------------
void MVFuncVisitor::addHeaders(std::list<std::string> S, FileID FID) {
  auto SM = Utils::getSourceMgr();
  for (auto I : S) {
    Rewrite.InsertText(SM->translateLineCol(FID, 1, 1),
                       "#include <" + I + ">\n", true, true);
  }
}

// ---------------------------------------------
void clearAllMappings() {
  // Be clean lol
  // TAC
  TAC::clear();
  // VectorIR
  VectorIR::clear();
  // SIMDGenerator
  SIMDGenerator::clearMappings();
}

// ---------------------------------------------
void MVFuncVisitor::scanScops(FunctionDecl *fd) {
  CompoundStmt *CS = dyn_cast<clang::CompoundStmt>(fd->getBody());
  if (!CS) {
    llvm::llvm_unreachable_internal();
  }

  // Clear all kind of mappings, since this is a new function
  clearAllMappings();

  std::list<std::string> DimsDeclFunc = {};

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
      for (auto S : SWrap->getTacList()) {
        S.printTAC();
        TL.push_back(S);
      }
    }
    if (!Scop->PA.SIMDCode) {
      MVInfo("[MVConsumer] "
             "No SIMD code to generate, just writing "
             "the code with the desired unrolling options");
      renderTACInPlace(SL, -1);
      // Rewriting loops
      DimsDeclFunc.splice(DimsDeclFunc.end(),
                          rewriteLoops(SL, &Rewrite, DimsDeclFunc));
    } else {
      // Creating the CDAG
      CDAG *G = CDAG::createCDAGfromTAC(TL);
      // Get SIMD generator according to the option chosen
      SIMDGenerator *SIMDGen = SIMDGeneratorFactory::getBackend(MVOptions::ISA);
      // Computing the cost model of the CDAG created
      auto SInfo = CDAG::computeCostModel(G, SIMDGen);

      if (SInfo.TotCost < StmtWrapper::computeSequentialCostStmtWrapper(SL)) {
        //// Printing the registers we are going to use
        for (auto InsSIMD : SIMDGen->renderSIMDRegister(SInfo.SIMDList)) {
          Rewrite.InsertText(SL.front()->getClangStmt()->getBeginLoc(),
                             InsSIMD + "\n", true, true);
        }
        for (auto InitRedux : SIMDGen->getInitReg()) {
          renderSIMDInstBeforePlace(InitRedux, SL);
        }
        for (auto InsSIMD : SInfo.SIMDList) {
          renderSIMDInstInPlace(InsSIMD, SL);
        }
        // Rewriting loops
        DimsDeclFunc.splice(DimsDeclFunc.end(),
                            rewriteLoops(SL, &Rewrite, DimsDeclFunc));

        // Add includes if option
        if (MVOptions::Headers) {
          addHeaders(SIMDGen->getHeadersNeeded(), Scop->FID);
          MVOptions::Headers = false;
        }

        // Comment statements
        commentReplacedStmts(SL);

        // FIXME: generate report
        // SInfo.printCost();
        MVInfo(
            "Region vectorized (SCOP = " + std::to_string(Scop->StartLine) +
            "). SIMD Cost = " + std::to_string(SInfo.TotCost) +
            "; Sequential cost = " +
            std::to_string(StmtWrapper::computeSequentialCostStmtWrapper(SL)));
      } else {
        MVWarn(
            "Region not vectorized (SCOP = " + std::to_string(Scop->StartLine) +
            "). SIMD Cost = " + std::to_string(SInfo.TotCost) +
            "; Sequential cost = " +
            std::to_string(StmtWrapper::computeSequentialCostStmtWrapper(SL)));
      }
      // delete G;
    }

    // Be clean
    // for (auto SWrap : SL) {
    //   delete SWrap;
    // }
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
