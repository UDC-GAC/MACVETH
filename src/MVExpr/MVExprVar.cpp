// MACVETH - MVExprVar.cpp
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
//     Louis-Noël Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Louis-Noël Pouchet <pouchet@colostate.edu>

#include "include/MVExpr/MVExprVar.h"
#include <string>

//------------------------------------------------
void MVExprVar::regUnrollDim(MVExpr *MVE, std::string Dim, int UF) {
  auto M = dyn_cast<MVExprVar>(MVE);
  if (!M) {
    return;
  }
  MVExprVar::RegDimUnrolled[MVE->getExprStr()].push_back(Dim);
}

//------------------------------------------------
void MVExprVar::regUndoUnrollDim(MVExpr *MVE, std::string Dim, int UF) {
  auto M = dyn_cast<MVExprVar>(MVE);
  if (!M) {
    return;
  }
  auto L = MVExprVar::RegDimUnrolled.find(MVE->getExprStr());
  if (L != MVExprVar::RegDimUnrolled.end()) {
    auto it = L->second;
    auto E = std::find(it.begin(), it.end(), Dim);
    if (E != it.end()) {
      it.erase(E);
    }
  }
}

//------------------------------------------------
bool MVExprVar::hasBeenUnrolled(std::string Reg, std::string Dim) {
  auto it = MVExprVar::RegDimUnrolled.find(Reg);
  if (it != RegDimUnrolled.end()) {
    for (auto D : it->second) {
      if (D == Dim) {
        return true;
      }
    }
  }
  return false;
}

//------------------------------------------------
MVExpr *MVExprVar::unrollExpr(int UF, std::string LL, bool SubstituteVal) {
  MVExprVar *NewExpr = new MVExprVar(this);
  if (this->getTempInfo() == MVExpr::MVExprInfo::TMP_RES) {
    NewExpr->setExprStr(NewExpr->getExprStr() + LL + std::to_string(UF));
    return NewExpr;
  } else if (this->getExprStr() == LL) {
    // Special case, when unrolling loop variable
    NewExpr->setExprStr("(" + (NewExpr->getExprStr()) + " + " +
                        std::to_string(UF) + ")");
    NewExpr->setDoNotLoadFromMem();
    return NewExpr;
  }
  return this;
}

//------------------------------------------------
MVExpr *unrollExprString(MVExprVar *V, std::string LL) {
  if (V->getTempInfo() == MVExpr::MVExprInfo::TMP_RES) {
    MVExprVar *NewExpr = new MVExprVar(V);
    NewExpr->setExprStr(NewExpr->getExprStr() + LL);
    return NewExpr;
  }
  return V;
}
