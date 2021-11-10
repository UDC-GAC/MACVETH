// MACVETH - PlcmntAlgo.h
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

#ifndef MACVETH_PLCMNTALGO_H
#define MACVETH_PLCMNTALGO_H

#include "include/CDAG.h"
#include "include/Node.h"

using namespace macveth;

namespace macveth {

class PlcmntAlgo {
public:
  /// Calculate free scheduling for a node (in-order)
  static void computeFreeSchedule(Node *N);

  /// Calculate free scheduling for a list of nodes (in-order)
  static void computeFreeSchedule(Node::NodeListType NL);

  /// Detect reductions in the list of input nodes
  static Node::NodeListType detectReductions(Node::NodeListType *NL);
  static void markReductions(Node::NodeListType *NL);

  /// Fuse reductions. Input **MUST** contain reductions in order to operate
  /// properly, in any other way it will not generate anything at all.
  static Node::NodeListType fuseReductions(Node::NodeListType NL);

  /// Set the placement from the input file
  static void setPlcmtFromFile(Node::NodeListType NL);

  /// Perform sorting of the list of nodes given
  static Node::NodeListType sortGraph(Node::NodeListType NL);

  /// Topological sort, very naive implementation
  static Node::NodeListType topSort(Node::NodeListType NL) {
    std::sort(NL.begin(), NL.end(),
              [](Node *Lhs, Node *Rhs) { return *Lhs < *Rhs; });
    return NL;
  }

  /// Breadth first search (BFS): this should perform badly in most cases
  static Node::NodeListType breadthFirstSearch(Node::NodeListType NL) {
    return NL;
  }
};

} // namespace macveth

#endif
