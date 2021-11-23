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
//     Louis-Noël Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Marcos Horro <marcos.horro@udc.es>

#include "include/MVRewriter.h"
#include "include/Debug.h"

// ---------------------------------------------
std::list<std::string>
MVRewriter::rewriteLoops(StmtWrapperVectorT SList,
                         std::list<std::string> DimAlreadyDecl) {
  std::list<std::string> DimsDeclared = {};
  for (auto SWrap : SList) {
    // Rewrite loop header
    if (SWrap->isLeftOver()) {
      continue;
    }
    auto Loop = SWrap->getLoopInfo();

    // Remove loop if already unrolled and so
    if (Loop.FullyUnrolled) {
      auto Begin = Loop.BegLoc;
      auto End = Loop.SRVarInc.getEnd().getLocWithOffset(3);
      auto SR =
          clang::CharSourceRange::getCharRange(clang::SourceRange(Begin, End));
      Rewrite.RemoveText(SR);
      Rewrite.RemoveText(Loop.LoopStmt->getEndLoc());
      rewriteLoops(SWrap->getStmtVector(), DimAlreadyDecl);
      continue;
    }

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
    if (!Loop.FullyUnrolled)
      rewriteLoops(SWrap->getStmtVector(), DimAlreadyDecl);
  }
  // Declare variables
  SourceLocation Loc = SList.front()->getClangStmt()->getBeginLoc();
  Rewrite.InsertTextBefore(
      Loc, StmtWrapper::LoopInfo::getDimDeclarations(DimsDeclared));
  StmtWrapper::LoopInfo::clearDims();
  return DimsDeclared;
}

// ---------------------------------------------
void MVRewriter::commentReplacedStmts(StmtWrapperVectorT SList) {
  for (auto S : SList) {
    if (S->isLoop()) {
      commentReplacedStmts(S->getStmtVector());
      continue;
    }
    Rewrite.RemoveText(
        clang::SourceRange(S->getClangStmt()->getBeginLoc(),
                           S->getClangStmt()->getEndLoc().getLocWithOffset(1)));
  }
}

// ---------------------------------------------
bool MVRewriter::renderSIMDInstBeforePlace(SIMDBackEnd::SIMDInst SI,
                                           StmtWrapperVectorT SL) {
  for (auto S : SL) {
    if (S->isLoop()) {
      if (renderSIMDInstBeforePlace(SI, S->getStmtVector())) {
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
                                          StmtWrapperVectorT SL) {
  for (auto S : SL) {
    if (S->isLoop()) {
      if (renderSIMDInstAfterPlace(SI, S->getStmtVector())) {
        // MACVETH_DEBUG("MVRewrite",
        //               "LOOP ORDER = " +
        //                   std::to_string(SI.getMVSourceLocation().getOrder()));
        Rewrite.InsertTextAfterToken(S->getClangStmt()->getEndLoc(),
                                     SI.render() + ";\n");
      }
    } else {
      for (auto T : S->getTacList()) {
        if (SI.getMVSourceLocation().getOrder() == (unsigned int)T.getTacID()) {
          if (S->isInLoop()) {
            return true;
          }
          // MACVETH_DEBUG(
          //     "MVRewrite",
          //     "ORDER = " +
          //     std::to_string(SI.getMVSourceLocation().getOrder()));

          Rewrite.InsertTextAfter(
              S->getClangStmt()->getEndLoc().getLocWithOffset(2),
              "\n" + SI.render() + ";");
          // Rewrite.InsertTextAfterToken(
          //     S->getClangStmt()->getEndLoc().getLocWithOffset(2),
          //    SI.render() + ";\n");
          return false;
        }
      }
    }
  }
  return false;
}

// ---------------------------------------------
void MVRewriter::renderSIMDInOrder(SIMDBackEnd::SIMDInst SI,
                                   StmtWrapperVectorT SL) {
  for (auto S : SL) {
    if (S->isLoop()) {
      renderSIMDInstInPlace(SI, S->getStmtVector());
    } else {
      for (auto T : S->getTacList()) {
        if (SI.getMVSourceLocation().getOrder() == (unsigned int)T.getTacID()) {
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
                                       StmtWrapperVectorT SL) {
  // Scalar case: is this needed?
  if (SI.isSequential() && SI.getMVSourceLocation().isInOrder()) {
    for (auto S : SL) {
      for (auto T : S->getTacList()) {
        if (SI.getMVSourceLocation().getOrder() == (unsigned int)T.getTacID()) {
          MACVETH_DEBUG("MVFuncVisitor",
                        "Scalar for " +
                            TAC::renderTacAsStmt(S->getTacList(), 0));
          renderTACInPlace({S}, SI.getMVSourceLocation().getOrder(),
                           SI.getMVSourceLocation().getOffset());
          return;
        }
      }
    }
  }

  // If instruction does not have content, then do nothing
  if (SI.isReduction() || SI.render().empty()) {
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
void MVRewriter::renderTACInPlace(StmtWrapperVectorT SL, long TacID,
                                  int Offset) {
  for (auto S : SL) {
    if (S->isLoop()) {
      renderTACInPlace(S->getStmtVector(), TacID, Offset);
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
void MVRewriter::addHeaders(std::vector<std::string> S, FileID FID) {
  auto SM = Utils::getSourceMgr();
  if (S.size() > 0) {
    Rewrite.InsertText(SM->translateLineCol(FID, 1, 1),
                       "/* BEGIN MACVETH headers*/\n", true, true);
  }
  for (auto I : S) {
    Rewrite.InsertText(SM->translateLineCol(FID, 1, 1), "#include " + I + "\n",
                       true, true);
  }
  if (S.size() > 0) {
    Rewrite.InsertText(SM->translateLineCol(FID, 1, 1), "/* END MACVETH */\n",
                       true, true);
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