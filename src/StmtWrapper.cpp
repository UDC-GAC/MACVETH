/**
 * File              : StmtWrapper.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 25 Nov 2019 13:48:24 MST
 * Last Modified Date: Lun 23 Mar 2020 18:29:59 CET
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 *
 * Copyright (c) 2019-2020 Marcos Horro <marcos.horro@udc.gal>
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

#include "include/StmtWrapper.h"
#include "include/MVAssert.h"
#include "include/TAC.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include <llvm-10/llvm/Support/ErrorHandling.h>
#include <string>

using namespace clang;
using namespace macveth;

// ---------------------------------------------
long StmtWrapper::getCost() {
  long Tot = 0;
  if (this->isLoop()) {
    for (auto S : this->getListStmt()) {
      Tot += S->getCost();
    }
  } else {
    for (auto T : this->getTacList()) {
      // FIXME: this is a very _naïve_ (i.e. stupid) metric...
      Tot += MVOp::getOperationCost(T.getMVOP());
      Tot += (T.getA()->getKind() == MVExpr::MVK_Array) ? 2 : 0;
      Tot += (T.getB()->getKind() == MVExpr::MVK_Array) ? 2 : 0;
      if (T.getC() != NULL) {
        Tot += (T.getC()->getKind() == MVExpr::MVK_Array) ? 2 : 0;
      }
    }
  }
  return Tot;
}

// ---------------------------------------------
long StmtWrapper::computeSequentialCostStmtWrapper(
    std::list<StmtWrapper *> SL) {
  long TotalCost = 0;
  for (auto S : SL) {
    TotalCost += S->getCost();
  }
  return TotalCost;
}

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
    unsigned int Start =
        Utils::getSourceMgr()->getExpansionLineNumber(ST->getBeginLoc());
    unsigned int End =
        Utils::getSourceMgr()->getExpansionLineNumber(ST->getEndLoc());
    if ((Scop->StartLine <= Start) && (Scop->EndLine >= End)) {
      Utils::printDebug("StmtWrapper genStmtWraps",
                        "new StmtWrapper => " + Utils::getStringFromStmt(ST));
      StmtWrapper *NewStmt = new StmtWrapper(ST);
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
                                "new StmtWrapper => " +
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
  LoopInfo(Loop);
  // Get location of the loop
  Loop.BegLoc = ForLoop->getBeginLoc();
  Loop.EndLoc = ForLoop->getEndLoc();

  // Get init val
  const DeclStmt *NameValInit = dyn_cast<DeclStmt>(ForLoop->getInit());
  const BinaryOperator *ValInit;
  const VarDecl *V = nullptr;
  // FIXME:
  if (NameValInit != nullptr) {
    // In cases like: int i = <expr>; i.e. the declaration of the variable
    // itself
    clang::Expr::EvalResult R;
    V = dyn_cast<VarDecl>(NameValInit->getSingleDecl());
    if (V != nullptr) {
      Loop.InitVal = Utils::getIntFromExpr(V->getInit(), Utils::getCtx());
      Loop.StrInitVal = Utils::getStringFromStmt(V->getInit());
    } else {
      Loop.StrInitVal = Utils::getStringFromStmt(NameValInit);
    }
    Loop.Dim = V->getDeclName().getAsString();
  } else {
    // In case the declaration is sth like: i = <expr>; i.e. i = t, i = 0, etc.
    ValInit = dyn_cast<BinaryOperator>(ForLoop->getInit());
    if (ValInit != nullptr) {
      Loop.InitVal = Utils::getIntFromExpr(ValInit->getRHS(), Utils::getCtx());
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
    Loop.UpperBound = Utils::getIntFromExpr(UpperBoundExpr, Utils::getCtx());
    if (Loop.UpperBound != -1) {
      if (Loop.UpperBoundComp == BinaryOperator::Opcode::BO_LE) {
        Loop.UpperBound++;
      }
      if (Loop.UpperBoundComp == BinaryOperator::Opcode::BO_GE) {
        Loop.UpperBound--;
      }
    }
  }

  // Get step value
  auto IncVarPos = ForLoop->getInc();
  if (dyn_cast<UnaryOperator>(ForLoop->getInc())) {
    Loop.Step = 1;
  } else if (auto CAO = dyn_cast<CompoundAssignOperator>(ForLoop->getInc())) {
    Loop.Step = Utils::getIntFromExpr(CAO->getRHS(), Utils::getCtx());
  }

  // Getting char sourcerange of the increment
  // We always expect increments like [++]var[++]; so this always works
  Loop.SRVarInc = clang::CharSourceRange::getTokenRange(
      IncVarPos->getBeginLoc(), IncVarPos->getEndLoc());
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
  int Tmp = 0;
  if (SWrap->isLeftOver()) {
    return Epilog;
  }
  std::list<StmtWrapper *> SVec = SWrap->ListStmt;
  LoopInfo Loop = SWrap->getLoopInfo();
  // FIXME:
  // Write new epilogs
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
    // Utils::printDebug("StmtWrapper", "unrollByDim loop");
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
        LI.push_front(this->LoopL);
        break;
      } else {
        this->LoopL.UnrollFactor = 1;
        this->LoopL.StepUnrolled = this->LoopL.Step * this->LoopL.UnrollFactor;
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
  auto TL = TAC::unrollTacList(this->getTacList(), L.Step, UB, L.Dim);
  this->setTacList(TL);
  return TL;
}