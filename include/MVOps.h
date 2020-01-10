/**
 * File              : MVOps.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 05 Xan 2020 11:51:39 MST
 * Last Modified Date: Xov 09 Xan 2020 21:16:38 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

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

  /// Given a string it returns the type of function based on the MVOpCode it is
  /// defined for this compiler
  static MVOpCode getTypeFromStr(std::string S) {
    if ((S == "max") || (S == "MAX"))
      return MVOpCode::MAX;
    if ((S == "min") || (S == "MIN"))
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
    if (MVOpCode::MAX)
      return "max";
    if (MVOpCode::MIN)
      return "min";
    if (MVOpCode::AVG)
      return "avg";
    if (MVOpCode::CEIL)
      return "ceil";
    if (MVOpCode::FLOOR)
      return "floor";
    if (MVOpCode::ROUND)
      return "round";
    if (MVOpCode::SIN)
      return "sin";
    if (MVOpCode::COS)
      return "cos";
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
    this->MVOpC = getTypeFromStr("");
  }
};

} // namespace macveth
