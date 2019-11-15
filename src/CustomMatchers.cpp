/**
 * File              : CustomMatchers.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 15 Nov 2019 09:23:38 MST
 * Last Modified Date: Ven 15 Nov 2019 14:41:39 MST
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
#include "CustomMatchers.h"

#include <iostream>
#include <string>

StatementMatcher matchers_utils::assignArrayBinOp(std::string name,
                                                  std::string lhs,
                                                  std::string rhs) {
    return binaryOperator(hasOperatorName("="),
                          hasLHS(arraySubscriptExpr().bind(lhs)),
                          hasRHS(binaryOperator().bind(rhs)))
        .bind(name);
}

StatementMatcher matchers_utils::forLoopMatcher(std::string name,
                                                StatementMatcher innerStmt) {
    return forStmt(
               hasLoopInit(declStmt(hasSingleDecl(
                   varDecl(hasInitializer(integerLiteral(equals(0))))
                       .bind(matchers_utils::varnames::nVarInit + name)))),
               hasIncrement(
                   unaryOperator(
                       hasOperatorName("++"),
                       hasUnaryOperand(
                           declRefExpr(to(varDecl(hasType(isInteger()))))
                               .bind(matchers_utils::varnames::nVarInc + name)))
                       .bind(matchers_utils::varnames::nVarIncPos + name)),
               hasCondition(binaryOperator(
                   hasOperatorName("<"),
                   hasLHS(ignoringParenImpCasts(declRefExpr(to(
                       varDecl(hasType(isInteger()))
                           .bind(matchers_utils::varnames::nVarCond + name))))),
                   hasRHS(expr(hasType(isInteger()))))),
               hasBody(anyOf(innerStmt,
                             compoundStmt(forEachDescendant(innerStmt)))))
        .bind("forLoop" + name);
}

StatementMatcher matchers_utils::forLoopNested(int numLevels,
                                               StatementMatcher innerStmt) {
    StatementMatcher nestedMatcher = innerStmt;
    for (int n = numLevels; n > 0; --n) {
        nestedMatcher =
            matchers_utils::forLoopMatcher(std::to_string(n), nestedMatcher);
    }
    return nestedMatcher;
}
