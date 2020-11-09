/*
 * File					 : include/MVSourceLocation.h
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:38 +02:00
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