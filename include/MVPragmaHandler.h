// MACVETH - MVPragmaHandler.h
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

#ifndef MACVETH_PRAGMAHANDLER_H
#define MACVETH_PRAGMAHANDLER_H

#include "include/Debug.h"
#include "include/MVAssert.h"
#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Parse/Parser.h"
#include <iostream>
#include <vector>

using namespace clang;

namespace macveth {

using PragmaTupleDim = std::vector<std::tuple<std::string, int>>;

/// The Location of the Scop, as delimited by macveth and endmacveth
/// pragmas by the user.
/// "macveth" and "endmacveth" are the source locations of the macveth and
/// endmacveth pragmas.
/// "StartLine" is the Line number of the Start position.
struct ScopLoc {
  ScopLoc() : End(0) {}

  /// File identifier of the scop
  FileID FID;

  /// Start location of the pragma
  clang::SourceLocation Scop;
  /// End location of the pragma
  clang::SourceLocation EndScop;
  unsigned StartLine;
  unsigned EndLine;
  unsigned Start;
  unsigned End;
  std::list<std::string> DimVisited = {};
  bool ScopHasBeenScanned = false;

  struct PragmaArgs {
    /// Generate SIMD code in the region: true by default
    bool SIMDCode = true;
    /// General option for unrolling code
    bool Unroll = true;
    /// Unroll factor (general)
    int UnrollFactor = 1;
    /// Perform unroll and jam
    bool UnrollAndJam = true;
    /// Full unrolling
    std::map<std::string, bool> FullUnroll;
    /// Custom option for each loop within the scope
    PragmaTupleDim UnrollDim;
  };

  /// Each scop has its own PragmaArgs, which are parsed at pre-processing time
  PragmaArgs PA;
};

/// List of pairs of #pragma macveth and #pragma endmacveth source locations
struct ScopHandler {
  static inline std::vector<ScopLoc *> List;

  /// Empty constructor
  ScopHandler(){};

  /// Return a SourceLocation for Line "Line", column "col" of file "FID".
  static SourceLocation translateLineCol(SourceManager &SM, FileID FID,
                                         unsigned Line, unsigned col) {
    return SM.translateLineCol(FID, Line, col);
  }

  /// Update scop to visited
  static void visitScop(ScopLoc S) {
    for (auto SV : List) {
      if (SV->StartLine == S.StartLine) {
        SV->ScopHasBeenScanned = true;
        return;
      }
    }
  }

  /// Get ROI of a concrete function
  static std::vector<ScopLoc *> funcGetScops(FunctionDecl *fd) {
    SourceManager &SM = fd->getParentASTContext().getSourceManager();
    unsigned int StartFD = SM.getExpansionLineNumber(fd->getBeginLoc());
    unsigned int EndFD = SM.getExpansionLineNumber(fd->getEndLoc());
    std::vector<ScopLoc *> SList;
    for (auto SL : List) {
      auto FunctionFileID = SM.getFileID(fd->getLocation());
      if (SL->FID != FunctionFileID) {
        continue;
      }
      unsigned int StartL = SM.getFileOffset(
          translateLineCol(SM, SM.getFileID(SL->Scop), StartFD, 1));
      unsigned int EndL = SM.getFileOffset(
          translateLineCol(SM, SM.getFileID(SL->EndScop), EndFD + 1, 1));

      if (((StartFD <= SL->StartLine) && (EndFD >= SL->EndLine)) &&
          ((StartL <= SL->Start) && (EndL >= SL->End)))
        SList.push_back(SL);
    }
    return SList;
  }

  /// Determine whether a function has or not ROI within
  static bool funcHasROI(FunctionDecl *fd) {
    return (funcGetScops(fd).size() > 0);
  }

  /// Add a new Start (#pragma macveth) Location to the list.
  /// If the last #pragma macveth did not have a matching
  /// #pragma endmacveth then overwrite it.
  /// "Start" points to the location of the macveth pragma.
  void addStart(SourceManager &SM, SourceLocation Start,
                ScopLoc::PragmaArgs PA) {
    ScopLoc *Loc = new ScopLoc();
    MACVETH_DEBUG("MVPragmaHandler", "addStart");
    Loc->FID = SM.getFileID(Start);
    Loc->PA = PA;
    Loc->Scop = Start;
    int Line = SM.getExpansionLineNumber(Start);
    Loc->StartLine = Line;
    Loc->Start = SM.getFileOffset(translateLineCol(SM, Loc->FID, Line, 1));
    if (List.size() == 0 || List[List.size() - 1]->End != 0) {
      List.push_back(Loc);
    } else {
      List[List.size() - 1] = Loc;
    }
  }

