/**
 * File              : RewriterBase.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 26 Nov 2019 14:32:10 MST
 * Last Modified Date: Mar 26 Nov 2019 14:40:08 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/EditTracker.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/Core/Replacement.h"

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

template <typename Type> struct TargetNodeTraits;

template <typename TargetNode>
class RewriterBase : public MatchFinder::MatchCallback {
public:
  explicit RewriterBase(std::set<Replacement> *replacements,
                        RenameCategory category)
      : replacements_(replacements), edit_tracker_(category) {}
  const TargetNode &GetTargetNode(const MatchFinder::MatchResult &result) {
    const TargetNode *target_node = result.Nodes.getNodeAs<TargetNode>(
        TargetNodeTraits<TargetNode>::GetName());
    assert(target_node);
    return *target_node;
  }
  std::set<clang::SourceLocation> &GetRewrittenLocs() {
    static auto &locations = *new std::set<clang::SourceLocation>();
    return locations;
  }
  bool GenerateReplacement(const MatchFinder::MatchResult &result,
                           clang::SourceLocation loc, llvm::StringRef old_name,
                           std::string new_name, Replacement *replacement) {
    const clang::ASTContext &context = *result.Context;
    const clang::SourceManager &source_manager = *result.SourceManager;
    if (loc.isMacroID()) {
      // Try to jump "above" the scratch buffer if |loc| is inside
      // token##Concatenation.
      const int kMaxJumps = 5;
      bool verified_out_of_scratch_space = false;
      for (int i = 0; i < kMaxJumps && !verified_out_of_scratch_space; i++) {
        clang::SourceLocation spell = source_manager.getSpellingLoc(loc);
        verified_out_of_scratch_space =
            source_manager.getBufferName(spell) != "<scratch space>";
        if (!verified_out_of_scratch_space)
          loc = source_manager.getImmediateMacroCallerLoc(loc);
      }
      if (!verified_out_of_scratch_space)
        return false;
    }
    // If the edit affects only the first character of the identifier, then
    // narrow down the edit to only this single character.  This is important
    // for dealing with toFooBar -> ToFooBar method renaming when the method
    // name is built using macro token concatenation like to##macroArgument - in
    // this case we should only rewrite "t" -> "T" and leave "o##macroArgument"
    // untouched.
    llvm::StringRef expected_old_text = old_name;
    llvm::StringRef new_text = new_name;
    if (loc.isMacroID() && expected_old_text.substr(1) == new_text.substr(1)) {
      expected_old_text = expected_old_text.substr(0, 1);
      new_text = new_text.substr(0, 1);
    }
    clang::SourceLocation spell = source_manager.getSpellingLoc(loc);
    clang::CharSourceRange range = clang::CharSourceRange::getCharRange(
        spell, spell.getLocWithOffset(expected_old_text.size()));
    // We need to ensure that |actual_old_text| is the same as
    // |expected_old_text| - it can be different if |actual_old_text| contains
    // a macro argument (see DEFINE_WITH_TOKEN_CONCATENATION2 in
    // macros-original.cc testcase).
    StringRef actual_old_text = clang::Lexer::getSourceText(
        range, source_manager, context.getLangOpts());
    if (actual_old_text != expected_old_text)
      return false;
    if (replacement) {
      // If there's already a replacement for this location, don't emit any
      // other replacements to avoid potential naming conflicts. This is
      // primarily to avoid problems when a function and a parameter are defined
      // by the same macro argument.
      if (!GetRewrittenLocs().emplace(spell).second)
        return false;
      *replacement = Replacement(source_manager, range, new_text);
    }
    return true;
  }
  virtual clang::SourceLocation
  GetTargetLoc(const MatchFinder::MatchResult &result) {
    return TargetNodeTraits<TargetNode>::GetLoc(GetTargetNode(result));
  }
  void AddReplacement(const MatchFinder::MatchResult &result,
                      llvm::StringRef old_name, std::string new_name) {
    if (old_name == new_name)
      return;
    clang::SourceLocation loc = GetTargetLoc(result);
    if (loc.isInvalid())
      return;
    Replacement replacement;
    if (!GenerateReplacement(result, loc, old_name, new_name, &replacement))
      return;
    replacements_->insert(std::move(replacement));
    edit_tracker_.Add(*result.SourceManager, loc, old_name, new_name);
  }
  const EditTracker *edit_tracker() const { return &edit_tracker_; }

private:
  std::set<Replacement> *const replacements_;
  EditTracker edit_tracker_;
};
