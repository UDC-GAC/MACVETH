/*
 * File					 : include/Node.h
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
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

#ifndef MACVETH_NODE_H
#define MACVETH_NODE_H

#include "include/MVExpr/MVExpr.h"
#include "include/MVExpr/MVExprLiteral.h"
#include "include/MVExpr/MVExprVar.h"
#include "include/StmtWrapper.h"
#include "include/TAC.h"
#include "clang/AST/Expr.h"

using namespace macveth;

namespace macveth {

/// All Nodes belong to a CDAG. Each node or vertex holds information regarding
/// the type of operation as well as its Edges (or links to another Nodes).
/// Nodes also hold information regarding the scheduling, i.e. the order on
/// which they are consumed.
class Node {
public:
  /// Simply universal identifier for each instance node. We have to be cautious
  /// with this value: it **does not** identify the object itself. Thus,
  /// whenever this object is cloned, this value is also cloned to the new
  /// object
  static inline int UUID = 0;

  /// Restart UUID numbering
  static void restart() { Node::UUID = 0; }

  /// Definition of NodeListType
  using NodeListType = std::vector<Node *>;

  /// Available types of nodes
  enum NodeType { NODE_MEM, NODE_STORE, NODE_OP, UNDEF };

  /// Types of stores for NODE_OP Node
  enum OutputType { TEMP_STORE, MEM_STORE };

  /// Holds information regarding the scheduling for this Node
  struct SchedInfo {
    /// Unique ID of the Statement/Node
    int NodeID = -1;
    /// TAC order
    int TacID = 0;
    /// Unroll factor
    int UnrollFactor = -1;
    /// Scope within program
    std::vector<int> Scop = {0};
    /// Topological order of this node
    int FreeSched = 0;
    /// TODO this value should be calculated by an algorithm
    int Plcmnt = 0;
    /// TODO this value stands for the scheduling order of this node; should
    /// be also calculated when the placement
    int Sched = 0;
    /// If this node belongs to a reduction
    bool IsReduction = false;
  };

  /// If the node is NODE_OP type, then it will hold a result value
  struct OutputInfo {
    /// MVExpr of output
    MVExpr *E;
    /// If the target variable is in memory it will be a MEM_STORE; TEMP_STORE
    /// the other way
    OutputType Type = MEM_STORE;
    /// Opcode
    MVOp MVOP;
    /// Tell if it is binary or not
    bool IsBinaryOp = false;
  };

  /// Get the output info given a TAC
  OutputInfo setOutputInfo(TAC T) {
    OutputInfo O;
    O.E = T.getA();
    O.Type = (T.getMVOP().isAssignment()) ? MEM_STORE : TEMP_STORE;
    O.MVOP = T.getMVOP();
    O.IsBinaryOp = T.getMVOP().getType() == MVOpType::CLANG_BINOP;
    return O;
  }

  void setNodeAsReduction() { this->SI.IsReduction = true; }
  void setNodeAsNonReduction() { this->SI.IsReduction = false; }
  bool belongsToAReduction() { return this->SI.IsReduction; }

  /// Setting the scheduling info without any other information
  void setSchedInfo() { this->SI.NodeID = Node::UUID++; }

  /// Update the output expression
  void setOutputExpr(MVExpr *E) { this->O.E = E; }

  /// Setting the scheduling info without any other information than the TAC
  void setSchedInfo(TAC T) {
    this->SI.NodeID = Node::UUID++;
    this->SI.TacID = T.getTacID();
    this->SI.UnrollFactor = T.getUnrollFactor();
    // FIXME: vector of scops
    this->SI.Scop[0] = T.getScop();
  }

  /// When creating a Node from a TempExpr, the connections will be created by
  /// the TAC which creates this Node
  Node(MVExpr *TE) {
    this->T = NODE_MEM;
    this->Value = TE->getExprStr();
    this->MV = TE;
    setSchedInfo();
  }

  /// Copy constructor for cloning
  Node(const Node &Rhs) {
    this->I = Rhs.I;
    this->O = Rhs.O;
    this->OutNodes = Rhs.OutNodes;
    this->SI = Rhs.SI;
    this->T = Rhs.T;
    this->Value = Rhs.Value;
    this->LoopName = Rhs.LoopName;
  }

  /// This is the unique constructor for nodes, as we will creating Nodes from
  /// CDAG, so each TAC corresponds to a = b op c
  Node(TAC T) {
    this->T = NODE_OP;
    this->MV = nullptr;
    this->Value = T.getMVOP().toString();
    this->O = setOutputInfo(T);
    setSchedInfo(T);
    this->LoopName = T.getLoopName();
    connectInput(new Node(T.getB()));
    if (T.getC() != nullptr) {
      connectInput(new Node(T.getC()));
    }
    updateIfStore();
  }

  /// This is the unique constructor for nodes, as we will creating Nodes from
  /// CDAG, so each TAC corresponds to a = b op c
  Node(TAC T, NodeListType L) {
    this->T = NODE_OP;
    this->MV = nullptr;
    this->Value = T.getMVOP().toString();
    this->O = setOutputInfo(T);
    setSchedInfo(T);
    this->LoopName = T.getLoopName();
    auto NB = findOutputNode(T.getB()->getExprStr(), L);
    if (NB == nullptr || this->getScop()[0] != NB->getScop()[0]) {
      connectInput(new Node(T.getB()));
    } else {
      connectInput(NB);
    }
    Node *NC = nullptr;
    if (T.getC() != nullptr) {
      NC = findOutputNode(T.getC()->getExprStr(), L);
      if (NC == nullptr || this->getScop()[0] != NC->getScop()[0]) {
        connectInput(new Node(T.getC()));
      } else {
        connectInput(NC);
      }
    }
    updateIfStore();
  }

  /// Update output information of the node if the type of the output is a
  /// memory store
  void updateIfStore() {
    if (this->O.Type == MEM_STORE) {
      this->getInputs().front()->T = NODE_STORE;
    }
    for (auto I : this->getInputs()) {
      if (I->T == NodeType::NODE_MEM) {
        I->SI.Scop = this->getScop();
        I->SI.TacID = this->getTacID();
        I->SI.UnrollFactor = this->getUnrollFactor();
      }
    }
  }

  /// Get MVExpr
  MVExpr *getMVExpr() { return this->MV; }

  /// Schedule info is needed for the algorithms to perform permutations in
  /// nodes
  void setFreeSchedInfo(int FS) { this->SI.FreeSched = FS; }

  /// Set the TAC order
  void setTacID(int TacID) { this->SI.TacID = TacID; }

  /// Set Plcmnt value
  void setPlcmnt(int Plcmnt) { this->SI.Plcmnt = Plcmnt; }

  /// Check if Node N is already in node list L
  static Node *findOutputNode(std::string NodeName, NodeListType L);

  /// Connect a Node as input
  void connectInput(Node *N);

  /// Get the scope of the node
  std::vector<int> getScop() { return this->SI.Scop; }

  /// Get the number of inputs in this Node
  int getOutputNum() { return this->OutNodes.size(); }
  /// Get the output Nodes
  NodeListType getOutputNodes() { return this->OutNodes; }
  /// Get the output information
  OutputInfo getOutputInfo() { return this->O; }
  /// Get the output information
  std::string getOutputInfoName() { return this->O.E->toString(); }
  /// Get the number of inputs in this Node
  int getInputNum() { return this->I.size(); }
  /// Get the list of node inputs in this Node
  NodeListType getInputs() { return this->I; }
  /// Get info regarding its scheduling
  SchedInfo getSchedInfo() { return this->SI; }
  /// Get TAC order
  int getTacID() { return this->SI.TacID; }
  /// Get unrolling factor
  int getUnrollFactor() { return this->SI.UnrollFactor; }
  /// Set the type of the node
  void setNodeType(NodeType T) { this->T = T; }

  /// Get data type of MVExpr as string
  std::string getDataType() {
    return (this->MV == nullptr) ? O.E->getTypeStr() : MV->getTypeStr();
  }

  /// Checks if output list is zero
  bool hasOutNodes();
  /// Checks if input list is zero
  bool hasInputs();
  /// Checks if output list is not zero
  bool isTerminal();
  /// Checks if given node is in the output list
  bool hasOutNode(Node *N);

  /// Is OP
  bool needsMemLoad() {
    return ((this->T == NODE_MEM) && (!dyn_cast<MVExprLiteral>(this->MV)) &&
            (this->MV->needsToBeLoaded()));
  }

  /// Check if Node type is NODE_STORE
  bool isStoreNodeOp() { return (this->T == NODE_STORE); }

  /// Get the node type
  NodeType getNodeType() { return this->T; }

  /// Returns the string value of the node
  std::string getValue() const { return this->Value; }

  /// Returns the variable/register value
  std::string getRegisterValue() const {
    if (this->T != NodeType::NODE_MEM) {
      return this->O.E->toString();
    }
    return this->getValue();
  }

  /// Setting the name of the output value
  // void setOutputName(std::string S) { this->O.Name = S; }

  /// Get loop name of the loop
  std::string getLoopName() { return this->LoopName; }

  /// Print node: debugging purposes
  std::string toString();

  /// Print node in a short fashion
  std::string toStringShort();

  /// This is useful for calculating the difference between two Nodes in terms
  /// of addresses, in some way
  int operator-(const Node &N) { return (*MV - *N.MV); }

  /// Two nodes are equal if and only if they have the same value. Seems
  /// pretty straight forward but it must be defined someway.
  bool operator==(const Node &N) {
    return (getRegisterValue() == N.getRegisterValue());
  }

  bool operator==(const Node *N) {
    return (getRegisterValue() == N->getRegisterValue());
  }

  /// Two nodes are not equal if and only if they have the same value. Seems
  /// pretty straight forward but it must be defined someway.
  bool operator!=(const Node &N) { return !operator==(N); }

  /// For sorting lists of nodes
  bool operator<(const Node &N) {
    if (this->getSchedInfo().FreeSched == N.SI.FreeSched) {
      if (this->getSchedInfo().TacID == N.SI.TacID) {
        return (this->getSchedInfo().NodeID < N.SI.NodeID);
      } else {
        return (this->getSchedInfo().TacID < N.SI.TacID);
      }
    } else {
      return (this->getSchedInfo().FreeSched < N.SI.FreeSched);
    }
  }

  /// For debugging purposes: show information regarding the node
  std::string getSchedInfoStr() {
    std::string S =
        "NodeID = " + std::to_string(this->getSchedInfo().NodeID) +
        "; FreeSched = " + std::to_string(this->getSchedInfo().FreeSched) +
        "; TacID = " + std::to_string(getSchedInfo().TacID) +
        "; PlcmntInfo = " + std::to_string(getSchedInfo().Plcmnt) +
        "; Scops = ";
    for (size_t i = 0; i < getSchedInfo().Scop.size() - 1; ++i) {
      S += std::to_string(getSchedInfo().Scop[i]) + ", ";
    }
    S += std::to_string(getSchedInfo().Scop[getSchedInfo().Scop.size() - 1]);
    return S;
  }

private:
  /// MVExpr info
  MVExpr *MV;
  /// Holds information regarding the scheduling
  SchedInfo SI;
  /// Value that identifies this node
  std::string Value = "";
  /// Type of node: MEMORY or OPERATION
  NodeType T = UNDEF;
  /// List of input nodes for this node
  NodeListType I;
  /// Info regarding the output register/node. This field only makes sense if
  /// we are dealing with an operation node
  OutputInfo O;
  /// FIXME: List of output nodes for this node. Is this needed?
  NodeListType OutNodes;
  /// FIXME: Loop variable where it belongs. This is awful.
  std::string LoopName = "";
};
} // namespace macveth
#endif /* !MACVETH_NODE_H */
