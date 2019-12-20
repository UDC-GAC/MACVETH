/**
 * File              : Utils.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Sáb 23 Nov 2019 10:51:03 MST
 * Last Modified Date: Ven 20 Dec 2019 14:29:28 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Utils.h"

using namespace macveth;

//-------------------------------------------------------------
SourceManager *Utils::getSourceMgr() { return Utils::SourceMgr; }

//-------------------------------------------------------------
LangOptions *Utils::getLangOpts() { return Utils::LangOpts; }

//-------------------------------------------------------------
ASTContext *Utils::getCtx() { return Utils::Ctx; }

//-------------------------------------------------------------
void Utils::setOpts(SourceManager *SO, LangOptions *LO, ASTContext *C) {
  Utils::SourceMgr = SO;
  Utils::LangOpts = LO;
  Utils::Ctx = C;
}

//-------------------------------------------------------------
int64_t Utils::getIntFromExpr(const Expr *E, const ASTContext *C) {
  clang::Expr::EvalResult R;
  if (E->EvaluateAsInt(R, *C)) {
    return R.Val.getInt().getExtValue();
  }
  return -1;
}

//-------------------------------------------------------------
std::string Utils::getNameTempReg(int Val) {
  return "temp" + std::to_string(Val);
}

//-------------------------------------------------------------
bool Utils::isNumericValue(Expr *E) {
  ASTContext *C = Utils::getCtx();
  clang::Expr::EvalResult R;
  /// Is numeric if it can be evaluated as a integer
  if (E->EvaluateAsInt(R, *C)) {
    return true;
  }
  llvm::APFloat *F;
  /// Is numeric if it can be evaluated as a float
  if (E->EvaluateAsFloat(*F, *C)) {
    return true;
  }

  return false;
}

//-------------------------------------------------------------
std::string Utils::getStringFromExpr(const clang::Expr *E) {
  SourceManager *SM = Utils::getSourceMgr();
  LangOptions *LO = Utils::getLangOpts();
  clang::CharSourceRange CharRangeExpr =
      clang::CharSourceRange::getTokenRange(E->getSourceRange());
  std::string Text = Lexer::getSourceText(CharRangeExpr, *SM, *LO);
  return Text;
}
