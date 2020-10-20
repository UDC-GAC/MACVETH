/*
 * File					 : include/PlcmntAlgo.h
 * Author				 : Marcos Horro
 * Date					 : Tue 14 Jul 2020 05:03 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:37 +02:00
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
