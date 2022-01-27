// MACVETH - TAC.cpp
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

#include "include/TAC.h"
#include "include/CDAG.h"
#include "include/Debug.h"
#include "include/MVExpr/MVExprArray.h"
#include "include/MVExpr/MVExprFactory.h"
#include "include/MVExpr/MVExprLiteral.h"
#include "include/MVExpr/MVExprVar.h"
#include "clang/AST/Expr.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Lex/Lexer.h"
#include "llvm/Support/ErrorHandling.h"

using namespace clang;
using namespace macveth;

// ---------------------------------------------
std::string getNameTempReg() { return "r" + std::to_string(TAC::RegVal); }

// ---------------------------------------------
std::string getNameTempReg(int Val) { return "r" + std::to_string(Val); }

// ---------------------------------------------
std::string getCurrentNameTempReg() {
  return "r" + std::to_string(TAC::RegVal++);
}

// ---------------------------------------------
clang::BinaryOperator *getBinOp(clang::Expr *E) {
  auto B = dyn_cast<clang::BinaryOperator>(E->IgnoreImpCasts());
  auto P = dyn_cast<clang::ParenExpr>(E->IgnoreImpCasts());
  while ((P = dyn_cast<clang::ParenExpr>(E->IgnoreImpCasts()))) {
    if ((B = dyn_cast<clang::BinaryOperator>(
             P->getSubExpr()->IgnoreImpCasts()))) {
      break;
    }
    P = dyn_cast<clang::ParenExpr>(P->getSubExpr()->IgnoreImpCasts());
  }
  return B;
}

// ---------------------------------------------
bool isTerminal(const clang::Stmt *ST) {
  auto S = dyn_cast<BinaryOperator>(ST);
  auto F = dyn_cast<CallExpr>(ST);
  auto Op = dyn_cast<CXXOperatorCallExpr>(ST);

  return !(S || (F && !Op));
}

// ---------------------------------------------
std::string replaceTmpTac(std::map<std::string, std::vector<std::string>> T,
                          std::string K) {
  if (T.count(K) > 0) {
    std::vector<std::string> ST = T.at(K);
    if (ST[3] == "BinOp") {
      return "(" + replaceTmpTac(T, ST[0]) + " " + ST[2] + " " +
             replaceTmpTac(T, ST[1]) + ")";
    } else {
      return ST[2] + "(" + replaceTmpTac(T, ST[0]) + ", " +
             replaceTmpTac(T, ST[1]) + ")";
    }
  } else {
    return K;
  }
}

// ---------------------------------------------
std::string TAC::renderTacAsStmt(TacListT TL, int Offset) {
  std::vector<std::string> TmpList;
  std::map<std::string, std::vector<std::string>> TmpResToReplace;
  TacListT TCopy(TL);
  for (auto T : TCopy) {
    if ((T.getUnrollFactor() != Offset) && (Offset != -1)) {
      continue;
    }
    auto A = T.getA()->getExprStr();
    auto B = T.getB()->getExprStr();
    std::string C = "";
    auto Op = T.getMVOP().toString();
    auto TOP = T.getMVOP().T == MVOpType::CLANG_BINOP ? "BinOp" : "Func";
    if (T.getC() != nullptr) {
      C = T.getC()->getExprStr();
    }
    if ((T.getA()->getTempInfo() == MVExpr::MVExprInfo::TMP_RES)) {
      TmpResToReplace[A] = {B, C, Op, TOP};
    }
  }
  TCopy.reverse();
  std::string FinalStr = "";
  for (auto T : TCopy) {
    if ((T.getUnrollFactor() != Offset) && (Offset != -1)) {
      continue;
    }
    auto A = T.getA()->getExprStr();
    auto B = T.getB()->getExprStr();
    std::string C = "";
    if (T.getC() != nullptr) {
      C = T.getC()->getExprStr();
    }
    // If this, then this TAC is an assignment (by design)
    if (A == B) {
      FinalStr =
          A + " = " + replaceTmpTac(TmpResToReplace, C) + ";\n" + FinalStr;
    }
  }
  return FinalStr;
}

