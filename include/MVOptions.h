/*
 * File					 : include/MVOptions.h
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:38 +02:00
 * Modified By	 : Marcos Horro (marcos.horro@udc.gal>)
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

#ifndef MACVETH_MVOPTIONS_H
#define MACVETH_MVOPTIONS_H

#include "include/CPUID.h"

#include <algorithm>
#include <cassert>
#include <list>
#include <map>
#include <string>

namespace macveth {
/// Supported ISA
enum MVISA {
  /// TODO: detect ISA
  AUTODETECT = -1,
  /// SSE support
  SSE = 10,
  /// AVX support
  AVX = 20,
  /// AVX2 support
  AVX2 = 21,
  /// AVX512 support
  AVX512 = 22
};

/// Costs models naming
enum MVSIMDCostModel {
  /// Vectorize always, despite the not vectorized estimation cost
  UNLIMITED,
  /// If the sequential code vs the SIMDized code has a better estimation, then
  /// MACVETH does not vectorize the code
  CONSERVATIVE,
  /// Region to vectorize can have parts where vectorization provides better
  /// performance
  AGGRESSIVE
};

/// Map costs models to strings
static std::map<MVSIMDCostModel, std::string> CostModelStr = {
    {UNLIMITED, "unlimited"},
    {CONSERVATIVE, "conservative"},
    {AGGRESSIVE, "conservative"}};

/// Mapping ISAs
static std::map<MVISA, std::string> MVISAStr = {
    {SSE, "SSE"}, {AVX, "AVX"}, {AVX2, "AVX2"}, {AVX512, "AVX512"}};

/// Supported architectures. This enum is util for computing the cost model, as
/// different architectures may differ in the latencies and thoughputs of
/// operations for the same ISA
enum MVArch {
  /// TODO: Autodetect the architecture underlying
  NATIVE,
  /// Default or not specified
  DEFAULT,
  /// 1st-gen Intel Nehalem (2009) architecture (tock): SSE4.2
  Nehalem,
  /// 1st-gen (and 2) Intel Westmere (2010) architecture (tick): SSE4.2
  Westmere,
  /// 2nd-gen Intel Sandy Bridge (2011) architecture (tock): AVX
  SandyBridge,
  /// 3rd-gen Intel Ivy Bridge (2012) architecture (tick): AVX
  IvyBridge,
  /// 4th-gen Intel Haswell (2013) architecture (tock): AVX2
  Haswell,
  /// 5th-gen Intel Broadwell (2014) architecture (tick): AVX2
  Broadwell,
  /// 6th-gen Intel Skylake (2015) architecture (tock): AVX2, AVX512
  /// (Skylake-SP)
  Skylake,
  /// 7th-gen Intel Kaby Lake (2016) architecture: AVX2
  KabyLake,
  /// 8-9th-gen Intel Coffee Lake (2017) architecture: AVX2
  CoffeeLake,
  /// 8th-gen (server) Intel Cascade Lake (2019) architecture: AVX512
  CascadeLake,
  /// 10th-gen Intel Cascade Lake (2019) architecture: AVX512
  IceLake,
  /// AMD Zen architecture: AVX2
  Zen,
  /// AMD Zen2 architecture: AVX2
  Zen2,
  /// AMD default
  AMDDef,
  /// Intel Default
  IntelDef,
};

static std::map<MVArch, std::string> MVArchStr = {{DEFAULT, "Default"},
                                                  {Nehalem, "Nehalem"},
                                                  {Westmere, "Westmere"},
                                                  {SandyBridge, "SandyBridge"},
                                                  {IvyBridge, "IvyBridge"},
                                                  {Haswell, "Haswell"},
                                                  {Broadwell, "Broadwell"},
                                                  {Skylake, "Skylake"},
                                                  {KabyLake, "KabyLake"},
                                                  {CoffeeLake, "CoffeeLake"},
                                                  {CascadeLake, "CascadeLake"},
                                                  {IceLake, "IceLake"},
                                                  {Zen, "Zen"},
                                                  {Zen2, "Zen2"},
                                                  {AMDDef, "Zen"},
                                                  {IntelDef, "Broadwell"}};

/// Mapping between the ISA and supported architectures
static std::map<MVISA, std::list<MVArch>> SupportedISAArch = {
    {SSE,
     {Nehalem, Westmere, SandyBridge, IvyBridge, Haswell, Broadwell, Skylake,
      KabyLake, CoffeeLake, CascadeLake, IceLake, Zen, Zen2, AMDDef, IntelDef}},
    {AVX,
     {SandyBridge, IvyBridge, Haswell, Broadwell, Skylake, KabyLake, CoffeeLake,
      CascadeLake, IceLake, Zen, Zen2, AMDDef, IntelDef}},
    {AVX2,
     {Haswell, Broadwell, Skylake, KabyLake, CoffeeLake, CascadeLake, IceLake,
      Zen, Zen2, AMDDef, IntelDef}},
    {AVX512, {Skylake, CascadeLake, IceLake, AMDDef, IntelDef}}};

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
  /// Name of the output debug file (-report-file=<file>)
  static inline std::string ReportFile = "";
  /// Target ISA
  static inline MVISA ISA = MVISA::AUTODETECT;
  /// Target architecture
  static inline MVArch Arch = MVArch::DEFAULT;
  /// FMA support
  static inline bool FMASupport = false;
  /// Disable FMA support
  static inline bool DisableFMA = false;
  /// Debug
  static inline bool Debug = false;
  /// Debug level
  static inline DebugLevel DLevel = DebugLevel::ALL;
  /// Generate or not macro-based code
  static inline bool MacroCode = false;
  /// Include headers (--no-headers)
  static inline bool Headers = true;
  /// SIMD Cost model
  static inline MVSIMDCostModel SIMDCostModel = MVSIMDCostModel::CONSERVATIVE;
  /// Intrinsics Short Vector Math Library (SVML)
  static inline bool IntrinsicsSVML = true;
  /// Target function
  static inline std::string TargetFunc = "";

  static MVArch getMVArch() {
    CPUInfo CInfo;
    // TODO: parse vendor string
    return MVArch::Broadwell;
  }

  static MVISA getMVISA() {
    CPUInfo CInfo;
    if (CInfo.isAVX512F()) {
      // TODO: this is not correct
      return MVISA::AVX512;
    }
    if (CInfo.isAVX2()) {
      return MVISA::AVX2;
    }
    if (CInfo.isAVX()) {
      return MVISA::AVX;
    }
    if (CInfo.isSSE42()) {
      return MVISA::SSE;
    }
    return MVISA::AVX2;
  }

  /// Print options as a string for reports and so
  static std::string toString() {
    std::string S = "-------------------------------\n";
    S += "\toutput file\t= " + OutFile + "\n";
    S += "\tCDAG file\t= " + InCDAGFile + "\n";
    S += "\toutput debug\t= " + OutDebugFile + "\n";
    S += "\tISA and arch\t= " + MVISAStr[ISA] + ", " + MVArchStr[Arch] + "\n";
    S += "\tFMA enabled\t= " + std::to_string(!DisableFMA) + "\n";
    S += "\tSIMD cost model\t= " + CostModelStr[SIMDCostModel] + "\n";
    S += "\tSVML enabled\t= " + std::to_string(IntrinsicsSVML) + "\n";
    S += "-------------------------------\n";
    return S;
  }

  /// Main function to check options of the compiler
  static void validateOptions() {
    if (MVOptions::ISA == MVISA::AUTODETECT) {
      MVOptions::ISA = getMVISA();
    }
    if (MVOptions::Arch == MVArch::NATIVE) {
      MVOptions::Arch = getMVArch();
    }
    checkIfArchISACompatible();
  }

  /// Dumb method to check compatibility between options
  static void checkIfArchISACompatible() {
    auto LArch = SupportedISAArch[MVOptions::ISA];
    assert((std::find(LArch.begin(), LArch.end(), MVOptions::Arch) !=
            LArch.end()) &&
           "Incompatible architecture and ISA");
    /// Check incompatible options:
    assert(
        !(MVOptions::FMASupport && MVOptions::DisableFMA) &&
        "FMA support enabled and disabled at the same time is not possible!");
  }
};
}; // namespace macveth

#endif /* !MACVETH_MVOPTIONS_H */