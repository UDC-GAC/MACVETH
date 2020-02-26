#include "include/MVFrontend.h"
#include "include/MVHandlers.h"
#include "include/MVOptions.h"
#include "include/MVPragmaHandler.h"
#include "include/Utils.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"

// ---------------------------------------------
void MACVETHConsumer::HandleTranslationUnit(ASTContext &Context) {
  // For vectorizable statements
  for (int N = 0; N < matchers_utils::MaxDepthForLoops; ++N) {
    StatementMatcher ForLoopNestedMatcherVec =
        matchers_utils::ROI(N, compoundStmt(has(expr())).bind("ROI"));
    MatcherVec.addMatcher(ForLoopNestedMatcherVec, &Handler);
  }
  // Run the matchers when we have the whole TU parsed.
  MatcherVec.matchAST(Context);
}

// ---------------------------------------------
std::unique_ptr<ASTConsumer>
MACVETHFrontendAction::CreateASTConsumer(CompilerInstance &CI, StringRef file) {
  // setSourceMgr: setter for the Rewriter
  // * SourceManager: handles loading and caching of source files into
  // memory. It can be queried for information such as SourceLocation
  // of objects.
  // * LangOptions: controls the dialect of C/C++ accepted
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
  Utils::setOpts(&CI.getSourceManager(), &CI.getLangOpts(),
                 &CI.getASTContext());
  Preprocessor &PP = CI.getPreprocessor();
  ScopHandler *scops = new ScopHandler();
  PP.AddPragmaHandler(new PragmaScopHandler(scops));
  PP.AddPragmaHandler(new PragmaEndScopHandler(scops));

  // std::make_unique is C++14, while LLVM 9 is written in C++11, this
  // is the reason of this custom implementation
#if LLVM_VERSION > 9
  return std::make_unique<MACVETHConsumer>(TheRewriter, &CI.getASTContext(),
                                           scops);
#else
  return llvm::make_unique<MACVETHConsumer>(TheRewriter, &CI.getASTContext(),
                                            scops);
#endif
}

// ---------------------------------------------
void MACVETHFrontendAction::EndSourceFileAction() {
  // 1.- Get RewriteBuffer from FileID
  // 2.- Write to Stream (in this case llvm::outs(), which is
  // std::out) the result of applying all changes to the original
  // buffer. Original buffer is modified before calling this function,
  // from the ASTConsumer
  SourceManager &SM = TheRewriter.getSourceMgr();
  std::error_code ErrorCode;
  std::string OutFileName = Utils::getExePath() + MVOptions::OutFile;
  llvm::raw_fd_ostream outFile(OutFileName, ErrorCode, llvm::sys::fs::F_None);
  TheRewriter.getEditBuffer(SM.getMainFileID()).write(outFile);
}
