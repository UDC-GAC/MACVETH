// MACVETH - Node.h
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

class Node;

using NodeVectorT = std::vector<Node *>;

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
    /// TODO: this value should be calculated by an algorithm
    int Plcmnt = 0;
    /// TODO: this value stands for the scheduling order of this node; should
    /// be also calculated when the placement
    int Sched = 0;
    /// If this node belongs to a reduction
    bool IsReduction = false;
  };

  /// If the node is NODE_OP type, then it will hold a result value
  struct OutputInfo {
    /// MVExpr of output
    MVExpr *E = nullptr;
    /// If the target variable is in memory it will be a MEM_STORE; TEMP_STORE
    /// the other way
    OutputType Type = MEM_STORE;
    /// Opcode
    MVOp MVOP;
    /// An output node may be an operation and, therefore, may be a binary
    /// operation
    bool IsBinaryOp = false;
    bool alreadyStored = false;
  };

  /// Set the output information of the given a TAC
  OutputInfo setOutputInfo(const TAC &T) {
    OutputInfo O;
    O.E = T.getA();
    O.Type = (T.getMVOP().isAssignment()) ? MEM_STORE : TEMP_STORE;
    O.MVOP = T.getMVOP();
    O.IsBinaryOp = T.getMVOP().getType() == MVOpType::CLANG_BINOP;
    return O;
  }

  void setNodeAsReduction() { this->SI.IsReduction = true; }
  void setNodeAsNonReduction() { this->SI.IsReduction = false; }
  void setAlreadyStored() { this->O.alreadyStored = true; }
  bool isAlreadyStored() { return this->O.alreadyStored; }
  bool belongsToAReduction() const { return this->SI.IsReduction; }

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

  /// Empty constructor
  Node() {}

  /// A node is emtpy if it is not initialized, basically.
  bool isEmptyNode() {
    return (MV == nullptr) && (T == UNDEF) && (Value.empty());
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
  Node(const TAC &T, NodeVectorT L) {
    this->T = NODE_OP;
    this->MV = nullptr;
    this->Value = T.getMVOP().toString();
    this->O = setOutputInfo(T);
    setSchedInfo(T);
    this->LoopName = T.getLoopName();
    auto NB = findOutputNode(T.getB()->getExprStr(), L);
    // if (NB == nullptr || this->getScop()[0] != NB->getScop()[0]) {
    if (NB == nullptr) {
      connectInput(new Node(T.getB()));
    } else {
      connectInput(NB);
    }
    Node *NC = nullptr;
    if (T.getC() != nullptr) {
      NC = findOutputNode(T.getC()->getExprStr(), L);
      // if (NC == nullptr || this->getScop()[0] != NC->getScop()[0]) {
      if (NC == nullptr) {
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
  MVExpr *getMVExpr() const {
    if ((this->MV == nullptr) && (this->isStoreNodeOp())) {
      return this->getOutputInfo().E;
    }
    return this->MV;
  }

  bool isOnLoop() const { return this->LoopName != ""; }

  /// Schedule info is needed for the algorithms to perform permutations in
  /// nodes
  void setFreeSchedInfo(int FS) { this->SI.FreeSched = FS; }

  /// Set the TAC order
  void setTacID(int TacID) { this->SI.TacID = TacID; }

  /// Set Plcmnt value
  void setPlcmnt(int Plcmnt) { this->SI.Plcmnt = Plcmnt; }

  /// Check if Node N is already in node list L
  static Node *findOutputNode(const std::string &NodeName, NodeVectorT L);

  /// Connect a Node as input
  void connectInput(Node *N);

  /// Get the scope of the node
  std::vector<int> getScop() { return this->SI.Scop; }

  /// Get the output information
  OutputInfo getOutputInfo() const { return this->O; }
  /// Get the output information
  std::string getOutputInfoName() { return this->O.E->toString(); }
  /// Get the output information
  MVExpr *getOutputMVExpr() { return this->O.E; }
  /// Get the number of inputs in this Node
  int getInputNum() { return this->I.size(); }
  /// Get the list of node inputs in this Node
  NodeVectorT getInputs() { return this->I; }
  /// Get info regarding its scheduling
  SchedInfo getSchedInfo() { return this->SI; }
  SchedInfo getSchedInfo() const { return this->SI; }
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

  /// Checks if input list is zero
  bool hasInputs();

  /// Is OP
  bool needsMemLoad() {
    return (((this->T == NODE_MEM) && (!dyn_cast<MVExprLiteral>(this->MV)) &&
             (this->MV->needsToBeLoaded())) ||
            isAlreadyStored());
  }

  /// Check if Node type is NODE_STORE
  bool isStoreNodeOp() const { return (this->T == NODE_STORE); }

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

  std::string getLoopName() { return this->LoopName; }

  std::string toString();

  std::string toStringShort();

  /// This is useful for calculating the difference between two Nodes in terms
  /// of addresses, in some way
  int operator-(const Node &N) { return (*MV - *N.MV); }

  /// Two nodes are equal if and only if they have the same value: output value
  /// (if output nodes) or node value
  bool operator==(const Node &N) {
    return (getRegisterValue() == N.getRegisterValue());
  }

  /// Two nodes are equal if and only if they have the same value: output value
  /// (if output nodes) or node value
  bool operator==(const Node *N) {
    return (getRegisterValue() == N->getRegisterValue());
  }

  /// Two nodes are not equal if and only if they are not equal (defined in
  /// operator==)
  bool operator!=(const Node &N) { return !operator==(N); }

  /// Precedence of nodes is defined by the scheduling information
  bool operator<(const Node &N) {
    // FIXME:
    // if (this->getSchedInfo().FreeSched == N.SI.FreeSched) {
    //   if (this->getSchedInfo().TacID == N.SI.TacID) {
    //     return (this->getSchedInfo().NodeID < N.SI.NodeID);
    //   } else {
    //     return (this->getSchedInfo().TacID < N.SI.TacID);
    //   }
    // } else {
    //   return (this->getSchedInfo().FreeSched < N.SI.FreeSched);
    // }

    // return (this->getSchedInfo().NodeID < N.SI.NodeID);

    if (this->getSchedInfo().TacID == N.SI.TacID) {
      if (this->getSchedInfo().UnrollFactor == N.SI.UnrollFactor) {
        if (this->getSchedInfo().FreeSched == N.SI.FreeSched) {
          return (this->getSchedInfo().NodeID < N.SI.NodeID);
        } else {
          return (this->getSchedInfo().FreeSched < N.SI.FreeSched);
        }
      } else {
        return (this->getSchedInfo().UnrollFactor < N.SI.UnrollFactor);
      }
    } else {
      if ((!this->belongsToAReduction()) && (!N.belongsToAReduction())) {
        if (this->getSchedInfo().FreeSched == N.SI.FreeSched) {
          return (this->getSchedInfo().NodeID < N.SI.NodeID);
        } else {
          return (this->getSchedInfo().FreeSched < N.SI.FreeSched);
        }
      }
      // else {
      //   return (this->getSchedInfo().FreeSched < N.SI.FreeSched);
      // }
      return (this->getSchedInfo().TacID < N.SI.TacID);
    }
  }

  /// For debugging purposes: show information regarding the scheduling of the
  /// node
  std::string getSchedInfoStr() {
    std::string S = "NID = " + std::to_string(this->getSchedInfo().NodeID) +
                    "; FS = " + std::to_string(this->getSchedInfo().FreeSched) +
                    "; TID = " + std::to_string(getSchedInfo().TacID) +
                    "; PI = " + std::to_string(getSchedInfo().Plcmnt) +
                    "; Scp = ";
    for (size_t i = 0; i < getSchedInfo().Scop.size() - 1; ++i) {
      S += std::to_string(getSchedInfo().Scop[i]) + ", ";
    }
    S += std::to_string(getSchedInfo().Scop[getSchedInfo().Scop.size() - 1]);
    return S;
  }

private:
  /// Expression holden in the node
  MVExpr *MV = nullptr;
  /// Each node holds information about scheduling
  SchedInfo SI;
  /// Value that identifies this node
  std::string Value = "";
  /// Type of node: MEMORY or OPERATION
  NodeType T = UNDEF;
  /// List of input nodes for this node
  NodeVectorT I;
  /// Info regarding the output register/node. This field only makes sense if
  /// we are dealing with an operation node
  OutputInfo O;
  /// FIXME: Loop variable where it belongs. This is awful.
  std::string LoopName = "";
};
} // namespace macveth
#endif /* !MACVETH_NODE_H */
