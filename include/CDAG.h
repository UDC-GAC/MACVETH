/*
 * File					 : include/CDAG.h
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:39 +02:00
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

#ifndef MACVETH_CDAG_H
#define MACVETH_CDAG_H

#include "include/Node.h"
#include "include/TAC.h"

using namespace macveth;

namespace macveth {

/// Computation Directed Acyclic Graph (CDAG).
/// The CDAG is in charge of connecting nodes, detecting data races and,
/// therefore depencies
class CDAG {
public:
  using DepMap = std::map<int, std::set<int>>;

  /// Given a list of TACs, create its correspondent CDAG
  static CDAG *createCDAGfromTAC(TacListType TL);

  /// Compute cost model for a set of Nodes
  static int computeCostModel(Node::NodeListType NL);

  /// Compute the free schedule or topology order
  static void computeFreeSchedule(CDAG *C);

  /// Compute the free schedule for a list of nodes (topology order)
  static void computeFreeSchedule(Node::NodeListType NL);

  /// Compute WAR anti-dependencies for a given node
  Node *findWARDataRace(Node *N, Node::NodeListType NL);

  /// Compute RAW dependencies for a given node
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
