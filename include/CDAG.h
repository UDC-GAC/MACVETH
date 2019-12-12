/**
 * File              : CDAG.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:51 MST
 * Last Modified Date: Mér 11 Dec 2019 17:25:45 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#ifndef MACVETH_CDAG_H
#define MACVETH_CDAG_H
#include <list>
#include <stdio.h>

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

/// All Nodes belong to a CDAG. Each node or vertex holds information regarding
/// the type of operation as well as its Edges (or links to another Nodes).
/// Nodes also hold information regarding the scheduling.
class Node {
public:
  /// Holds information regarding the scheduling for this Node
  struct SchedInfo {
    int Plcmnt;
    int Sched;
  };

  /// Definition of NodeListType
  typedef std::list<Node> NodeListType;
  /// Available types of nodes
  enum NodeType { NODE_EXPR, NODE_OP, UNDEF };
  /// FIXME
  inline static std::map<std::string, std::list<int>> CostsMap = {
      {"reduction", {1, 1, 1}}, {"load", {1, 1, 1}}, {"sub", {1, 1, 1}},
      {"add", {1, 1, 1}},       {"div", {1, 1, 1}},  {"mul", {1, 1, 1}}};

  /// This is the unique constructor for nodes, as we will creating Nodes from
  /// CDAG, so each TAC corresponds to a = b op c
  Node(TAC T) {
    this->T = NODE_OP;
    this->Value = BOtoValue[T.getOP()];
    connectOutput(new Node(T.getA()));
    connectInput(new Node(T.getB()));
    connectInput(new Node(T.getC()));
  }

  /// TODO
  void setSchedInfo();

  void connectInput(Node *N);
  void connectOutput(Node *N);

  /// Get the output Node
  NodeListType getOuput() { return this->Out; }
  /// Get the number of inputs in this Node
  int getInputNum() { return this->In.size(); }
  /// Get the list of node inputs in this Node
  NodeListType getInputs() { return this->In; }

  /// Checks if output list is zero
  bool hasOut();
  /// Checks if input list is zero
  bool hasInputs();
  /// Checks if output list is not zero
  bool isTerminal();
  /// Checks if given node is in the output list
  bool hasOutNode(Node *N);
  /// Checks if given node is in the input list
  bool hasInNode(Node *N);

  void mergeIfFound(Node NIn);
  int replaceInputWithNode(Node *NewIn);

  /// Returns the string value of the node
  std::string getValue() const { return this->Value; }

  /// Two nodes are equal if and only if they have the same value. Seems pretty
  /// straigthforward but it must be defined someway.
  bool operator==(const Node &N) { return !getValue().compare(N.getValue()); }

private:
  /// Holds information regarding the scheduling
  SchedInfo SI;
  /// Value that identifies this node
  std::string Value = "";
  /// Type of node
  NodeType T = UNDEF;
  /// List of inputs for this node
  NodeListType In;
  /// List of outputs for this node
  NodeListType Out;

  /// When creating a Node from a TempExpr, the connections will be created by
  /// the TAC which creates this Node
  Node(MVExpr *TE) {
    this->T = NODE_EXPR;
    this->Value = TE->getExprStr();
  }
};

class CDAG {
public:
  /// Given a list of TACs, create its correspondent CDAG
  static CDAG *createCDAGfromTAC(TacListType TL);

  static int costModel(CDAG *C);

  Node::NodeListType getNodeList() { return this->NL; }
  void addNodeToList(Node N) { this->NL.push_back(N); }

private:
  Node insertTac(TAC T);
  bool connectNode(Node N);
  Node::NodeListType NL;
};

} // namespace macveth
#endif