  /// Set the end location (#pragma endmacveth) of the last pair
  /// in the list.
  /// If there is no such pair of if the end of that pair
  /// is already set, then ignore the spurious #pragma endmacveth.
  /// "end" points to the location of the endmacveth pragma.
  void addEnd(SourceManager &SM, SourceLocation End) {
    if (List.size() == 0 || List[List.size() - 1]->End != 0)
      return;
    List[List.size() - 1]->EndScop = End;
    int Line = SM.getExpansionLineNumber(End);
    End = translateLineCol(SM, SM.getFileID(End), Line + 1, 1);
    List[List.size() - 1]->End = SM.getFileOffset(End);
    List[List.size() - 1]->EndLine = Line;
  }
};

/// Get IdentifierInfo from Token
static IdentifierInfo *getValue(Token &token) {
  if (token.isNot(tok::identifier)) {
    return nullptr;
  }
  if (token.getKind() == tok::eod) {
    return nullptr;
  }
  return token.getIdentifierInfo();
}

/// Parse arguments using the preprocessor
static ScopLoc::PragmaArgs parseArguments(Preprocessor &PP) {
  ScopLoc::PragmaArgs PA;
  IdentifierInfo *II;
  bool UnrollOptParsed = false;
  Token Tok;
  PP.Lex(Tok);
  while ((II = getValue(Tok)) != nullptr) {
    PP.Lex(Tok);
    if (((II->isStr("nounroll")) || (II->isStr("unroll")) ||
         (II->isStr("unrollandjam"))) &&
        (UnrollOptParsed)) {
      assert(false &&
             "You can not specify twice unrolling options in the same scop!");
    }
    // Check if unroll
    if (II->isStr("nounroll") || II->isStr("unrollandjam") ||
        II->isStr("unroll")) {
      PA.Unroll = (II->isStr("unrollandjam") || II->isStr("unroll"));
      PA.UnrollAndJam = (II->isStr("unrollandjam"));
      UnrollOptParsed = true;
      continue;
    }
    // Check if no SIMD code
    if (II->isStr("scalar")) {
      PA.SIMDCode = false;
      continue;
    }

    // Otherwise it will be a unrolling dimension
    if ((!Tok.isLiteral())) {
      // assert(false && "Bad format! Need an integer for the unrolling
      // factor");
      auto IINext = getValue(Tok);
      if (IINext == nullptr) {
        MVErr("Pragma bad formatting");
      }
      if (IINext->isStr("full")) {
        PA.FullUnroll[II->getName().str()] = true;
        PA.UnrollDim.push_back(
            std::tuple<std::string, int>(II->getName().str(), 1));
        PP.Lex(Tok);
        continue;
      } else {
        assert(false && "Something went wrong...");
      }
    }

    auto IINext = Tok.getLiteralData();
    if (IINext == nullptr) {
      assert(false &&
             "Bad format: needed a unrolling factor for the dimension");
    }

    int UnrollFactor = atoi(IINext);
    if (UnrollFactor == 0) {
      assert(false && "Bad value for unrolling factor");
    }
    PA.UnrollDim.push_back(
        std::tuple<std::string, int>(II->getName().str(), UnrollFactor));
    PP.Lex(Tok);
  }
  return PA;
}

/// Handle pragmas of the form
///
///  #pragma macveth [unroll|nounroll|unrollandjam] [loopIdentifier
///  unrollFactor] [nosimd]
///
/// In particular, store the Location of the line containing
/// the pragma in the list "Scops". The intention of this pragma is to tell the
/// compiler where the ROI starts.
struct PragmaScopHandler : public PragmaHandler {
  ScopHandler *Scops;

  PragmaScopHandler(ScopHandler *Scops)
      : PragmaHandler("macveth"), Scops(Scops) {}

  virtual void HandlePragma(Preprocessor &PP, PragmaIntroducer Introducer,
                            Token &ScopTok) {
    SourceManager &SM = PP.getSourceManager();
    SourceLocation Sloc = ScopTok.getLocation();
    Scops->addStart(SM, Sloc, parseArguments(PP));
  }
};

/// Handle pragmas of the form
///
///  #pragma endmacveth
///
/// In particular, store the Location of the line following the one containing
/// the pragma in the list "Scops". The intention of this pragma is to tell the
/// compiler where the ROI ends.
struct PragmaEndScopHandler : public PragmaHandler {
  ScopHandler *Scops;

  PragmaEndScopHandler(ScopHandler *Scops)
      : PragmaHandler("endmacveth"), Scops(Scops) {}

  virtual void HandlePragma(Preprocessor &PP, PragmaIntroducer Introducer,
                            Token &EndScopTok) {
    SourceManager &SM = PP.getSourceManager();
    SourceLocation Sloc = EndScopTok.getLocation();
    Scops->addEnd(SM, Sloc);
  }
};
} // namespace macveth
#endif /* !MACVETH_PRAGMAHANDLER_H */
