/**
 * File              : MVPragmaHandler.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 06 Xan 2020 10:54:41 MST
 * Last Modified Date: Lun 06 Xan 2020 18:09:33 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include <iostream>
#include <vector>

#include "clang/Basic/SourceManager.h"
#include "clang/Parse/Parser.h"

using namespace clang;

/* The location of the scop, as delimited by scop and endscop
 * pragmas by the user.
 * "scop" and "endscop" are the source locations of the scop and
 * endscop pragmas.
 * "start_line" is the line number of the start position.
 */
struct ScopLoc {
  ScopLoc() : end(0) {}

  clang::SourceLocation scop;
  clang::SourceLocation endscop;
  unsigned start_line;
  unsigned start;
  unsigned end;
};

/* List of pairs of #pragma scop and #pragma endscop locations.
 */
struct ScopLocList {
  std::vector<ScopLoc> list;

  /* Return a SourceLocation for line "line", column "col" of file "FID".
   */
  SourceLocation translateLineCol(SourceManager &SM, FileID FID, unsigned line,
                                  unsigned col) {
    return SM.translateLineCol(FID, line, col);
  }
  /* Add a new start (#pragma scop) location to the list.
   * If the last #pragma scop did not have a matching
   * #pragma endscop then overwrite it.
   * "start" points to the location of the scop pragma.
   */
  void add_start(SourceManager &SM, SourceLocation start) {
    ScopLoc loc;

    loc.scop = start;
    int line = SM.getExpansionLineNumber(start);
    start = translateLineCol(SM, SM.getFileID(start), line, 1);
    loc.start_line = line;
    loc.start = SM.getFileOffset(start);
    if (list.size() == 0 || list[list.size() - 1].end != 0)
      list.push_back(loc);
    else
      list[list.size() - 1] = loc;
  }

  /* Set the end location (#pragma endscop) of the last pair
   * in the list.
   * If there is no such pair of if the end of that pair
   * is already set, then ignore the spurious #pragma endscop.
   * "end" points to the location of the endscop pragma.
   */
  void add_end(SourceManager &SM, SourceLocation end) {
    if (list.size() == 0 || list[list.size() - 1].end != 0)
      return;
    list[list.size() - 1].endscop = end;
    int line = SM.getExpansionLineNumber(end);
    end = translateLineCol(SM, SM.getFileID(end), line + 1, 1);
    list[list.size() - 1].end = SM.getFileOffset(end);
  }
};

/* Handle pragmas of the form
 *
 *	#pragma scop
 *
 * In particular, store the location of the line containing
 * the pragma in the list "scops".
 */
struct PragmaScopHandler : public PragmaHandler {
  ScopLocList &scops;

  PragmaScopHandler(ScopLocList &scops) : PragmaHandler("scop"), scops(scops) {}

  virtual void HandlePragma(Preprocessor &PP, PragmaIntroducer Introducer,
                            Token &ScopTok) {
    SourceManager &SM = PP.getSourceManager();
    SourceLocation sloc = ScopTok.getLocation();
    scops.add_start(SM, sloc);
    std::cout << "BEGIN SCOP\n";
  }
};

/* Handle pragmas of the form
 *
 *	#pragma endscop
 *
 * In particular, store the location of the line following the one containing
 * the pragma in the list "scops".
 */
struct PragmaEndScopHandler : public PragmaHandler {
  ScopLocList &scops;

  PragmaEndScopHandler(ScopLocList &scops)
      : PragmaHandler("endscop"), scops(scops) {}

  virtual void HandlePragma(Preprocessor &PP, PragmaIntroducer Introducer,
                            Token &EndScopTok) {
    SourceManager &SM = PP.getSourceManager();
    SourceLocation sloc = EndScopTok.getLocation();
    scops.add_end(SM, sloc);
    std::cout << "END SCOP\n";
  }
};
