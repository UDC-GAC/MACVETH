// MACVETH - MVSourceLocation.h
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

#ifndef MACVETH_SOURCELOCATION_H
#define MACVETH_SOURCELOCATION_H

namespace macveth {
/// MVSourceLocation serves to identify the order of certain instructions,
/// such as reductions, initializations, etc.
class MVSourceLocation {
public:
  /// Position specifies where the statement should be placed
  enum Position {
    /// Place before a certain loop nest
    PREOUTERMOST,
    /// Before the TAC. This is useful for initializations
    PREORDER,
    /// Where the TAC is originally
    INORDER,
    /// After the TAC. This is useful for statements that changed their location
    /// due to transformations, e.g. loops
    POSORDER,
    /// Place after nested loops
    POSOUTERMOST
  };

  bool isPreOutermost() {
    return this->getPosition() == MVSourceLocation::Position::PREOUTERMOST;
  }

  bool isPreOrder() {
    return this->getPosition() == MVSourceLocation::Position::PREORDER;
  }

  bool isInOrder() {
    return this->getPosition() == MVSourceLocation::Position::INORDER;
  }

  bool isPosOrder() {
    return this->getPosition() == MVSourceLocation::Position::POSORDER;
  }

  bool isPosOutermost() {
    return this->getPosition() == MVSourceLocation::Position::POSOUTERMOST;
  }

  /// Get the order
  int getOffset() { return this->Offset; }

  // Set order
  void setOffset(unsigned int Offset) { this->Offset = Offset; }

  /// Get the order
  unsigned int getOrder() { return this->Order; }

  /// Set order
  void setOrder(unsigned int Order) { this->Order = Order; }

  /// Get the position
  Position getPosition() { return this->P; }

  // Set position P
  void setPosition(Position P) { this->P = P; }

  /// Empty constructor
  MVSourceLocation() { this->P = Position::INORDER; };

  /// Constructor given position
  MVSourceLocation(Position P) { this->P = P; };

  /// Constructor given position and order
  MVSourceLocation(Position P, unsigned int Order) {
    this->P = P;
    this->Order = Order;
  };

  /// Constructor given position, order and offset
  MVSourceLocation(Position P, unsigned int Order, int Offset) {
    this->P = P;
    this->Order = Order;
    this->Offset = Offset;
  };

  bool operator==(MVSourceLocation M) {
    return (M.getOffset() == this->getOffset()) &&
           (M.getOrder() == this->getOrder()) &&
           (M.getPosition() == this->getPosition());
  }

private:
  /// This is the order where the statement or instruction should be placed
  /// according to the original order, i.e. the TAC ID
  unsigned int Order = 0;
  /// This is the offset when having different locations with the same ID
  int Offset = -1;
  /// This is the order where the statement or instruction should be placed
  /// regarding the order
  Position P = Position::INORDER;
};

} // namespace macveth

#endif /* !MACVETH_SOURCELOCATION_H */