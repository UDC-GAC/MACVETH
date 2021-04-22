// MACVETH - Utils.cpp
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

#include "include/Utils.h"
#if __GNUC__ >= 8
#include <filesystem>
#else
#include <experimental/filesystem>
#endif
#include "llvm/ADT/APFloat.h"

using namespace macveth;

#ifdef WIN32
#include <direct.h>
//-------------------------------------------------------------
std::string Utils::getExePath() {
  // FIXME:
  char result[MAX_PATH];
  int found;
  GetModuleFileName(nullptr, result, MAX_PATH);
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
bool Utils::contains(std::list<T> ListOfElements, T Element) {
  return std::find(ListOfElements.begin(), ListOfElements.end(), Element) !=
         ListOfElements.end();
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
int64_t evaluateBinaryOperator(const BinaryOperator *OP) {
  auto LHS = Utils::getIntFromExpr(OP->getLHS());
  auto RHS = Utils::getIntFromExpr(OP->getRHS());
  if ((LHS == -1) || (RHS == -1)) {
    return -1;
  }
  switch (OP->getOpcode()) {
  case BinaryOperator::Opcode::BO_Add:
    return LHS + RHS;
  case BinaryOperator::Opcode::BO_Sub:
    return LHS - RHS;
  case BinaryOperator::Opcode::BO_Mul:
    return LHS * RHS;
  default:
    return LHS + RHS;
  }
}
//-------------------------------------------------------------
int64_t Utils::getIntFromExpr(const Expr *E) {
  if (E == nullptr) {
    return -1;
  }
  auto C = Utils::getCtx();
  clang::Expr::EvalResult R;
  // First try general case
  if (E->EvaluateAsInt(R, *C)) {
    return R.Val.getInt().getExtValue();
  }
  // Reference to a variable already declared
  if (auto D = dyn_cast<DeclRefExpr>(E->IgnoreImpCasts())) {
    if (auto VD = dyn_cast<VarDecl>(D->getDecl())) {
      return getIntFromExpr(VD->getInit());
    }
  }
  // Reference to a variable already declared
  if (auto BO = dyn_cast<BinaryOperator>(E->IgnoreImpCasts())) {
    return evaluateBinaryOperator(BO);
  }
  return -1;
}

//-------------------------------------------------------------
bool Utils::isNumericValue(Expr *E) {
  auto C = Utils::getCtx();
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
  auto SM = Utils::getSourceMgr();
  auto LO = Utils::getLangOpts();
  auto CharRangeExpr =
      clang::CharSourceRange::getTokenRange(S->getSourceRange());
  return Lexer::getSourceText(CharRangeExpr, *SM, *LO).str();
}

//-------------------------------------------------------------
std::string Utils::getStringFromExpr(const clang::Expr *E) {
  auto SM = Utils::getSourceMgr();
  auto LO = Utils::getLangOpts();
  auto CharRangeExpr =
      clang::CharSourceRange::getTokenRange(E->getSourceRange());
  return Lexer::getSourceText(CharRangeExpr, *SM, *LO).str();
}
