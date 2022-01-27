// MACVETH - CostTable.h
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

#ifndef MACVETH_COSTTABLE_H
#define MACVETH_COSTTABLE_H

#include "include/Utils.h"
#include <list>
#include <map>
#include <sstream>
#include <string>

namespace macveth {

/// Auxiliary class which holds the operations equivalences for the
/// different architectures
class CostTable {
public:
  /// Null value in generated tables
  static const inline std::string NullValue = "NA";
  /// Scalar value in generated tables
  static const inline std::string SeqValue = "seq";
  /// Unknown cost value
  static constexpr int UnknownCost = 10;

  /// Latency helper class to parse properly latencies format+
  struct Latency {
    /// Minimum latency
    unsigned int MinLatency = 0;
    /// Maximum latency
    unsigned int MaxLatency = 0;
    /// Address latency
    unsigned int AddrLatency = 0;
    /// Memory latency
    unsigned int MemLatency = 0;

    /// FIXME: Get latency
    unsigned int getLatency() {
      if ((MinLatency > MaxLatency) || (MinLatency == MaxLatency)) {
        /// Is this garbage? Yyyyyes sir
        return MinLatency;
      }
      return (MaxLatency + MinLatency) / 2;
    }

    /// Get memory or address latency
    unsigned int getLatency(std::string K) {
      if ((K == "addr") && (AddrLatency != 0)) {
        return AddrLatency;
      }
      if ((K == "mem") && (MemLatency != 0)) {
        return MemLatency;
      }
      return getLatency();
    }

    /// Empty constructor
    Latency(){};

    /// Latency constructor based on format: # | [#;#] | [#;(#,#)] where # is a
    /// integer
    Latency(std::string L) {
      if (L == NullValue) {
        return;
      }
      auto open = L.find("[");
      auto close = L.find("]") - 1;
      if (open++ == std::string::npos) {
        // # case
        MinLatency = MaxLatency = std::stoi(L.c_str());
        return;
      } else {
        // [#;something] case
        std::string W;
        auto SubStr = L.substr(open, close);
        std::stringstream TMP(SubStr);
        while (getline(TMP, W, ';')) {
          auto open = W.find("(");
          auto close = W.find(")") - 1;
          if (open++ == std::string::npos) {
            if (MinLatency == 0) {
              MinLatency = std::stoi(W.c_str());
            } else {
              MaxLatency = std::stoi(W.c_str());
            }
            continue;
          }
          std::string Win;
          auto SubSubStr = W.substr(open, close);
          std::stringstream TMPin(SubSubStr);
          while (getline(TMPin, Win, ',')) {
            if (AddrLatency == 0) {
              AddrLatency = std::stoi(Win.c_str());
            } else {
              MemLatency = std::stoi(Win.c_str());
            }
          }
        }
      }
    };
  };

  /// This is a future feature, if is implemented any port algorithm
  struct Ports {
    /// Number of micro-instructions
    unsigned int NUops = 0;
    /// List of usable ports
    std::vector<int> P;
  };

  using PortList = std::vector<Ports>;

  /// Structure to define the table of costs for the architectures
  struct Row {
    /// MVOP
    std::string MVOP = "";
    /// Intrinsics
    std::string Intrinsics = "";
    /// XED iform
    /// More documentation on:
    std::string XED_iform = "";
    /// ASM
    std::string Asm = "";
    /// CPUID
    std::string CPUID = "";
    /// Latency of the operation
    Latency Lat;
    /// Throughput of the operation: number of cycles it takes to the processor
    /// to perform the calculations or operations of that instruction
    double Throughput = 0;
    /// Number of micro-instructions issued
    int NUops = 1;
    /// Number of ports
    PortList NPorts = {};

    /// Empty constructor for initialization reasons
    Row(){};

    Row(std::string MVOP, std::string Intrinsics, std::string XED_iform,
        std::string Asm, std::string CPUID, Latency Lat, double Throughput,
        int NUops)
        : MVOP(MVOP), Intrinsics(Intrinsics), XED_iform(XED_iform), Asm(Asm),
          CPUID(CPUID), Lat(Lat), Throughput(Throughput), NUops(NUops){};
  };

  /// Each Table identifies an architecture
  using Table = std::map<std::string, Row>;

  /// Table map for intrinsics
  static inline Table Tintrin;

  /// Table map for MVOP
  static inline Table TMVOP;

  /// Table map for XED iform
  static inline Table TXED;

  /// Get row of table for intrinsics
  static CostTable::Row getOpInfo(std::string Op) {
    return CostTable::Tintrin[Op];
  }

  /// Get row of table for intrinsics
  static CostTable::Row getOpInfoMVOP(std::string Op) {
    return CostTable::TMVOP[Op];
  }

  /// Get latency according to the XED iform
  static unsigned int getLatency(std::string Op) {
    return CostTable::TXED[Op].Lat.getLatency();
  }

  /// Get latency of operation given an architecture and the desired intrinsics
  /// instruction, e.g. _mm256_set_pd()
  static unsigned int getLatencyIntrin(std::string Op) {
    if (CostTable::Tintrin[Op].Asm == SeqValue) {
      unsigned int Lat = 0;
      if (CostTable::Tintrin[Op].XED_iform == SeqValue) {
        return UnknownCost;
      }
      std::string W;
      std::stringstream TMP(CostTable::Tintrin[Op].XED_iform);
      while (getline(TMP, W, ';')) {
        Lat += CostTable::getLatency(W);
      }
      return Lat;
    }
    return CostTable::Tintrin[Op].Lat.getLatency();
  }

  /// Get latency according to the MACVETH's operation
  static unsigned int getLatencyMVOP(std::string Op) {
    if (CostTable::TMVOP.find(Op) != CostTable::TMVOP.end()) {
      return CostTable::getLatency(CostTable::TMVOP[Op].XED_iform);
    }
    return UnknownCost;
  }

  static Row getIntrinRow(std::string Intrin) {
    return CostTable::Tintrin[Intrin];
  }

  static Row getMVOPRow(std::string MVOP) { return CostTable::TMVOP[MVOP]; }

  /// Add row to tables. This is an entry point, we will have different tables
  /// according to the type of instruction we want to store
  static void addRow(std::string MVOP, std::string Intrinsics, std::string ASM,
                     std::string XED_iform, std::string Lat, std::string Th,
                     std::string Uops, std::string Ports) {
    Row R(MVOP, Intrinsics, XED_iform, ASM, "", Latency(Lat),
          std::atof(Th.c_str()), std::atoi(Uops.c_str()));
    if (MVOP != NullValue) {
      CostTable::TMVOP[MVOP] = R;
    }
    if (Intrinsics != NullValue) {
      CostTable::Tintrin[Intrinsics] = R;
    }
    if (XED_iform != SeqValue) {
      CostTable::TXED[XED_iform] = R;
    }
  }
};

} // namespace macveth

#endif /* !MACVETH_COSTTABLE_H */
