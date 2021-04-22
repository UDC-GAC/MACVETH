// MACVETH - Utils.h
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
  ext

      /// Truncate file: create or rewrite if already created
      static void
      initFile(std::string FileName) {
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
