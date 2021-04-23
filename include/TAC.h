// MACVETH - TAC.h
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

#ifndef MACVETH_TAC_H
#define MACVETH_TAC_H

#include "include/Debug.h"
#include "include/MVExpr/MVExpr.h"
#include "include/MVOps.h"
#include "include/Utils.h"
#include "clang/AST/AST.h"
#include <iostream>
#include <list>
#include <string>

using namespace clang;
using namespace macveth;

namespace macveth {

/// Class TAC: three-address-code. This abstraction is a way of representing the
/// Single-Statement Assignment (SSA).
/// This class is meant to hold structures such as:
///                  a = b op c
/// Where a, b and c are Expr and op is an Opcode
class TAC {

public:
  /// Unique ID of each TAC
  inline static int TacUUID = 0;
  /// Unique ID of each region of each part of the #pragma
  inline static long TacScop = 0;
  /// For generating TACs
  static inline long RegVal = 0;

  static void clear() {
    /// Unique ID of each TAC
    TAC::TacUUID = 0;
    /// Unique ID of each region of each part of the #pragma
    TAC::TacScop = 0;
    /// For generating TACs
    TAC::RegVal = 0;
  }

  /// Empty constructor
  TAC(){};

  /// Destructor
  ~TAC() {}

  /// Constructor when using MVOp for the operator
  TAC(MVExpr *A, MVExpr *B, MVExpr *C, MVOp MVOP)
      : A(A), B(B), C(C), MVOP(MVOP), TacID(TAC::TacUUID++),
        TScop(TAC::TacScop) {}
  /// Constructor given a TAC ID
  TAC(MVExpr *A, MVExpr *B, MVExpr *C, MVOp MVOP, int TacID)
      : A(A), B(B), C(C), MVOP(MVOP), TacID(TacID), TScop(TAC::TacScop) {}

  /// Get first (result) operand of the TAC expression
  MVExpr *getA() { return this->A; };
  /// Set first (result) operand of the TAC expression
  void setA(MVExpr *A) { this->A = A; };
  /// Get second operand of the TAC expression
  MVExpr *getB() { return this->B; };
  /// Set second operand of the TAC expression
  void setB(MVExpr *B) { this->B = B; };
  /// Get third operand of the TAC expression
  MVExpr *getC() { return this->C; };
  /// Set third operand of the TAC expression
  void setC(MVExpr *C) { this->C = C; };
  /// Set TacID value
  void setTacID(int TacID) { this->TacID = TacID; }
  /// Set scope of the TAC
  void setScop(long TScop) { this->TScop = TScop; }
  /// Get the scope of the TAC
  long getScop() { return this->TScop; }
  /// Get that TacID value
  int getTacID() { return this->TacID; }
  /// Get the unroll factor
  int getUnrollFactor() { return this->TUnroll; }
  /// Set loop name
  void setLoopName(std::string LoopName) { this->LoopName = LoopName; }
  /// Get loop name
  std::string getLoopName() { return this->LoopName; }

  /// Get macveth operation type
  MVOp getMVOP() { return this->MVOP; };

  /// To string method
  std::string toString() {
    auto C = (this->getC() != nullptr) ? ", " + this->getC()->getExprStr() : "";
    auto Op = this->MVOP.toString();
    Op = "t: " + this->getA()->getExprStr() + ", " +
         this->getB()->getExprStr() + C + ", " + Op +
         "; (loop = " + this->getLoopName() +
         "; scop = " + std::to_string(this->getScop()) +
         "); ID = " + std::to_string(this->getTacID()) +
         "; UnrollID = " + std::to_string(this->getUnrollFactor());
    return Op;
  }

  /// Just for debugging purposes
  void printTAC() { MACVETH_DEBUG("TAC", this->toString()); }

  /// Inserts TACs in the input TacList and outputs the relation between the
  /// statements and the ordering of the TACs
  static std::list<TAC> stmtToTAC(clang::Stmt *ST);

  /// Print/render TAC as regular statements
  static std::string renderTacAsStmt(std::list<TAC> TL, int Offset);

  /// Unrolls TacList given onto a new list
  static std::list<TAC> unrollTacList(std::list<TAC> Tac, int UnrollFactor,
                                      int UpperBound, std::string LoopLevel,
                                      bool FullUnroll = false);

  /// Unroll a TAC given a LoopLevel, besides its mask, unroll factor, and
  /// the S value which holds the iteration of the unrolling basically
  static TAC unroll(TAC Tac, int UnrollFactor, int S, std::string LoopLevel,
                    bool FullUnroll = false);

private:
  /// TAC result
  MVExpr *A;
  /// First operand of TAC
  MVExpr *B;
  /// Second operand of TAC
  MVExpr *C;
  /// Type of operation
  MVOp MVOP;
  /// Name of the loop level where the TAC is
  std::string LoopName = "";
  /// TAC identifier: as TAC may be unrolled, the ID is only meant for
  /// identifying it
  int TacID = -1;
  /// TAC Unroll factor from the original, as unrolled TACs have the same ID
  int TUnroll = -1;
  /// Represents with a unique ID the part of the
  long TScop = 0;
};

/// List of TACs
using TacListType = std::list<TAC>;

} // namespace macveth
#endif /* !MACVETH_TAC_H */