// ---------------------------------------------
void stmtToTACRecursive(const clang::Stmt *ST, TacListT &TacList, int Val) {
  Expr *S1 = nullptr;
  Expr *S2 = nullptr;
  bool IsTerminalS1 = false;
  bool IsTerminalS2 = false;
  bool IsUnary = false;
  MVOp Op;
  // FIXME: for LLVM < 11 this does not seem to work for +=
  auto S = dyn_cast<BinaryOperator>(ST);
  auto F = dyn_cast<CallExpr>(ST);
  // This is something like Lhs [+-]= Rhs
  if (S) {
    S1 = S->getLHS()->IgnoreImpCasts();
    S2 = S->getRHS()->IgnoreImpCasts();
    clang::BinaryOperator *LhsBin = nullptr;
    clang::BinaryOperator *RhsBin = nullptr;
    Op = MVOp(S->getOpcode());
    if (S->getOpcode() == BinaryOperator::Opcode::BO_AddAssign) {
#if defined(LLVM_VERSION_MAJOR) && (LLVM_VERSION_MAJOR < 11)
      auto NewOp = clang::BinaryOperator(S1, S2, BinaryOperator::Opcode::BO_Add,
                                         S1->getType(), S1->getValueKind(),
                                         S1->getObjectKind(), S1->getBeginLoc(),
                                         clang::FPOptions());
      S2 = &NewOp;
#else
      auto NewOp = clang::BinaryOperator::Create(
          *(Utils::getCtx()), S1, S2, BinaryOperator::Opcode::BO_Add,
          S1->getType(), S1->getValueKind(), S1->getObjectKind(),
          S1->getBeginLoc(), clang::FPOptionsOverride());
      S2 = NewOp;
#endif
      Op = MVOp(BinaryOperator::Opcode::BO_Assign);
    }
    if (S->getOpcode() == BinaryOperator::Opcode::BO_SubAssign) {
#if defined(LLVM_VERSION_MAJOR) && (LLVM_VERSION_MAJOR < 11)
      auto NewOp = clang::BinaryOperator(S1, S2, BinaryOperator::Opcode::BO_Sub,
                                         S1->getType(), S1->getValueKind(),
                                         S1->getObjectKind(), S1->getBeginLoc(),
                                         clang::FPOptions());
      S2 = &NewOp;
#else
      auto NewOp = clang::BinaryOperator::Create(
          *(Utils::getCtx()), S1, S2, BinaryOperator::Opcode::BO_Sub,
          S1->getType(), S1->getValueKind(), S1->getObjectKind(),
          S1->getBeginLoc(), clang::FPOptionsOverride());
      S2 = NewOp;
#endif
      Op = MVOp(BinaryOperator::Opcode::BO_Assign);
    }
    if ((LhsBin = getBinOp(S1))) {
      S1 = LhsBin;
    }
    if ((RhsBin = getBinOp(S2))) {
      S2 = RhsBin;
    }
    IsTerminalS1 = isTerminal(S1);
    IsTerminalS2 = isTerminal(S2);
  } else if (F) {
    auto Args = F->getArgs();
    assert((F->getNumArgs() <= 2) && "Function not binary!! Not supported yet");
    auto MT = dyn_cast<MaterializeTemporaryExpr>(Args[0]);
    if (MT) {
      S1 = const_cast<Expr *>(MT->getSubExpr()->IgnoreImpCasts());
    } else {
      S1 = const_cast<Expr *>(Args[0]);
    }
    IsTerminalS1 = isTerminal(S1->IgnoreImpCasts());
    if (F->getNumArgs() > 1) {
      MT = dyn_cast<MaterializeTemporaryExpr>(Args[1]);
      if (MT) {
        S2 = const_cast<Expr *>(MT->getSubExpr()->IgnoreImpCasts());
      } else {
        S2 = const_cast<Expr *>(Args[1]);
      }
      IsTerminalS2 = isTerminal(S2->IgnoreImpCasts());
    } else {
      IsTerminalS2 = true;
      IsUnary = true;
    }
    Op = MVOp(Utils::getStringFromExpr(F->getCallee()));
  }

  assert(((S1 != nullptr) && ((IsUnary) || (S2 != nullptr))) &&
         "Something went wrong...");

  // FIXME: this part is very sensible and is done for parsing properly
  // classes such as std::vector
  auto T = S1->getType().getAsString();
  if (isa<clang::TypedefType>(S1->getType())) {
    auto aT = dyn_cast<clang::TypedefType>(S1->getType());
    if (!aT->isSugared()) {
      MACVETH_DEBUG("TAC", "Not sugared!!");
      llvm::llvm_unreachable_internal();
    }
    auto ET = dyn_cast<ElaboratedType>(aT->desugar());
    auto TT = dyn_cast<TypedefType>(ET->desugar());
    auto Subs = dyn_cast<SubstTemplateTypeParmType>(TT->desugar());
    T = Subs->desugar().getAsString();
  }

  /// Since this is a recursive function, we have to test the first case
  auto TmpA = (Val == -1) ? MVExprFactory::createMVExpr(S1)
                          : MVExprFactory::createMVExpr(getCurrentNameTempReg(),
                                                        true, T);

  auto TmpB = (!IsTerminalS1) ? MVExprFactory::createMVExpr(
                                    getNameTempReg(), true, TmpA->getExprStr())
                              : MVExprFactory::createMVExpr(S1);

  int Inc = 1;
  if (!IsTerminalS1) {
    stmtToTACRecursive(S1, TacList, Val + Inc);
  }

  MVExpr *TmpC = nullptr;
  if (!IsUnary) {
    TmpC = (!IsTerminalS2) ? MVExprFactory::createMVExpr(getNameTempReg(), true,
                                                         TmpA->getExprStr())
                           : MVExprFactory::createMVExpr(S2);
  }

  if (!IsTerminalS2) {
    stmtToTACRecursive(S2, TacList, Val + Inc + 1);
  }

  TacList.push_back(TAC(TmpA, TmpB, TmpC, Op));
}

