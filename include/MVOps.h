/*
 * File					 : include/MVOps.h
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:38 +02:00
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

#ifndef MACVETH_MVOPS_H
#define MACVETH_MVOPS_H

#include "include/MVAssert.h"
#include "clang/AST/Expr.h"

namespace macveth {

/// MVOp possible types
enum MVOpType {
  /// BinaryOperator::Opcode type
  CLANG_BINOP,
  /// MVOpCode type
  MVFUNC
};

/// Custom MVFUNC
enum MVOpCode {
  /// Operation not known/defined
  UNDEF,
  /// Addition
  ADD,
  /// Substraction
  SUB,
  /// Multiplication
  MUL,
  /// Division
  DIV,
  /// Exponential
  EXP,
  /// Power
  POW,
  /// Max operation
  MAX,
  /// Minimum
  MIN,
  /// Average value
  AVG,
  /// Ceil values
  CEIL,
  /// Floor values
  FLOOR,
  /// Round values
  ROUND,
  /// Sinus
  SIN,
  /// Cosine
  COS
};

/// MVOp is meant to identify those operations that Clang lacks. This way
/// parsing is easier for backends and more extensible. It is implemented using
/// a 'union' over 'std::variant' (C++14) since both possible types are enums
/// and may have the same values; this way we can explicitly handle each case
struct MVOp {
  /// Prefix
  std::string Prefix = "";
  /// Type of the MVOp
  MVOpType T;
  /// We can handle Clang BinOps or MVOps
  union {
    /// When CLANG_BINOP
    clang::BinaryOperator::Opcode ClangOP;
    /// When MVFUNC
    MVOpCode MVOpC;
  };

  /// Given a string it returns the type of function based on the MVOpCode it
  /// is defined for this compiler
  static MVOpCode getTypeFromStr(std::string S) {
    if ((S == "std::max") || (S == "max") || (S == "MAX"))
      return MVOpCode::MAX;
    if ((S == "std::min") || (S == "min") || (S == "MIN"))
      return MVOpCode::MIN;
    if ((S == "avg") || (S == "AVG"))
      return MVOpCode::AVG;
    if ((S == "round") || (S == "ROUND"))
      return MVOpCode::ROUND;

    MVAssertSkip(MVOptions::IntrinsicsSVML,
                 "No SVML, so function " + S + " can not be vectorized",
                 GotoStartScop);

    if ((S == "ceil") || (S == "CEIL"))
      return MVOpCode::CEIL;
    if ((S == "floor") || (S == "FLOOR"))
      return MVOpCode::FLOOR;
    if ((S == "exp") || (S == "EXP"))
      return MVOpCode::EXP;
    if ((S == "pow") || (S == "powf") || (S == "powl"))
      return MVOpCode::POW;
    if ((S == "sin") || (S == "SIN"))
      return MVOpCode::SIN;
    if ((S == "cos") || (S == "COS"))
      return MVOpCode::COS;

    return MVOpCode::UNDEF;
  }

  /// Return the equivalent string given a MVOpCode
  std::string getStrFromMVCode(MVOpCode C) {
    switch (C) {
    case MVOpCode::ADD:
      return "add";
    case MVOpCode::SUB:
      return "sub";
    case MVOpCode::DIV:
      return "div";
    case MVOpCode::MUL:
      return "mul";
    case MVOpCode::EXP:
      return "exp";
    case MVOpCode::MAX:
      return "max";
    case MVOpCode::MIN:
      return "min";
    case MVOpCode::AVG:
      return "avg";
    case MVOpCode::CEIL:
      return "ceil";
    case MVOpCode::FLOOR:
      return "floor";
    case MVOpCode::ROUND:
      return "round";
    case MVOpCode::SIN:
      return "sin";
    case MVOpCode::COS:
      return "cos";
    default:
      return "undef";
    }
    return "undef";
  }

  /// Return the equivalent string given a MVOpCode
  std::string
  getStrFromBinaryOperatorOpcode(clang::BinaryOperator::Opcode ClangOP) {
    switch (ClangOP) {
    case clang::BinaryOperator::Opcode::BO_Add:
      return "add";
    case clang::BinaryOperator::Opcode::BO_Sub:
      return "sub";
    case clang::BinaryOperator::Opcode::BO_Div:
      return "div";
    case clang::BinaryOperator::Opcode::BO_Mul:
      return "mul";
    default:
      return "undef";
    }
    return "undef";
  }

  /// Shortcut to check whether a MVOp is of type BO_Assign
  bool isAssignment() {
    return (getType() == MVOpType::CLANG_BINOP) &&
           (this->ClangOP == clang::BinaryOperator::Opcode::BO_Assign);
  }

  /// Check whether the operation is binary or not. An operation is binary if
  /// it is placed between two operands, in other case if is a function
  bool isBinaryOperation() { return (getType() == MVOpType::CLANG_BINOP); }

  /// Get equivalent MVOP string for searching in the Table Cost
  std::string getTableMVOPstr(std::string T) {
    std::string Suffix = "_" + Utils::toUppercase(T);
    if (getType() == MVOpType::CLANG_BINOP) {
      return Utils::toUppercase(getStrFromBinaryOperatorOpcode(this->ClangOP)) +
             Suffix;
    }
    return Utils::toUppercase(getStrFromMVCode(this->MVOpC)) + Suffix;
  }

  std::string getPrefixFromStr(std::string S) {
    std::string P = "";
    std::string NameSpace = "::";
    auto FoundNameSpace = S.find(NameSpace);
    if (FoundNameSpace != std::string::npos) {
      P = S.substr(0, FoundNameSpace + NameSpace.size());
    }
    return P;
  }

  /// Convert MVOp to string
  std::string toString() {
    if (getType() == MVOpType::CLANG_BINOP) {
      return clang::BinaryOperator::getOpcodeStr(this->ClangOP).str();
    }
    return getStrFromMVCode(this->MVOpC);
  }

  /// Get the prefix
  std::string getOpPrefix() { return this->Prefix; }

  /// Return the type of the operation
  MVOpType getType() { return T; }

  /// Constructor given a string: will be a MVFUNC
  MVOp(std::string S) {
    this->T = MVOpType::MVFUNC;
    this->MVOpC = getTypeFromStr(S);
    this->Prefix = getPrefixFromStr(S);
  }

  /// Given a BinaryOperator::Opcode the MVOp will be of CLANG_BINOP type
  MVOp(clang::BinaryOperator::Opcode OP) {
    this->T = MVOpType::CLANG_BINOP;
    this->ClangOP = OP;
  }

  /// Empty constructor: UNDEF MVOp
  MVOp() {
    this->T = MVOpType::MVFUNC;
    this->MVOpC = MVOpCode::UNDEF;
  }
};

} // namespace macveth
#endif /* !MACVETH_MVOPS_H */