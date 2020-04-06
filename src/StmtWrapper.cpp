/**
 * File              : StmtWrapper.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 25 Nov 2019 13:48:24 MST
 * Last Modified Date: Lun 23 Mar 2020 18:29:59 CET
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

#include "include/StmtWrapper.h"
#include "include/TAC.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include <llvm-10/llvm/Support/ErrorHandling.h>
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
    unsigned int Start =
        Utils::getSourceMgr()->getExpansionLineNumber(ST->getBeginLoc());
    unsigned int End =
        Utils::getSourceMgr()->getExpansionLineNumber(ST->getEndLoc());
    if ((Scop->StartLine <= Start) && (Scop->EndLine >= End)) {
      Utils::printDebug("StmtWrapper genStmtWraps",
                        "new StmtWrapper => " + Utils::getStringFromStmt(ST));
      StmtWrapper *NewStmt = new StmtWrapper(ST);
      SList.push_back(NewStmt);
    }
  }

  return SList;
}

// ---------------------------------------------
StmtWrapper::StmtWrapper(clang::Stmt *S) {
  this->ClangStmt = S;
  if (auto Loop = dyn_cast<ForStmt>(S)) {
    this->LoopL = getLoop(Loop);
    CompoundStmt *Body = dyn_cast<CompoundStmt>(Loop->getBody());
    if (Body) {
      for (auto S : Body->body()) {
        this->ListStmt.push_back(new StmtWrapper(S));
      }
    } else {
      llvm::llvm_unreachable_internal();
    }
  } else {
    this->setTacList(TAC::stmtToTAC(S));
    Utils::printDebug("StmtWrapper", "adding new stmt, TACs: ");
    for (auto T : this->getTacList()) {
      T.printTAC();
    }
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
  if (NameValInit != nullptr) {
    clang::Expr::EvalResult R;
    V = dyn_cast<VarDecl>(NameValInit->getSingleDecl());
    if (V->getInit()->EvaluateAsInt(R, *Utils::getCtx())) {
      Loop.InitVal = (long)R.Val.getInt().getExtValue();
    }
    Loop.Dim = V->getDeclName().getAsString();
  } else {
    ValInit = dyn_cast<BinaryOperator>(ForLoop->getInit());
    if (ValInit != nullptr) {
      Loop.InitVal = Utils::getIntFromExpr(ValInit->getRHS(), Utils::getCtx());
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
  const Expr *UpperBoundExpr = Cond->getRHS();
  if (UpperBoundExpr != nullptr) {
    Loop.StrUpperBound = Utils::getStringFromExpr(UpperBoundExpr);
    Loop.UpperBound = Utils::getIntFromExpr(UpperBoundExpr, Utils::getCtx());
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
  // Write new epilogs
  std::string EpiInit = Loop.Dim + " = " +
                        ((Tmp++ == 0) ? "(" + Loop.StrUpperBound + " / " +
                                            std::to_string(Loop.Step) +
                                            " ) * " + std::to_string(Loop.Step)
                                      : "0");
  std::string EpiCond = Loop.Dim + " < " + Loop.StrUpperBound;
  std::string EpiInc = Loop.Dim + " += " + std::to_string(Loop.Step);
  Epilog += "\nfor (" + EpiInit + "; " + EpiCond + "; " + EpiInc + ") {";
  for (auto S : SVec) {
    Epilog += "\n" + Utils::getStringFromStmt(S->getClangStmt()) + ";";
  }
  Epilog += "\n}";

  return Epilog;
}

// ---------------------------------------------
bool StmtWrapper::unrollAndJam(std::list<LoopInfo> LI) {
  bool FullUnroll = true;
  if (this->isLoop()) {
    Utils::printDebug("StmtWrapper", "unrollAndJam loop");
    LI.push_front(this->LoopL);
    for (auto S : this->ListStmt) {
      S->unrollAndJam(LI);
      TacListType TL = this->getTacList();
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
                        Utils::getStringFromStmt(this->getClangStmt()));
  long UB = (L.UpperBound == -1) ? L.UnrollFactor : (L.UpperBound - L.InitVal);
  auto TL = TAC::unrollTacList(this->getTacList(), L.Step, UB, L.Dim);
  this->setTacList(TL);
  return TL;
}
