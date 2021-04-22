// MACVETH - StmtWrapper.cpp
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

#include "include/StmtWrapper.h"
#include "include/MVAssert.h"
#include "include/TAC.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include "llvm/Support/ErrorHandling.h"
#include <string>

using namespace clang;
using namespace macveth;

// ---------------------------------------------
std::list<StmtWrapper *> StmtWrapper::genStmtWraps(CompoundStmt *CS,
                                                   ScopLoc *Scop) {
  std::list<StmtWrapper *> SList;

  /// Get loop information
  ScopHandler::visitScop(*Scop);
  for (auto StmtWithin : CS->body()) {
    auto ST = dyn_cast<Stmt>(StmtWithin);
    if (!ST) {
      continue;
    }
    unsigned Start =
        Utils::getSourceMgr()->getExpansionLineNumber(ST->getBeginLoc());
    unsigned End =
        Utils::getSourceMgr()->getExpansionLineNumber(ST->getEndLoc());
    if ((Scop->StartLine <= Start) && (Scop->EndLine >= End)) {
      Utils::printDebug("StmtWrapper genStmtWraps",
                        "new StmtWrapper =\n" + Utils::getStringFromStmt(ST));
      auto NewStmt = new StmtWrapper(ST);
      SList.push_back(NewStmt);
    } else {
      // Could be inside the loop the region of interest
      auto FL = dyn_cast<ForStmt>(ST);
      if (FL) {
        auto B = dyn_cast<CompoundStmt>(FL->getBody());
        if (B) {
          SList.splice(SList.end(), StmtWrapper::genStmtWraps(B, Scop));
        } else {
          auto B = dyn_cast<Stmt>(FL->getBody());
          if (B) {
            unsigned int Start =
                Utils::getSourceMgr()->getExpansionLineNumber(B->getBeginLoc());
            unsigned int End =
                Utils::getSourceMgr()->getExpansionLineNumber(B->getEndLoc());
            if ((Scop->StartLine <= Start) && (Scop->EndLine >= End)) {
              Utils::printDebug("StmtWrapper genStmtWraps",
                                "new StmtWrapper =\n" +
                                    Utils::getStringFromStmt(B));
              StmtWrapper *NewStmt = new StmtWrapper(B);
              SList.push_back(NewStmt);
            }
          }
        }
      }
    }
  }

  return SList;
}

// ---------------------------------------------
StmtWrapper::StmtWrapper(clang::Stmt *S) {
  this->ClangStmt = S;
  if (auto Loop = dyn_cast<ForStmt>(S)) {
    this->LoopL = getLoop(Loop);
    auto Body = dyn_cast<CompoundStmt>(Loop->getBody());
    if (Body) {
      TAC::TacScop++;
      for (auto S : Body->body()) {
        auto SW = new StmtWrapper(S);
        SW->setInnerLoopName(this->getLoopInfo().Dim);
        this->ListStmt.push_back(SW);
        Utils::printDebug("StmtWrapper", "adding new stmt, TACs in loop = " +
                                             this->getLoopInfo().Dim);
        for (auto T : SW->getTacList()) {
          T.printTAC();
        }
      }
      TAC::TacScop++;
    } else {
      auto SW = new StmtWrapper(Loop->getBody());
      SW->setInnerLoopName(this->getLoopInfo().Dim);
      this->ListStmt.push_back(SW);
      Utils::printDebug("StmtWrapper", "adding new stmt, TACs in loop = " +
                                           this->getLoopInfo().Dim);
      for (auto T : SW->getTacList()) {
        T.printTAC();
      }
    }
  } else {
    this->setTacList(TAC::stmtToTAC(S));
  }
}

