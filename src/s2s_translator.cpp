/**
 * File              : s2s_translator.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 06 Nov 2019 12:29:24 MST
 * Last Modified Date: Lun 18 Nov 2019 14:37:32 MST
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

#include "CustomMatchers.h"
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
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/raw_ostream.h"

// Do not use namespace std
// using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

// needed for the global variable, definition below
class TAC;

// global variables
static const clang::SourceManager* S2SSourceManager;
static const clang::LangOptions* S2SLangOpts;
// static std::vector<std::tuple<const BinaryOperator*, std::list<TAC>>>
// ExprToTac;

// FIXME: refactor this

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
    TempExpr(std::string E) : ExprStr(E) {}

    void setClangExpr(clang::Expr* ClangExpr) { this->ClangExpr = ClangExpr; }
    clang::Expr* getClangExpr() { return this->ClangExpr; }

    void setExprStr(std::string Expr) { this->ExprStr = ExprStr; }
    std::string getExprStr() { return this->ExprStr; }

    static std::string getNameTempReg(int val) {
        return "t" + std::to_string(val);
    }

    // A hack for translating Expr to string
    static TempExpr* getTempExprFromExpr(Expr* E) {
        clang::CharSourceRange CharRangeExpr =
            clang::CharSourceRange::getTokenRange(E->getSourceRange());
        const std::string Text = Lexer::getSourceText(
            CharRangeExpr, *S2SSourceManager, *S2SLangOpts);
        TempExpr* Temp = new TempExpr(Text);
        Temp->setClangExpr(E);
        return Temp;
    }

   private:
    std::string TypeStr;
    std::string ExprStr;
    clang::Expr* ClangExpr;
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
        : A(A), B(B), C(C), OP(OP) {}

    TempExpr* getA() { return this->A; };
    TempExpr* getB() { return this->B; };
    TempExpr* getC() { return this->C; };
    clang::BinaryOperator::Opcode getOP() { return this->OP; };

    // Just for debugging purposes
    void printTAC() {
        std::cout << "t: " << this->getA()->getExprStr() << ", "
                  << this->getB()->getExprStr() << ", "
                  << this->getC()->getExprStr() << ", " << this->getOP()
                  << std::endl;
    }

    static Expr* createExprFromTAC(TAC* TacExpr) { return NULL; }

    Expr* getExprFromTAC() { return TAC::createExprFromTAC(this); }

    // Return list of 3AC from a starting Binary Operator
    static void binaryOperator2TAC(const clang::BinaryOperator* S,
                                   std::list<TAC>* TacList, int Val) {
        // cout << "[DEBUG]: bo2TAC" << endl;
        Expr* Lhs = S->getLHS();
        Expr* Rhs = S->getRHS();
        bool LhsTypeBin = false;
        bool RhsTypeBin = false;
        clang::BinaryOperator* LhsBin = NULL;
        clang::BinaryOperator* RhsBin = NULL;
        if (LhsBin = dyn_cast<clang::BinaryOperator>(Lhs)) {
            LhsTypeBin = true;
        }
        if (RhsBin = dyn_cast<clang::BinaryOperator>(Rhs)) {
            RhsTypeBin = true;
        }

        // recursive part, to delve deeper into BinaryOperators
        if ((LhsTypeBin == true) && (RhsTypeBin == true)) {
            // both true
            TempExpr* TmpA = new TempExpr(TempExpr::getNameTempReg(Val));
            TempExpr* TmpB = new TempExpr(TempExpr::getNameTempReg(Val + 1));
            TempExpr* TmpC = new TempExpr(TempExpr::getNameTempReg(Val + 2));
            TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
            TacList->push_back(NewTac);
            binaryOperator2TAC(RhsBin, TacList, Val + 1);
            binaryOperator2TAC(LhsBin, TacList, Val + 2);
        } else if ((LhsTypeBin == false) && (RhsTypeBin == true)) {
            TempExpr* TmpA = new TempExpr(TempExpr::getNameTempReg(Val));
            TempExpr* TmpB = TempExpr::getTempExprFromExpr(Lhs);
            TmpB->setClangExpr(Lhs);
            TempExpr* TmpC = new TempExpr(TempExpr::getNameTempReg(Val + 1));
            TAC NewTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
            TacList->push_back(NewTac);
            binaryOperator2TAC(RhsBin, TacList, Val + 1);
        } else if ((LhsTypeBin == true) && (RhsTypeBin == false)) {
            TempExpr* TmpA = new TempExpr(TempExpr::getNameTempReg(Val));
            TempExpr* TmpB = new TempExpr(TempExpr::getNameTempReg(Val + 1));
            TempExpr* TmpC = TempExpr::getTempExprFromExpr(Rhs);
            TmpC->setClangExpr(Rhs);
            TAC newTac = TAC(TmpA, TmpB, TmpC, S->getOpcode());
            TacList->push_back(newTac);
            binaryOperator2TAC(LhsBin, TacList, Val + 1);
        } else {
            TempExpr* TmpA = new TempExpr(TempExpr::getNameTempReg(Val));
            TAC NewTac =
                TAC(TmpA, TempExpr::getTempExprFromExpr(Lhs),
                    TempExpr::getTempExprFromExpr(Rhs), S->getOpcode());
            TacList->push_back(NewTac);
        }
        return;
    }

   private:
    TempExpr* A;
    TempExpr* B;
    TempExpr* C;
    clang::BinaryOperator::Opcode OP;
};

class LoopStmtClassVisitor : public RecursiveASTVisitor<LoopStmtClassVisitor> {
   public:
    explicit LoopStmtClassVisitor(ASTContext* Context) : Context(Context) {}

    bool VisitCXXRecordDecl(CXXRecordDecl* Declaration) {
        if (Declaration->getQualifiedNameAsString() == "n::m::C") {
            FullSourceLoc FullLocation =
                Context->getFullLoc(Declaration->getBeginLoc());
            if (FullLocation.isValid())
                llvm::outs() << "Found declaration at "
                             << FullLocation.getSpellingLineNumber() << ":"
                             << FullLocation.getSpellingColumnNumber() << "\n";
        }
        return true;
    }

   private:
    ASTContext* Context;
};

// IteartionHandler is called every time we find an assignment like:
// BinaryOperator(lhs, rhs, "=") inside a loop
class IterationHandler : public MatchFinder::MatchCallback {
   public:
    IterationHandler(Rewriter& R) : Rewrite(R) {}

    std::list<TAC> wrapperStmt2TAC(const clang::BinaryOperator* S) {
        // cout << "[DEBUG]: wrapperStmt2TAC" << endl;
        std::list<TAC> TacList;
        TAC::binaryOperator2TAC(S, &TacList, 0);
        std::tuple<const BinaryOperator*, std::list<TAC>> Tup =
            std::make_tuple(S, TacList);
        // exprToTac.push_back(tup);
        for (TAC Tac : TacList) {
            Tac.printTAC();
        }
        return TacList;
    }

    // Perform unrolling
    virtual void run(const MatchFinder::MatchResult& Result) {
        std::list<const DeclRefExpr*> IncVarList;
        const BinaryOperator* TacBinOp =
            Result.Nodes.getNodeAs<clang::BinaryOperator>("assignArrayBinOp");
        std::list<TAC> TacList = IterationHandler::wrapperStmt2TAC(TacBinOp);
        int NLevel = 2;
        int UnrollFactor = 5;

        // Unroll factor applied to the for header
        for (int Inc = NLevel; Inc > 0; --Inc) {
            const UnaryOperator* IncVarPos =
                Result.Nodes.getNodeAs<clang::UnaryOperator>(
                    matchers_utils::varnames::NameVarIncPos +
                    std::to_string(Inc));
            const DeclRefExpr* IncVarName = Result.Nodes.getNodeAs<DeclRefExpr>(
                matchers_utils::varnames::NameVarInc + std::to_string(Inc));
            clang::CharSourceRange charRange =
                clang::CharSourceRange::getTokenRange(IncVarPos->getBeginLoc(),
                                                      IncVarPos->getEndLoc());
            Rewrite.ReplaceText(charRange,
                                IncVarName->getNameInfo().getAsString() +
                                    "+=" + std::to_string(UnrollFactor));
        }

        clang::CharSourceRange CharRangeStr =
            clang::CharSourceRange::getTokenRange(TacBinOp->getSourceRange());

        Rewrite.InsertText(
            TacBinOp->getBeginLoc(),
            "/* unrolling factor " + std::to_string(UnrollFactor) + " */\n",
            true, true);
        // tacBinOp = statement to unroll
        // need to find all the array subscripts
        for (int Unroll = 0; Unroll < UnrollFactor; ++Unroll) {
        }
    }

   private:
    Rewriter& Rewrite;
};

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser. It registers a couple of matchers and runs them on
// the AST.
class S2SConsumer : public ASTConsumer {
   public:
    S2SConsumer(Rewriter& R, ASTContext* C) : HandlerIteration(R), Context(C) {
        // EXPERIMENTAL MATCHER
        // This matcher works for 2-level for loops
        StatementMatcher ForLoopNestedMatcher = matchers_utils::forLoopNested(
            2,
            matchers_utils::assignArrayBinOp("assignArrayBinOp", "lhs", "rhs"));
        Matcher.addMatcher(ForLoopNestedMatcher, &HandlerIteration);
    }

