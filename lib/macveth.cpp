/**
 * File              : macveth.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 06 Nov 2019 12:29:24 MST
 * Last Modified Date: Mar 17 Mar 2020 18:58:40 CET
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 * Original Code     : Eli Bendersky <eliben@gmail.com>
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

#include "include/MVFrontend.h"
#include "include/MVOptions.h"
#include "include/MVPragmaHandler.h"
#include "include/Utils.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Signals.h"

#ifndef LLVM_VERSION
#define LLVM_VERSION 10
#endif

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;
using namespace macveth;

// Set up the command line options
static llvm::cl::OptionCategory MacvethCategory("Macveth Options");
static llvm::cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
// Custom cmd options
static llvm::cl::opt<std::string>
    OutputFile("o", cl::cat(MacvethCategory),
               llvm::cl::desc("Output file to write the code, otherwise "
                              "it will just print int std outputt"));
static llvm::cl::opt<std::string>
    CDAGInFile("input-cdag", cl::cat(MacvethCategory),
               llvm::cl::desc("Input file to read the custom CDAG placement"));

static llvm::cl::opt<MVISA>
    ISA("misa", llvm::cl::desc("Target ISA"), llvm::cl::init(MVISA::NATIVE),
        llvm::cl::cat(MacvethCategory),
        llvm::cl::values(clEnumValN(MVISA::NATIVE, "native",
                                    "Detect ISA of the architecture"),
                         clEnumValN(MVISA::AVX, "sse", "SSE ISA"),
                         clEnumValN(MVISA::AVX, "avx", "AVX ISA"),
                         clEnumValN(MVISA::AVX2, "avx2", "AVX2 ISA"),
                         clEnumValN(MVISA::AVX512, "avx512", "AVX512 ISA")));

static llvm::cl::opt<MVArch> Architecture(
    "march", llvm::cl::desc("Target architecture"),
    llvm::cl::init(MVArch::AUTODETECT), llvm::cl::cat(MacvethCategory),
    llvm::cl::values(
        clEnumValN(MVArch::AUTODETECT, "native", "Detect the architecture"),
        clEnumValN(MVArch::Nehalem, "nehalem",
                   "Intel Nehalem (2009) architecture (tock): SSE4.2"),
        clEnumValN(MVArch::Westmere, "westmere",
                   "Intel Westmere (2010) architecture (tick): SSE4.2"),
        clEnumValN(MVArch::SandyBridge, "sandybridge",
                   "Intel SandyBridge (2011) architecture (tock): AVX"),
        clEnumValN(MVArch::IvyBridge, "ivybridge",
                   "Intel IvyBridge (2012) architecture (tick): AVX"),
        clEnumValN(MVArch::Haswell, "haswell",
                   "Intel Haswell (2013) architecture (tock): AVX2"),
        clEnumValN(MVArch::Broadwell, "broadwell",
                   "Intel Broadwell (2014) architecture (tick): AVX2"),
        clEnumValN(MVArch::Skylake, "skylake",
                   "Intel Skylake (2015) architecture (tock): AVX512"),
        clEnumValN(MVArch::KabyLake, "kabylake",
                   "Intel Kaby Lake (2016) architecture (tock): AVX2"),
        clEnumValN(MVArch::CoffeeLake, "coffeelake",
                   "Intel Coffee Lake (2017) architecture (tock): AVX2"),
        clEnumValN(MVArch::CascadeLake, "cascadelake",
                   "Intel Cascade Lake (2019) architecture (tock): AVX512")

            ));

/// FMA support flag
static llvm::cl::opt<bool> FMA("fma",
                               llvm::cl::desc("Explicitly tell if FMA support"),
                               llvm::cl::init(false),
                               llvm::cl::cat(MacvethCategory));

/// Debug flag
static llvm::cl::opt<bool> Debug("debug",
                                 llvm::cl::desc("Print debug information"),
                                 llvm::cl::init(false),
                                 llvm::cl::cat(MacvethCategory));
/// Macro-free code
static llvm::cl::opt<bool> MacroFree(
    "macro-free",
    llvm::cl::desc("If set, do not use macro for generating vectorized code"),
    llvm::cl::init(false), llvm::cl::cat(MacvethCategory));

/// Output debug
static llvm::cl::opt<std::string>
    DebugFile("debug-file", cl::cat(MacvethCategory),
              llvm::cl::desc("Output file to print the debug information"));

// Main program
int main(int argc, const char **argv) {
  llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);

  // Parser for options common to all cmd-line Clang tools
  CommonOptionsParser Op(argc, argv, MacvethCategory);
  // Utility to run a FrontendAction over a set of files
  // * getCompilations(): contains compile cmd lines for the given source
  // paths
  // * getSourcePathList(): source files to run over
  ClangTool Tool(Op.getCompilations(), Op.getSourcePathList());

  /// MVOptions
  MVOptions::OutFile =
      OutputFile.getValue() == "" ? "macveth_output.c" : OutputFile.getValue();
  MVOptions::InCDAGFile = CDAGInFile.getValue();
  MVOptions::OutDebugFile = DebugFile.getValue();
  MVOptions::ISA = ISA.getValue();
  MVOptions::Arch = Architecture.getValue();
  MVOptions::FMASupport = FMA.getValue();
  MVOptions::Debug = Debug.getValue();
  MVOptions::MacroFree = MacroFree.getValue();

  /// Create needed files
  Utils::initFile(MVOptions::OutFile);
  Utils::initFile(MVOptions::OutDebugFile);

  // Runs ToolAction over all files specified in the cmd line
  // newFrontendActionFactory returns a new FrontendActionFactory for
  // a given type, in this case our FrontendAction, declared above
  return Tool.run(newFrontendActionFactory<MVFrontendAction>().get());
}
