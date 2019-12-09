/**
 * File              : Utils.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Sáb 23 Nov 2019 10:51:03 MST
 * Last Modified Date: Dom 08 Dec 2019 20:34:34 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Utils.h"

using namespace macveth;

//-------------------------------------------------------------
SourceManager *Utils::getSourceMgr() { return Utils::SourceMgr; }

//-------------------------------------------------------------
LangOptions *Utils::getLangOpts() { return Utils::LangOpts; }

//-------------------------------------------------------------
const ASTContext *Utils::getCtx() { return Utils::Ctx; }

//-------------------------------------------------------------
void Utils::setOpts(SourceManager *SO, LangOptions *LO, ASTContext *C) {
  Utils::SourceMgr = SO;
  Utils::LangOpts = LO;
  Utils::Ctx = C;
}

//-------------------------------------------------------------
std::string Utils::getNameTempReg(int Val) {
  return "temp" + std::to_string(Val);
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
