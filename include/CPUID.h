// MACVETH - CPUID.h
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

#ifndef MACVETH_CPUID_H
#define MACVETH_CPUID_H

#include <algorithm>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <intrin.h>
#include <limits.h>
typedef unsigned __int32 uint32_t;
#else
#include <stdint.h>
#endif

#define MAX_INTEL_TOP_LVL 4

namespace macveth {

class CPUID {
  /// EAX, EBX, ECX, EDX
  uint32_t Regs[4];

public:
  explicit CPUID(unsigned FuncId, unsigned SubFuncId) {
    // ECX is set to zero for CPUID function 4
#ifdef _WIN32
    __cpuidex((int *)Regs, (int)FuncId, (int)SubFuncId);
#else
    asm volatile("cpuid"
                 : "=a"(Regs[0]), "=b"(Regs[1]), "=c"(Regs[2]), "=d"(Regs[3])
                 : "a"(FuncId), "c"(SubFuncId));
#endif
  }

  /// Return EAX register
  const uint32_t &EAX() const { return Regs[0]; }
  /// Return EBX register
  const uint32_t &EBX() const { return Regs[1]; }
  /// Return ECX register
  const uint32_t &ECX() const { return Regs[2]; }
  /// Return EDX register
  const uint32_t &EDX() const { return Regs[3]; }
};

class MVCPUInfo {
public:
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

  /// Supported architectures. This enum is util for computing the cost model,
  /// as different architectures may differ in the latencies and thoughputs of
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

