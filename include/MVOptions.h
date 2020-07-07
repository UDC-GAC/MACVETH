#ifndef MACVETH_MVOPTIONS_H
#define MACVETH_MVOPTIONS_H

#include <algorithm>
#include <cassert>
#include <list>
#include <map>
#include <string>

namespace macveth {
/// Supported ISA
enum MVISA {
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

static std::map<MVISA, std::string> MVISAStr = {
    {SSE, "SSE"}, {AVX, "AVX"}, {AVX2, "AVX2"}, {AVX512, "AVX512"}};

/// Supported architectures. This enum is util for computing the cost model, as
/// different architectures may differ in the latencies and thoughputs of
/// operations for the same ISA
enum MVArch {
  /// TODO Autodetect the architecture underlying
  AUTODETECT,
  /// Intel Nehalem (2009) architecture (tock): SSE4.2
  Nehalem,
  /// Intel Westmere (2010) architecture (tick): SSE4.2
  Westmere,
  /// Intel Sandy Bridge (2011) architecture (tock): AVX
  SandyBridge,
  /// Intel Ivy Bridge (2012) architecture (tick): AVX
  IvyBridge,
  /// Intel Haswell (2013) architecture (tock): AVX2
  Haswell,
  /// Intel Broadwell (2014) architecture: AVX2
  Broadwell,
  /// Intel Skylake (2015) architecture: AVX2, AVX512 (Skylake-SP)
  Skylake,
  /// Intel Kaby Lake (2016) architecture: AVX2
  KabyLake,
  /// Intel Coffee Lake (2017) architecture: AVX2
  CoffeeLake,
  /// Intel Cascade Lake (2019) architecture: AVX512
  CascadeLake,
  /// AMD Zen architecture: AVX2
  Zen,
};

static std::map<MVArch, std::string> MVArchStr = {{Nehalem, "Nehalem"},
                                                  {Westmere, "Westmere"},
                                                  {SandyBridge, "SandyBridge"},
                                                  {IvyBridge, "IvyBridge"},
                                                  {Haswell, "Haswell"},
                                                  {Broadwell, "Broadwell"},
                                                  {Skylake, "Skylake"},
                                                  {KabyLake, "KabyLake"},
                                                  {CoffeeLake, "CoffeeLake"},
                                                  {CascadeLake, "CascadeLake"},
                                                  {Zen, "Zen"}};

/// Mapping between the ISA and supported architectures
static std::map<MVISA, std::list<MVArch>> SupportedISAArch = {
    {SSE,
     {Nehalem, Westmere, SandyBridge, IvyBridge, Haswell, Broadwell, Skylake,
      KabyLake, CoffeeLake, CascadeLake}},
    {AVX,
     {SandyBridge, IvyBridge, Haswell, Broadwell, Skylake, KabyLake, CoffeeLake,
      CascadeLake}},
    {AVX2,
     {Haswell, Broadwell, Skylake, KabyLake, CoffeeLake, CascadeLake, Zen}},
    {AVX512, {Skylake, CascadeLake}}};

enum DebugLevel {
  /// All levels below
  ALL = 0,
  /// Very very verbose when creating the CDAG, TACs, etc.
  LOW = 1,
  /// Checkpoints such as: TACs created, etc.
  MID = 2,
  /// For concrete functions
  HIGH = 3,
  /// When you are testing something
  TEST = 4
};

/// Parse and hold information regarding CLI arguments
struct MVOptions {
  /// Name of the output file (-o=<file>)
  static inline std::string OutFile = "";
  /// Name of the input CDAG file (-input-cdag=<file>)
  static inline std::string InCDAGFile = "";
  /// Name of the output debug file (-output-debug=<file>)
  static inline std::string OutDebugFile = "";
  /// Target ISA
  static inline MVISA ISA = MVISA::NATIVE;
  /// Target architecture
  static inline MVArch Arch = MVArch::AUTODETECT;
  /// FMA support
  static inline bool FMASupport = false;
  /// Disable FMA support
  static inline bool DisableFMA = false;
  /// Debug
  static inline bool Debug = false;
  /// Debug
  static inline DebugLevel DLevel = DebugLevel::ALL;
  /// Generate or not macro-based code
  static inline bool MacroCode = false;
  /// Include headers (--no-headers)
  static inline bool Headers = true;

  /// Main function to check options of the compiler
  static void validateOptions() {
    if (MVOptions::ISA == MVISA::NATIVE) {
    }
    if (MVOptions::Arch == MVArch::AUTODETECT) {
    }
    checkIfArchISACompatible();
  }

  /// Dumb method to check compatibility between options
  static void checkIfArchISACompatible() {
    if ((MVOptions::ISA == MVISA::NATIVE) ||
        (MVOptions::Arch == MVArch::AUTODETECT)) {
      return;
    }
    auto LArch = SupportedISAArch[MVOptions::ISA];
    assert((std::find(LArch.begin(), LArch.end(), MVOptions::Arch) !=
            LArch.end()) &&
           "Incompatible architecture and ISA");
  }
};
}; // namespace macveth

#endif /* !MACVETH_MVOPTIONS_H */