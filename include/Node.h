/**
 * File              : Node.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 18 Dec 2019 17:03:50 MST
 * Last Modified Date: Mér 15 Xan 2020 12:09:57 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
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
  typedef std::list<Node *> NodeListType;

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
    /// Topological order of this node
    int FreeSched = 0;
    /// TODO this value should be calculated by an algorithm
    int Plcmnt = 0;
    /// TODO this value stands for the scheduling order of this node; should
    /// be also calculated when the placement
    int Sched = 0;
  };

  /// If the node is NODE_OP type, then it will hold a result value
  struct OutputInfo {
    /// Name of the register or memory variable as output
    std::string Name = "";
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
    O.Name = T.getA()->getExprStr();
    O.Type = (T.getMVOP().isAssignment()) ? MEM_STORE : TEMP_STORE;
    O.MVOP = T.getMVOP();
    O.IsBinaryOp = T.getMVOP().getType() == MVOpType::CLANG_BINOP;
    return O;
  }

  /// When creating a Node from a TempExpr, the connections will be created by
  /// the TAC which creates this Node
  Node(MVExpr *TE) {
    this->T = NODE_MEM;
    this->Value = TE->getExprStr();
    this->MV = TE;
    this->SI.NodeID = Node::UUID++;
  }

  /// Copy constructor for cloning
  Node(const Node &rhs) {
    this->I = rhs.I;
    this->O = rhs.O;
    this->OutNodes = rhs.OutNodes;
    this->SI = rhs.SI;
    this->T = rhs.T;
    this->Value = rhs.Value;
    this->LoopName = rhs.LoopName;
  }

  /// This is the unique constructor for nodes, as we will creating Nodes from
  /// CDAG, so each TAC corresponds to a = b op c
  Node(TAC T) {
    this->T = NODE_OP;
    this->MV = nullptr;
    this->Value = T.getMVOP().toString();
    this->O = setOutputInfo(T);
    this->SI.NodeID = Node::UUID++;
    this->SI.TacID = T.getTacID();
    this->LoopName = T.getLoopName();
    connectInput(new Node(T.getB()));
    connectInput(new Node(T.getC()));
    updateIfStore();
  }

  /// This is the unique constructor for nodes, as we will creating Nodes from
  /// CDAG, so each TAC corresponds to a = b op c
  Node(TAC T, NodeListType L) {
    this->T = NODE_OP;
    this->MV = nullptr;
    this->Value = T.getMVOP().toString();
    this->O = setOutputInfo(T);
    this->SI.NodeID = Node::UUID++;
    this->SI.TacID = T.getTacID();
    this->LoopName = T.getLoopName();
    Node *NB = findOutputNode(T.getB()->getExprStr(), L);
    Node *NC = findOutputNode(T.getC()->getExprStr(), L);
    connectInput(NB == NULL ? new Node(T.getB()) : NB);
    connectInput(NC == NULL ? new Node(T.getC()) : NC);
    updateIfStore();
  }

  void updateIfStore() {
    if (this->O.Type == MEM_STORE) {
      this->getInputs().front()->T = NODE_STORE;
    }
  }

  /// Schedule info is needed for the algorithms to perform permutations in
  /// nodes
  void setFreeSchedInfo(int FS) { this->SI.FreeSched = FS; }

  /// Set the TAC order
  void setTacID(int TacID) { this->SI.TacID = TacID; }

  /// Set Plcmnt value
  void setPlcmnt(int Plcmnt) { this->SI.Plcmnt = Plcmnt; }

  /// Check if Node N is already in node list L
  static Node *findOutputNode(std::string NodeName, NodeListType L);

  /// Get Node
  static Node *findWARDataRace(Node *N, Node::NodeListType NL);

  /// Connect a Node as input
  void connectInput(Node *N);

  /// Get the number of inputs in this Node
  int getOutputNum() { return this->OutNodes.size(); }
  /// Get the output Nodes
  NodeListType getOuputNodes() { return this->OutNodes; }
  /// Get the output information
  OutputInfo getOutputInfo() { return this->O; }
  /// Get the output information
  std::string getOutputInfoName() { return this->O.Name; }
  /// Get the number of inputs in this Node
  int getInputNum() { return this->I.size(); }
  /// Get the list of node inputs in this Node
  NodeListType getInputs() { return this->I; }
  /// Get info regarding its scheduling
  SchedInfo getSchedInfo() { return this->SI; }
  /// Get TAC order
  int getTacID() { return this->SI.TacID; }
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
      return this->O.Name;
    }
    return this->getValue();
  }

  /// Setting the name of the output value
  void setOutputName(std::string S) { this->O.Name = S; }

  /// Get loop name of the loop
  std::string getLoopName() { return this->LoopName; }

  /// Print node: debugging purposes
  std::string toString();

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
      return (this->getSchedInfo().TacID < N.SI.TacID);
    } else {
      return (this->getSchedInfo().FreeSched < N.SI.FreeSched);
    }
  }

  /// For debugging purposes: show information regarding the node
  std::string getSchedInfoStr() {
    return "NodeID = " + std::to_string(this->getSchedInfo().NodeID) +
           "; FreeSched = " + std::to_string(this->getSchedInfo().FreeSched) +
           "; TacID = " + std::to_string(getSchedInfo().TacID) +
           "; PlcmntInfo = " + std::to_string(getSchedInfo().Plcmnt);
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
  /// List of output nodes for this node
  NodeListType OutNodes;
  /// Loop variable where it belongs
  std::string LoopName = "";
};
} // namespace macveth
#endif // MACVETH_NODE_H
