// MACVETH - MVRewriter.cpp
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

#include "include/MVRewriter.h"

// ---------------------------------------------
std::list<std::string>
MVRewriter::rewriteLoops(std::list<StmtWrapper *> SList,
                         std::list<std::string> DimAlreadyDecl) {
  std::list<std::string> DimsDeclared = {};
  for (auto SWrap : SList) {
    // Rewrite loop header
    if (SWrap->isLeftOver()) {
      continue;
    }
    auto Loop = SWrap->getLoopInfo();
    Utils::printDebug("MVConsumers", "Rewriting loop = " + Loop.Dim);

    // Rewrite headers
    Rewrite.ReplaceText(Loop.SRVarInc,
                        Loop.Dim + " += " + std::to_string(Loop.StepUnrolled));
    // IMPORTANT: need to put ";" at the end since the range is calculated
    // as the -1 offset of the location of the increment. This is done like
    // this because the SourceLocation of the UpperBound could be a macro
    // variable located in another place. This happens, for instance, with
    // the loop bounds in PolyBench suite
    if (!Loop.FullyUnrolled) {
      auto UpperBound = (Loop.UpperBound != -1)
                            ? std::to_string(Loop.UpperBound)
                            : Loop.StrUpperBound;
      Rewrite.ReplaceText(Loop.SRVarCond,
                          "(" + Loop.Dim + " + " +
                              std::to_string(Loop.StepUnrolled) +
                              ") <= " + UpperBound + ";");
    }
    Rewrite.ReplaceText(Loop.SRVarInit,
                        Loop.Dim + " = " +
                            ((Loop.InitVal != -1) ? std::to_string(Loop.InitVal)
                                                  : Loop.StrInitVal));

    if (Loop.Declared) {
      auto L = StmtWrapper::LoopInfo::DimDeclared;
      if ((!(std::find(L.begin(), L.end(), Loop.Dim) != L.end())) &&
          (!(std::find(DimsDeclared.begin(), DimsDeclared.end(), Loop.Dim) !=
             DimsDeclared.end())) &&
          ((!(std::find(DimAlreadyDecl.begin(), DimAlreadyDecl.end(),
                        Loop.Dim) != DimAlreadyDecl.end())))) {
        StmtWrapper::LoopInfo::DimDeclared.push_back(Loop.Dim);
        DimsDeclared.push_back(Loop.Dim);
      }
    }

    // If it has been fully unrolled no need to write the epilog of it
    if ((!Loop.FullyUnrolled) && (Loop.UpperBound != Loop.StepUnrolled)) {
      auto Epilog = StmtWrapper::LoopInfo::getEpilogs(SWrap);
      Rewrite.InsertTextAfterToken(Loop.EndLoc, Epilog + "\n");
    }
    rewriteLoops(SWrap->getListStmt(), DimAlreadyDecl);
  }
  // Declare variables
  SourceLocation Loc = SList.front()->getClangStmt()->getBeginLoc();
  Rewrite.InsertTextBefore(
      Loc, StmtWrapper::LoopInfo::getDimDeclarations(DimsDeclared));
  StmtWrapper::LoopInfo::clearDims();
  return DimsDeclared;
}

// ---------------------------------------------
void MVRewriter::commentReplacedStmts(std::list<StmtWrapper *> SList) {
  for (auto S : SList) {
    if (S->isLoop()) {
      commentReplacedStmts(S->getListStmt());
      continue;
    }
    Rewrite.RemoveText(
        clang::SourceRange(S->getClangStmt()->getBeginLoc(),
                           S->getClangStmt()->getEndLoc().getLocWithOffset(1)));
  }
}

// ---------------------------------------------
bool MVRewriter::renderSIMDInstBeforePlace(SIMDBackEnd::SIMDInst SI,
                                           std::list<StmtWrapper *> SL) {
  for (auto S : SL) {
    if (S->isLoop()) {
      if (renderSIMDInstBeforePlace(SI, S->getListStmt())) {
        Rewrite.InsertTextBefore(S->getClangStmt()->getBeginLoc(),
                                 SI.render() + ";\n");
      }
    } else {
      for (auto T : S->getTacList()) {
        if (SI.getMVSourceLocation().getOrder() == (unsigned int)T.getTacID()) {
          if (S->isInLoop()) {
            return true;
          }
          Rewrite.InsertTextBefore(S->getClangStmt()->getBeginLoc(),
                                   SI.render() + ";\n");
          return false;
        }
      }
    }
  }
  return false;
}

