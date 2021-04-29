// MACVETH - MVOptions.h
//
// Copyright (c) Colorado State University. 2019-2021
// Copyright (c) Universidade da Coruña. 2020-2021
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Authors:
//     Marcos Horro <marcos.horro@udc.es>
//     Louis-Nöel Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Louis-Nöel Pouchet <pouchet@colostate.edu>

#ifndef MACVETH_MVOPTIONS_H
#define MACVETH_MVOPTIONS_H

#include "include/CPUID.h"

#include <algorithm>
#include <cassert>
#include <list>
#include <map>
#include <string>

namespace macveth {
/// Costs models naming
enum MVSIMDCostModel {
  /// Vectorize always, despite the not vectorized estimation cost
  UNLIMITED,
  /// If the sequential code vs the SIMD-zed code has a better estimation, then
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
static std::map<MVCPUInfo::MVISA, std::string> MVISAStr = {
    {MVCPUInfo::MVISA::SSE, "SSE"},
    {MVCPUInfo::MVISA::AVX, "AVX"},
    {MVCPUInfo::MVISA::AVX2, "AVX2"},
    {MVCPUInfo::MVISA::AVX512, "AVX512"}};

static std::map<MVCPUInfo::MVArch, std::string> MVArchStr = {
    {MVCPUInfo::MVArch::DEFAULT, "Default"},
    {MVCPUInfo::MVArch::Nehalem, "Nehalem"},
    {MVCPUInfo::MVArch::Westmere, "Westmere"},
    {MVCPUInfo::MVArch::SandyBridge, "SandyBridge"},
    {MVCPUInfo::MVArch::IvyBridge, "IvyBridge"},
    {MVCPUInfo::MVArch::Haswell, "Haswell"},
    {MVCPUInfo::MVArch::Broadwell, "Broadwell"},
    {MVCPUInfo::MVArch::Skylake, "Skylake"},
    {MVCPUInfo::MVArch::KabyLake, "KabyLake"},
    {MVCPUInfo::MVArch::CoffeeLake, "CoffeeLake"},
    {MVCPUInfo::MVArch::CascadeLake, "CascadeLake"},
    {MVCPUInfo::MVArch::IceLake, "IceLake"},
    {MVCPUInfo::MVArch::Zen, "Zen"},
    {MVCPUInfo::MVArch::Zen2, "Zen2"},
    {MVCPUInfo::MVArch::AMDDef, "Zen"},
    {MVCPUInfo::MVArch::IntelDef, "Broadwell"}};

/// Mapping between the ISA and supported architectures
static std::map<MVCPUInfo::MVISA, std::list<MVCPUInfo::MVArch>>
    SupportedISAArch = {
        {MVCPUInfo::MVISA::SSE,
         {MVCPUInfo::MVArch::Nehalem, MVCPUInfo::MVArch::Westmere,
          MVCPUInfo::MVArch::SandyBridge, MVCPUInfo::MVArch::IvyBridge,
          MVCPUInfo::MVArch::Haswell, MVCPUInfo::MVArch::Broadwell,
          MVCPUInfo::MVArch::Skylake, MVCPUInfo::MVArch::KabyLake,
          MVCPUInfo::MVArch::CoffeeLake, MVCPUInfo::MVArch::CascadeLake,
          MVCPUInfo::MVArch::IceLake, MVCPUInfo::MVArch::Zen,
          MVCPUInfo::MVArch::Zen2, MVCPUInfo::MVArch::AMDDef,
          MVCPUInfo::MVArch::IntelDef}},
        {MVCPUInfo::MVISA::AVX,
         {MVCPUInfo::MVArch::SandyBridge, MVCPUInfo::MVArch::IvyBridge,
          MVCPUInfo::MVArch::Haswell, MVCPUInfo::MVArch::Broadwell,
          MVCPUInfo::MVArch::Skylake, MVCPUInfo::MVArch::KabyLake,
          MVCPUInfo::MVArch::CoffeeLake, MVCPUInfo::MVArch::CascadeLake,
          MVCPUInfo::MVArch::IceLake, MVCPUInfo::MVArch::Zen,
          MVCPUInfo::MVArch::Zen2, MVCPUInfo::MVArch::AMDDef,
          MVCPUInfo::MVArch::IntelDef}},
        {MVCPUInfo::MVISA::AVX2,
         {MVCPUInfo::MVArch::Haswell, MVCPUInfo::MVArch::Broadwell,
          MVCPUInfo::MVArch::Skylake, MVCPUInfo::MVArch::KabyLake,
          MVCPUInfo::MVArch::CoffeeLake, MVCPUInfo::MVArch::CascadeLake,
          MVCPUInfo::MVArch::IceLake, MVCPUInfo::MVArch::Zen,
          MVCPUInfo::MVArch::Zen2, MVCPUInfo::MVArch::AMDDef,
          MVCPUInfo::MVArch::IntelDef}},
        {MVCPUInfo::MVISA::AVX512,
         {MVCPUInfo::MVArch::Skylake, MVCPUInfo::MVArch::CascadeLake,
          MVCPUInfo::MVArch::IceLake, MVCPUInfo::MVArch::AMDDef,
          MVCPUInfo::MVArch::IntelDef}}};

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
  /// Name of the output debug file (-simd-info=<file>)
  static inline std::string SIMDReportFile = "";
  /// Name of the output debug file (-simd-info-missed=<file>)
  static inline std::string SIMDReportMissFile = "";
  /// Format style output
  static inline std::string Style = "";
  /// Fallback format style output
  static inline std::string FallbackStyle = "";
  /// Target ISA
  static inline MVCPUInfo::MVISA ISA = MVCPUInfo::MVISA::AUTODETECT;
  /// Target architecture
  static inline MVCPUInfo::MVArch Arch = MVCPUInfo::MVArch::DEFAULT;
  /// FMA support
  static inline bool FMASupport = false;
  /// Disable FMA support
  static inline bool DisableFMA = false;
  /// Enable use of scatter instruction
  static inline bool ScatterInstruction = false;
  /// Print debug information
  static inline bool Debug = false;
  /// Debug level
  static inline DebugLevel DLevel = DebugLevel::ALL;
  /// No reformatting the final code
  static inline bool NoReformat = false;
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

  static MVCPUInfo::MVArch getMVArch() {
    MVCPUInfo CInfo;
    return CInfo.getArchitecture();
  }

  static MVCPUInfo::MVISA getMVISA() {
    MVCPUInfo CInfo;
    if (CInfo.isAVX512F()) {
      // FIXME: this is not correct
      return MVCPUInfo::MVISA::AVX512;
    }
    if (CInfo.isAVX2()) {
      return MVCPUInfo::MVISA::AVX2;
    }
    if (CInfo.isAVX()) {
      return MVCPUInfo::MVISA::AVX;
    }
    if (CInfo.isSSE42()) {
      return MVCPUInfo::MVISA::SSE;
    }
    return MVCPUInfo::MVISA::AVX2;
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
    if (MVOptions::ISA == MVCPUInfo::MVISA::AUTODETECT) {
      MVOptions::ISA = getMVISA();
    }
    if (MVOptions::Arch == MVCPUInfo::MVArch::NATIVE) {
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