  MVCPUInfo() {
    // Get vendor name EAX=0
    CPUID CpuID0(0, 0);
    uint32_t HFS = CpuID0.EAX();
    VendorId += std::string((const char *)&CpuID0.EBX(), 4);
    VendorId += std::string((const char *)&CpuID0.EDX(), 4);
    VendorId += std::string((const char *)&CpuID0.ECX(), 4);

    // Get processor brand std::string
    // This seems to be working for both Intel & AMD vendors
    for (auto i = 0x80000002; i < 0x80000005; ++i) {
      CPUID CpuID(i, 0);
      ModelName += std::string((const char *)&CpuID.EAX(), 4);
      ModelName += std::string((const char *)&CpuID.EBX(), 4);
      ModelName += std::string((const char *)&CpuID.ECX(), 4);
      ModelName += std::string((const char *)&CpuID.EDX(), 4);
    }

    // Get SSE instructions availability
    CPUID CpuID1(1, 0);
    ArchName = getArchitectureFromCPUID(CpuID1);
    IsHTT = CpuID1.EDX() & AVX_POS;
    IsSSE = CpuID1.EDX() & SSE_POS;
    IsSSE2 = CpuID1.EDX() & SSE2_POS;
    IsSSE3 = CpuID1.ECX() & SSE3_POS;
    IsSSE41 = CpuID1.ECX() & SSE41_POS;
    IsSSE42 = CpuID1.ECX() & SSE41_POS;
    IsAVX = CpuID1.ECX() & AVX_POS;
    // Get AVX2 instructions availability
    CPUID CpuID7(7, 0);
    IsAVX2 = CpuID7.EBX() & AVX2_POS;
    // Get AVX512 instructions availability EBX
    IsAVX512F = CpuID7.EBX() & AVX512F_POS;
    IsAVX512DQ = CpuID7.EBX() & AVX512DQ_POS;
    IsAVX512IFMA = CpuID7.EBX() & AVX512IFMA_POS;
    IsAVX512BW = CpuID7.EBX() & AVX512BW_POS;
    IsAVX512VL = CpuID7.EBX() & AVX512VL_POS;
    // Get AVX512 instructions availability ECX
    IsAVX512VBMI = CpuID7.ECX() & AVX512VBMI_POS;
    IsAVX512VBMI2 = CpuID7.ECX() & AVX512VBMI2_POS;
    IsAVX512VNNI = CpuID7.ECX() & AVX512VNNI_POS;
    IsAVX512BITALG = CpuID7.ECX() & AVX512BITALG_POS;
    // Get AVX512 instructions availability EDX
    IsAVX5124VNNIW = CpuID7.EDX() & AVX5124VNNIW_POS;
    IsAVX5124FMAPS = CpuID7.EDX() & AVX5124VFMAPS_POS;
    IsAVX512VP2INTERSECT = CpuID7.EDX() & IsAVX512VP2INTERSECT;

    CPUID CpuID71(7, 1);
    IsAVX512VP2INTERSECT = CpuID71.EAX() & AVX512BF16_POS;

    std::string UpVId = VendorId;
    for_each(UpVId.begin(), UpVId.end(), [](char &in) { in = ::toupper(in); });
    // Get num of cores
    if (UpVId.find("INTEL") != std::string::npos) {
      if (HFS >= 11) {
        for (auto lvl = 0; lvl < MAX_INTEL_TOP_LVL; ++lvl) {
          CPUID CpuID4(0x0B, lvl);
          uint32_t currLevel = (LVL_TYPE & CpuID4.ECX()) >> 8;
          switch (currLevel) {
          case 0x01:
            NumSMT = LVL_CORES & CpuID4.EBX();
            break;
          case 0x02:
            NumLogCpus = LVL_CORES & CpuID4.EBX();
            break;
          default:
            break;
          }
        }
        NumCores = NumLogCpus / NumSMT;
      } else {
        if (HFS >= 1) {
          NumLogCpus = (CpuID1.EBX() >> 16) & 0xFF;
          if (HFS >= 4) {
            NumCores = 1 + ((CPUID(4, 0).EAX() >> 26) & 0x3F);
          }
        }
        if (IsHTT) {
          if (!(NumCores > 1)) {
            NumCores = 1;
            NumLogCpus = (NumLogCpus >= 2 ? NumLogCpus : 2);
          }
        } else {
          NumCores = NumLogCpus = 1;
        }
      }
    } else if (UpVId.find("AMD") != std::string::npos) {
      if (HFS >= 1) {
        NumLogCpus = (CpuID1.EBX() >> 16) & 0xFF;
        if (CPUID(0x80000000, 0).EAX() >= 8) {
          NumCores = 1 + (CPUID(0x80000008, 0).ECX() & 0xFF);
        }
      }
      if (IsHTT) {
        if (!(NumCores > 1)) {
          NumCores = 1;
          NumLogCpus = (NumLogCpus >= 2 ? NumLogCpus : 2);
        }
      } else {
        NumCores = NumLogCpus = 1;
      }
    } else {
      // FIXME: do something for ARM for instance...
    }
  }
  /// TODO: Get architecture name from values in EAX
  MVArch getArchitectureFromCPUID(CPUID C) { return MVArch::IntelDef; }
  /// Get architecture name
  MVArch getArchitecture() { return ArchName; }
  /// Get vendor name
  std::string vendor() const { return VendorId; }
  /// Get model name
  std::string model() const { return ModelName; }
  /// Get number of cores
  int cores() const { return NumCores; }
  /// Get speed in MHz
  float cpuSpeedInMHz() const { return CPUMHz; }
  bool isSSE() const { return IsSSE; }
  bool isSSE2() const { return IsSSE2; }
  bool isSSE3() const { return IsSSE3; }
  bool isSSE41() const { return IsSSE41; }
  bool isSSE42() const { return IsSSE42; }
  bool isAVX() const { return IsAVX; }
  bool isAVX2() const { return IsAVX2; }
  bool isAVX512F() const { return IsAVX512F; }
  bool isAVX512CD() const { return IsAVX512CD; }
  bool isAVX512ER() const { return IsAVX512ER; }
  bool isAVX512PF() const { return IsAVX512PF; }
  bool isHyperThreaded() const { return IsHTT; }
  int logicalCpus() const { return NumLogCpus; }

private:
  // Bit positions for data extractions
  // EAX=1: Processor Info and Feature Bits - EBX
  static constexpr uint32_t SSE_POS = 0x02000000;
  static constexpr uint32_t SSE2_POS = 0x04000000;
  static constexpr uint32_t SSE3_POS = 0x00000001;
  static constexpr uint32_t SSE41_POS = 0x00080000;
  static constexpr uint32_t SSE42_POS = 0x00100000;
  static constexpr uint32_t AVX_POS = 0x10000000;
  // EAX=7, ECX=0: Extended Features - EBX
  static constexpr uint32_t AVX2_POS = 0x00000020;
  static constexpr uint32_t AVX512F_POS = 0x00010000;
  static constexpr uint32_t AVX512DQ_POS = 0x00020000;
  static constexpr uint32_t AVX512IFMA_POS = 0x00200000;
  static constexpr uint32_t AVX512PF_POS = 0x00400000;
  static constexpr uint32_t AVX512ER_POS = 0x00800000;
  static constexpr uint32_t AVX512CD_POS = 0x01000000;
  static constexpr uint32_t AVX512BW_POS = 0x04000000;
  static constexpr uint32_t AVX512VL_POS = 0x08000000;
  // EAX=7, ECX=0: Extended Features - ECX
  static constexpr uint32_t AVX512VBMI_POS = 0x00000002;
  static constexpr uint32_t AVX512VBMI2_POS = 0x00000040;
  static constexpr uint32_t AVX512VNNI_POS = 0x00000800;
  static constexpr uint32_t AVX512BITALG_POS = 0x00001000;
  // EAX=7, ECX=0: Extended Features - EDX
  static constexpr uint32_t AVX5124VNNIW_POS = 0x00000004;
  static constexpr uint32_t AVX5124VFMAPS_POS = 0x00000008;
  static constexpr uint32_t AVX5124VP2INTERSECT_POS = 0x00000100;
  // EAX=7, ECX=1: Extended Features - EAX
  static constexpr uint32_t AVX512BF16_POS = 0x00000020;

