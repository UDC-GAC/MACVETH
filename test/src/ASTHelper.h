/**
 * File              : ASTHelper.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 05 Dec 2019 12:17:25 MST
 * Last Modified Date: Xov 05 Dec 2019 18:01:06 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/TAC.h"
#include "clang/AST/Expr.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Tooling/Tooling.h"

using namespace macveth;
using namespace clang;
using namespace clang::tooling;

namespace macveth::testing {

std::unique_ptr<ASTUnit> TestSimpleBinOp = tooling::buildASTFromCode(
    R"(
    double a = b + c;
    )");
std::unique_ptr<ASTUnit> TestMultipleBinOp = tooling::buildASTFromCode(
    R"(
    int N = 100;
    double a[N],b[N],c[N];
    double a[0] = b[0] + c[0];
    doubel b[0] = a[0] + 2*c[0]; 
    )");

clang::TranslationUnitDecl *getTUDecl(std::unique_ptr<ASTUnit> T) {
  clang::ASTContext *pctx = &(T->getASTContext());
  return pctx->getTranslationUnitDecl();
}

/// From the code created in TestBinOp creates a BinaryOperator
BinaryOperator *getSimpleBinOp() { return nullptr; }

/// From the code created in TestBinOp creates a BinaryOperator
BinaryOperator *getiMultipleBinOp() { return nullptr; }

} // namespace macveth::testing
