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

using namespace clang;
using namespace macveth;

namespace macveth {

class Utils {
public:
  /// Type of string list
  using StrList = std::list<std::string>;

  /// When declaring static members of class, when assigning them you need
  /// to redeclare them or since C++17 you can just put in

  inline static clang::SourceManager *SourceMgr;
  inline static clang::LangOptions *LangOpts;
  inline static clang::ASTContext *Ctx;

  /// Check if list contains an element given
  template <typename T>
  static bool contains(std::list<T> listOfElements, T element);

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

  /// For printing debug information to file if specified
  static void printDebug(std::string M, std::string Msg) {
    if (MVOptions::Debug) {
      if (MVOptions::OutDebugFile != "") {
        std::ofstream O;
        O.open(getExePath() + MVOptions::OutDebugFile,
               std::ios_base::app); // append instead of overwrite
        O << "[" << M << "] " << Msg << std::endl;
        O.close();
      } else {
        std::cout << "[" << M << "] " << Msg << std::endl;
      }
    }
  }

  static void printDebug(std::string M, std::string Msg, DebugLevel DB) {
    if (!MVOptions::DLevel) {
      printDebug(M, Msg);
      return;
    }
    if (DB <= MVOptions::DLevel) {
      printDebug(M, Msg);
    }
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

} // namespace macveth
#endif
