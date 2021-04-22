// MACVETH - MVFrontend.cpp
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
//     Louis-Nöel Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Louis-Nöel Pouchet <pouchet@colostate.edu>

#include "include/MVFrontend.h"
#include "include/CostModel/MVCostModel.h"
#include "include/MVAssert.h"
#include "include/MVOptions.h"
#include "include/TAC.h"
#include "include/Utils.h"
#include "include/Vectorization/SIMD/SIMDBackEndFactory.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Format/Format.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "llvm/Support/ErrorHandling.h"

jmp_buf GotoStartScop, GotoEndScop;

// ---------------------------------------------
ScopLoc *getScopLoc(StmtWrapper *S) {
  auto SLoc = S->getClangStmt()->getBeginLoc();
  for (size_t n = 0; n < ScopHandler::List.size(); ++n) {
    if ((SLoc >= ScopHandler::List[n]->Scop) &&
        (SLoc <= ScopHandler::List[n]->EndScop)) {
      return ScopHandler::List[n];
    }
  }
  return nullptr;
}

// ---------------------------------------------
void MVFuncVisitor::performUnrolling(std::list<StmtWrapper *> SL) {
  auto CouldFullyUnroll = false;
  std::list<StmtWrapper::LoopInfo> LI;
  for (auto S : SL) {
    if (S->isLeftOver()) {
      continue;
    }
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
  SIMDBackEnd::RegistersMapT RegistersDeclared;

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
    performUnrolling(SL);

    SIMDInfo SInfo;
    if (!Scop->PA.SIMDCode) {
      MVInfo("[MVConsumer] "
             "No SIMD code to generate, just writing "
             "the code with the desired unrolling options");
      MVR.renderTACInPlace(SL, -1, -1);
      // Rewriting loops
      DimsDeclFunc.splice(DimsDeclFunc.end(),
                          MVR.rewriteLoops(SL, DimsDeclFunc));
    } else {
      // Get SIMD back end according to the option chosen
      auto SIMDGen = SIMDBackEndFactory::getBackend(MVOptions::ISA);

      int Code = setjmp(GotoEndScop);
      if (Code) {
        if (IsLastScop) {
          goto last_scop;
        }
        continue;
      }

      // Computing the cost model of the CDAG created
      SInfo = MVCostModel::computeCostModel(SL, SIMDGen);

      if (SInfo.isThereAnyVectorization()) {
        // Comment statements
        MVR.commentReplacedStmts(SL);
        // Render
        for (auto InsSIMD : SInfo.SIMDList) {
          MVR.renderSIMDInstInPlace(InsSIMD, SL);
        }

        // Render initializations if needed for special cases such as
        // reductions
        for (auto InitRedux : SIMDGen->getInitReg()) {
          MVR.renderSIMDInstBeforePlace(InitRedux, SL);
        }

        // Rewrite loops
        DimsDeclFunc.splice(DimsDeclFunc.end(),
                            MVR.rewriteLoops(SL, DimsDeclFunc));

        // Add includes if option
        if (MVOptions::Headers) {
          MVR.addHeaders(SIMDGen->getHeadersNeeded(), Scop->FID);
          MVOptions::Headers = false;
        }

        // TODO: generate file
        // SInfo.generateReport();
        MVInfo("Region vectorized (SCOP = " + std::to_string(Scop->StartLine) +
               "). SIMD Cost = " + std::to_string(SInfo.TotCost) +
               "; Scalar cost = " +
               MVCostModel::computeCostForStmtWrapperList(SL).toString());
      } else {
        MVWarn(
            "Region not vectorized (SCOP = " + std::to_string(Scop->StartLine) +
            "). SIMD Cost = " + std::to_string(SInfo.TotCost) +
            "; Scalar cost = " +
            MVCostModel::computeCostForStmtWrapperList(SL).toString());
      }

    last_scop:
      // Render the registers we are going to use, declarations
      if (IsLastScop) {
        for (auto InsSIMD : SIMDGen->renderSIMDRegister(SInfo.SIMDList)) {
          Rewrite.InsertTextBefore(RegDeclLoc, InsSIMD + "\n");
        }
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

  // FIXME: Be clean
  // MVRewriter.clearAllMappings();

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
#if defined(LLVM_VERSION_MAJOR) && (LLVM_VERSION_MAJOR > 9)
  return std::make_unique<MVConsumer>(TheRewriter, &CI.getASTContext(), Scops);
#else
  return llvm::make_unique<MVConsumer>(TheRewriter, &CI.getASTContext(), Scops);
#endif
}

// ---------------------------------------------
static FileID createInMemoryFile(StringRef FileName, MemoryBuffer *Source,
                                 SourceManager &Sources, FileManager &Files,
                                 llvm::vfs::InMemoryFileSystem *MemFS) {
  MemFS->addFileNoOwn(FileName, 0, Source);
  auto File = Files.getFile(FileName);
  return Sources.createFileID(File ? *File : nullptr, SourceLocation(),
                              SrcMgr::C_User);
}

// ---------------------------------------------
static bool fillRanges(MemoryBuffer *Code,
                       std::vector<tooling::Range> &Ranges) {
  IntrusiveRefCntPtr<llvm::vfs::InMemoryFileSystem> InMemoryFileSystem(
      new llvm::vfs::InMemoryFileSystem);
  FileManager Files(FileSystemOptions(), InMemoryFileSystem);
  DiagnosticsEngine Diagnostics(
      IntrusiveRefCntPtr<DiagnosticIDs>(new DiagnosticIDs),
      new DiagnosticOptions);
  SourceManager Sources(Diagnostics, Files);
  FileID ID = createInMemoryFile("<irrelevant>", Code, Sources, Files,
                                 InMemoryFileSystem.get());

  SourceLocation Start = Sources.getLocForStartOfFile(ID);
  SourceLocation End = Sources.getLocForEndOfFile(ID);
  unsigned long Offset = Sources.getFileOffset(Start);
  unsigned long Length = Sources.getFileOffset(End);
  Ranges.push_back(tooling::Range(Offset, Length));
  return false;
}

// ---------------------------------------------
static bool formatMACVETH(StringRef FileName) {
  /// This code is inspired (basically a copy) of clang/tool/ClangFormat.cpp [1]
  /// https://github.com/llvm/llvm-project/blob/master/clang/tools/clang-format/ClangFormat.cpp

  // On Windows, overwriting a file with an open file mapping doesn't work,
  // so read the whole file into memory when formatting in-place.
  ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> CodeOrErr =
      MemoryBuffer::getFileAsStream(FileName);
  if (std::error_code EC = CodeOrErr.getError()) {
    MVErr("Formatting file failed: " + EC.message() +
          " for file: " + FileName.str());
  }

  std::unique_ptr<llvm::MemoryBuffer> Code = std::move(CodeOrErr.get());
  if (Code->getBufferSize() == 0) {
    // Empty files are formatted correctly.
    MVInfo("File empty");
    return false;
  }
  std::vector<tooling::Range> Ranges;
  if (fillRanges(Code.get(), Ranges)) {
    return true;
  }
  llvm::Expected<clang::format::FormatStyle> FormatStyle =
      clang::format::getStyle(MVOptions::Style, FileName,
                              MVOptions::FallbackStyle, Code->getBuffer());
  if (!FormatStyle) {
    MVInfo("Formatting file failed: bad format style " +
           llvm::toString(FormatStyle.takeError()));
    return true;
  }

  unsigned CursorPosition = 0;
  FormatStyle->SortIncludes = true;
  Replacements Replaces = clang::format::sortIncludes(
      *FormatStyle, Code->getBuffer(), Ranges, FileName, &CursorPosition);
  auto ChangedCode = tooling::applyAllReplacements(Code->getBuffer(), Replaces);
  if (!ChangedCode) {
    MVInfo("Formatting file failed: nothing changed");
    return true;
  }
  // Get new affected ranges after sorting "#includes"
  Ranges = tooling::calculateRangesAfterReplacements(Replaces, Ranges);
  clang::format::FormattingAttemptStatus Status;
  Replacements FormatChanges = clang::format::reformat(
      *FormatStyle, *ChangedCode, Ranges, FileName, &Status);

  Replaces = Replaces.merge(FormatChanges);

  IntrusiveRefCntPtr<llvm::vfs::InMemoryFileSystem> InMemoryFileSystem(
      new llvm::vfs::InMemoryFileSystem);
  FileManager Files(FileSystemOptions(), InMemoryFileSystem);

  DiagnosticsEngine Diagnostics(
      IntrusiveRefCntPtr<DiagnosticIDs>(new DiagnosticIDs),
      new DiagnosticOptions);

  SourceManager Sources(Diagnostics, Files);
  createInMemoryFile(FileName, Code.get(), Sources, Files,
                     InMemoryFileSystem.get());
  Rewriter Rewrite(Sources, LangOptions());
  if (!tooling::applyAllReplacements(Replaces, Rewrite)) {
    MVInfo("Formatting file failed: something went wrong saving file (applying "
           "replacements)!");
    return true;
  }
  if (Rewrite.overwriteChangedFiles()) {
    // Something went wrong
    MVInfo("Formatting file failed: something went wrong saving file!");
    return true;
  }
  // File formatted properly
  return false;
}

// ---------------------------------------------
void MVFrontendAction::EndSourceFileAction() {
  auto &SM = TheRewriter.getSourceMgr();
  std::error_code ErrorCode;
  auto OutFileName = Utils::getExePath() + MVOptions::OutFile;
  llvm::raw_fd_ostream outFile(OutFileName, ErrorCode, llvm::sys::fs::F_None);
  // According to [1] it is not safe to do .write(). Nonetheless, I have not
  // found any other way to save a RewriterBuffer onto another file.
  // [1] (as of 16th November 2020)
  // https://clang.llvm.org/doxygen/classclang_1_1RewriteBuffer.html#a0b72d3db59db1731c63217c291929ced
  TheRewriter.getEditBuffer(SM.getMainFileID()).write(outFile);
  outFile.close();
  if (!MVOptions::NoReformat) {
    formatMACVETH(OutFileName);
  }
}
