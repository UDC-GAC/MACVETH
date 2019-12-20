/**
 * File              : Node.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mér 18 Dec 2019 17:03:50 MST
 * Last Modified Date: Ven 20 Dec 2019 15:07:02 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#ifndef MACVETH_NODE_H
#define MACVETH_NODE_H

#include "include/MVExpr/MVExpr.h"
#include "include/StmtWrapper.h"
#include "include/TAC.h"

using namespace macveth;

namespace macveth {

/// FIXME this is garbage
inline static std::map<BinaryOperator::Opcode, std::string> BOtoValue = {
    {BO_Mul, "mul"},
    {BO_Div, "div"},
    {BO_Add, "add"},
    {BO_Sub, "sub"},
    {BO_Assign, "store"}};

/// FIXME
inline static std::map<std::string, std::list<int>> CostsMap = {
    {"reduction", {1, 1, 1}}, {"load", {1, 1, 1}}, {"sub", {1, 1, 1}},
    {"add", {1, 1, 1}},       {"div", {1, 1, 1}},  {"mul", {1, 1, 1}}};

/// All Nodes belong to a CDAG. Each node or vertex holds information regarding
/// the type of operation as well as its Edges (or links to another Nodes).
/// Nodes also hold information regarding the scheduling.
class Node {
public:
  /// Simply universal identifier for each instance node. We have to be cautious
  /// with this value: it **does not** identify the object itself. Thus,
  /// whenever this object is cloned, this value is also cloned to the new
  /// object
  static inline int uuid = 0;
  /// Definition of NodeListType
  typedef std::list<Node *> NodeListType;
  /// Available types of nodes
  enum NodeType { NODE_MEM, NODE_OP, UNDEF };
  enum OutputType { TEMP_STORE, MEM_STORE };
  /// Holds information regarding the scheduling for this Node
  struct SchedInfo {
    int StmtID = -1;
    int FreeSched = 0;
    int Plcmnt = 0;
    int Sched = 0;
  };
  /// If the node is NODE_OP type, then it will hold a result value
  struct OutputInfo {
    std::string Name = "";
    OutputType Type = MEM_STORE;
  };

  /// Copy constructor for cloning
  Node(const Node &rhs) {
    this->I = rhs.I;
    this->O = rhs.O;
    this->OutNodes = rhs.OutNodes;
    this->SI = rhs.SI;
    this->T = rhs.T;
    this->Value = rhs.Value;
  }

  /// FIXME
  /// This is the unique constructor for nodes, as we will creating Nodes from
  /// CDAG, so each TAC corresponds to a = b op c
  Node(TAC T) {
    this->T = NODE_OP;
    this->MV = nullptr;
    this->Value = BOtoValue[T.getOP()];
    this->O.Name = T.getA()->getExprStr();
    connectInput(new Node(T.getB()));
    connectInput(new Node(T.getC()));
    this->SI.StmtID = Node::uuid++;
  }

  /// This is the unique constructor for nodes, as we will creating Nodes from
  /// CDAG, so each TAC corresponds to a = b op c
  Node(TAC T, NodeListType L) {
    this->T = NODE_OP;
    this->Value = BOtoValue[T.getOP()];
    this->O.Name = T.getA()->getExprStr();
    Node *NB = findOutputNode(T.getB()->getExprStr(), L);
    Node *NC = findOutputNode(T.getC()->getExprStr(), L);
    connectInput(NB == NULL ? new Node(T.getB()) : NB);
    connectInput(NC == NULL ? new Node(T.getC()) : NC);
    this->SI.StmtID = Node::uuid++;
  }

  /// Schedule info is needed for the algorithms to perform permutations in
  /// nodes
  void setFreeSchedInfo(int Value);

  /// Check if Node N is already in node list L
  Node *findOutputNode(std::string NodeName, NodeListType L);

  /// Connect a Node as input
  void connectInput(Node *N);
  /// Connect a Node as output
  void connectOutput(Node *N);

  /// Get the number of inputs in this Node
  int getOutputNum() { return this->OutNodes.size(); }
  /// Get the output Nodes
  NodeListType getOuputNodes() { return this->OutNodes; }
  /// Get the output information
  OutputInfo getOuputInfo() { return this->O; }
  /// Get the output information
  std::string getOuputInfoValue() { return this->O.Name; }
  /// Get the number of inputs in this Node
  int getInputNum() { return this->I.size(); }
  /// Get the list of node inputs in this Node
  NodeListType getInputs() { return this->I; }
  /// Get info regarding its scheduling
  SchedInfo getSchedInfo() { return this->SI; }

  /// Checks if output list is zero
  bool hasOutNodes();
  /// Checks if input list is zero
  bool hasInputs();
  /// Checks if output list is not zero
  bool isTerminal();
  /// Checks if given node is in the output list
  bool hasOutNode(Node *N);
  /// Checks if given node is in the input list
  bool hasInNode(Node *N);

  /// Returns the string value of the node
  std::string getValue() const { return this->Value; }
  /// Returns the variable/register value
  std::string getRegisterValue() const {
    if (this->T == NodeType::NODE_MEM) {
      return this->O.Name;
    }
    return this->getValue();
  }

  /// Print node: debugging purposes
  void printNode();

  /// Two nodes are equal if and only if they have the same value. Seems
  /// pretty straigthforward but it must be defined someway.
  bool operator==(const Node &N) { return !getValue().compare(N.getValue()); }
  bool operator==(const Node *N) { return !getValue().compare(N->getValue()); }
  /// Two nodes are not equal if and only if they have the same value. Seems
  /// pretty straigthforward but it must be defined someway.
  bool operator!=(const Node &N) { return !operator==(N); }
  bool operator!=(const Node *N) { return !operator==(N); }
  /// For sorting lists of nodes
  bool operator<(const Node &N) {
    if (this->getSchedInfo().FreeSched == N.SI.FreeSched) {
      return (this->getSchedInfo().StmtID < N.SI.StmtID);
    } else {
      return (this->getSchedInfo().FreeSched < N.SI.FreeSched);
    }
  }
  bool operator<(const Node *N) { return operator<(*N); }

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

  /// When creating a Node from a TempExpr, the connections will be created by
  /// the TAC which creates this Node
  Node(MVExpr *TE) {
    this->T = NODE_MEM;
    this->Value = TE->getExprStr();
    this->MV = TE;
    this->SI.StmtID = Node::uuid++;
  }
};
} // namespace macveth
#endif
