/*
 * File					 : include/MVRewriter.h
 * Author				 : Marcos Horro
 * Date					 : Wed 18 Nov 2020 01:06 +01:00
 *
 * Last Modified : Wed 18 Nov 2020 01:06 +01:00
 * Modified By	 : Marcos Horro (marcos.horro@udc.gal>)
 *
 * MIT License
 *
 * Copyright (c) 2020 Colorado State University
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
  Rewriter R;
};
} // namespace macveth

#endif