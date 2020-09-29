/**
 * File              : MVExprArray.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:03:14 MST
 * Last Modified Date: Mér 15 Xan 2020 12:07:59 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_MVEXPRARRAY_H
#define MACVETH_MVEXPRARRAY_H

#include "include/MVExpr/MVExpr.h"
#include <llvm-10/llvm/Support/Casting.h>
#include <string.h>

using namespace macveth;

namespace macveth {

/// Abstraction of arrays for simplicity
class MVExprArray : public MVExpr {
public:
  /// Abstraction to handle affine indices in MACVETH
  struct MVAffineIndex {
    /// Left part of the expression, if it has
    MVAffineIndex *LHS = nullptr;
    /// Right part of the expression, if it has
    MVAffineIndex *RHS = nullptr;
    /// Value of the expression (we do not care of its type, we handle strings)
    std::string Val = "";
    /// Unrolled
    std::map<std::string, bool> Unrolled;
    /// BinaryOperator of the expression if it has any
    clang::BinaryOperator::Opcode OP;

    /// Check if index is terminal or not (has real Val)
    bool isTerminal() {
      return ((Val != "") && (LHS == nullptr) && (RHS == nullptr));
    }

    /// Copy constructor
    static MVAffineIndex copy(MVAffineIndex O) {
      MVAffineIndex New;
      New.Val = O.Val;
      New.Unrolled = O.Unrolled;
      New.OP = O.OP;
      New.LHS = nullptr;
      if (O.LHS != nullptr) {
        New.LHS = new MVAffineIndex(O.LHS);
      }
      New.RHS = nullptr;
      if (O.RHS != nullptr) {
        New.RHS = new MVAffineIndex(O.RHS);
      }
      return New;
    }

    /// Empty constructor
    MVAffineIndex() {}

    /// Create a copy of an MVAffineIndex
    MVAffineIndex(MVAffineIndex *M) {
      if (M == nullptr) {
        return;
      }
      this->Val = M->Val;
      this->Unrolled = M->Unrolled;
      this->OP = M->OP;
      if (M->LHS != nullptr) {
        this->LHS = new MVAffineIndex(M->LHS);
      }
      if (M->RHS != nullptr) {
        this->RHS = new MVAffineIndex(M->RHS);
      }
    }

    /// If we set a Val, then there it has no leaves
    void setVal(std::string Val) {
      this->Val = Val;
      this->LHS = nullptr;
      this->RHS = nullptr;
    }

    /// Delete safely the LHS of the expression
    void deleteLhs() {
      this->Val = this->RHS->Val;
      if (this->RHS->isTerminal()) {
        this->LHS = nullptr;
        this->RHS = nullptr;
      } else {
        auto Rhs = this->RHS;
        this->LHS = Rhs->LHS;
        this->RHS = Rhs->RHS;
        this->OP = Rhs->OP;
      }
    }

    /// Delete safely the RHS of the expression
    void deleteRhs() {
      this->Val = this->LHS->Val;
      if (this->LHS->isTerminal()) {
        this->LHS = nullptr;
        this->RHS = nullptr;
      } else {
        auto Lhs = this->LHS;
        this->LHS = Lhs->LHS;
        this->RHS = Lhs->RHS;
        this->OP = Lhs->OP;
      }
    }

    /// Simplify expression in order to better calculate indices and so
    void simplifyIndex() {
      if (this->isTerminal()) {
        return;
      }
      // Addition or substraction: E +/-
      if ((this->OP == BinaryOperator::Opcode::BO_Add) ||
          (this->OP == BinaryOperator::Opcode::BO_Sub)) {
        if (this->LHS->isTerminal() && (this->LHS->Val == "0")) {
          deleteLhs();
          return;
        }
        if (this->RHS->isTerminal() && (this->RHS->Val == "0")) {
          deleteRhs();
          return;
        }
      }
      // Multiplication: E * 0 or E * 1
      if (this->OP == BinaryOperator::Opcode::BO_Mul) {
        if ((this->LHS->isTerminal() && (this->LHS->Val == "0")) ||
            (this->RHS->isTerminal() && (this->RHS->Val == "0"))) {
          this->setVal("0");
          return;
        }
        if (this->LHS->isTerminal() && (this->LHS->Val == "1")) {
          deleteLhs();
          return;
        }
        if (this->RHS->isTerminal() && (this->RHS->Val == "1")) {
          deleteRhs();
          return;
        }
      }
      // Division: N / 1
      if (this->OP == BinaryOperator::Opcode::BO_Div) {
        if (this->RHS->isTerminal() && (this->RHS->Val == "1")) {
          deleteRhs();
          return;
        }
      }
    }

    /// Main constructor
    MVAffineIndex(const Expr *E) {
      if (dyn_cast<DeclRefExpr>(E->IgnoreImpCasts()) ||
          dyn_cast<IntegerLiteral>(E->IgnoreImpCasts()) ||
          dyn_cast<UnaryOperator>(E->IgnoreImpCasts())) {
        this->Val = Utils::getStringFromExpr(E);
        this->LHS = nullptr;
        this->RHS = nullptr;
      }
      if (auto Op = dyn_cast<BinaryOperator>(E->IgnoreImpCasts())) {
        this->LHS = new MVAffineIndex(Op->getLHS());
        this->LHS->simplifyIndex();
        this->RHS = new MVAffineIndex(Op->getRHS());
        this->RHS->simplifyIndex();
        this->OP = Op->getOpcode();
        this->Val = "";
        this->simplifyIndex();
      }
    }

    /// Constructor when given only a string
    MVAffineIndex(std::string S) {
      this->LHS = nullptr;
      this->RHS = nullptr;
      this->Val = S;
      this->Unrolled[S] = true;
    }

    /// Update index recursively
    bool updateIndex(int UF, std::string LL) {
      if ((isTerminal()) && (LL != this->Val)) {
        return false;
      }

      if ((isTerminal()) && (LL == this->Val) && !Unrolled[LL]) {
        this->Val = "";
        this->LHS = new MVAffineIndex(LL);
        this->LHS->Unrolled[LL] = true;
        this->RHS = new MVAffineIndex(std::to_string(UF));
        this->RHS->Unrolled[LL] = true;
        this->OP = BinaryOperator::Opcode::BO_Add;
        this->Unrolled[LL] = true;
        return false;
      }
      if (Unrolled[LL]) {
        return true;
      }
      if (this->LHS != nullptr) {
        if (this->LHS->updateIndex(UF, LL)) {
          this->LHS->RHS = new MVAffineIndex(std::to_string(UF));
        }
      }
      if (this->RHS != nullptr) {
        if (this->RHS->updateIndex(UF, LL)) {
          this->RHS->RHS = new MVAffineIndex(std::to_string(UF));
        }
      }
      return false;
    }

    // This is awful, but should do the job. Recursive functions as a way of
    // life lol xd
    int operator-(const MVAffineIndex &M) {
      if (!this->isTerminal()) {
        if (M.OP != this->OP) {
          return INT_MIN;
        }
        auto Lhs = ((*this->LHS) - (*M.LHS));
        auto Rhs = ((*this->RHS) - (*M.RHS));
        if ((Lhs == INT_MIN + 2) || (Rhs == INT_MIN + 2)) {
          return INT_MIN;
        } else {
          if (this->OP == BinaryOperator::Opcode::BO_Mul) {
            if ((Lhs == 0) && (this->LHS->isTerminal())) {
              char *P = NULL;
              auto I = strtol(this->LHS->Val.c_str(), &P, 10);
              if (!(*P)) {
                Lhs = I;
              }
            }
            if ((Rhs == 0) && (this->RHS->isTerminal())) {
              char *P = NULL;
              auto I = strtol(this->RHS->Val.c_str(), &P, 10);
              if (!(*P)) {
                Rhs = I;
              }
            }
            return Lhs * Rhs;
          }
          return std::max(Lhs, Rhs);
        }
      } else {
        char *P0 = NULL, *P1 = NULL;
        auto I0 = strtol(M.Val.c_str(), &P0, 10);
        auto I1 = strtol(this->Val.c_str(), &P1, 10);
        auto IsANumber = !((*P0) || (*P1));
        auto SameVal = M.Val == this->Val;
        if (SameVal) {
          return 0;
        }
        if (!IsANumber) {
          return INT_MIN + 2;
        }
        return I1 - I0;
      }
      return INT_MIN;
    }

    /// Convert to string recursively
    std::string toString() {
      if (isTerminal()) {
        return this->Val;
      } else {
        return ("(" + this->LHS->toString() + " " +
                BinaryOperator::getOpcodeStr(this->OP).str() + " " +
                this->RHS->toString() + ")");
      }
    }
  }; /* !MVAffineIndex */

  /// Definition of a vector of indices
  typedef std::vector<MVAffineIndex> IdxVector;

  /// Destructor
  virtual ~MVExprArray(){};

  /// Basic constructor
  MVExprArray(Expr *E) : MVExpr(MVK_Array, E) {
    if (ArraySubscriptExpr *ASE =
            dyn_cast<clang::ArraySubscriptExpr>(E->IgnoreImpCasts())) {
      const Expr *TmpExpr = getArrayBaseExprAndIdxs(ASE, this->Idx);
      this->BaseName = Utils::getStringFromExpr(TmpExpr);
      return;
    }
    if (CXXOperatorCallExpr *CXX =
            dyn_cast<clang::CXXOperatorCallExpr>(E->IgnoreImpCasts())) {
      const Expr *TmpExpr = getArrayBaseExprAndIdxs(CXX, this->Idx);
      this->BaseName = Utils::getStringFromExpr(TmpExpr);
      return;
    }
    llvm::llvm_unreachable_internal();
  }

  /// Copy constructor
  MVExprArray(MVExprArray *E) : MVExpr(MVK_Array, E->getClangExpr()) {
    this->BaseName = E->BaseName;
    IdxVector Copy;
    for (auto I : E->Idx) {
      Copy.push_back(MVAffineIndex::copy(I));
    }
    this->Idx = Copy;
  }

  /// Get base name
  std::string getBaseName() { return this->BaseName; }

  /// Get index
  IdxVector getIndex() { return this->Idx; }

  /// To string
  virtual std::string toString() const {
    std::string T = this->BaseName;
    for (auto I : this->Idx) {
      T += "[" + I.toString() + "]";
    }
    return T;
  }

  /// Implementation of unrolling for arrays. In this case we will need to
  /// create a new MVExpr
  virtual MVExpr *unrollExpr(int UF, std::string LL) override;

  /// As we may not know the total size of the array at compilation time, we
  /// can only know the difference between two indices
  virtual int operator-(const MVExpr &MVE) override {
    if ((getClangExpr() != NULL) && (MVE.getClangExpr() != NULL)) {
      int Diff = 0;
      return Diff;
    } else {
      return -1;
    }
  }

  /// In order to be able to use RTTI
  static bool classof(const MVExpr *S) { return S->getKind() == MVK_Array; }

private:
  /// Given a dimension LL and a unrolling factor UF, regenerates the expression
  /// with the indexes updated
  void updateIndex(int UF, std::string LL);
  /// Given a ArraySubscriptExpr, recursively gets the base name and the indexes
  /// from the outermost to the innermost
  const Expr *getArrayBaseExprAndIdxs(const ArraySubscriptExpr *ASE,
                                      IdxVector &Idxs);
  /// Given a ArraySubscriptExpr, recursively gets the base name and the indexes
  /// from the outermost to the innermost
  const Expr *getArrayBaseExprAndIdxs(const CXXOperatorCallExpr *ASE,
                                      IdxVector &Idxs);

private:
  /// Base name of the array
  std::string BaseName;
  /// Custom array of indices
  IdxVector Idx;
};
} // namespace macveth
#endif /* !MACVETH_MVEXPRARRAY_H */
