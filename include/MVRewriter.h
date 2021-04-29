// MACVETH - MVRewriter.h
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

#ifndef MACVETH_REWRITER_H
#define MACVETH_REWRITER_H

#include "include/CDAG.h"
#include "include/MVPragmaHandler.h"
#include "include/StmtWrapper.h"
#include "include/Vectorization/SIMD/SIMDBackEnd.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;

namespace macveth {
class MVRewriter {
public:
  explicit MVRewriter(Rewriter &R) : Rewrite(R) {}

  std::list<std::string> rewriteLoops(std::list<StmtWrapper *> SList,
                                      std::list<std::string> DimAlreadyDecl);
  /// Comment those stmts which are replaced
  void commentReplacedStmts(std::list<StmtWrapper *> SList);
  //// Add includes to files if needed
  void addHeaders(std::list<std::string> S, FileID FID);
  /// Rewrite TAC as regular statements
  void renderTACInPlace(std::list<StmtWrapper *> SL, long TacID, int Offset);
  /// Render SIMD instructions where they should be (regular map operations)
  void renderSIMDInstInPlace(SIMDBackEnd::SIMDInst SI,
                             std::list<StmtWrapper *> SL);
  /// Render SIMD after a statement (for reductions, for instance)
  bool renderSIMDInstAfterPlace(SIMDBackEnd::SIMDInst SI,
                                std::list<StmtWrapper *> SL);

  /// Render SIMD where the original scalar code is
  void renderSIMDInOrder(SIMDBackEnd::SIMDInst SI, std::list<StmtWrapper *> SL);

  /// Render SIMD before a statement (for initializing reductions, for instance)
  bool renderSIMDInstBeforePlace(SIMDBackEnd::SIMDInst SI,
                                 std::list<StmtWrapper *> SL);

private:
  Rewriter &Rewrite;
};
} // namespace macveth

#endif