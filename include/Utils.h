/*
 * File					 : include/Utils.h
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:36 +02:00
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

#ifndef MACVETH_UTILS_H
#define MACVETH_UTILS_H

#include "include/MVOptions.h"
#include "clang/AST/AST.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace clang;
using namespace macveth;

namespace macveth {

class Utils {
public:
  /// Type of string list
  using StrList = std::vector<std::string>;

  /// When declaring static members of class, when assigning them you need
  /// to redeclare them or since C++17 you can just put in

  static inline clang::SourceManager *SourceMgr;
  static inline clang::LangOptions *LangOpts;
  static inline clang::ASTContext *Ctx;

  /// Check if list contains an element given
  template <typename T>
  static bool contains(std::vector<T> listOfElements, T element);

  /// Converting clang expressions to strings
  static std::string getStringFromStmt(const Stmt *S);

  /// Converting clang expressions to strings
  static std::string getStringFromExpr(const Expr *E);

  /// Given an Expr, get its integer value
  static int64_t getIntFromExpr(const Expr *E);

  /// Check whether expression is or not numeric
  static bool isNumericValue(Expr *E);

  /// String to uppercase
  static std::string toUppercase(std::string S) {
    std::transform(S.begin(), S.end(), S.begin(),
                   [](unsigned char C) { return std::toupper(C); });
    return S;
  }

  /// String to lowercase
  static std::string toLowercase(std::string S) {
    std::transform(S.begin(), S.end(), S.begin(),
                   [](unsigned char C) { return std::tolower(C); });
    return S;
  }

  /// Truncate file: create or rewrite if already created
  static void initFile(std::string FileName) {
    if (FileName != "") {
      std::ofstream NewFile;
      NewFile.open(getExePath() + FileName, std::ios_base::trunc);
      NewFile << MVOptions::OutFile << std::endl;
      NewFile.close();
    }
  }

  /// Get clang SourceManager
  static clang::SourceManager *getSourceMgr();
  /// Get clang LangOptions
  static clang::LangOptions *getLangOpts();
  /// Get clang ASTContext
  static clang::ASTContext *getCtx();
  /// Set some clang resources
  static void setOpts(SourceManager *SO, LangOptions *LO, ASTContext *C);
  /// Get execution path of the binary; not just a getcwd() call
  static std::string getExePath();
};

/// Split string according to a delimiter D (char)
inline std::vector<std::string> split(const std::string &S, char D) {
  std::vector<std::string> Tokens;
  std::string Token;
  std::stringstream TokenStream(S);
  while (std::getline(TokenStream, Token, D)) {
    Tokens.push_back(Token);
  }
  return Tokens;
}

template <char D> class Split {
public:
  static std::vector<std::string> split(const std::string &S);
};

template <char D>
std::vector<std::string> Split<D>::split(const std::string &S) {
  std::vector<std::string> Tokens;
  std::string Token;
  std::stringstream TokenStream(S);
  while (std::getline(TokenStream, Token, D)) {
    Tokens.push_back(Token);
  }
  return Tokens;
}

} // namespace macveth
#endif
