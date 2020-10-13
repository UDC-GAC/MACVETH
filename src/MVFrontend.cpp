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
#include "include/MVAssert.h"
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

jmp_buf GotoStartScop, GotoEndScop;

// ---------------------------------------------
ScopLoc *getScopLoc(StmtWrapper *S) {
  auto SLoc = S->getClangStmt()->getBeginLoc();
  for (int n = 0; n < ScopHandler::List.size(); ++n) {
    if ((SLoc >= ScopHandler::List[n]->Scop) &&
        (SLoc <= ScopHandler::List[n]->EndScop)) {
      return ScopHandler::List[n];
    }
  }
  return nullptr;
}

// ---------------------------------------------
void MVFuncVisitor::unrollOptions(std::list<StmtWrapper *> SL) {
  auto CouldFullyUnroll = false;
  std::list<StmtWrapper::LoopInfo> LI;
  for (auto S : SL) {
    if (S->isLeftOver()) {
      continue;
    }
    auto SLoc = S->getClangStmt()->getBeginLoc();
    auto Scop = getScopLoc(S);
    assert(Scop != nullptr && "Scop not found for these statements");
    if (Scop->PA.UnrollAndJam) {
      Utils::printDebug("MVConsumer", "unroll and jam...");
      CouldFullyUnroll = S->unrollAndJam(LI, Scop);
      assert(CouldFullyUnroll &&
             "Need to be able to full unroll when having leftovers");
    } else if (Scop->PA.Unroll) {
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
      auto L = StmtWrapper::LoopInfo::DimDeclared;
      if ((!(std::find(L.begin(), L.end(), Loop.Dim) != L.end())) &&
          (!(std::find(DimsDeclared.begin(), DimsDeclared.end(), Loop.Dim) !=
             DimsDeclared.end())) &&
          ((!(std::find(DimAlreadyDecl.begin(), DimAlreadyDecl.end(),
                        Loop.Dim) != DimAlreadyDecl.end())))) {
        StmtWrapper::LoopInfo::DimDeclared.push_back(Loop.Dim);
        DimsDeclared.push_back(Loop.Dim);
      }
    }
    // If it has been fully unrolled no need to write the epilog of it
    if (!Loop.FullyUnrolled) {
      auto Epilog = StmtWrapper::LoopInfo::getEpilogs(SWrap);
      Rewrite->InsertTextAfterToken(Loop.EndLoc, Epilog + "\n");
    }
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
                         "// stmt vectorized: ", true, true);
    } else {
      Rewrite.InsertText(S->getClangStmt()->getBeginLoc(),
                         "// stmt not vectorized: ", true, true);
    }
  }
}