    void HandleTranslationUnit(ASTContext& Context) override {
        // Run the matchers when we have the whole TU parsed.
        Matcher.matchAST(Context);
    }

   private:
    ASTContext* Context;
    IterationHandler HandlerIteration;
    MatchFinder Matcher;
};

// SECOND STEP
// For each source file provided to the tool, a new ASTFrontendAction is
// created, which inherits from FrontendAction (abstract class)
class S2SFrontendAction : public ASTFrontendAction {
   public:
    // empty constructor
    S2SFrontendAction() {}

    // This routine is called in BeginSourceFile(), from
    // CreateWrapperASTConsumer.
    // * CompilterInstance CI: got from getCompilerInstance()
    // * StringRef file: input file, provided by getCurrentFile()
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& CI,
                                                   StringRef file) override {
        S2SSourceManager = &CI.getSourceManager();
        S2SLangOpts = &CI.getLangOpts();
        // setSourceMgr: setter for the Rewriter
        // * SourceManager: handles loading and caching of source files into
        // memory. It can be queried for information such as SourceLocation
        // of objects.
        // * LangOptions: controls the dialect of C/C++ accepted
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        // std::make_unique is C++14, while LLVM is written in C++11, this
        // is the reason of this custom implementation
        return llvm::make_unique<S2SConsumer>(TheRewriter, &CI.getASTContext());
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
    llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);

    // FIRST STEP
    // Parser for options common to all cmd-line Clang tools
    CommonOptionsParser Op(argc, argv, MatcherSampleCategory);
    // Utility to run a FrontendAction over a set of files
    // * getCompilations(): contains compile cmd lines for the given source
    // paths
    // * getSourcePathList(): source files to run over
    ClangTool Tool(Op.getCompilations(), Op.getSourcePathList());

    // Runs ToolAction over all files specified in the cmd line
    // newFrontendActionFactory returns a new FrontendActionFactory for
    // a given type, in this case out S2SFrontendAction, declared above
    return Tool.run(newFrontendActionFactory<S2SFrontendAction>().get());
}
