/**
 * File              : MVExprArray.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Xov 12 Dec 2019 10:03:14 MST
 * Last Modified Date: Ven 03 Xan 2020 14:56:25 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#ifndef MACVETH_MVEXPRARRAY_H
#define MACVETH_MVEXPRARRAY_H
#include "include/MVExpr/MVExpr.h"

#include <string.h>

using namespace macveth;

namespace macveth {

/// Abstraction of arrays for simplicity
class MVExprArray : public MVExpr {
public:
  struct MVExprArrIdx {
    int Dim = 0;
    std::string Name;
  };
  typedef std::list<std::string> IdxVector;

  virtual ~MVExprArray(){};
  MVExprArray(Expr *E) : MVExpr(E) {
    if (ArraySubscriptExpr *ASE =
            dyn_cast<clang::ArraySubscriptExpr>(E->IgnoreImpCasts())) {
      const Expr *TmpExpr = getArrayBaseExprAndIdxs(ASE, this->Idx);
      this->BaseName = Utils::getStringFromExpr(TmpExpr);
    }
  }
  MVExprArray(MVExprArray *E) : MVExpr(E->getClangExpr()) {
    this->BaseName = E->BaseName;
    this->Idx = E->Idx;
  }

  /// Implementation of unrolling for arrays. In this case we will need to
  /// create a new MVExpr
  virtual MVExpr *unrollExpr(int UF, std::string LL);

  /// As we may not know the total size of the array at compilation time, we
  /// can only know the difference between two indices
  int operator-(const MVExprArray &MVE) {
    if ((getClangExpr() != NULL) && (MVE.getClangExpr() != NULL)) {
      int Diff = 0;
      std::string::size_type sz; // alias of size_t
      std::string A = Idx.back();
      std::string B = MVE.Idx.back();
      Diff = std::stoi(B, &sz) - std::stoi(A, &sz);
      return Diff;
    } else {
      return 0;
    }
  }

private:
  /// Given a dimension LL and a unrolling factor UF, regenerates the expression
  /// with the indexes updated
  void updateIndex(int UF, std::string LL);
  /// Given a ArraySubscriptExpr, recursively gets the base name and the indexes
  /// from the outermost to the innermost
  const Expr *getArrayBaseExprAndIdxs(const ArraySubscriptExpr *ASE,
                                      IdxVector &Idxs);

private:
  std::string BaseName;
  IdxVector Idx;
};
} // namespace macveth
#endif