// ---------------------------------------------
bool MVFuncVisitor::renderSIMDInstBeforePlace(SIMDGenerator::SIMDInst SI,
                                              std::list<StmtWrapper *> SL) {
  for (auto S : SL) {
    if (S->isLoop()) {
      if (auto L = renderSIMDInstBeforePlace(SI, S->getListStmt())) {
        Rewrite.InsertText(S->getClangStmt()->getBeginLoc(),
                           SI.render() + ";\t// latency = " +
                               std::to_string(SI.Cost.Latency) + "\n");
      }
    } else {
      for (auto T : S->getTacList()) {
        if (SI.getSourceLocationOrder() == T.getTacID()) {
          if (!S->isInLoop()) {
            Rewrite.InsertTextBefore(S->getClangStmt()->getBeginLoc(),
                                     SI.render() + ";\t// latency = " +
                                         std::to_string(SI.Cost.Latency) +
                                         "\n");
            return false;
          } else {
            return true;
          }
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
      if (auto L = renderSIMDInstAfterPlace(SI, S->getListStmt())) {
        Rewrite.InsertTextAfterToken(
            S->getClangStmt()->getEndLoc(),
            SI.render() + ";\t// latency = " + std::to_string(SI.Cost.Latency) +
                "\n");
      }
    } else {
      for (auto T : S->getTacList()) {
        if (SI.getSourceLocationOrder() == T.getTacID()) {
          if (!S->isInLoop()) {
            Rewrite.InsertTextAfterToken(
                S->getClangStmt()->getEndLoc().getLocWithOffset(1),
                "\n" + SI.render() +
                    ";\t// latency = " + std::to_string(SI.Cost.Latency));
            return false;
          } else {
            return true;
          }
        }
      }
    }
  }
  return false;
}

// ---------------------------------------------
void MVFuncVisitor::renderSIMDInstInPlace(SIMDGenerator::SIMDInst SI,
                                          std::list<StmtWrapper *> SL) {
  // Sequential case: is this needed?
  if (SI.isSequential() && SI.isInOrder()) {
    Utils::printDebug("MVFuncVisitor",
                      "Sequential rendering for " + SI.render());
    for (auto S : SL) {
      for (auto T : S->getTacList()) {
        if (SI.getSourceLocationOrder() == T.getTacID()) {
          renderTACInPlace({S}, SI.getSourceLocationOrder(),
                           SI.getSourceLocationOffset());
          return;
        }
      }
    }
  }

  if (SI.isReduction()) {
    return;
  }

  if (SI.render() == "") {
    return;
  }

  if (SI.isPreOrder()) {
    renderSIMDInstBeforePlace(SI, SL);
  } else if (SI.isPosOrder()) {
    renderSIMDInstAfterPlace(SI, SL);
  } else {
    // Rest of SIMD operations
    for (auto S : SL) {
      if (S->isLoop()) {
        renderSIMDInstInPlace(SI, S->getListStmt());
      } else {
        for (auto T : S->getTacList()) {
          if (SI.getSourceLocationOrder() == T.getTacID()) {
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
void MVFuncVisitor::renderTACInPlace(std::list<StmtWrapper *> SL, long TacID,
                                     int Offset) {
  for (auto S : SL) {
    if (S->isLoop()) {
      renderTACInPlace(S->getListStmt(), TacID, Offset);
      continue;
    }
    if (((TacID == S->getTacList().back().getTacID()) &&
         (Offset == S->getTacList().back().getUnrollFactor())) ||
        (TacID == -1)) {
      S->setNotVectorized();
      Rewrite.InsertText(S->getClangStmt()->getBeginLoc(),
                         TAC::renderTacAsStmt(S->getTacList(), Offset), true,
                         true);
    }
  }
}

// ---------------------------------------------
void MVFuncVisitor::addHeaders(std::list<std::string> S, FileID FID) {
  auto SM = Utils::getSourceMgr();
  if (S.size() > 0) {
    Rewrite.InsertText(SM->translateLineCol(FID, 1, 1),
                       "// begin MACVETH: headers added\n", true, true);
  }
  for (auto I : S) {
    Rewrite.InsertText(SM->translateLineCol(FID, 1, 1),
                       "#include <" + I + ">\n", true, true);
  }
  if (S.size() > 0) {
    Rewrite.InsertText(SM->translateLineCol(FID, 1, 1), "// end MACVETH\n",
                       true, true);
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
  auto CS = dyn_cast<clang::CompoundStmt>(fd->getBody());
  if (!CS) {
    Utils::printDebug("MVFuncVisitor", "scanScops: no CompoundStmt");
    llvm::llvm_unreachable_internal();
  }

  int Code = setjmp(GotoStartScop);
  if (Code == MVSkipCode) {
    return;
  }

  std::list<std::string> DimsDeclFunc = {};
  SIMDGenerator::RegistersMapT RegistersDeclared;

  SourceLocation RegDeclLoc;
  auto Scops = ScopHandler::funcGetScops(fd);
  // For each scop in the function
  for (auto Scop : Scops) {
    auto IsFirstScop = (Scop == Scops[0]);
    auto IsLastScop = (Scop == Scops[Scops.size() - 1]);

    // Check if ST is within the ROI or not
    Utils::printDebug("MVConsumer",
                      "scop = " + std::to_string(Scop->StartLine));

    // Get the info about the loops surrounding this statement
    auto SL = StmtWrapper::genStmtWraps(CS, Scop);

    if (IsFirstScop) {
      RegDeclLoc = SL.front()->getClangStmt()->getBeginLoc();
    }

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
      renderTACInPlace(SL, -1, -1);
      // Rewriting loops
      DimsDeclFunc.splice(DimsDeclFunc.end(),
                          rewriteLoops(SL, &Rewrite, DimsDeclFunc));
    } else {
      // Creating the CDAG
      auto G = CDAG::createCDAGfromTAC(TL);

      // Get SIMD generator according to the option chosen
      auto SIMDGen = SIMDGeneratorFactory::getBackend(MVOptions::ISA);

      // Computing the cost model of the CDAG created
      auto SInfo = SIMDGenerator::computeCostModel(G, SIMDGen);

      // Vectorize according to the SIMD cost model selected and the total
      // cost of the SIMD operations
      auto Vectorize =
          ((MVOptions::SIMDCostModel == MVSIMDCostModel::UNLIMITED) ||
           ((MVOptions::SIMDCostModel == MVSIMDCostModel::CONSERVATIVE) &&
            (SInfo.TotCost <
             StmtWrapper::computeSequentialCostStmtWrapper(SL))));

      if (Vectorize) {
        // Render the registers we are going to use, declarations
        if (IsLastScop) {
          for (auto InsSIMD : SIMDGen->renderSIMDRegister(SInfo.SIMDList)) {
            Rewrite.InsertText(RegDeclLoc, InsSIMD + "\n", true, true);
          }
        }

        // Render initializations if needed for special cases such as
        // reductions
        for (auto InitRedux : SIMDGen->getInitReg()) {
          renderSIMDInstBeforePlace(InitRedux, SL);
        }
        // Render
        for (auto InsSIMD : SInfo.SIMDList) {
          renderSIMDInstInPlace(InsSIMD, SL);
        }
        // Rewrite loops
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
    }
  }

  return;
}

// ---------------------------------------------
bool MVFuncVisitor::VisitFunctionDecl(FunctionDecl *F) {
  // Continue if empty function
  if (!F->hasBody()) {
    return true;
  }

  // Check if pragmas to parse
  if (!ScopHandler::funcHasROI(F)) {
    return true;
  }

  /// Be clean
  clearAllMappings();

  if ((MVOptions::TargetFunc != "") &&
      (F->getNameAsString() != MVOptions::TargetFunc)) {
    return true;
  }

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
  auto &PP = CI.getPreprocessor();
  auto Scops = new ScopHandler();
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

  auto &SM = TheRewriter.getSourceMgr();
  std::error_code ErrorCode;
  auto OutFileName = Utils::getExePath() + MVOptions::OutFile;
  llvm::raw_fd_ostream outFile(OutFileName, ErrorCode, llvm::sys::fs::F_None);
  TheRewriter.getEditBuffer(SM.getMainFileID()).write(outFile);
}
