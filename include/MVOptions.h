#ifndef MACVETH_MVOPTIONS_H
#define MACVETH_MVOPTIONS_H

#include "Vectorization/SIMD/SIMDGeneratorFactory.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;

namespace macveth {
// Set up the command line options
static llvm::cl::OptionCategory MacvethCategory("Macveth Options");
static llvm::cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
// Custom cmd options
static llvm::cl::opt<std::string>
    OutputFile("o", cl::cat(MacvethCategory),
               llvm::cl::desc("Output file to write the code, otherwise "
                              "it will just print int std outputt"));

/// Possible architectures
enum MArch {
  SSE = SIMDGeneratorFactory::Arch::AVX,
  AVX = SIMDGeneratorFactory::Arch::AVX,
  AVX2 = SIMDGeneratorFactory::Arch::AVX2,
  AVX512 = SIMDGeneratorFactory::Arch::AVX512,
  NATIVE = -1
};
static llvm::cl::opt<MArch> Architecture(
    "march", llvm::cl::desc("Target architecture"),
    llvm::cl::init(MArch::NATIVE), llvm::cl::cat(MacvethCategory),
    llvm::cl::values(clEnumValN(MArch::NATIVE, "native",
                                "Detect ISA of the architecture"),
                     clEnumValN(MArch::AVX, "sse", "SSE ISA"),
                     clEnumValN(MArch::AVX, "avx", "AVX ISA"),
                     clEnumValN(MArch::AVX2, "avx2", "AVX2 ISA"),
                     clEnumValN(MArch::AVX512, "avx512", "AVX512 ISA")));

/// FMA support flag
static llvm::cl::opt<bool> FMA("fma",
                               llvm::cl::desc("Explicitly tell if FMA support"),
                               llvm::cl::init(false),
                               llvm::cl::cat(MacvethCategory));

/// Debug flag
static llvm::cl::opt<bool> DEBUG("debug",
                                 llvm::cl::desc("Print debug information"),
                                 llvm::cl::init(false),
                                 llvm::cl::cat(MacvethCategory));

/// Parse and hold information regarding CLI arguments
struct MVOptions {
  /// Name of the output file (-o=<file>)
  static inline std::string OutFile = "";
  /// Target architecture
  static inline MArch Arch = MArch::NATIVE;
  /// FMA support
  static inline bool FMASupport = false;
  /// Debug
  static inline bool Debug = false;

  /// Parsing method for retrieving all the options
  static void parseOptions() {
    MVOptions::OutFile = OutputFile.getValue();
    MVOptions::Arch = Architecture.getValue();
    MVOptions::FMASupport = FMA.getValue();
    MVOptions::Debug = DEBUG.getValue();
  }
};

}; // namespace macveth

#endif
