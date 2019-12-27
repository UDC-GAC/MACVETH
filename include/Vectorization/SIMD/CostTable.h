/**
 * File              : CostTable.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 26 Dec 2019 09:55:43 MST
 * Last Modified Date: Ven 27 Dec 2019 09:28:26 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#ifndef MACVETH_COSTTABLE_H
#define MACVETH_COSTTABLE_H

#include <map>
#include <string.h>

namespace macveth {

/// Auxiliary class which holds the operations equivalences for the
/// different architectures
class CostTable {
private:
  /// Structure to define the table of costs for the architectures
  struct Row {
    /// Latency of the operation
    unsigned int Latency = 1;
    /// Throughput of the operation: number of cycles it takes to the processor
    /// to perform the calculations or operations of that instruction
    double Throughput = 1;
    /// Number of units available for executing this type of operation
    /// simultaneously
    unsigned int NUnits = 1;
    std::string Pattern = "";
  };

  /// Each Table identifies an architecture
  typedef std::map<std::string, Row> Table;

public:
  /// This acts like a database using the std::map to identify each arhitecture
  /// and its correspondant Table, which is basically another map where each
  /// operation identifies each Row, that holds information regarding the
  /// latency, througput, number of units, and the pattern
  static inline std::map<std::string, Table> Tables;

  /// Add Row to Arch Table taking into account all parameters described in the
  /// Row
  static void addRow(std::string Arch, std::string Op, unsigned int C,
                     double TP, unsigned int NU, std::string Pattern) {
    CostTable::Tables[Arch][Op] = {C, TP, NU, Pattern};
  }

  /// Get latency of operation given an architecture and the desired operation
  /// Op
  static unsigned int getLatency(std::string Arch, std::string Op) {
    return CostTable::Tables[Arch][Op].Latency;
  }

  /// Get throughput of operation given an architecture and the desired
  /// operation
  static double getThroughput(std::string Arch, std::string Op) {
    return CostTable::Tables[Arch][Op].Throughput;
  }

  /// Get number of units available for executing this type of operation
  static unsigned int getUnits(std::string Arch, std::string Op) {
    return CostTable::Tables[Arch][Op].NUnits;
  }

  /// Get Row given an architecture and the desired operation Op
  static std::string getPattern(std::string Arch, std::string Op) {
    return CostTable::Tables[Arch][Op].Pattern;
  }

  /// Add Row to Arch Table but with throughput 1 and number of units 1
  static void addRow(std::string Arch, std::string Op, unsigned int Cost,
                     std::string Pattern) {
    CostTable::Tables[Arch][Op] = {Cost, 1, 1, Pattern};
  }
};
} // namespace macveth

#endif
