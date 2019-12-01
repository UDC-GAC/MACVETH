/**
 * File              : IntrinsicsGenerator.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 19 Nov 2019 09:49:18 MST
 * Last Modified Date: Sáb 30 Nov 2019 20:46:11 MST
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

#ifndef MACVETH_INTRINSICS_H
#define MACVETH_INTRINSICS_H

#include "include/TAC.h"
#include "include/TempExpr.h"
#include "include/Utils.h"
#include <cstdarg>
#include <iostream>
#include <map>
#include <memory>
#include <string>

/// FIXME: this class is meant for AVX2. What should be done is a refactoring in
///        in order to have subclasses which implement the proper declarations
///        depending on the different ISA used.

///
/// Data types in AVX
/// __m128	128-bit vector containing 4 floats
/// __m128d	128-bit vector containing 2 doubles
/// __m128i	128-bit vector containing   integers
/// __m256	256-bit vector containing 8 floats
/// __m256d	256-bit vector containing 4 doubles
/// __m256i	256-bit vector containing   integers
///
/// Function naming convention:
///
/// _mm<width>_<name>_<datatype>
///
/// width: [256], for 128 bits empty
/// name: function name
/// datatype:
///     * ps: floats
///     * pd: doubles
///     * epi8/epi16/epi32/epi64: vectors contain 8-bit/16-bit/32-bit/64-bit
///     signed integers
///     * epu8/epu16/epu32/epu64: vectors contain 8-bit/16-bit/32-bit/64-bit
///     unsigned integer
///     * si128/si256: unspecified 128-bit vector or 256-bit vector
///     * m128/m128i/m128d/m256/m256i/m256d: identifies input vector types when
///     they're different than the type of the returned vector
///
/// Something important to remember:
/// * Vector variables are not related with the register variables that the
///   compiler uses, therefore it is not needed to use xmmX, ymmX nor zmmX when
///   writing code.
///

namespace macveth {

// to avoid long declarations
typedef std::list<std::string> InstListType;

class IntrinsicsInsGen {
public:
  // Main function:
  // It basically performs three passes:
  // 1.- Looks for memory loads
  // 2.- Looks for arithmetic operations
  // 3.- Generates memory stores
  static std::list<InstListType> translateTAC(std::list<macveth::TAC> TacList);
  /// Generate instructions for reduction given a input data; results will be
  /// stored in the OutOp operand. The algorithm is based on [1], by Clay B.
  /// [1] https://software.intel.com/en-us/blogs/2016/04/12/\
  /// can-you-write-a-vectorized-reduction-operation
  static InstListType reduceVector(TempExpr *InOp, TempExpr *OutOp);

private:
  inline static std::list<std::string> RegDeclared;
  inline static std::list<std::string> TempRegDeclared;
  inline static std::map<std::string, std::string> RegMap;

  inline static std::map<std::string, int> TypeToWidth;
  inline static std::map<std::string, std::string> TypeToDataType;
  inline static std::map<BinaryOperator::Opcode, std::string> BOtoIntrinsic;

  /// Get a register given the operand, to check whether it is already mapped or
  /// if it needs a new declaration
  static std::string getRegister(TempExpr *Op);

  /// Retrieves mapping from OpCode to string
  static std::string getFuncFromOpCode(BinaryOperator::Opcode Op) {
    return BOtoIntrinsic[Op];
  }

  /// Get bit width from data type
  static int getBitWidthFromType(clang::QualType ExprType) {
    return IntrinsicsInsGen::TypeToWidth[ExprType.getAsString()];
  }

  /// Get intrinsics instruction suffix from data type
  static std::string getDataTypeFromType(clang::QualType ExprType) {
    return IntrinsicsInsGen::TypeToDataType[ExprType.getAsString()];
  }

  /// Generates the body of the AVX instruction, e.g. _mm256_add_pd,
  /// without any arguments. It is an auxiliary function
  static std::string genAVXIns(int BitWidth, std::string Name,
                               std::string DataType) {
    std::string Width = (BitWidth >= 256) ? std::to_string(BitWidth) : "";

    std::string IntrinsicsIns = "_mm" + Width + "_" + Name + "_" + DataType;
    return IntrinsicsIns;
  }

  /// FIXME
  /// Generate AVX declarations
  static std::string genRegDecl(std::string Name, int Width) {
    std::string DataType = "d"; // FIXME double
    std::string RegDecl = "__m" + std::to_string(Width) + DataType + " " + Name;
    return RegDecl;
  }

  /// Creates a loadReg# string
  static std::string loadRegToStr(int RegNo) {
    return "loadReg" + std::to_string(RegNo);
  }

  /// Debugging purposes
  static void printInstList(std::list<InstListType> List) {
    for (InstListType L : List) {
      printList(L);
    }
  }

  /// Debugging purposes
  static void printList(InstListType List) {
    for (std::string S : List) {
      std::cout << S << std::endl;
    }
  }

  /// Maps a register allocation for an operator
  static std::string getAvailableReg(TempExpr *Op);

  /// Retrieves the source code for a function given its name and the list of
  /// operands present in the function. First operand/expression will be the
  /// result operand, so it will be the LHS
  static std::string getGenericFunction(std::string FuncName,
                                        std::list<TempExpr> ExprList);

  /// Given the name of the function and a list of operands, creates the source
  /// code for it
  static std::string genVarArgsFunc(std::string NameFunc,
                                    std::list<std::string> Operands);

  /// Generate load instruction from operand
  static std::string genLoad(TempExpr *Op);

  /// Generates the store instruction
  static std::string genStore(TempExpr *St, TempExpr *Val);

  /// Generates the assignment code given LHS and RHS
  static std::string genAssignment(std::string Lhs, std::string Rhs) {
    std::string AssignmentStr = Lhs + " = " + Rhs;
    return AssignmentStr;
  }

  /// This is a fucking awful hack
  /// TempExpr are used as wrappers of Expr and new Nodes. This class has
  /// been created for the ease of TAC handling. So, if a TAC does not
  /// belong to Expr class.
  static bool needLoad(macveth::TempExpr *Op) {
    if (IntrinsicsInsGen::RegMap.find(Op->getExprStr()) !=
        IntrinsicsInsGen::RegMap.end()) {
      return false;
    }
    // std::cout << "checking if it is not Clang" << std::endl;
    return !Op->isNotClang();
  }

  /// Main function when generating instructions: takes a list of TAC and
  /// iteratively creates for each its equivalent in intrinsics code
  static InstListType genIntrinsicsIns(std::list<macveth::TAC> TacList);

  /// Generates a FMA from two consecutives TACs
  static std::string generateFMA(macveth::TAC *PrevTAC,
                                 macveth::TAC *ActualTAC);

  /// Checks whether a FMA can be generated
  static bool potentialFMA(macveth::TAC *PrevTAC, macveth::TAC *Tac);

private:
  /// Singleton pattern declaratio
  static IntrinsicsInsGen *Singleton;
  IntrinsicsInsGen() {
    TypeToWidth = {{"double", 256}, {"float", 256}};
    TypeToDataType = {{"double", "pd"}, {"float", "sp"}};
    BOtoIntrinsic = {{BO_Mul, "mul"},
                     {BO_Div, "div"},
                     {BO_Add, "add"},
                     {BO_Sub, "sub"},
                     {BO_Assign, "store"}};
  };

public:
  static IntrinsicsInsGen *getInstance();
}; // class IntrinsicsInsGen

} // namespace macveth
#endif
