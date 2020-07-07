/**
 * File              : MVOps.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 05 Xan 2020 11:51:39 MST
 * Last Modified Date: Xov 09 Xan 2020 21:16:38 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
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
  /// Tyep of the MVOp
  MVOpType T;
  /// We can handle Clang BinOps or MVOps
  union {
    /// When CLANG_BINOP
    clang::BinaryOperator::Opcode ClangOP;
    /// When MVFUNC
    MVOpCode MVOpC;
  };

  /// Get operation cost
  /// FIXME: this should be architecture-dependent
  static long getOperationCost(MVOp Op) {
    long Cost = 100000;
    auto Type = Op.getType();
    if (Type == MVOpType::MVFUNC) {
      auto Code = Op.MVOpC;
      switch (Code) {
      case UNDEF:
        return Cost;
      case EXP:
      case POW:
      case MAX:
      case MIN:
      case AVG:
      case CEIL:
      case FLOOR:
      case ROUND:
      case SIN:
      case COS:
      default:
        return 10000;
      }
    }
    if (Type == MVOpType::CLANG_BINOP) {
      Cost = 2;
      if (Op.ClangOP == BinaryOperator::Opcode::BO_Mul) {
        Cost = 12;
      }
      if (Op.ClangOP == BinaryOperator::Opcode::BO_Div) {
        Cost = 20;
      }
      if (Op.ClangOP == BinaryOperator::Opcode::BO_Xor) {
        Cost = 15;
      }
    }
    return Cost;
  }

  /// Given a string it returns the type of function based on the MVOpCode it
  /// is defined for this compiler
  static MVOpCode getTypeFromStr(std::string S) {
    if ((S == "exp") || (S == "EXP"))
      return MVOpCode::EXP;
    if ((S == "pow") || (S == "powf") || (S == "powl"))
      return MVOpCode::POW;
    if ((S == "std::max") || (S == "max") || (S == "MAX"))
      return MVOpCode::MAX;
    if ((S == "std::min") || (S == "min") || (S == "MIN"))
      return MVOpCode::MIN;
    if ((S == "avg") || (S == "AVG"))
      return MVOpCode::AVG;
    if ((S == "ceil") || (S == "CEIL"))
      return MVOpCode::CEIL;
    if ((S == "floor") || (S == "FLOOR"))
      return MVOpCode::FLOOR;
    if ((S == "round") || (S == "ROUND"))
      return MVOpCode::ROUND;
    if ((S == "sin") || (S == "SIN"))
      return MVOpCode::SIN;
    if ((S == "cos") || (S == "COS"))
      return MVOpCode::COS;
    return MVOpCode::UNDEF;
  }

  /// Return the equivalent string given a MVOpCode
  std::string getStrFromMVCode(MVOpCode C) {
    switch (C) {
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
  /// Shortcut to check whether a MVOp is of type BO_Assign
  bool isAssignment() {
    return (getType() == MVOpType::CLANG_BINOP) &&
           (this->ClangOP == clang::BinaryOperator::Opcode::BO_Assign);
  }

  /// Convert MVOp to string
  std::string toString() {
    if (getType() == MVOpType::CLANG_BINOP) {
      return clang::BinaryOperator::getOpcodeStr(this->ClangOP);
    } else if (getType() == MVOpType::MVFUNC) {
      return getStrFromMVCode(this->MVOpC);
    }
    MVAssert(false, "This can not ever happen!");
    return "";
  }

  /// Return the type of the operation
  MVOpType getType() { return T; }

  /// Constructor given a string: will be a MVFUNC
  MVOp(std::string S) {
    this->T = MVOpType::MVFUNC;
    this->MVOpC = getTypeFromStr(S);
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
}; // namespace macveth

} // namespace macveth
#endif /* !MACVETH_MVOPS_H */