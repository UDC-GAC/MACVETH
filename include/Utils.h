/**
 * File              : Utils.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 18 Nov 2019 15:16:05 MST
 * Last Modified Date: Ven 20 Dec 2019 14:37:04 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 *
 * Copyright (c) 2019 Marcos Horro <marcos.horro@udc.gal>
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

#include <iostream>

#include "clang/AST/AST.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include <llvm-10/llvm/ADT/APFloat.h>

using namespace clang;

namespace macveth {

class Utils {
public:
  /// Type of string list
  typedef std::list<std::string> StrList;

  // When declaring static members of class, when assigning them you need
  // to redeclare them or since C++17 you can just put in

  inline static clang::SourceManager *SourceMgr;
  inline static clang::LangOptions *LangOpts;
  inline static clang::ASTContext *Ctx;

  /// Check if list contains an element given
  template <typename T>
  static bool contains(std::list<T> &listOfElements, const T &element);

  /// Converting clang expressions to strings
  static std::string getStringFromExpr(const Expr *E);
  /// Dummy function to get the name of a temp register given a value
  static std::string getNameTempReg(int Val);

  /// Given an Expr, get its integer value
  static int64_t getIntFromExpr(const Expr *E, const ASTContext *C);

  /// Check whether expression is or not numeric
  static bool isNumericValue(Expr *E);

  /// Get clang SourceManager
  static clang::SourceManager *getSourceMgr();
  /// Get clang LangOptions
  static clang::LangOptions *getLangOpts();
  /// Get clang ASTContext
  static clang::ASTContext *getCtx();
  /// Set some clang resources
  static void setOpts(SourceManager *SO, LangOptions *LO, ASTContext *C);
  /// Print function for debugging purposes
  // static void printDebug(std::string M, std::string S);
};

//// ---------------------------------------------
// void Utils::printDebug(std::string M, std::string S) {
//  std::cout << "[" << M << " DEBUG] " << S << std::endl;
//}

//-------------------------------------------------------------
template <typename T>
bool Utils::contains(std::list<T> &listOfElements, const T &element) {
  // Find the iterator if element in list
  auto it = std::find(listOfElements.begin(), listOfElements.end(), element);
  // return if iterator points to end or not. It points to end then it
  // means element does not exists in list
  return it != listOfElements.end();
}

} // namespace macveth
#endif
