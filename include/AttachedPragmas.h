//===--- Cleanup.h - "Umbrella" header for AST library ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Hacks and fun related to the code rewriter.
//
//===----------------------------------------------------------------------===//

#ifndef SCOUT_CLANGADDONS_ASTPROCESSING_ATTACHEDPRAGMAS_H
#define SCOUT_CLANGADDONS_ASTPROCESSING_ATTACHEDPRAGMAS_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include <list>
#include <memory>
#include <boost/variant.hpp>
#include <string>
#include <map>

//--------------------------------------------------------- 
namespace clang {

//--------------------------------------------------------- forwards
class IdentifierInfo;
class Preprocessor;
class Decl;
class Expr;
class ASTContext;

//--------------------------------------------------------- 
namespace ASTProcessing {

class StmtCollector;

// Attached pragmas are pragmas just in front of a Statement influencing the 
// processing of that statement. A pragma is attached to a particular statement 
// if there are no other statements between the pragma line and the statement 
// line. Thus more than one pragma can be attached to a statement.
// A pragma used by the ASTProcessing lib is build of a namespace, a domain, 
// an action (or property) and any number of arguments.
// Example:  NS     DOM        ACT    ARGS
// #pragma scout loop      unroll    5
// or
// #pragma scout condition invariant

// the argument info 
class PragmaArgumentInfo
{
  typedef std::map<std::string, Expr*> tArguments;
  tArguments m_Arguments;

public:
  void insertArgument(const llvm::StringRef& name, Expr* E);
  bool hasArgument(const char* pName) const;
  bool findValueForArgument(const char* pName, const ASTContext& Ctx, int& value) const;
  Expr* getExprForArgument(const char* pName) const;
};

//--------------------------------------------------------- 
class AttachedPragmaCollector
{
  class Handler;

  Preprocessor &PP;
  std::string  m_Namespace;
  typedef std::list< std::shared_ptr<Handler> > tHandlers;
  tHandlers PragmaHandlers;

public:
  AttachedPragmaCollector(Preprocessor &PreProc, const char* pNamespaceName);
  ~AttachedPragmaCollector();

  void addDomain(const char* pDomainName);
  void collectParsedPragmas(StmtCollector& collector) const;
  void collectDeclarationPragmas(Decl* D);
};


//--------------------------------------------------------- inlines
inline AttachedPragmaCollector::AttachedPragmaCollector(
  Preprocessor &PreProc, const char* pNamespaceName) :
  PP(PreProc),
  m_Namespace(pNamespaceName)
{}

//--------------------------------------------------------- 
} // namespace ASTProcessing

} // namespace clang

#endif  //SCOUT_CLANGADDONS_ASTPROCESSING_ATTACHEDPRAGMAS_H
