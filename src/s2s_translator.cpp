/**
 * File              : s2s_translator.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 06 Nov 2019 12:29:24 MST
 * Last Modified Date: Xov 14 Nov 2019 17:41:39 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 * Original Code     : Eli Bendersky (eliben@gmail.com)
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
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <tuple>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ParentMap.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Analysis/CFG.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

// Do not use namespace std
// using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;

// needed for the global variable, definition below
class TAC;

// global variables
static const clang::SourceManager* mySourceManager;
static const clang::LangOptions* myLangOpts;
static std::vector<std::tuple<const BinaryOperator*, std::list<TAC>>> exprToTac;

// FIXME: refactor this
static StatementMatcher assignArrayBinOp =
    binaryOperator(hasOperatorName("="),
                   hasLHS(arraySubscriptExpr().bind("lhs")),
                   hasRHS(binaryOperator().bind("rhs")))
        .bind("assignArrayBinOp");

static StatementMatcher forLoopInner =
    forStmt(hasLoopInit(declStmt(
                hasSingleDecl(varDecl(hasInitializer(integerLiteral(equals(0))))
                                  .bind("initVarInner")))),
            hasIncrement(unaryOperator(hasOperatorName("++"),
                                       hasUnaryOperand(declRefExpr(
                                           to(varDecl(hasType(isInteger()))))))
                             .bind("incVarInner")),
            hasCondition(binaryOperator(
                hasOperatorName("<"),
                hasLHS(ignoringParenImpCasts(declRefExpr(
                    to(varDecl(hasType(isInteger())).bind("condVarInner"))))),
                hasRHS(expr(hasType(isInteger()))))),
            hasBody(compoundStmt(forEachDescendant(assignArrayBinOp))
                        .bind("forLoopBodyInner")))
        .bind("forLoopInner");

static StatementMatcher forLoopOuter =
    forStmt(hasLoopInit(declStmt(
                hasSingleDecl(varDecl(hasInitializer(integerLiteral(equals(0))))
                                  .bind("initVarOuter")))),
            hasIncrement(unaryOperator(
                hasOperatorName("++"),
                hasUnaryOperand(declRefExpr(to(varDecl(hasType(isInteger()))))
                                    .bind("incVarOuter")))),
            hasCondition(binaryOperator(
                hasOperatorName("<"),
                hasLHS(ignoringParenImpCasts(declRefExpr(
                    to(varDecl(hasType(isInteger())).bind("condVarOuter"))))),
                hasRHS(expr(hasType(isInteger()))))),
            hasBody(forLoopInner))
        .bind("forLoopOuter");

// SOME NOTES:
// * Stmt - statements, could be a for loop, while, a single statement
// * Decl - declarations (or defenitions) of variables, typedef, function...
// * Type - types, CanonicalType, Builtin-type
// * Expr - expressions, they inherit from Stmt tho; this is quite weird for
// me...

// This class holds a double identity for expressions: a string name (for
// debugging purposes and for clarity) and the actual Clang Expr, for
// transformations
class TempExpr {
   public:
    TempExpr(std::string e) : expr(e) {}

    void setClangExpr(clang::Expr* clangExpr) { this->clangExpr = clangExpr; }
    clang::Expr* getClangExpr() { return this->clangExpr; }

    void setExprStr(std::string expr) { this->expr = expr; }
    std::string getExprStr() { return this->expr; }

    // A hack for translating Expr to string
    static TempExpr* getTempExprFromExpr(Expr* E) {
        clang::CharSourceRange charRange =
            clang::CharSourceRange::getTokenRange(E->getSourceRange());
        const std::string text =
            Lexer::getSourceText(charRange, *mySourceManager, *myLangOpts);
        TempExpr* tmp = new TempExpr(text);
        tmp->setClangExpr(E);
        return tmp;
    }

   private:
    std::string expr;
    clang::Expr* clangExpr;
};

// Class TAC: three-address-code
// This class is no more than a wrapper for holding three Expr and a Opcode, in
// a way that:
class TAC {
    // This class is meant to hold structures such as:
    //                  a = b op c
    // Where a, b and c are Expr and op is an Opcode
   public:
    // Consturctor
    TAC(TempExpr* A, TempExpr* B, TempExpr* C, clang::BinaryOperator::Opcode OP)
        : a(A), b(B), c(C), op(OP) {}

    TempExpr* getA() { return this->a; };
    TempExpr* getB() { return this->b; };
    TempExpr* getC() { return this->c; };
    clang::BinaryOperator::Opcode getOP() { return this->op; };

    // Just for debugging purposes
    void printTAC() {
        std::cout << "t: " << this->getA()->getExprStr() << ", "
                  << this->b->getExprStr() << ", " << this->c->getExprStr()
                  << ", " << this->getOP() << std::endl;
    }

   private:
    TempExpr* a;
    TempExpr* b;
    TempExpr* c;
    clang::BinaryOperator::Opcode op;
};

// IteartionHandler is called every time we find an assignment like:
// BinaryOperator(lhs, rhs, "=") inside a loop
class IterationHandler : public MatchFinder::MatchCallback {
   public:
    IterationHandler(Rewriter& R) : Rewrite(R) {}

    static std::string getNameTempReg(int val) {
        return "t" + std::to_string(val);
    }

    std::list<TAC> wrapperStmt2TAC(const clang::BinaryOperator* S) {
        // cout << "[DEBUG]: wrapperStmt2TAC" << endl;
        std::list<TAC> tacList;
        binaryOperator2TAC(S, &tacList, 0);
        std::tuple<const BinaryOperator*, std::list<TAC>> tup =
            std::make_tuple(S, tacList);
        exprToTac.push_back(tup);
        for (TAC tac : tacList) {
            tac.printTAC();
        }
        return tacList;
    }

    // Return list of 3AC from a starting Binary Operator
    void binaryOperator2TAC(const clang::BinaryOperator* S,
                            std::list<TAC>* tacList, int val) {
        // cout << "[DEBUG]: bo2TAC" << endl;
        Expr* lhs = S->getLHS();
        Expr* rhs = S->getRHS();
        bool lhsTypeBin = false;
        bool rhsTypeBin = false;
        clang::BinaryOperator* lhsBin = NULL;
        clang::BinaryOperator* rhsBin = NULL;
        if (lhsBin = dyn_cast<clang::BinaryOperator>(lhs)) {
            lhsTypeBin = true;
        }
        if (rhsBin = dyn_cast<clang::BinaryOperator>(rhs)) {
            rhsTypeBin = true;
        }

        // recursive part, to delve deeper into BinaryOperators
        if ((lhsTypeBin == true) && (rhsTypeBin == true)) {
            // both true
            TempExpr* tmpA = new TempExpr(getNameTempReg(val));
            TempExpr* tmpB = new TempExpr(getNameTempReg(val + 1));
            TempExpr* tmpC = new TempExpr(getNameTempReg(val + 2));
            TAC newTac = TAC(tmpA, tmpB, tmpC, S->getOpcode());
            tacList->push_back(newTac);
            binaryOperator2TAC(rhsBin, tacList, val + 1);
            binaryOperator2TAC(lhsBin, tacList, val + 2);
        } else if ((lhsTypeBin == false) && (rhsTypeBin == true)) {
            TempExpr* tmpA = new TempExpr(getNameTempReg(val));
            TempExpr* tmpB = TempExpr::getTempExprFromExpr(lhs);
            tmpB->setClangExpr(lhs);
            TempExpr* tmpC = new TempExpr(getNameTempReg(val + 1));
            TAC newTac = TAC(tmpA, tmpB, tmpC, S->getOpcode());
            tacList->push_back(newTac);
            binaryOperator2TAC(rhsBin, tacList, val + 1);
        } else if ((lhsTypeBin == true) && (rhsTypeBin == false)) {
            TempExpr* tmpA = new TempExpr(getNameTempReg(val));
            TempExpr* tmpB = new TempExpr(getNameTempReg(val + 1));
            TempExpr* tmpC = TempExpr::getTempExprFromExpr(rhs);
            tmpC->setClangExpr(rhs);
            TAC newTac = TAC(tmpA, tmpB, tmpC, S->getOpcode());
            tacList->push_back(newTac);
            binaryOperator2TAC(lhsBin, tacList, val + 1);
        } else {
            TempExpr* tmpA = new TempExpr(getNameTempReg(val));
            TAC newTac =
                TAC(tmpA, TempExpr::getTempExprFromExpr(lhs),
                    TempExpr::getTempExprFromExpr(rhs), S->getOpcode());
            tacList->push_back(newTac);
        }
        return;
    }

    virtual void run(const MatchFinder::MatchResult& Result) {
        const BinaryOperator* tacBinOp =
            Result.Nodes.getNodeAs<clang::BinaryOperator>("assignArrayBinOp");
        std::list<TAC> tacList = IterationHandler::wrapperStmt2TAC(tacBinOp);

        // charRange generated this way does not include the ;, therefore when
        // replacing, the ; is not. Need to find explanation
        clang::CharSourceRange charRange =
            clang::CharSourceRange::getTokenRange(tacBinOp->getSourceRange());
        // Can not use this function because I do not have
        // llvm/clang/lib/include/ARCMT/Transformations.h, dunno why tho
        //        clang::SourceLocation SemiLoc =
        //            clang::arcmt::trans::findSemiAfterLocation(
        //                tacBinOp->getLocEnd(), tacBinOp->getASTContext());
        Rewrite.ReplaceText(charRange, "/* converting to 3AC */");
        // Rewrite.InsertText(charRange, "/* converting to 3AC */\n", true,
        // true);
    }

   private:
    Rewriter& Rewrite;
};

