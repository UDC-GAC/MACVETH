/**
 * File              : MVOps.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 05 Xan 2020 11:51:39 MST
 * Last Modified Date: Dom 05 Xan 2020 12:34:24 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "clang/AST/Expr.h"

namespace macveth {

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

/// MVOp possible types
enum MVOpType {
  /// BinaryOperator::Opcode type
  CLANG_BINOP,
  /// MVOpCode type
  MVFUNC
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
  MVOpCode getTypeFromStr(std::string S) {
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

  /// Return the type of the operation
  MVOpType getType() { return T; }
};

} // namespace macveth
