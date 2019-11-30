/**
 * File              : Utils.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Sáb 23 Nov 2019 10:51:03 MST
 * Last Modified Date: Xov 28 Nov 2019 16:04:31 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Utils.h"

using namespace macveth;

//-------------------------------------------------------------
SourceManager *Utils::getSourceMgr() { return Utils::SourceMgr; }

//-------------------------------------------------------------
LangOptions *Utils::getLangOpts() { return Utils::LangOpts; }

//-------------------------------------------------------------
void Utils::setOpts(SourceManager *SO, LangOptions *LO) {
  Utils::SourceMgr = SO;
  Utils::LangOpts = LO;
}

//-------------------------------------------------------------
std::string Utils::getStringFromExpr(clang::Expr *E) {
  SourceManager *SM = Utils::getSourceMgr();
  LangOptions *LO = Utils::getLangOpts();
  clang::CharSourceRange CharRangeExpr =
      clang::CharSourceRange::getTokenRange(E->getSourceRange());
  std::string Text = Lexer::getSourceText(CharRangeExpr, *SM, *LO);
  return Text;
}
