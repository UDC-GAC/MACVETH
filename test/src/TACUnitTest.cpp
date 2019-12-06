/**
 * File              : TACUnitTest.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 24 Nov 2019 19:54:18 MST
 * Last Modified Date: Xov 05 Dec 2019 17:27:04 MST
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

#include "ASTHelper.h"
#include "include/TAC.h"
#include "clang/AST/Expr.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Tooling/Tooling.h"
#include "gtest/gtest.h"

using namespace clang;
using namespace clang::tooling;
using namespace macveth;

namespace {

class TACTest : public ::testing::Test {
protected:
  void SetUp() override {}

  // void TearDown() override {}
  TAC *T1;
  TAC *T2;
  TAC *T3;
};

/// Default contrstructor
TEST(TestTAC, DefaultConstructor) {
  TAC Tac;
  ASSERT_EQ(Tac, nullptr);
}

/// BinaryOperator2TAC
TEST(TestTAC, BinaryOperator2TAC) {
  std::list<TAC> TacList;
  TAC::binaryOperator2TAC(TestBinOp, &TacList, -1);
}
} // namespace