// ---------------------------------------------
bool MVRewriter::renderSIMDInstAfterPlace(SIMDBackEnd::SIMDInst SI,
                                          std::list<StmtWrapper *> SL) {
  for (auto S : SL) {
    if (S->isLoop()) {
      if (renderSIMDInstAfterPlace(SI, S->getListStmt())) {
        Rewrite.InsertTextAfterToken(S->getClangStmt()->getEndLoc(),
                                     SI.render() + ";\n");
      }
    } else {
      for (auto T : S->getTacList()) {
        if (SI.getMVSourceLocation().getOrder() == (unsigned int)T.getTacID()) {
          if (S->isInLoop()) {
            return true;
          }
          Rewrite.InsertText(S->getClangStmt()->getEndLoc().getLocWithOffset(2),
                             "\n" + SI.render() + ";", true, false);
          return false;
        }
      }
    }
  }
  return false;
}

// ---------------------------------------------
void MVRewriter::renderSIMDInOrder(SIMDBackEnd::SIMDInst SI,
                                   std::list<StmtWrapper *> SL) {
  for (auto S : SL) {
    if (S->isLoop()) {
      renderSIMDInstInPlace(SI, S->getListStmt());
    } else {
      for (auto T : S->getTacList()) {
        if (SI.getMVSourceLocation().getOrder() == (unsigned int)T.getTacID()) {
          // FIXME:
          Rewrite.InsertText(S->getClangStmt()->getBeginLoc(),
                             SI.render() + ";\n");
          return;
        }
      }
    }
  }
}

// ---------------------------------------------
void MVRewriter::renderSIMDInstInPlace(SIMDBackEnd::SIMDInst SI,
                                       std::list<StmtWrapper *> SL) {
  // Scalar case: is this needed?
  if (SI.isSequential() && SI.getMVSourceLocation().isInOrder()) {
    Utils::printDebug("MVFuncVisitor", "Scalar for " + SI.render());
    for (auto S : SL) {
      for (auto T : S->getTacList()) {
        if (SI.getMVSourceLocation().getOrder() == (unsigned int)T.getTacID()) {
          renderTACInPlace({S}, SI.getMVSourceLocation().getOrder(),
                           SI.getMVSourceLocation().getOffset());
          return;
        }
      }
    }
  }

  // If instruction does not have content, then do nothing
  if (SI.isReduction() || SI.render() == "") {
    return;
  }

  switch (SI.getMVSourceLocation().getPosition()) {
  case MVSourceLocation::Position::PREOUTERMOST:
  case MVSourceLocation::Position::PREORDER:
    renderSIMDInstBeforePlace(SI, SL);
    break;
  case MVSourceLocation::Position::INORDER:
    renderSIMDInOrder(SI, SL);
    break;
  case MVSourceLocation::Position::POSORDER:
    renderSIMDInstAfterPlace(SI, SL);
    break;
  case MVSourceLocation::Position::POSOUTERMOST:
  default:
    break;
  }
}

// ---------------------------------------------
void MVRewriter::renderTACInPlace(std::list<StmtWrapper *> SL, long TacID,
                                  int Offset) {
  for (auto S : SL) {
    if (S->isLoop()) {
      renderTACInPlace(S->getListStmt(), TacID, Offset);
      continue;
    }
    if (((TacID == S->getTacList().back().getTacID()) &&
         (Offset == S->getTacList().back().getUnrollFactor())) ||
        (TacID == -1)) {
      S->setNotVectorized();
      Rewrite.InsertText(S->getClangStmt()->getBeginLoc(),
                         TAC::renderTacAsStmt(S->getTacList(), Offset), true,
                         true);
    }
  }
}

// ---------------------------------------------
void MVRewriter::addHeaders(std::list<std::string> S, FileID FID) {
  auto SM = Utils::getSourceMgr();
  if (S.size() > 0) {
    Rewrite.InsertText(SM->translateLineCol(FID, 1, 1),
                       "/* begin INFO MACVETH: headers added */\n", true, true);
  }
  for (auto I : S) {
    Rewrite.InsertText(SM->translateLineCol(FID, 1, 1),
                       "#include <" + I + ">\n", true, true);
  }
  if (S.size() > 0) {
    Rewrite.InsertText(SM->translateLineCol(FID, 1, 1),
                       "/* end INFO MACVETH */\n", true, true);
  }
}

// ---------------------------------------------
void clearAllMappings() {
  // Be clean lol
  // TAC
  TAC::clear();
  // VectorIR
  VectorIR::clear();
  // SIMDBackEnd
  SIMDBackEnd::clearMappings();
}