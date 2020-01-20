#ifndef MACVETH_MVOPTIONS_H
#define MACVETH_MVOPTIONS_H

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
enum MArch { SSE, AVX, AVX2, AVX512, NATIVE };
static llvm::cl::opt<MArch> Architecture(
    "march", llvm::cl::desc("Target architecture"),
    llvm::cl::init(MArch::NATIVE), llvm::cl::cat(MacvethCategory),
    llvm::cl::values(clEnumValN(MArch::SSE, "sse", "SSE ISA"),
                     clEnumValN(MArch::AVX, "avx", "AVX ISA"),
                     clEnumValN(MArch::AVX2, "avx2", "AVX2 ISA"),
                     clEnumValN(MArch::AVX512, "avx512", "AVX512 ISA"),
                     clEnumValN(MArch::NATIVE, "native",
                                "Detect ISA of the architecture")));

/// Parse and hold information regarding CLI arguments
struct MVOptions {
  /// Name of the output file (-o=<file>)
  static inline std::string OutFile = "";
  /// Target architecture
  static inline MArch Arch = MArch::NATIVE;

  /// Parsing method for retrieving all the options
  static void parseOptions() {
    MVOptions::OutFile = OutputFile.getValue();
    MVOptions::Arch = Architecture.getValue();
  }
};

}; // namespace macveth

#endif
