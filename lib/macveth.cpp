/*
 * File                  : lib/macveth.cpp
 * Author                : Marcos Horro
 * Date                  : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 04:06 +02:00
 * Modified By   : Marcos Horro (marcos.horro@udc.gal>)
 *
 * MIT License
 *
 * Copyright (c) 2020 Colorado State University
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
                              "it will just print int std output"));
static llvm::cl::opt<std::string>
    SIMDInfoFile("simd-info", cl::cat(MacvethCategory),
                 llvm::cl::desc("Report with all the SIMD information"));
static llvm::cl::opt<std::string> SIMDInfoMissedFile(
    "simd-info-missed", cl::cat(MacvethCategory),
    llvm::cl::desc("Report with all missed SIMD opportunities"));
static llvm::cl::opt<std::string> TargetFunc("func", cl::cat(MacvethCategory),
                                             llvm::cl::desc("Target function"));
static llvm::cl::opt<std::string>
    CDAGInFile("input-cdag", cl::cat(MacvethCategory),
               llvm::cl::desc("Input file to read the custom CDAG placement"));

static llvm::cl::opt<MVSIMDCostModel> SIMDCostModel(
    "simd-cost-model", llvm::cl::desc("SIMD cost model"),
    llvm::cl::init(MVSIMDCostModel::CONSERVATIVE), cl::cat(MacvethCategory),
    llvm::cl::values(
        clEnumValN(MVSIMDCostModel::CONSERVATIVE, "conservative",
                   "Vectorize if and only if the sequential estimation is "
                   "worse than the vectorized"),
        clEnumValN(MVSIMDCostModel::AGGRESSIVE, "aggressive",
                   "Vectorize partially if benefitial according to cost model"),
        clEnumValN(MVSIMDCostModel::UNLIMITED, "unlimited",
                   "Unlimited SIMD cost, i.e. vectorize regardless the cost")));

static llvm::cl::opt<MVCPUInfo::MVISA>
    ISA("misa", llvm::cl::desc("Target ISA"),
        llvm::cl::init(MVCPUInfo::MVISA::AUTODETECT),
        llvm::cl::cat(MacvethCategory),
        llvm::cl::values(clEnumValN(MVCPUInfo::MVISA::AUTODETECT, "native",
                                    "Detect ISA of the architecture"),
                         clEnumValN(MVCPUInfo::MVISA::SSE, "sse", "SSE ISA"),
                         clEnumValN(MVCPUInfo::MVISA::AVX, "avx", "AVX ISA"),
                         clEnumValN(MVCPUInfo::MVISA::AVX2, "avx2", "AVX2 ISA"),
                         clEnumValN(MVCPUInfo::MVISA::AVX512, "avx512",
                                    "AVX512 ISA")));

static llvm::cl::opt<MVCPUInfo::MVArch> Architecture(
    "march", llvm::cl::desc("Target architecture"),
    llvm::cl::init(MVCPUInfo::MVArch::NATIVE), llvm::cl::cat(MacvethCategory),
    llvm::cl::values(
        clEnumValN(MVCPUInfo::MVArch::NATIVE, "native",
                   "Detect the architecture"),
        clEnumValN(MVCPUInfo::MVArch::Nehalem, "nehalem",
                   "Intel Nehalem (2009) architecture (tock): SSE4.2"),
        clEnumValN(MVCPUInfo::MVArch::Westmere, "westmere",
                   "Intel Westmere (2010) architecture (tick): SSE4.2"),
        clEnumValN(MVCPUInfo::MVArch::SandyBridge, "sandybridge",
                   "Intel SandyBridge (2011) architecture (tock): AVX"),
        clEnumValN(MVCPUInfo::MVArch::IvyBridge, "ivybridge",
                   "Intel IvyBridge (2012) architecture (tick): AVX"),
        clEnumValN(MVCPUInfo::MVArch::Haswell, "haswell",
                   "Intel Haswell (2013) architecture (tock): AVX2"),
        clEnumValN(MVCPUInfo::MVArch::Broadwell, "broadwell",
                   "Intel Broadwell (2014) architecture (tick): AVX2"),
        clEnumValN(MVCPUInfo::MVArch::Skylake, "skylake",
                   "Intel Skylake (2015) architecture (tock): AVX512"),
        clEnumValN(MVCPUInfo::MVArch::KabyLake, "kabylake",
                   "Intel Kaby Lake (2016) architecture (tock): AVX2"),
        clEnumValN(MVCPUInfo::MVArch::CoffeeLake, "coffeelake",
                   "Intel Coffee Lake (2017) architecture (tock): AVX2"),
        clEnumValN(MVCPUInfo::MVArch::CascadeLake, "cascadelake",
                   "Intel Cascade Lake (2019) architecture (tock): AVX512"),
        clEnumValN(MVCPUInfo::MVArch::IceLake, "icelake",
                   "Intel Ice Lake (2020) architecture (tick): AVX512"),
        clEnumValN(MVCPUInfo::MVArch::Zen, "zen",
                   "AMD Zen (2019) architecture: AVX2"),
        clEnumValN(MVCPUInfo::MVArch::Zen2, "zen2",
                   "AMD Zen 2 (2020) architecture: AVX2"),
        clEnumValN(MVCPUInfo::MVArch::AMDDef, "amd",
                   "AMD architecture not specified"),
        clEnumValN(MVCPUInfo::MVArch::IntelDef, "intel",
                   "Intel architecture not specified")));

/// FMA support flag
static llvm::cl::opt<bool> FMA("fma",
                               llvm::cl::desc("Explicitly tell if FMA support"),
                               llvm::cl::init(false),
                               llvm::cl::cat(MacvethCategory));

/// Disable Intrinsics SVML
static llvm::cl::opt<bool> NoSVML("no-svml",
                                  llvm::cl::desc("Disable Intrinsics SVML"),
                                  llvm::cl::init(false),
                                  llvm::cl::cat(MacvethCategory));

/// Disable FMA support flag
static llvm::cl::opt<bool> DisableFMA(
    "nofma",
    llvm::cl::desc("Explicitly tell to not generate FMA instructions even if "
                   "architecture supports them"),
    llvm::cl::init(false), llvm::cl::cat(MacvethCategory));

/// Debug flag
static llvm::cl::opt<bool> Debug("debug-mv",
                                 llvm::cl::desc("Print debug information"),
                                 llvm::cl::init(false),
                                 llvm::cl::cat(MacvethCategory));

/// Reformat code
static llvm::cl::opt<bool> NoFormatCode(
    "no-format",
    llvm::cl::desc("MACVETH by defaults reformats code as "
                   "clang-format does, using LLVM style. If this option is "
                   "enabled, then no reformatting is applied"),
    llvm::cl::init(false), llvm::cl::cat(MacvethCategory));

/// Do not include headers in top of the file
static llvm::cl::opt<bool> NoHeaders(
    "no-headers",
    llvm::cl::desc(
        "If set, do *not* include header files, such as <immintrin.h>"),
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
  MVOptions::DisableFMA = DisableFMA.getValue();
  MVOptions::Debug = Debug.getValue();
  MVOptions::NoReformat = NoFormatCode.getValue();
  // MVOptions::MacroCode = MacroCode.getValue();
  MVOptions::Headers = !NoHeaders.getValue();
  MVOptions::SIMDCostModel = SIMDCostModel.getValue();
  MVOptions::IntrinsicsSVML = !NoSVML.getValue();
  MVOptions::TargetFunc = TargetFunc.getValue();

  // Check if there are incompatible options
  MVOptions::validateOptions();

  /// Create needed files
  Utils::initFile(MVOptions::OutFile);
  Utils::initFile(MVOptions::OutDebugFile);

  // Runs ToolAction over all files specified in the cmd line
  // newFrontendActionFactory returns a new FrontendActionFactory for
  // a given type, in this case our FrontendAction, declared above
  return Tool.run(newFrontendActionFactory<MVFrontendAction>().get());
}