  static constexpr uint32_t LVL_TYPE = 0x0000FF00;
  static constexpr uint32_t LVL_CORES = 0x0000FFFF;

  /// Name of the architecture, e.g. "broadwell", "skylake", "knights landing"
  MVArch ArchName = MVArch::Broadwell;
  /// 12-character string which represent, basically, the manufacturer, e.g.
  /// "AMDisbetter!", "AuthenticAMD", "GenuineIntel", etc.
  std::string VendorId = "";
  /// Should work for AMD and Intel and represents the name of the processor
  std::string ModelName = "";
  /// Number of threads
  int NumSMT = 1;
  /// Number of physical cores
  int NumCores = 1;
  /// Number of logical cores
  int NumLogCpus = 1;
  /// CPU frequency in MHz
  float CPUMHz = 1000;
  /// CPU with hyper-threading capability
  bool IsHTT = false;
  /// SSE flag
  bool IsSSE = false;
  /// SSE2 flag
  bool IsSSE2 = false;
  /// SSE3 flag
  bool IsSSE3 = false;
  /// SSE4.1 flag
  bool IsSSE41 = false;
  /// SSE4.1 flag
  bool IsSSE42 = false;
  /// AVX flag
  bool IsAVX = false;
  /// AVX2 flag
  bool IsAVX2 = false;
  /// AVX-512F flag
  bool IsAVX512F = false;
  /// AVX-512DQ flag
  bool IsAVX512DQ = false;
  /// AVX-512IFMA flag
  bool IsAVX512IFMA = false;
  /// AVX-512PF flag
  bool IsAVX512PF = false;
  /// AVX-512ER flag
  bool IsAVX512ER = false;
  /// AVX-512CD flag
  bool IsAVX512CD = false;
  /// AVX-512BW flag
  bool IsAVX512BW = false;
  /// AVX-512VL flag
  bool IsAVX512VL = false;
  /// AVX-512VBMI flag
  bool IsAVX512VBMI = false;
  /// AVX-512VBMI2 flag
  bool IsAVX512VBMI2 = false;
  /// AVX-512VBMI flag
  bool IsAVX512VNNI = false;
  /// AVX-512BITALG flag
  bool IsAVX512BITALG = false;
  /// AVX-5124VNNIW flag
  bool IsAVX5124VNNIW = false;
  /// AVX-5124FMAPS flag
  bool IsAVX5124FMAPS = false;
  /// AVX-512VP2INTERSECT flag
  bool IsAVX512VP2INTERSECT = false;
  /// AVX-512BF16 flag
  bool IsAVX512BF16 = false;
};

} // namespace macveth
#endif /* !MACVETH_CPUID_H */