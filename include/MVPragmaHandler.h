/**
 * File              : MVPragmaHandler.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 06 Xan 2020 10:54:41 MST
 * Last Modified Date: Ven 20 Mar 2020 10:24:19 CET
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_PRAGMAHANDLER_H
#define MACVETH_PRAGMAHANDLER_H

#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Parse/Parser.h"
#include <iostream>
#include <vector>

using namespace clang;

/// The Location of the Scop, as delimited by macveth and endmacveth
/// pragmas by the user.
/// "macveth" and "endmacveth" are the source locations of the macveth and
/// endmacveth pragmas.
/// "StartLine" is the Line number of the Start position.
struct ScopLoc {
  ScopLoc() : End(0) {}

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
    bool Unroll = true;
    int UnrollFactor = 1;
    bool UnrollAndJam = true;
    std::vector<std::tuple<std::string, int>> UnrollDim;
  };

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
      if ((StartFD <= SL->StartLine) && (EndFD >= SL->EndLine))
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

    Loc->PA = PA;
    Loc->Scop = Start;
    int Line = SM.getExpansionLineNumber(Start);
    Start = translateLineCol(SM, SM.getFileID(Start), Line, 1);
    Loc->StartLine = Line;
    Loc->Start = SM.getFileOffset(Start);
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
  if (token.isNot(tok::identifier))
    return NULL;
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
  while ((II = getValue(Tok)) != NULL) {
    PP.Lex(Tok);
    Utils::printDebug("ScopLoc", "Token = " + II->getName().str());
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
    // Otherwise it will be a unrolling dimension
    if (!Tok.isLiteral()) {
      assert(false && "Bad format! Need an integer for the unrolling factor");
    }
    auto IINext = Tok.getLiteralData();
    if (IINext == NULL) {
      assert(false &&
             "Bad format: needed a unrolling factor for the dimension");
    }
    int UnrollFactor = atoi(IINext);
    if (UnrollFactor == 0) {
      assert(false && "Bad value for unrolling factor");
    }
    Utils::printDebug("ScopLoc",
                      "Adding unrolling factor = " + II->getName().str() +
                          ", " + std::to_string(UnrollFactor));
    PA.UnrollDim.push_back(
        std::tuple<std::string, int>(II->getName().str(), UnrollFactor));
    PP.Lex(Tok);
  }
  return PA;
}

/// Handle pragmas of the form
///
///  #pragma macveth [unroll|nounroll|unrollandjam] [loopIdentifier
///  unrollFactor]
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

#endif
