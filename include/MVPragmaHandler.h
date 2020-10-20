/*
 * File					 : include/MVPragmaHandler.h
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:38 +02:00
 * Modified By	 : Marcos Horro (marcos.horro@udc.gal>)
 *
 * MIT License
 *
 * Copyright (c) 2020 Colorado State University
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

#ifndef MACVETH_PRAGMAHANDLER_H
#define MACVETH_PRAGMAHANDLER_H

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

/// List of pairs of #pragma macveth and #pragma macvethend Locations.
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
      if (SL->FID != SM.getFileID(fd->getBeginLoc())) {
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
    Utils::printDebug("MVPragmaHandler", "addStart");
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
  return token.getIdentifierInfo();
}

/// Parse arguments using the preprocessor
static ScopLoc::PragmaArgs parseArguments(Preprocessor &PP) {
  ScopLoc::PragmaArgs PA;
  IdentifierInfo *II;
  IdentifierInfo *IIPrev;
  bool UnrollOptParsed = false;
  bool DimensionFound = false;
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
    if (II->isStr("nosimd")) {
      PA.SIMDCode = false;
      continue;
    }

    // Otherwise it will be a unrolling dimension
    if ((!Tok.isLiteral())) {
      // assert(false && "Bad format! Need an integer for the unrolling
      // factor");
      auto IINext = getValue(Tok);
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