// ---------------------------------------------
StmtWrapper::LoopInfo StmtWrapper::getLoop(clang::ForStmt *ForLoop) {
  LoopInfo Loop;
  // Get location of the loop
  Loop.BegLoc = ForLoop->getBeginLoc();
  Loop.EndLoc = ForLoop->getEndLoc();

  // Get init val
  const DeclStmt *NameValInit = dyn_cast<DeclStmt>(ForLoop->getInit());
  const BinaryOperator *ValInit;
  const VarDecl *V = nullptr;
  if (NameValInit != nullptr) {
    // In cases like: int i = <expr>; i.e. the declaration of the variable
    // itself
    clang::Expr::EvalResult R;
    V = dyn_cast<VarDecl>(NameValInit->getSingleDecl());
    if (V != nullptr) {
      Loop.InitVal = Utils::getIntFromExpr(V->getInit());
      Loop.StrInitVal = Utils::getStringFromStmt(V->getInit());
    } else {
      Loop.StrInitVal = Utils::getStringFromStmt(NameValInit);
    }
    Loop.Dim = V->getDeclName().getAsString();
  } else {
    // In case the declaration is sth like: i = <expr>; i.e. i = t, i = 0, etc.
    ValInit = dyn_cast<BinaryOperator>(ForLoop->getInit());
    if (ValInit != nullptr) {
      Loop.InitVal = Utils::getIntFromExpr(ValInit->getRHS());
      if (Loop.InitVal == -1) {
        Loop.StrInitVal = Utils::getStringFromStmt(ValInit->getRHS());
      }
    } else {
      Loop.StrInitVal = Utils::getStringFromStmt(ForLoop->getInit());
    }
  }

  // Get name of variable
  const DeclRefExpr *VN = nullptr;
  Loop.Declared = (V != nullptr);
  if (!Loop.Declared) {
    VN = dyn_cast<DeclRefExpr>(ValInit->getLHS());
    Loop.Dim = Utils::getStringFromExpr(VN);
  }

  // Get UpperBound
  const BinaryOperator *Cond = dyn_cast<BinaryOperator>(ForLoop->getCond());
  Loop.UpperBoundComp = Cond->getOpcode();
  const Expr *UpperBoundExpr = Cond->getRHS();
  if (UpperBoundExpr != nullptr) {
    Loop.StrUpperBound = Utils::getStringFromExpr(UpperBoundExpr);
    Loop.UpperBound = Utils::getIntFromExpr(UpperBoundExpr);
    if (Loop.UpperBound != -1) {
      if (Loop.UpperBoundComp == BinaryOperator::Opcode::BO_LE) {
        Loop.UpperBound++;
      }
      if (Loop.UpperBoundComp == BinaryOperator::Opcode::BO_GE) {
        Loop.UpperBound--;
      }
    }
  }
  assert(UpperBoundExpr != nullptr && "UpperBoundExpr can not be null");

  // Locations in Clang are quite messy and tedious in my opinion...
  auto IncVarPos = ForLoop->getInc();
  // Getting char sourcerange of the increment
  // We always expect increments like [++]var[++]; so this always works. Even
  // though you need an offset, depending whether it is prefix or postfix
  Loop.SRVarInc =
      clang::CharSourceRange::getCharRange(IncVarPos->getSourceRange());
  // Loop.SRVarInc.setEnd(Loop.SRVarInc.getEnd().getLocWithOffset(1));
  if (auto IncExpr = dyn_cast<UnaryOperator>(ForLoop->getInc())) {
    Loop.Step = 1;
    auto V = Utils::getStringFromStmt(IncExpr->getSubExpr());
    if (IncExpr->isPostfix()) {
      Loop.SRVarInc.setEnd(Loop.SRVarInc.getEnd().getLocWithOffset(1));
    }
    if (IncExpr->isPrefix()) {
      Loop.SRVarInc.setEnd(Loop.SRVarInc.getEnd().getLocWithOffset(V.length()));
    }
  } else if (auto CAO = dyn_cast<CompoundAssignOperator>(ForLoop->getInc())) {
    Loop.Step = Utils::getIntFromExpr(CAO->getRHS());
    if (CAO->getRHS()->getBeginLoc().isMacroID()) {
      Loop.SRVarInc = clang::CharSourceRange::getTokenRange(
          IncVarPos->getBeginLoc(),
          Utils::getSourceMgr()
              ->getImmediateExpansionRange(CAO->getRHS()->getEndLoc())
              .getEnd());

    } else {
      Loop.SRVarInc = clang::CharSourceRange::getTokenRange(
          IncVarPos->getBeginLoc(), CAO->getRHS()->getEndLoc());
    }
  } else {
    MVErr("Step for dimension " + Loop.Dim + " is not admitted. Quitting...");
  }

  // IMPORTANT: why this is calculated as this?
  // Because the SourceLocation of the upperbound may be in another place; for
  // instance if the preprocessor makes a substitution of a macro defined. For
  // the same reason, when replacing this range, we also need to add ";" as
  // we are also substituting it... Honestly, I do not like that much this
  // workaround but tests are passing; seems robust.
  Loop.SRVarCond = clang::CharSourceRange::getTokenRange(
      Cond->getBeginLoc(), Loop.SRVarInc.getBegin().getLocWithOffset(-1));
  // This should always work
  if (Loop.Declared) {
    Loop.SRVarInit =
        clang::CharSourceRange::getTokenRange(V->getBeginLoc(), V->getEndLoc());
  } else {
    Loop.SRVarInit = clang::CharSourceRange::getTokenRange(
        VN->getBeginLoc(), ValInit->getEndLoc());
  }

  // Get step or stride and leftover
  if (Loop.UpperBound != -1) {
    // Loop.Step = Loop.UpperBound;
    Loop.LeftOver = 0;
  }

  Loop.StepUnrolled =
      (Loop.UpperBound == -1)
          ? Loop.StepUnrolled
          : (Loop.UpperBound - ((Loop.InitVal == -1) ? 0 : Loop.InitVal)) -
                ((Loop.UpperBound - ((Loop.InitVal == -1) ? 0 : Loop.InitVal)) %
                 Loop.UnrollFactor);

  Utils::printDebug("LoopInfo", Loop.toString());
  return Loop;
}

