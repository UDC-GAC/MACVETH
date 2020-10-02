/**
 * File              : MVSourceLocation.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 02 Out 2020 09:43:25 CET
 * Last Modified Date: Ven 02 Out 2020 09:43:25 CET
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 *
 * Copyright (c) 2020 Marcos Horro <marcos.horro@udc.gal>
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
class MVSourceLocation {
public:
  // Position specifies where the statement should be placed
  enum Position {
    // Before the TAC. This is useful for initializations
    PREORDER,
    // Where the TAC is originally
    INORDER,
    // After the TAC. This is useful for statements that changed their location
    // due to transformations
    POSORDER
  };

  // Get the order
  unsigned int getOrder() { return this->Order; }

  // Set order
  void setOrder(unsigned int Order) { this->Order = Order; }

  // Get the position
  Position getPosition() { return this->P; }

  // Set position P
  void setPosition(Position P) { this->P = P; }

  // Empty constructor
  MVSourceLocation() { this->P = Position::INORDER; };

  // Constructor given position
  MVSourceLocation(Position P) { this->P = P; };

  // Constructor given position and order
  MVSourceLocation(Position P, unsigned int Order) {
    this->P = P;
    this->Order = Order;
  };

private:
  // This is the order where the statement or instruction should be placed
  // according to the original order, i.e. the offset
  unsigned int Order = 0;
  // This is the order where the statement or instruction should be placed
  // regarding the order
  Position P = Position::INORDER;
};

} // namespace macveth

#endif /* !MACVETH_SOURCELOCATION_H */