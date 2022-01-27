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
//     Louis-Noël Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Marcos Horro <marcos.horro@udc.es>

#ifndef MACVETH_MVOPTIONS_H
#define MACVETH_MVOPTIONS_H

#include "include/CPUID.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Host.h"
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
    {MVCPUInfo::MVArch::nehalem, "nehalem"},
    {MVCPUInfo::MVArch::westmere, "westmere"},
    {MVCPUInfo::MVArch::sandybridge, "sandybridge"},
    {MVCPUInfo::MVArch::ivybridge, "ivybridge"},
    {MVCPUInfo::MVArch::haswell, "haswell"},
    {MVCPUInfo::MVArch::broadwell, "broadwell"},
    {MVCPUInfo::MVArch::skylake, "skylake"},
    {MVCPUInfo::MVArch::kabylake, "kabylake"},
    {MVCPUInfo::MVArch::coffeelake, "coffeelake"},
    {MVCPUInfo::MVArch::cascadelake, "cascadelake"},
    {MVCPUInfo::MVArch::icelake, "icelake"},
    {MVCPUInfo::MVArch::znver1, "znver1"},
    {MVCPUInfo::MVArch::znver2, "znver2"},
    {MVCPUInfo::MVArch::znver3, "znver3"},
    {MVCPUInfo::MVArch::AMDDef, "znver1"},
    {MVCPUInfo::MVArch::IntelDef, "cascadelake"}};

static std::map<std::string, MVCPUInfo::MVArch> MVStrArch = {
    {"Default", MVCPUInfo::MVArch::DEFAULT},
    {"nehalem", MVCPUInfo::MVArch::nehalem},
    {"westmere", MVCPUInfo::MVArch::westmere},
    {"sandybridge", MVCPUInfo::MVArch::sandybridge},
    {"ivybridge", MVCPUInfo::MVArch::ivybridge},
    {"haswell", MVCPUInfo::MVArch::haswell},
    {"broadwell", MVCPUInfo::MVArch::broadwell},
    {"skylake", MVCPUInfo::MVArch::skylake},
    {"kabylake", MVCPUInfo::MVArch::kabylake},
    {"coffeelake", MVCPUInfo::MVArch::coffeelake},
    {"cascadelake", MVCPUInfo::MVArch::cascadelake},
    {"icelake", MVCPUInfo::MVArch::icelake},
    {"znver1", MVCPUInfo::MVArch::znver1},
    {"znver2", MVCPUInfo::MVArch::znver2},
    {"znver3", MVCPUInfo::MVArch::znver3},
    {"znver1", MVCPUInfo::MVArch::AMDDef},
    {"cascadelake", MVCPUInfo::MVArch::IntelDef}};

/// Mapping between the ISA and supported architectures
static std::map<MVCPUInfo::MVISA, std::vector<MVCPUInfo::MVArch>>
    SupportedISAArch = {
        {MVCPUInfo::MVISA::SSE,
         {MVCPUInfo::MVArch::nehalem, MVCPUInfo::MVArch::westmere,
          MVCPUInfo::MVArch::sandybridge, MVCPUInfo::MVArch::ivybridge,
          MVCPUInfo::MVArch::haswell, MVCPUInfo::MVArch::broadwell,
          MVCPUInfo::MVArch::skylake, MVCPUInfo::MVArch::kabylake,
          MVCPUInfo::MVArch::coffeelake, MVCPUInfo::MVArch::cascadelake,
          MVCPUInfo::MVArch::icelake, MVCPUInfo::MVArch::znver1,
          MVCPUInfo::MVArch::znver2, MVCPUInfo::MVArch::znver3,
          MVCPUInfo::MVArch::AMDDef, MVCPUInfo::MVArch::IntelDef}},
        {MVCPUInfo::MVISA::AVX,
         {MVCPUInfo::MVArch::sandybridge, MVCPUInfo::MVArch::ivybridge,
          MVCPUInfo::MVArch::haswell, MVCPUInfo::MVArch::broadwell,
          MVCPUInfo::MVArch::skylake, MVCPUInfo::MVArch::kabylake,
          MVCPUInfo::MVArch::coffeelake, MVCPUInfo::MVArch::cascadelake,
          MVCPUInfo::MVArch::icelake, MVCPUInfo::MVArch::znver1,
          MVCPUInfo::MVArch::znver2, MVCPUInfo::MVArch::znver3,
          MVCPUInfo::MVArch::AMDDef, MVCPUInfo::MVArch::IntelDef}},
        {MVCPUInfo::MVISA::AVX2,
         {MVCPUInfo::MVArch::haswell, MVCPUInfo::MVArch::broadwell,
          MVCPUInfo::MVArch::skylake, MVCPUInfo::MVArch::kabylake,
          MVCPUInfo::MVArch::coffeelake, MVCPUInfo::MVArch::cascadelake,
          MVCPUInfo::MVArch::icelake, MVCPUInfo::MVArch::znver1,
          MVCPUInfo::MVArch::znver2, MVCPUInfo::MVArch::znver3,
          MVCPUInfo::MVArch::AMDDef, MVCPUInfo::MVArch::IntelDef}},
        {MVCPUInfo::MVISA::AVX512,
         {MVCPUInfo::MVArch::skylake, MVCPUInfo::MVArch::cascadelake,
          MVCPUInfo::MVArch::icelake, MVCPUInfo::MVArch::AMDDef,
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
  /// Fuse reductions
  static inline bool FuseReductions = false;
  /// Disable the vectorization of orphan reductions
  static inline bool DisableVectOrphanRedux = false;
  /// Reductions windows size
  static inline int ReduxWinSize = 16;
  /// Minimum number of reductions that can be packed together
  static inline int MinReduxSize = 2;

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
      auto HostArch = std::string(llvm::sys::getHostCPUName());
      assert(
          (MVStrArch.find(HostArch) != MVStrArch.end()) &&
          "Architecture not supported: try skylake, broadwell, znver1, etc.");
      MVOptions::Arch = MVStrArch[HostArch];
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