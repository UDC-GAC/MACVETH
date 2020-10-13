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
  // EAX, EBX, ECX, EDX
  uint32_t Regs[4];

public:
  explicit CPUID(unsigned FuncId, unsigned SubFuncId) {
#ifdef _WIN32
    __cpuidex((int *)Regs, (int)FuncId, (int)SubFuncId);

#else
    asm volatile("cpuid"
                 : "=a"(Regs[0]), "=b"(Regs[1]), "=c"(Regs[2]), "=d"(Regs[3])
                 : "a"(FuncId), "c"(SubFuncId));
    // ECX is set to zero for CPUID function 4
#endif
  }

  const uint32_t &EAX() const { return Regs[0]; }
  const uint32_t &EBX() const { return Regs[1]; }
  const uint32_t &ECX() const { return Regs[2]; }
  const uint32_t &EDX() const { return Regs[3]; }
};

class CPUInfo {
public:
  CPUInfo() {
    // Get vendor name EAX=0
    CPUID CpuID0(0, 0);
    uint32_t HFS = CpuID0.EAX();
    VendorId += std::string((const char *)&CpuID0.EBX(), 4);
    VendorId += std::string((const char *)&CpuID0.EDX(), 4);
    VendorId += std::string((const char *)&CpuID0.ECX(), 4);
    // Get SSE instructions availability
    CPUID CpuID1(1, 0);
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
        for (int lvl = 0; lvl < MAX_INTEL_TOP_LVL; ++lvl) {
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
            NumCores = 1 + (CPUID(4, 0).EAX() >> 26) & 0x3F;
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
      // MVErr("Unexpected vendor id");
    }
    // Get processor brand std::string
    // This seems to be working for both Intel & AMD vendors
    for (int i = 0x80000002; i < 0x80000005; ++i) {
      CPUID CpuID(i, 0);
      ModelName += std::string((const char *)&CpuID.EAX(), 4);
      ModelName += std::string((const char *)&CpuID.EBX(), 4);
      ModelName += std::string((const char *)&CpuID.ECX(), 4);
      ModelName += std::string((const char *)&CpuID.EDX(), 4);
    }
  }
  std::string vendor() const { return VendorId; }
  std::string model() const { return ModelName; }
  int cores() const { return NumCores; }
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
  static const uint32_t SSE_POS = 0x02000000;
  static const uint32_t SSE2_POS = 0x04000000;
  static const uint32_t SSE3_POS = 0x00000001;
  static const uint32_t SSE41_POS = 0x00080000;
  static const uint32_t SSE42_POS = 0x00100000;
  static const uint32_t AVX_POS = 0x10000000;
  // EAX=7, ECX=0: Extended Features - EBX
  static const uint32_t AVX2_POS = 0x00000020;
  static const uint32_t AVX512F_POS = 0x00010000;
  static const uint32_t AVX512DQ_POS = 0x00020000;
  static const uint32_t AVX512IFMA_POS = 0x00200000;
  static const uint32_t AVX512PF_POS = 0x00400000;
  static const uint32_t AVX512ER_POS = 0x00800000;
  static const uint32_t AVX512CD_POS = 0x01000000;
  static const uint32_t AVX512BW_POS = 0x04000000;
  static const uint32_t AVX512VL_POS = 0x08000000;
  // EAX=7, ECX=0: Extended Features - ECX
  static const uint32_t AVX512VBMI_POS = 0x00000002;
  static const uint32_t AVX512VBMI2_POS = 0x00000040;
  static const uint32_t AVX512VNNI_POS = 0x00000800;
  static const uint32_t AVX512BITALG_POS = 0x00001000;
  // EAX=7, ECX=0: Extended Features - EDX
  static const uint32_t AVX5124VNNIW_POS = 0x00000004;
  static const uint32_t AVX5124VFMAPS_POS = 0x00000008;
  static const uint32_t AVX5124VP2INTERSECT_POS = 0x00000100;
  // EAX=7, ECX=1: Extended Features - EAX
  static const uint32_t AVX512BF16_POS = 0x00000020;

  static const uint32_t LVL_TYPE = 0x0000FF00;
  static const uint32_t LVL_CORES = 0x0000FFFF;

  // Attributes
  std::string VendorId = "";
  std::string ModelName = "";
  int NumSMT = 1;
  int NumCores = 1;
  int NumLogCpus = 1;
  float CPUMHz = 1000;
  bool IsHTT = false;
  bool IsSSE = false;
  bool IsSSE2 = false;
  bool IsSSE3 = false;
  bool IsSSE41 = false;
  bool IsSSE42 = false;
  bool IsAVX = false;
  bool IsAVX2 = false;
  bool IsAVX512F = false;
  bool IsAVX512DQ = false;
  bool IsAVX512IFMA = false;
  bool IsAVX512PF = false;
  bool IsAVX512ER = false;
  bool IsAVX512CD = false;
  bool IsAVX512BW = false;
  bool IsAVX512VL = false;
  bool IsAVX512VBMI = false;
  bool IsAVX512VBMI2 = false;
  bool IsAVX512VNNI = false;
  bool IsAVX512BITALG = false;
  bool IsAVX5124VNNIW = false;
  bool IsAVX5124FMAPS = false;
  bool IsAVX512VP2INTERSECT = false;
  bool IsAVX512BF16 = false;
};

} // namespace macveth
#endif