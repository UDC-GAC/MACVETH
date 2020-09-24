/**
 * File              : Utils.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Sáb 23 Nov 2019 10:51:03 MST
 * Last Modified Date: Lun 13 Xan 2020 19:35:31 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Utils.h"
#include <filesystem>
#include <llvm-10/llvm/ADT/APFloat.h>

using namespace macveth;

#ifdef WIN32
#include <direct.h>
//-------------------------------------------------------------
std::string Utils::getExePath() {
  // FIXME:
  char result[MAX_PATH];
  int found;
  GetModuleFileName(NULL, result, MAX_PATH);
  found = string(result).find_last_of("\\");
  return (string(result).substr(0, found) + "\\");
}
#else
#include <unistd.h>
//-------------------------------------------------------------
std::string Utils::getExePath() {
  // This function has been programmed in many different ways, being the use of
  // PWD environment variable the only one fittable. This is because MACVETH
  // wants to know the absolute path where it is being executed, regardless
  // where it has been executed or where the executable is located (e.g.
  // /usr/bin/). Thus, PWD provides this absolute location.
  // FIXME: is PWD POSIX?
  return getenv("PWD") + std::string("/");
}
#endif

//-------------------------------------------------------------
template <typename T>
bool Utils::contains(std::list<T> listOfElements, T element) {
  // Find the iterator if element in list
  auto it = std::find(listOfElements.begin(), listOfElements.end(), element);
  // return if iterator points to end or not. It points to end then it
  // means element does not exists in list
  return it != listOfElements.end();
}

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
bool Utils::isNumericValue(Expr *E) {
  ASTContext *C = Utils::getCtx();
  clang::Expr::EvalResult R;
  /// Is numeric if it can be evaluated as a integer
  if (E->EvaluateAsInt(R, *C)) {
    return true;
  }
  llvm::APFloat F(0.0);
  /// Is numeric if it can be evaluated as a float
  if (E->EvaluateAsFloat(F, *C)) {
    return true;
  }

  return false;
}

//-------------------------------------------------------------
std::string Utils::getStringFromStmt(const clang::Stmt *S) {
  SourceManager *SM = Utils::getSourceMgr();
  LangOptions *LO = Utils::getLangOpts();
  clang::CharSourceRange CharRangeExpr =
      clang::CharSourceRange::getTokenRange(S->getSourceRange());
  std::string Text = Lexer::getSourceText(CharRangeExpr, *SM, *LO);
  return Text;
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