class IncrementForLoopHandler : public MatchFinder::MatchCallback {
   public:
    IncrementForLoopHandler(Rewriter& Rewrite) : Rewrite(Rewrite) {}

    virtual void run(const MatchFinder::MatchResult& Result) {
        const DeclRefExpr* incVarOuter =
            Result.Nodes.getNodeAs<DeclRefExpr>("incVarOuter");
        const UnaryOperator* incVarInner =
            Result.Nodes.getNodeAs<UnaryOperator>("incVarInner");
        const BinaryOperator* tacBinOp =
            Result.Nodes.getNodeAs<clang::BinaryOperator>("assignArrayBinOp");
        clang::CharSourceRange charRange =
            clang::CharSourceRange::getTokenRange(incVarInner->getBeginLoc(),
                                                  incVarInner->getEndLoc());
        Rewrite.ReplaceText(charRange, "j+=4");
        charRange = clang::CharSourceRange::getTokenRange(
            incVarOuter->getBeginLoc(), incVarOuter->getEndLoc());
        Rewrite.ReplaceText(charRange, "i+=4");
        Rewrite.InsertText(tacBinOp->getBeginLoc(), "/* unrolling */\n", true,
                           true);
        //        Rewrite.InsertText(tacBinOp->getBeginLoc(), "/* converting to
        //        3AC */\n",
        //                           true, true);
    }

