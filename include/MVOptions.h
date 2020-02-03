#ifndef MACVETH_MVOPTIONS_H
#define MACVETH_MVOPTIONS_H

#include <string>

namespace macveth {
/// Supported architectures
enum MArch {
  /// TODO: detect architecture
  NATIVE = -1,
  /// SSE support
  SSE = 10,
  /// AVX support
  AVX = 20,
  /// AVX2 support
  AVX2 = 21,
  /// AVX512 support
  AVX512 = 22
};

/// Parse and hold information regarding CLI arguments
struct MVOptions {
  /// Name of the output file (-o=<file>)
  static inline std::string OutFile = "";
  /// Name of the input CDAG file (-input-cdag=<file>)
  static inline std::string InCDAGFile = "";
  /// Name of the output debug file (-output-debug=<file>)
  static inline std::string OutDebugFile = "";
  /// Target architecture
  static inline MArch Arch = MArch::NATIVE;
  /// FMA support
  static inline bool FMASupport = false;
  /// Debug
  static inline bool Debug = false;
  /// Generate or not macro-based code
  static inline bool MacroFree = false;
};

}; // namespace macveth

#endif
