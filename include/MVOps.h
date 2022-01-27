// MACVETH - MVOps.h
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
//     Louis-Noël Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Marcos Horro <marcos.horro@udc.es>

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
  static MVOpCode getTypeFromStr(const std::string &S) {
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
  std::string getStrFromMVCode(const MVOpCode &C) const {
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
  std::string getStrFromBinaryOperatorOpcode(
      const clang::BinaryOperator::Opcode &ClangOP) const {
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
  bool isAssignment() const {
    return (getType() == MVOpType::CLANG_BINOP) &&
           (this->ClangOP == clang::BinaryOperator::Opcode::BO_Assign);
  }

  /// Check whether the operation is binary or not. An operation is binary if
  /// it is placed between two operands, in other case if is a function
  bool isBinaryOperation() const {
    return (getType() == MVOpType::CLANG_BINOP);
  }

  /// Check whether the operation is an addition or a substraction. This is
  /// useful for reductions
  bool isAddOrSub() {
    return (ClangOP == BinaryOperator::Opcode::BO_Add) ||
           (ClangOP == BinaryOperator::Opcode::BO_Sub);
  }

  /// Get equivalent MVOP string for searching in the Table Cost
  std::string getTableMVOPstr(const std::string &T) const {
    std::string Suffix = "_" + Utils::toUppercase(T);
    if (getType() == MVOpType::CLANG_BINOP) {
      return Utils::toUppercase(getStrFromBinaryOperatorOpcode(ClangOP)) +
             Suffix;
    }
    return Utils::toUppercase(getStrFromMVCode(MVOpC)) + Suffix;
  }

  /// Get the namespace of the expression
  std::string getPrefixFromStr(const std::string &S) const {
    std::string P = "";
    std::string NameSpace = "::";
    auto FoundNameSpace = S.find(NameSpace);
    if (FoundNameSpace != std::string::npos) {
      P = S.substr(0, FoundNameSpace + NameSpace.size());
    }
    return P;
  }

  /// Convert MVOp to string
  std::string toString() const {
    if (getType() == MVOpType::CLANG_BINOP) {
      return clang::BinaryOperator::getOpcodeStr(ClangOP).str();
    }
    return getStrFromMVCode(MVOpC);
  }

  /// Get the prefix
  std::string getOpPrefix() const { return Prefix; }

  /// Return the type of the operation
  MVOpType getType() const { return T; }

  /// Constructor given a string: will be a MVFUNC
  MVOp(const std::string &S) {
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