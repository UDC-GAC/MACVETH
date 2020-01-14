/**
 * File              : MVPragmaHandler.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 06 Xan 2020 10:54:41 MST
 * Last Modified Date: Mar 14 Xan 2020 12:04:51 MST
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

/// The Location of the Scop, as delimited by macveth and endmv
/// pragmas by the user.
/// "macveth" and "endmv" are the source locations of the macveth and
/// endmv pragmas.
/// "StartLine" is the Line number of the Start position.
struct ScopLoc {
  ScopLoc() : End(0) {}

  /// Start location of the pragma
  clang::SourceLocation Scop;
  /// End location of the pragma
  clang::SourceLocation EndScop;
  unsigned StartLine;
  unsigned Start;
  unsigned End;
  bool Visited = false;

  struct PragmaArgs {
    bool Unroll = true;
    int UnrollFactor = 1;
    bool UnrollAndJam = true;
    std::list<std::string> UnrollDim;
  };

  PragmaArgs PA;
};

/// List of pairs of #pragma macveth and #pragma mvend Locations.
struct ScopHandler {
  std::vector<ScopLoc> List;

  /// Empty constructor
  ScopHandler(){};

  /// Return a SourceLocation for Line "Line", column "col" of file "FID".
  SourceLocation translateLineCol(SourceManager &SM, FileID FID, unsigned Line,
                                  unsigned col) {
    return SM.translateLineCol(FID, Line, col);
  }

  /// Add a new Start (#pragma macveth) Location to the list.
  /// If the last #pragma macveth did not have a matching
  /// #pragma endmv then overwrite it.
  /// "Start" points to the location of the macveth pragma.
  void addStart(SourceManager &SM, SourceLocation Start,
                ScopLoc::PragmaArgs PA) {
    ScopLoc Loc;

    Loc.PA = PA;
    Loc.Scop = Start;
    int Line = SM.getExpansionLineNumber(Start);
    Start = translateLineCol(SM, SM.getFileID(Start), Line, 1);
    Loc.StartLine = Line;
    Loc.Start = SM.getFileOffset(Start);
    if (List.size() == 0 || List[List.size() - 1].End != 0) {
      List.push_back(Loc);
    } else {
      List[List.size() - 1] = Loc;
    }
  }

  /// Set the end location (#pragma endmv) of the last pair
  /// in the list.
  /// If there is no such pair of if the end of that pair
  /// is already set, then ignore the spurious #pragma endmv.
  /// "end" points to the location of the endmv pragma.
  void addEnd(SourceManager &SM, SourceLocation End) {
    if (List.size() == 0 || List[List.size() - 1].End != 0)
      return;
    List[List.size() - 1].EndScop = End;
    int Line = SM.getExpansionLineNumber(End);
    End = translateLineCol(SM, SM.getFileID(End), Line + 1, 1);
    List[List.size() - 1].End = SM.getFileOffset(End);
  }
};

static IdentifierInfo *getValue(Token &token) {
  if (token.isNot(tok::identifier))
    return NULL;
  return token.getIdentifierInfo();
}

static ScopLoc::PragmaArgs parseArguments(Preprocessor &PP) {
  ScopLoc::PragmaArgs PA;
  IdentifierInfo *II;
  Token Tok;
  PP.Lex(Tok);
  if ((II = getValue(Tok)) != NULL) {
    if (II->isStr("nounroll")) {
      PA.Unroll = false;
      PA.UnrollAndJam = false;
    }
  }
  return PA;
}

/// Handle pragmas of the form
///
///  #pragma macveth
///
/// In particular, store the Location of the line containing
/// the pragma in the list "Scops".
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
///  #pragma endmv
///
/// In particular, store the Location of the line following the one containing
/// the pragma in the list "Scops".
struct PragmaEndScopHandler : public PragmaHandler {
  ScopHandler *Scops;

  PragmaEndScopHandler(ScopHandler *Scops)
      : PragmaHandler("endmv"), Scops(Scops) {}

  virtual void HandlePragma(Preprocessor &PP, PragmaIntroducer Introducer,
                            Token &EndScopTok) {
    SourceManager &SM = PP.getSourceManager();
    SourceLocation Sloc = EndScopTok.getLocation();
    Scops->addEnd(SM, Sloc);
  }
};

#endif