   private:
    Rewriter& Rewrite;
};

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser. It registers a couple of matchers and runs them on
// the AST.
class MyASTConsumer : public ASTConsumer {
   public:
    MyASTConsumer(Rewriter& R) : HandlerIteration(R), HandlerForFor(R) {
        // Handling iteration
        // For each inner loop, looks in the body a compoundStmt, in order
        // to get the binaryOperator which has an assignment
        // (hasOperatorName("=")) in the following way:
        //    array[] = BinaryOperator(lhs,rhs,op)
        // This way we invoke HandlerIteration to
        // perform 3AC/TAC transformation
        // Matcher.addMatcher(
        //     forStmt(hasBody(compoundStmt(forEachDescendant(assignArrayBinOp))
        //                         .bind("bodyForLoop"))),
        //     &HandlerIteration);

        Matcher.addMatcher(forLoopOuter, &HandlerForFor);

        // EXPERIMENTAL MATCHER
        // ***This matcher only works for the inner loop***
        // Add a complex matcher for finding 'for' loops with an initializer
        // set to 0, < comparison in the codition and an increment. For
        // example:
        //
        //  for (int i = 0; i < N; ++i)
        // Matcher.addMatcher(
        //    forStmt(
        //        hasLoopInit(declStmt(hasSingleDecl(
        //            varDecl(hasInitializer(integerLiteral(equals(0))))
        //                .bind("initVarName")))),
        //        hasIncrement(unaryOperator(
        //            hasOperatorName("++"),
        //            hasUnaryOperand(declRefExpr(to(
        //                varDecl(hasType(isInteger())).bind("incVarName")))))),
        //        hasCondition(binaryOperator(
        //            hasOperatorName("<"),
        //            hasLHS(ignoringParenImpCasts(declRefExpr(to(
        //                varDecl(hasType(isInteger())).bind("condVarName"))))),
        //            hasRHS(expr(hasType(isInteger()))))))
        //        .bind("forLoop"),
        //    &HandlerForFor);
        // Matcher.addMatcher(
        //    forStmt(
        //        hasLoopInit(declStmt(hasSingleDecl(
        //            varDecl(hasInitializer(integerLiteral(equals(0))))
        //                .bind("initVarName")))),
        //        hasIncrement(unaryOperator(
        //            hasOperatorName("++"),
        //            hasUnaryOperand(declRefExpr(to(
        //                varDecl(hasType(isInteger())).bind("incVarName")))))),
        //        hasCondition(binaryOperator(
        //            hasOperatorName("<"),
        //            hasLHS(ignoringParenImpCasts(declRefExpr(to(
        //                varDecl(hasType(isInteger())).bind("condVarName"))))),
        //            hasRHS(expr(hasType(isInteger()))))),
        //        hasBody(compoundStmt(hasDescendant(assignArrayBinOp))
        //                    .bind("forLoopBody")))
        //        .bind("forLoop"),
        //    &HandlerForFor);
    }

