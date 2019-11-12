/**
 * File              : s2s_translator.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 06 Nov 2019 12:29:24 MST
 * Last Modified Date: Lun 11 Nov 2019 22:20:53 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 * Original Code     : Eli Bendersky (eliben@gmail.com)
 */
#include <iostream>
#include <list>
#include <string>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;

// name for the category
static llvm::cl::OptionCategory MatcherSampleCategory("Matcher Sample");

static const clang::SourceManager* mySourceManager;
static const clang::LangOptions* myLangOpts;

class TempExpr {
   public:
    TempExpr(string e) : expr(e) {}

    void setClangExpr(clang::Expr* clangExpr) { this->clangExpr = clangExpr; }

    string getExprStr() { return expr; }

    // helper function
    static TempExpr* getTempExprFromExpr(Expr* E) {
        cout << "[DEBUG]: getTempExpr" << endl;
        clang::CharSourceRange charRange =
            clang::CharSourceRange::getTokenRange(E->getSourceRange());
        const string text =
            Lexer::getSourceText(charRange, *mySourceManager, *myLangOpts);
        TempExpr* tmp = new TempExpr(text);
        tmp->setClangExpr(E);
        return tmp;
    }

   private:
    string expr;
    clang::Expr* clangExpr;
};

// a = b op c
class TAC {
   public:
    // consturctor
    TAC(TempExpr* A, TempExpr* B, TempExpr* C, clang::BinaryOperator::Opcode OP)
        : a(A), b(B), c(C), op(OP) {}

    TempExpr* getA() { return this->a; };
    TempExpr* getB() { return this->b; };
    TempExpr* getC() { return this->c; };

    void printTAC() {
        cout << "t: " << this->getA()->getExprStr() << ", "
             << this->b->getExprStr() << ", " << this->c->getExprStr() << endl;
    }

   private:
    TempExpr* a;
    TempExpr* b;
    TempExpr* c;
    clang::BinaryOperator::Opcode op;
};

class IterationHandler : public MatchFinder::MatchCallback {
   public:
    IterationHandler(Rewriter& R) : Rewrite(R) {}

    static std::string getNameTempReg(int val) {
        return "t" + std::to_string(val);
    }

    list<TAC> wrapperStmt2TAC(const clang::BinaryOperator* S) {
        cout << "[DEBUG]: wrapperStmt2TAC" << endl;
        list<TAC> tacList;
        binaryOperator2TAC(S, &tacList, 0);
        // tacList.reverse();
        for (TAC tac : tacList) {
            tac.printTAC();
        }
        return tacList;
    }

    // Return list of 3AC from a starting Binary Operator
    void binaryOperator2TAC(const clang::BinaryOperator* S, list<TAC>* tacList,
                            int val) {
        cout << "[DEBUG]: bo2TAC" << endl;
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
            string tmpA = getNameTempReg(val);
            string tmpB = getNameTempReg(val + 1);
            string tmpC = getNameTempReg(val + 2);
            TAC newTac = TAC(new TempExpr(tmpA), new TempExpr(tmpB),
                             new TempExpr(tmpC), S->getOpcode());
            tacList->push_back(newTac);
            binaryOperator2TAC(rhsBin, tacList, val + 1);
            binaryOperator2TAC(lhsBin, tacList, val + 2);
        } else if ((lhsTypeBin == false) && (rhsTypeBin == true)) {
            string tmpA = getNameTempReg(val);
            string tmpC = getNameTempReg(val + 1);
            TAC newTac =
                TAC(new TempExpr(tmpA), TempExpr::getTempExprFromExpr(lhs),
                    new TempExpr(tmpC), S->getOpcode());
            tacList->push_back(newTac);
            binaryOperator2TAC(rhsBin, tacList, val + 1);
        } else if ((lhsTypeBin == true) && (rhsTypeBin == false)) {
            string tmpA = getNameTempReg(val);
            string tmpB = getNameTempReg(val + 1);
            TAC newTac =
                TAC(new TempExpr(tmpA), new TempExpr(tmpB),
                    TempExpr::getTempExprFromExpr(rhs), S->getOpcode());
            tacList->push_back(newTac);
            binaryOperator2TAC(lhsBin, tacList, val + 1);
        } else {
            string tmpA = getNameTempReg(val);
            TAC newTac =
                TAC(new TempExpr(tmpA), TempExpr::getTempExprFromExpr(lhs),
                    TempExpr::getTempExprFromExpr(rhs), S->getOpcode());
            tacList->push_back(newTac);
        }
        return;
    }

    virtual void run(const MatchFinder::MatchResult& Result) {
        const BinaryOperator* tacBinOp =
            Result.Nodes.getNodeAs<clang::BinaryOperator>("tacStmt");
        list<TAC> tacList = IterationHandler::wrapperStmt2TAC(tacBinOp);

        Rewrite.InsertText(tacBinOp->getBeginLoc(), "/* converting to 3AC */\n",
                           true, true);
    }

   private:
    Rewriter& Rewrite;
};

class IncrementForLoopHandler : public MatchFinder::MatchCallback {
   public:
    IncrementForLoopHandler(Rewriter& Rewrite) : Rewrite(Rewrite) {}

    virtual void run(const MatchFinder::MatchResult& Result) {
        const VarDecl* IncVar = Result.Nodes.getNodeAs<VarDecl>("incVarName");
        Rewrite.InsertText(IncVar->getBeginLoc(), "/* increment */", true,
                           true);
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
        // (hasOperatorName("=")). This way we invoke HandlerIteration to
        // perform 3AC/TAC transformation
        Matcher.addMatcher(
            forStmt(hasBody(
                compoundStmt(
                    forEachDescendant(
                        binaryOperator(hasOperatorName("="),
                                       hasLHS(arraySubscriptExpr().bind("lhs")),
                                       hasRHS(binaryOperator().bind("rhs")))
                            .bind("tacStmt")))
                    .bind("bodyForLoop"))),
            &HandlerIteration);

        // Add a complex matcher for finding 'for' loops with an initializer
        // set to 0, < comparison in the codition and an increment. For
        // example:
        //
        //  for (int i = 0; i < N; ++i)
        Matcher.addMatcher(
            forStmt(
                hasLoopInit(declStmt(hasSingleDecl(
                    varDecl(hasInitializer(integerLiteral(equals(0))))
                        .bind("initVarName")))),
                hasIncrement(unaryOperator(
                    hasOperatorName("++"),
                    hasUnaryOperand(declRefExpr(to(
                        varDecl(hasType(isInteger())).bind("incVarName")))))),
                hasCondition(binaryOperator(
                    hasOperatorName("<"),
                    hasLHS(ignoringParenImpCasts(declRefExpr(to(
                        varDecl(hasType(isInteger())).bind("condVarName"))))),
                    hasRHS(expr(hasType(isInteger()))))))
                .bind("forLoop"),
            &HandlerForFor);
    }

    void HandleTranslationUnit(ASTContext& Context) override {
        // Run the matchers when we have the whole TU parsed.
        Matcher.matchAST(Context);
    }

   private:
    IterationHandler HandlerIteration;
    // IfStmtHandler HandlerForIf;
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
