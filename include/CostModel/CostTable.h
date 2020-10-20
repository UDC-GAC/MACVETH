/*
 * File					 : include/CostModel/CostTable.h
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:33 +02:00
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

#ifndef MACVETH_COSTTABLE_H
#define MACVETH_COSTTABLE_H

#include <map>
#include <string.h>

namespace macveth {

/// Auxiliary class which holds the operations equivalences for the
/// different architectures
class CostTable {
public:
  /// Structure to define the table of costs for the architectures
  struct Row {
    /// Latency of the operation
    unsigned int Latency = 1;
    /// Throughput of the operation: number of cycles it takes to the processor
    /// to perform the calculations or operations of that instruction
    double Throughput = 0;
    /// Number of units available for executing this type of operation
    /// simultaneously
    unsigned int NUnits = 0;
    /// Number of micro-instructions issued
    unsigned int NUops = 1;
    /// Pattern of the instruction
    std::string Pattern = "";
  };

  /// Each Table identifies an architecture
  using Table = std::map<std::string, Row>;

  /// This acts like a database using the std::map to identify each arhitecture
  /// and its correspondant Table, which is basically another map where each
  /// operation identifies each Row, that holds information regarding the
  /// latency, througput, number of units, and the pattern
  // static inline std::map<std::string, Table> Tables;
  static inline Table Tables;

  /// Get row of table
  static CostTable::Row getRow(std::string Arch, std::string Op) {
    return CostTable::Tables[Op];
  }

  /// Get latency of operation given an architecture and the desired operation
  /// Op
  static unsigned int getLatency(std::string Arch, std::string Op) {
    return CostTable::Tables[Op].Latency;
  }

  /// Get throughput of operation given an architecture and the desired
  /// operation
  static double getThroughput(std::string Arch, std::string Op) {
    return CostTable::Tables[Op].Throughput;
  }

  /// Get number of units available for executing this type of operation
  static unsigned int getUnits(std::string Arch, std::string Op) {
    return CostTable::Tables[Op].NUnits;
  }

  /// Get Row given an architecture and the desired operation Op
  static std::string getPattern(std::string Arch, std::string Op) {
    return CostTable::Tables[Op].Pattern;
  }

  /// Add Row to Arch Table but with throughput 1 and number of units 1
  static void addRow(std::string Arch, std::string Op, unsigned int Cost,
                     std::string Pattern) {
    CostTable::Tables[Op] = {Cost, 0, 0, 1, Pattern};
  }

  /// Add Row to Arch Table taking into account all parameters described in
  /// the Row
  static void addRow(std::string Arch, std::string Op, unsigned int C,
                     double TP, unsigned int NU, std::string Pattern) {
    CostTable::Tables[Op] = {C, TP, NU, 1, Pattern};
  }
  /// Add Row to Arch Table taking into account all parameters described in
  /// the Row
  static void addRow(std::string Arch, std::string Op, unsigned int C,
                     double TP, unsigned int NU, unsigned int NUops,
                     std::string Pattern) {
    CostTable::Tables[Op] = {C, TP, NU, NUops, Pattern};
  }
};
} // namespace macveth

#endif /* !MACVETH_COSTTABLE_H */