    void HandleTranslationUnit(ASTContext& Context) override {
        // Run the matchers when we have the whole TU parsed.
        Matcher.matchAST(Context);
    }

   private:
    IterationHandler HandlerIteration;
    IncrementForLoopHandler HandlerForFor;
    MatchFinder Matcher;
};

// SECOND STEP
// For each source file provided to the tool, a new ASTFrontendAction is
// created, which inherits from FrontendAction (abstract class)
class MyFrontendAction : public ASTFrontendAction {
   public:
    // empty constructor
    MyFrontendAction() {}

    // This routine is called in BeginSourceFile(), from
    // CreateWrapperASTConsumer.
    // * CompilterInstance CI: got from getCompilerInstance()
    // * StringRef file: input file, provided by getCurrentFile()
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& CI,
                                                   StringRef file) override {
        mySourceManager = &CI.getSourceManager();
        myLangOpts = &CI.getLangOpts();
        // setSourceMgr: setter for the Rewriter
        // * SourceManager: handles loading and caching of source files into
        // memory. It can be queried for information such as SourceLocation
        // of objects.
        // * LangOptions: controls the dialect of C/C++ accepted
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        // std::make_unique is C++14, while LLVM is written in C++11, this
        // is the reason of this custom implementation
        return llvm::make_unique<MyASTConsumer>(TheRewriter);
    }

    // this is called only following a successful call to
    // BeginSourceFileAction (and BeginSourceFile)
    void EndSourceFileAction() override {
        // 1.- Get RewriteBuffer from FileID
        // 2.- Write to Stream (in this case llvm::outs(), which is
        // std::out) the result of applying all changes to the original
        // buffer. Original buffer is modified before calling this function,
        // from the ASTConsumer
        TheRewriter.getEditBuffer(TheRewriter.getSourceMgr().getMainFileID())
            .write(llvm::outs());
    }

   private:
    // Main interfacer to the rewrite buffers: dispatches high-level
    // requests to the low-level RewriteBuffers involved.
    Rewriter TheRewriter;
};

// Set up the command line options
static llvm::cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static llvm::cl::OptionCategory MatcherSampleCategory("Matcher Sample");

int main(int argc, const char** argv) {
    // FIRST STEP
    // Parser for options common to all cmd-line Clang tools
    CommonOptionsParser op(argc, argv, MatcherSampleCategory);
    // Utility to run a FrontendAction over a set of files
    // * getCompilations(): contains compile cmd lines for the given source
    // paths
    // * getSourcePathList(): source files to run over
    ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    // Runs ToolAction over all files specified in the cmd line
    // newFrontendActionFactory returns a new FrontendActionFactory for
    // a given type, in this case out MyFrontendAction, declared above
    return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}
