/**
 * File              : CustomMatchers.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 15 Nov 2019 09:23:38 MST
 * Last Modified Date: Sáb 23 Nov 2019 09:57:41 MST
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
#include "include/CustomMatchers.h"

#include <iostream>
#include <string>

using namespace macveth;

StatementMatcher matchers_utils::assignArrayBinOp(std::string Name,
                                                  std::string Lhs,
                                                  std::string Rhs) {
    return binaryOperator(hasOperatorName("="),
                          hasLHS(anyOf(declRefExpr().bind(Lhs),
                                       arraySubscriptExpr().bind(Lhs))),
                          hasRHS(binaryOperator().bind(Rhs)))
        .bind(Name);
}

StatementMatcher matchers_utils::forLoopMatcher(std::string Name,
                                                StatementMatcher InnerStmt) {
    return forStmt(
               hasLoopInit(declStmt(hasSingleDecl(
                   varDecl(hasInitializer(integerLiteral(equals(0))))
                       .bind(matchers_utils::varnames::NameVarInit + Name)))),
               hasIncrement(
                   unaryOperator(
                       hasOperatorName("++"),
                       hasUnaryOperand(
                           declRefExpr(to(varDecl(hasType(isInteger()))))
                               .bind(matchers_utils::varnames::NameVarInc +
                                     Name)))
                       .bind(matchers_utils::varnames::NameVarIncPos + Name)),
               hasCondition(binaryOperator(
                   hasOperatorName("<"),
                   hasLHS(ignoringParenImpCasts(declRefExpr(
                       to(varDecl(hasType(isInteger()))
                              .bind(matchers_utils::varnames::NameVarCond +
                                    Name))))),
                   hasRHS(expr(hasType(isInteger()))))),
               hasBody(anyOf(InnerStmt, compoundStmt(forEach(InnerStmt)))))
        // hasBody(has(compoundStmt(hasDescendant(InnerStmt)))))
        .bind("forLoop" + Name);
}

StatementMatcher matchers_utils::forLoopNested(int NumLevels,
                                               StatementMatcher InnerStmt) {
    StatementMatcher NestedMatcher = InnerStmt;
    for (int N = NumLevels; N > 0; --N) {
        NestedMatcher =
            matchers_utils::forLoopMatcher(std::to_string(N), NestedMatcher);
    }
    return NestedMatcher;
}
