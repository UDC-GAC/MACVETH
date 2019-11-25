/**
 * File              : TAC_unittest.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 24 Nov 2019 19:54:18 MST
 * Last Modified Date: Dom 24 Nov 2019 20:34:38 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#include "include/TAC.h"

#include "MatchVerifier.h"
#include "clang/AST/Expr.h"
#include "gtest/gtest.h"

using namespace clang;
using namespace macveth;

namespace {
std::unique_ptr<ASTUnit> TestBinOp = tooling::buildASTFromCode(
    R"(
    double a = b + c;
    )");

TEST(TAC, DefaultConstructor) {
    TAC Tac;
    ASSERT_EQ(Tac, nullptr);
}

TEST(TAC, binaryOperator2TAC) {
    std::list<TAC> TacList;
    binaryOperator2TAC(TestBinOp, &TacList, -1);
}
}  // namespace
