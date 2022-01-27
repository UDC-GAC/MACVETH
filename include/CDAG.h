// MACVETH - CDAG.h
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

#ifndef MACVETH_CDAG_H
#define MACVETH_CDAG_H

#include "include/Node.h"
#include "include/TAC.h"

using namespace macveth;

namespace macveth {

/// Structure for dependency mappings
using DepMap = std::map<int, std::set<int>>;

/// Computation Directed Acyclic Graph (CDAG).
/// The CDAG is in charge of connecting nodes, detecting data races and,
/// therefore, depencies
class CDAG {
public:
  /// Given a list of TACs, create its correspondent CDAG
  static CDAG createCDAGfromTAC(const TacListT &TL);

  /// Compute WAR (anti-dependencies) for a given node
  Node *findWARDataRace(Node *N, NodeVectorT &NL);

  /// Compute RAW (data dependencies) for a given node
  Node *findRAWDataRace(Node *N, NodeVectorT &NL);

  /// Get the node of list registered for this CDAG
  NodeVectorT getNodeListOps() { return NLOps; }

  DepMap getRAWs() { return MapRAW; }
  DepMap getWARs() { return MapWAR; }

private:
  /// Insert TAC abstraction to the CDAG
  Node *insertTac(TAC T, NodeVectorT L);
  /// List of OP nodes
  NodeVectorT NLOps;
  // /// List of memory nodes
  // NodeVectorT NLMem;
  /// RAW dependencies
  DepMap MapRAW;
  /// WAR dependencies
  DepMap MapWAR;
};

} // namespace macveth
#endif /* !MACVETH_CDAG_H */
