#include "include/MVExpr/MVExprArray.h"

//------------------------------------------------
MVExpr *MVExprArray::unrollExpr(int UF, std::string LL) {
  MVExprArray *NewExpr = new MVExprArray(this);
  IdxVector NV;
  for (auto I : NewExpr->Idx) {
    (&I)->updateIndex(UF, LL);
    NV.push_back(I);
  }
  auto NewStrExpr = this->BaseName;
  for (auto I : NV) {
    NewStrExpr += "[" + I.toString() + "]";
  }
  NewExpr->setExprStr(NewStrExpr);
  NewExpr->Idx = NV;
  return NewExpr;
}

//------------------------------------------------
bool checkIfIndexIsAffine(const Expr *E) {
  if (dyn_cast<CallExpr>(E->IgnoreImpCasts())) {
    return false;
  }
  if ((dyn_cast<DeclRefExpr>(E->IgnoreImpCasts())) ||
      (dyn_cast<IntegerLiteral>(E->IgnoreImpCasts()))) {
    return true;
  }

  if (auto Op = dyn_cast<UnaryOperator>(E->IgnoreImpCasts())) {
    return (((Op->getOpcode() == UnaryOperator::Opcode::UO_Minus) ||
             (Op->getOpcode() == UnaryOperator::Opcode::UO_Plus)));
  }
  if (auto Op = dyn_cast<BinaryOperator>(E->IgnoreImpCasts())) {
    return checkIfIndexIsAffine(Op->getLHS()) &&
           checkIfIndexIsAffine(Op->getRHS());
  }
  if (auto P = dyn_cast<ParenExpr>(E)) {
    return checkIfIndexIsAffine(P->getSubExpr());
  }
  return false;
}

//------------------------------------------------
const Expr *MVExprArray::getArrayBaseExprAndIdxs(const ArraySubscriptExpr *ASE,
                                                 IdxVector &Idxs) {
  const Expr *BaseE = nullptr;
  while (ASE) {
    auto IdxE = ASE->getIdx();
    assert(checkIfIndexIsAffine(IdxE) && "Index must be affine");
    BaseE = ASE->getBase()->IgnoreParenCasts();
    ASE = dyn_cast<ArraySubscriptExpr>(BaseE);
    Idxs.push_back(IdxE);
  }

  std::reverse(Idxs.begin(), Idxs.end());
  return BaseE;
}

//------------------------------------------------
const Expr *MVExprArray::getArrayBaseExprAndIdxs(const CXXOperatorCallExpr *C,
                                                 IdxVector &Idxs) {
  Expr *BaseE = nullptr;
  CXXOperatorCallExpr *CXX = const_cast<CXXOperatorCallExpr *>(C);
  while (CXX) {
    if (CXX->getDirectCallee()->getNameAsString() != "operator[]") {
      Utils::printDebug("MVExprArray", "Operator []");
      llvm::llvm_unreachable_internal();
    }
    // FIXME: WTF this is fucking awful
    // Update: not that bad...
    auto E = CXX->arg_begin();
    BaseE = const_cast<Expr *>(*E++);
    CXX = dyn_cast<CXXOperatorCallExpr>(BaseE);
    assert(checkIfIndexIsAffine(*E) && "Index must be affine");
    Idxs.push_back(MVAffineIndex(*E));
  }

  std::reverse(Idxs.begin(), Idxs.end());
  return BaseE;
}