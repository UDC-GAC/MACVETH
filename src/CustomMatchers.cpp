/**
 * File              : CustomMatchers.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 15 Nov 2019 09:23:38 MST
 * Last Modified Date: Ven 15 Nov 2019 11:17:27 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#include "CustomMatchers.h"

#include <string>

StatementMatcher utils::assignArrayBinOp(std::string name, std::string lhs,
                                         std::string rhs) {
    return binaryOperator(hasOperatorName("="),
                          hasLHS(arraySubscriptExpr().bind(lhs)),
                          hasRHS(binaryOperator().bind(rhs)))
        .bind(name);
}

StatementMatcher utils::forLoopMatcher(std::string name,
                                       StatementMatcher innerStmt) {
    return forStmt(
               hasLoopInit(declStmt(hasSingleDecl(
                   varDecl(hasInitializer(integerLiteral(equals(0))))
                       .bind("initVar" + name)))),
               hasIncrement(unaryOperator(hasOperatorName("++"),
                                          hasUnaryOperand(declRefExpr(to(
                                              varDecl(hasType(isInteger()))))))
                                .bind("incVar" + name)),
               hasCondition(binaryOperator(
                   hasOperatorName("<"),
                   hasLHS(ignoringParenImpCasts(declRefExpr(to(
                       varDecl(hasType(isInteger())).bind("condVar" + name))))),
                   hasRHS(expr(hasType(isInteger()))))),
               hasBody(anyOf(innerStmt,
                             compoundStmt(forEachDescendant(innerStmt)))))
        .bind("forLoop" + name);
}

StatementMatcher utils::forLoopNested(int numLevels,
                                      StatementMatcher innerStmt) {
    StatementMatcher nestedMatcher = innerStmt;
    for (int n = numLevels; n < 0; --n) {
        nestedMatcher = utils::forLoopMatcher(std::to_string(n), nestedMatcher);
    }
    return nestedMatcher;
}
