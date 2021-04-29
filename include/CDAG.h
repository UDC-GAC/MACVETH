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
//     Louis-Nöel Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Louis-Nöel Pouchet <pouchet@colostate.edu>

#ifndef MACVETH_CDAG_H
#define MACVETH_CDAG_H

#include "include/Node.h"
#include "include/TAC.h"

using namespace macveth;

namespace macveth {

/// Computation Directed Acyclic Graph (CDAG).
/// The CDAG is in charge of connecting nodes, detecting data races and,
/// therefore, depencies
class CDAG {
public:
  /// Structure for dependency mappings
  using DepMap = std::map<int, std::set<int>>;

  /// Given a list of TACs, create its correspondent CDAG
  static CDAG *createCDAGfromTAC(TacListType TL);

  /// Compute cost model for a set of Nodes
  static int computeCostModel(Node::NodeListType NL);

  /// Compute the free schedule or topology order
  static void computeFreeSchedule(CDAG *C);

  /// Compute the free schedule for a list of nodes (topology order)
  static void computeFreeSchedule(Node::NodeListType NL);

  /// Compute WAR (anti-dependencies) for a given node
  Node *findWARDataRace(Node *N, Node::NodeListType NL);

  /// Compute RAW (data dependencies) for a given node
  Node *findRAWDataRace(Node *N, Node::NodeListType NL);

  /// Get the node of list registered for this CDAG
  Node::NodeListType getNodeListOps() { return this->NLOps; }

  DepMap getRAWs() { return this->MapRAW; }
  DepMap getWARs() { return this->MapWAR; }

private:
  /// Insert TAC abstraction to the CDAG
  Node *insertTac(TAC T, Node::NodeListType L);
  /// List of OP nodes
  Node::NodeListType NLOps;
  /// List of memory nodes
  Node::NodeListType NLMem;
  /// RAW dependencies
  DepMap MapRAW;
  /// WAR dependencies
  DepMap MapWAR;
};

} // namespace macveth
#endif /* !MACVETH_CDAG_H */
