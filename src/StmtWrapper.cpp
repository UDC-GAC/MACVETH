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
//     Louis-Noël Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Marcos Horro <marcos.horro@udc.es>

#include "include/StmtWrapper.h"
#include "include/Debug.h"
#include "include/MVAssert.h"
#include "include/TAC.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include "llvm/Support/ErrorHandling.h"
#include <string>

using namespace clang;
using namespace macveth;

// ---------------------------------------------
StmtWrapperVectorT StmtWrapper::genStmtWraps(CompoundStmt *CS, ScopLoc *Scop) {
  StmtWrapperVectorT SList;

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
      MACVETH_DEBUG("StmtWrapper genStmtWraps",
                    "new StmtWrapper =\n" + Utils::getStringFromStmt(ST));
      auto NewStmt = new StmtWrapper(ST);
      SList.push_back(NewStmt);
    } else {
      // Could be inside the loop the region of interest
      auto ForLoop = dyn_cast<ForStmt>(ST);
      if (ForLoop) {
        auto Body = dyn_cast<CompoundStmt>(ForLoop->getBody());
        if (Body) {
          auto NewVector = StmtWrapper::genStmtWraps(Body, Scop);
          // SList.splice(SList.end(), );
          SList.insert(SList.end(), NewVector.begin(), NewVector.end());
        } else {
          auto Body = dyn_cast<Stmt>(ForLoop->getBody());
          if (Body) {
            unsigned int Start =
                Utils::getSourceMgr()->getExpansionLineNumber(Body->getBeginLoc());
            unsigned int End =
                Utils::getSourceMgr()->getExpansionLineNumber(Body->getEndLoc());
            if ((Scop->StartLine <= Start) && (Scop->EndLine >= End)) {
              MACVETH_DEBUG("StmtWrapper genStmtWraps",
                            "new StmtWrapper =\n" +
                                Utils::getStringFromStmt(Body));
              StmtWrapper *NewStmt = new StmtWrapper(Body);
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
    this->LoopInfoStmt = getLoop(Loop);
    auto Body = dyn_cast<CompoundStmt>(Loop->getBody());
    bool IsInner = true;
    if (Body) {
      TAC::TacScop++;
      for (auto S : Body->body()) {
        auto SW = new StmtWrapper(S);
        IsInner &= !SW->isLoop();
        SW->setInnerLoopInfo(this->getLoopInfo());
        this->ListStmt.push_back(SW);
        MACVETH_DEBUG("StmtWrapper", "adding new stmt, TACs in loop = " +
                                         this->getLoopInfo().getDim());
        for (auto T : SW->getTacList()) {
          MACVETH_DEBUG("TAC", T.toString());
        }
      }
      TAC::TacScop++;
    } else {
      auto SW = new StmtWrapper(Loop->getBody());
      IsInner &= !SW->isLoop();
      SW->setInnerLoopInfo(this->getLoopInfo());
      this->ListStmt.push_back(SW);
      MACVETH_DEBUG("StmtWrapper", "adding new stmt, TACs in loop = " +
                                       this->getLoopInfo().getDim());
      for (auto T : SW->getTacList()) {
        MACVETH_DEBUG("TAC", T.toString());
      }
    }
    this->LoopInfoStmt.InnerLoop = IsInner;
    MACVETH_DEBUG("StmtWrapper", "Updated loop information: " + this->getLoopInfo().toString());
  } else {
    this->setTacList(TAC::stmtToTAC(S));
  }
}

// ---------------------------------------------
StmtWrapper::LoopInfo StmtWrapper::getLoop(clang::ForStmt *ForLoop) {
  LoopInfo Loop;
  // Get location of the loop
  Loop.LoopStmt = ForLoop;
  Loop.BegLoc = ForLoop->getBeginLoc();
  Loop.EndLoc = ForLoop->getEndLoc();

  // Get init val
  auto NameValInit = dyn_cast<DeclStmt>(ForLoop->getInit());
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
  auto Cond = dyn_cast<BinaryOperator>(ForLoop->getCond());
  Loop.UpperBoundComp = Cond->getOpcode();
  const Expr *UpperBoundExpr = Cond->getRHS();
  if (UpperBoundExpr != nullptr) {
    Loop.UpperBound = Utils::getIntFromExpr(UpperBoundExpr);
    if (Loop.UpperBound != -1) {
      if (Loop.UpperBoundComp == BinaryOperator::Opcode::BO_LE) {
        Loop.UpperBound++;
      }
      // if (Loop.UpperBoundComp == BinaryOperator::Opcode::BO_LT) {
      //   Loop.UpperBound--;
      // }
      Loop.StrUpperBound = std::to_string(Loop.UpperBound);
    } else {
      Loop.StrUpperBound = Utils::getStringFromExpr(UpperBoundExpr);
      if (Loop.UpperBoundComp == BinaryOperator::Opcode::BO_LE) {
        Loop.StrUpperBound += " + 1";
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
      Loop.SRVarInc.setEnd(Loop.SRVarInc.getEnd().getLocWithOffset(2));
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

  // My eyes are  ...
  Loop.StepUnrolled =
      (Loop.UpperBound == -1)
          ? Loop.StepUnrolled
          : (Loop.UpperBound - ((Loop.InitVal == -1) ? 0 : Loop.InitVal)) -
                ((Loop.UpperBound - ((Loop.InitVal == -1) ? 0 : Loop.InitVal)) %
                 Loop.UnrollFactor);

  MACVETH_DEBUG("LoopInfo", Loop.toString());
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
  if (SWrap->isLeftOver()) {
    return "";
  }
  LoopInfo Loop = SWrap->getLoopInfo();
  // We have to assume that the loop variable has not been altered
  auto EpiInit = Loop.Dim + " = " + Loop.Dim;
  auto EpiCond = Loop.Dim + " < " + Loop.StrUpperBound;
  auto EpiInc = Loop.Dim + " += " + std::to_string(Loop.Step);
  std::string Epilog =
      "\nfor (" + EpiInit + "; " + EpiCond + "; " + EpiInc + ") {";
  for (auto S : SWrap->getStmtVector()) {
    Epilog += "\n" + Utils::getStringFromStmt(S->getClangStmt()) + ";";
  }
  Epilog += "\n}";

  return Epilog;
}

// ---------------------------------------------
bool StmtWrapper::unrollLoopList(std::list<LoopInfo> LI) {
  bool FullUnroll = false;
  for (auto L : LI) {
    FullUnroll = (L.knownBounds()) && FullUnroll;
    this->unroll(L);
  }
  return FullUnroll;
}

// ---------------------------------------------
bool StmtWrapper::unrollAndJam(std::list<LoopInfo> LI, ScopLoc *Scop) {
  bool FullUnroll = true;
  if (this->isLoop()) {
    MACVETH_DEBUG("StmtWrapper",
                  "unrollAndJam loop = " + this->LoopInfoStmt.Dim);
    for (auto D : Scop->PA.UnrollDim) {
      auto ScopDim = std::get<0>(D);
      if ((this->getLoopInfo().isInnerLoop() && ScopDim == "inner") ||
          (this->LoopInfoStmt.Dim == ScopDim)) {
        auto KeyPA = ScopDim == "inner" ? ScopDim : this->LoopInfoStmt.Dim;
        if (Scop->PA.FullUnroll[KeyPA]) {
          MVAssertSkip(this->LoopInfoStmt.knownBounds(),
                       "Can not full unroll if upperbound is not known",
                       GotoStartScop);
          this->LoopInfoStmt.UnrollFactor = this->LoopInfoStmt.UpperBound;
          this->LoopInfoStmt.FullyUnrolled = true;
        } else {
          this->LoopInfoStmt.UnrollFactor = std::get<1>(D);
        }
        this->LoopInfoStmt.StepUnrolled =
            this->LoopInfoStmt.Step * this->LoopInfoStmt.UnrollFactor;
        break;
      }
    }
    LI.push_front(this->LoopInfoStmt);
    for (auto S : this->ListStmt) {
      S->unrollAndJam(LI, Scop);
      auto TL = this->getTacList();
      TL.splice(TL.end(), S->getTacList());
      this->setTacList(TL);
    }
  } else {
    FullUnroll = unrollLoopList(LI);
  }
  return FullUnroll;
}

// ---------------------------------------------
bool StmtWrapper::unrollByDim(std::list<LoopInfo> LI, ScopLoc *Scop) {
  bool FullUnroll = true;
  if (this->isLoop()) {
    for (auto D : Scop->PA.UnrollDim) {
      auto ScopDim = std::get<0>(D);
      bool InnerUnroll = (this->getLoopInfo().isInnerLoop() && ScopDim == "inner");
      MACVETH_DEBUG("StmtWrapper", "Dimension = " + this->LoopInfoStmt.Dim + "; Scop = " + ScopDim);
      if (InnerUnroll)
        MACVETH_DEBUG("StmtWrapper", "Unrolling by inner keyword dim = " + this->LoopInfoStmt.Dim);
      if (InnerUnroll || (this->LoopInfoStmt.Dim == ScopDim)) {
        auto KeyPA = ScopDim == "inner" ? ScopDim : this->LoopInfoStmt.Dim;
        if (Scop->PA.FullUnroll[KeyPA]) {
          MVAssertSkip(this->LoopInfoStmt.knownBounds(),
                       "Can not full unroll if upperbound is not known",
                       GotoStartScop);
          this->LoopInfoStmt.UnrollFactor = this->LoopInfoStmt.UpperBound;
          this->LoopInfoStmt.FullyUnrolled = true;
        } else {
          this->LoopInfoStmt.UnrollFactor = std::get<1>(D);
        }
        this->LoopInfoStmt.StepUnrolled =
            this->LoopInfoStmt.Step * this->LoopInfoStmt.UnrollFactor;
        if ((this->LoopInfoStmt.knownBounds()) &&
            (this->LoopInfoStmt.StepUnrolled >=
             this->LoopInfoStmt.UpperBound)) {
          this->LoopInfoStmt.StepUnrolled = this->LoopInfoStmt.UnrollFactor;
          this->LoopInfoStmt.FullyUnrolled = true;
        }
        LI.push_front(this->LoopInfoStmt);
        break;
      } else {
        this->LoopInfoStmt.UnrollFactor = 1;
        this->LoopInfoStmt.StepUnrolled = this->LoopInfoStmt.Step;
      }
    }
    for (auto S : this->ListStmt) {
      S->unrollByDim(LI, Scop);
      auto TL = this->getTacList();
      TL.splice(TL.end(), S->getTacList());
      this->setTacList(TL);
    }
  } else {
    FullUnroll = unrollLoopList(LI);
  }
  return FullUnroll;
}

// ---------------------------------------------
TacListT StmtWrapper::unroll(LoopInfo L) {
  MACVETH_DEBUG("StmtWrapper",
                "unrolling Dim " + L.Dim + "; Stmt = " +
                    Utils::getStringFromStmt(this->getClangStmt()) +
                    "; Step = " + std::to_string(L.Step) +
                    "; StepUnrolled = " + std::to_string(L.StepUnrolled));
  long UB = ((L.UpperBound != -1) && (L.FullyUnrolled))
                ? (L.UpperBound - L.InitVal)
                : L.StepUnrolled;
  if (UB > L.UpperBound) {
    this->LoopInfoStmt.FullyUnrolled = true;
    UB = L.UpperBound;
  }
  if (UB == -1) {
    UB = L.StepUnrolled;
  }
  bool FullUnroll = (UB == L.UpperBound);
  auto TL =
      TAC::unrollTacList(this->getTacList(), L.Step, UB, L.Dim, FullUnroll);
  this->setTacList(TL);
  return TL;
}