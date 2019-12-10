/**
 * File              : CDAG.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 09 Dec 2019 15:10:51 MST
 * Last Modified Date: Mar 10 Dec 2019 12:07:52 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#ifndef MACVETH_CDAG_H
#define MACVETH_CDAG_H
#include <list>
#include <stdio.h>

#include "StmtWrapper.h"
#include "include/TAC.h"

using namespace macveth;

namespace macveth {

class Node {
  /// FIXME this is garbage
  inline static std::map<BinaryOperator::Opcode, std::string> BOtoValue = {
      {BO_Mul, "mul"},
      {BO_Div, "div"},
      {BO_Add, "add"},
      {BO_Sub, "sub"},
      {BO_Assign, "store"}};

public:
  typedef std::list<Node> NodeList;
  enum NodeType { NODE_EXPR, NODE_OP, UNDEF };

private:
  /// When creating a Node from a TempExpr, the connections will be created by
  /// the TAC which creates this Node
  Node(TempExpr *TE) {
    this->T = NODE_EXPR;
    this->Value = TE->getExprStr();
  }

public:
  /// This is the unique constructor for nodes, as we will creating Nodes from
  /// CDAG.
  Node(TAC T) {
    this->T = NODE_OP;
    this->Value = BOtoValue[T.getOP()];
    connectOutput(new Node(T.getA()));
    connectInput(new Node(T.getB()));
    connectInput(new Node(T.getC()));
  }

  void connectInput(Node *N);
  void connectOutput(Node *N);

  /// Get the output Node
  Node *getOuput() { return this->Out; }
  /// Get the number of inputs in this Node
  int getInputNum() { return this->In.size(); }
  /// Get the list of node inputs in this Node
  NodeList getInputs() { return this->In; }

  bool hasOut();
  bool isTerminal();
  bool hasOutNode(Node *N);
  bool hasInNode(Node *N);

  /// Replace input
  int replaceInputWithNode(Node *NewIn);

  std::string getValue() const { return this->Value; }

  /// Two nodes are equal if and only if they have the same value
  bool operator==(const Node &N) { return !this->Value.compare(N.Value); }

private:
  std::string Value = "";
  NodeType T = UNDEF;
  NodeList In;
  Node *Out = NULL;
};

bool operator==(const Node &N1, const Node &N2) {
  return !N1.getValue().compare(N2.getValue());
}

class CDAG {
public:
  /// Given a list of TACs, create its correspondent CDAG
  static CDAG *createCDAGfromTAC(TacListType TL);

  Node::NodeList getNodeList() { return this->NL; }

private:
  Node *insertTac(TAC T);
  bool connectNode(Node *N);

private:
  Node::NodeList NL;
};

} // namespace macveth
#endif