// ---------------------------------------------
TacListT TAC::stmtToTAC(clang::Stmt *const ST) {
  TacListT TL;

  auto S = dyn_cast<clang::Expr>(ST);
  if (!S) {
    MVErr("Statement type not allowed");
  }
  // Check if the expression is binary
  if (auto STypeBin = getBinOp(S->IgnoreImpCasts())) {
    stmtToTACRecursive(STypeBin, TL, -1);
    return TL;
  }

  assert(TL.size() > 1 && "This type of statement is not allowed!");
  return TL;
}

// ---------------------------------------------
TAC TAC::unroll(TAC Tac, int UnrollFactor, std::string LoopLevel,
                bool FullUnroll) {
  auto NewA = Tac.getA()->unrollExpr(UnrollFactor, LoopLevel, FullUnroll);
  auto NewB = Tac.getB()->unrollExpr(UnrollFactor, LoopLevel, FullUnroll);
  auto NewC = Tac.getC() != nullptr
                  ? Tac.getC()->unrollExpr(UnrollFactor, LoopLevel, FullUnroll)
                  : nullptr;
  TAC UnrolledTac(NewA, NewB, NewC, Tac.getMVOP(), Tac.getTacID());
  UnrolledTac.TUnroll = UnrollFactor;
  UnrolledTac.setScop(Tac.getScop());
  UnrolledTac.setLoopName(Tac.getLoopName());
  return UnrolledTac;
}

// ---------------------------------------------
TacListT TAC::unrollTacList(const TacListT &TacList, int UnrollFactor,
                            int UpperBound, std::string LoopLevel,
                            bool FullUnroll) {
  TacListT NewTacList;
  auto Steps = UpperBound / UnrollFactor;
  for (int S = 0; S < Steps; S++) {
    std::for_each(
        TacList.begin(), TacList.end(),
        [&NewTacList, S, UnrollFactor, LoopLevel, FullUnroll](auto T) {
          NewTacList.push_back(
              TAC::unroll(T, UnrollFactor * S, LoopLevel, FullUnroll));
        });
    // for (auto T : TacList) {
    //   NewTacList.push_back(
    //       TAC::unroll(T, UnrollFactor * S, LoopLevel, FullUnroll));
    // }
  }
  return NewTacList;
}
