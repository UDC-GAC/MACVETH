// MACVETH - MVExprArray.cpp
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

#include "include/MVExpr/MVExprArray.h"

//------------------------------------------------
MVExpr *MVExprArray::unrollExpr(int UF, std::string LL, bool SubstituteValue) {
  MVExprArray *NewExpr = new MVExprArray(this);
  IdxVector NV;
  for (auto I : NewExpr->Idx) {
    (&I)->updateIndex(UF, LL, SubstituteValue);
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