// ---------------------------------------------
std::string
StmtWrapper::LoopInfo::getDimDeclarations(std::list<std::string> DimsDeclared) {
  if (DimsDeclared.size() != 0) {
    std::string S = "int ";
    int Aux = 0;
    for (auto D : DimsDeclared) {
      S += ((Aux++ == 0) ? "" : ",") + D;
    }
    return S + ";\n";
  }
  return "";
}

// ---------------------------------------------
std::string StmtWrapper::LoopInfo::getEpilogs(StmtWrapper *SWrap) {
  std::string Epilog = "";
  if (SWrap->isLeftOver()) {
    return Epilog;
  }
  std::list<StmtWrapper *> SVec = SWrap->ListStmt;
  LoopInfo Loop = SWrap->getLoopInfo();
  // FIXME:
  // Write new epilogs
  // int Tmp = 0;
  // auto EpiInit = Loop.Dim + " = " +
  //                ((Tmp++ == 0) ? "(" + Loop.StrUpperBound + " / " +
  //                                    std::to_string(Loop.StepUnrolled) +
  //                                    " ) * " +
  //                                    std::to_string(Loop.StepUnrolled)
  //                              : "0");
  // We have to assume that the loop variable has not been altered
  auto EpiInit = Loop.Dim + " = " + Loop.Dim;
  auto EpiCond = Loop.Dim + " < " + Loop.StrUpperBound;
  auto EpiInc = Loop.Dim + " += " + std::to_string(Loop.Step);
  Epilog += "\nfor (" + EpiInit + "; " + EpiCond + "; " + EpiInc + ") {";
  for (auto S : SVec) {
    Epilog += "\n" + Utils::getStringFromStmt(S->getClangStmt()) + ";";
  }
  Epilog += "\n}";

  return Epilog;
}

