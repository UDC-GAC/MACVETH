/*
 * File					 : include/MVExpr/MVExprFactory.h
 * Author				 : Marcos Horro
 * Date					 : Tue 14 Jul 2020 05:03 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:35 +02:00
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

#ifndef MACVETH_MVEXPRFACTORY_H
#define MACVETH_MVEXPRFACTORY_H

#include "include/MVExpr/MVExpr.h"

using namespace macveth;

namespace macveth {

/// Create MVExpr depending on the dynamic cast clang does
class MVExprFactory {
public:
  /// Types available:
  /// * ARRAY, e.g. a[i], b[i][j]
  /// * LITERAL, e.g. 5.0, 42
  /// * VARIABLE, otherwise
  enum MVExprType { ARRAY, LITERAL, VARIABLE };

  /// Detect the type of MVExpr
  static MVExprType getTempTypeFromExpr(Expr *E);
  /// Create MVExpr from Clang expression
  static MVExpr *createMVExpr(Expr *E);
  /// Create temporal MVExpr (as MVExprVar)
  static MVExpr *createMVExpr(std::string E, bool Temp, std::string Type);
};
} // namespace macveth
#endif /* !MACVETH_MVEXPRFACTORY_H */
