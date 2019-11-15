/**
 * File              : matchers.hpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 14 Nov 2019 15:47:54 MST
 * Last Modified Date: Xov 14 Nov 2019 15:47:54 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
StatementMatcher assignment =
    binaryOperator(hasOperatorName("="),
                   hasLHS(arraySubscriptExpr().bind("lhs")),
                   hasRHS(binaryOperator().bind("rhs")))
        .bind("tacStmt");