// ---------------------------------------------
bool StmtWrapper::unrollAndJam(std::list<LoopInfo> LI, ScopLoc *Scop) {
  bool FullUnroll = true;
  if (this->isLoop()) {
    Utils::printDebug("StmtWrapper", "unrollAndJam loop = " + this->LoopL.Dim);
    for (auto D : Scop->PA.UnrollDim) {
      if (this->LoopL.Dim == std::get<0>(D)) {
        if (Scop->PA.FullUnroll[this->LoopL.Dim]) {
          MVAssertSkip(this->LoopL.knownBounds(),
                       "Can not full unroll if upperbound is not known",
                       GotoStartScop);
          this->LoopL.UnrollFactor = this->LoopL.UpperBound;
          this->LoopL.FullyUnrolled = true;
        } else {
          this->LoopL.UnrollFactor = std::get<1>(D);
        }
        this->LoopL.StepUnrolled = this->LoopL.Step * this->LoopL.UnrollFactor;
        break;
      }
    }
    LI.push_front(this->LoopL);
    for (auto S : this->ListStmt) {
      S->unrollAndJam(LI, Scop);
      auto TL = this->getTacList();
      TL.splice(TL.end(), S->getTacList());
      this->setTacList(TL);
    }
  } else {
    for (auto L : LI) {
      FullUnroll = (L.knownBounds()) && FullUnroll;
      this->unroll(L);
    }
  }
  return FullUnroll;
}

// ---------------------------------------------
bool StmtWrapper::unrollByDim(std::list<LoopInfo> LI, ScopLoc *Scop) {
  bool FullUnroll = true;
  if (this->isLoop()) {
    for (auto D : Scop->PA.UnrollDim) {
      if (this->LoopL.Dim == std::get<0>(D)) {
        if (Scop->PA.FullUnroll[this->LoopL.Dim]) {
          MVAssertSkip(this->LoopL.knownBounds(),
                       "Can not full unroll if upperbound is not known",
                       GotoStartScop);
          this->LoopL.UnrollFactor = this->LoopL.UpperBound;
          this->LoopL.FullyUnrolled = true;
        } else {
          this->LoopL.UnrollFactor = std::get<1>(D);
        }
        this->LoopL.StepUnrolled = this->LoopL.Step * this->LoopL.UnrollFactor;
        if ((this->LoopL.knownBounds()) &&
            (this->LoopL.StepUnrolled >= this->LoopL.UpperBound)) {
          this->LoopL.StepUnrolled = this->LoopL.UnrollFactor;
          this->LoopL.FullyUnrolled = true;
        }
        LI.push_front(this->LoopL);
        break;
      } else {
        this->LoopL.UnrollFactor = 1;
        this->LoopL.StepUnrolled = this->LoopL.Step;
      }
    }
    for (auto S : this->ListStmt) {
      S->unrollByDim(LI, Scop);
      auto TL = this->getTacList();
      TL.splice(TL.end(), S->getTacList());
      this->setTacList(TL);
    }
  } else {
    for (auto L : LI) {
      FullUnroll = (L.knownBounds()) && FullUnroll;
      this->unroll(L);
    }
  }
  return FullUnroll;
}

// ---------------------------------------------
TacListType StmtWrapper::unroll(LoopInfo L) {
  Utils::printDebug("StmtWrapper",
                    "unrolling dimension " + L.Dim + " stmt = " +
                        Utils::getStringFromStmt(this->getClangStmt()) +
                        "; Step = " + std::to_string(L.Step) +
                        "; StepUnrolled = " + std::to_string(L.StepUnrolled));
  long UB = ((L.UpperBound != -1) && (L.FullyUnrolled))
                ? (L.UpperBound - L.InitVal)
                : L.StepUnrolled;
  // FIXME:
  if (UB > L.UpperBound) {
    this->LoopL.FullyUnrolled = true;
    UB = L.UpperBound;
  }
  bool FullUnroll = UB == L.UpperBound;
  auto TL =
      TAC::unrollTacList(this->getTacList(), L.Step, UB, L.Dim, FullUnroll);
  this->setTacList(TL);
  return TL;
}