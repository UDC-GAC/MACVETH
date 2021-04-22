// MACVETH - VectorIR.cpp
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

#include "include/Vectorization/VectorIR.h"
#include "include/MVExpr/MVExprArray.h"
#include "include/Utils.h"
#include <algorithm>

// ---------------------------------------------
bool opsAreSequential(int VL, Node::NodeListType &VOps) {
  bool Sequential = true;
  for (int i = 1; i < VL; ++i) {
    auto PrevOpInfo = VOps[i - 1]->getSchedInfo();
    auto ActualOpInfo = VOps[i]->getSchedInfo();
    auto CondSched = (PrevOpInfo.FreeSched < ActualOpInfo.FreeSched);
    if ((!CondSched) && (PrevOpInfo.FreeSched > ActualOpInfo.FreeSched)) {
      CondSched = (PrevOpInfo.TacID < ActualOpInfo.TacID);
    }
    Sequential &= (CondSched);
  }
  // If there is only one operation, then it is also a sequential operation
  return (VL == 1) || Sequential;
}

// ---------------------------------------------
bool rawDependencies(int VL, Node::NodeListType &VOps,
                     Node::NodeListType &VLoad) {
  for (int i = 0; i < VL - 1; ++i) {
    if (VOps[i]->getOutputInfoName() == VLoad[i + 1]->getRegisterValue()) {
      return true;
    }
  }

  return false;
}

// ---------------------------------------------
bool isAtomic(int VL, Node::NodeListType &VOps, Node::NodeListType &VLoad) {
  for (int i = 0; i < VL; ++i) {
    if (VOps[i]->getSchedInfo().FreeSched <=
        VLoad[i]->getSchedInfo().FreeSched) {
      return false;
    }
  }
  return true;
}

// ---------------------------------------------
bool VectorIR::VOperand::checkIfVectorAssigned(int VL, Node::NodeListType &V,
                                               MVDataType::VWidth Width) {
  for (int n = 0; n < VL; ++n) {
    if (MapRegToVReg.find(std::make_tuple(V[n]->getRegisterValue(), Width)) ==
        MapRegToVReg.end()) {
      return false;
    }
  }
  return true;
}

// ---------------------------------------------
std::string VectorIR::VOperand::toString() {
  std::string Str;
  Str = Name + "[" + (IsTmpResult ? "TempResult, " : "") +
        (IsLoad ? "LoadOp, " : "") + (IsStore ? "StoreOp, " : "");
  if (this->Size < 1) {
    return Str + "]";
  }
  Str += (UOP[0]->getRegisterValue());
  for (size_t i = 1; i < this->VSize; ++i) {
    Str += ", " + (UOP[i]->getRegisterValue());
  }
  Str += "]";
  Str += " (" + std::to_string(this->Order) +
         (this->Offset == -1 ? ")" : std::to_string(this->Offset) + ")");
  return Str;
}

// ---------------------------------------------
std::string VectorIR::VectorOP::toString() {
  if (this->VT == VectorIR::VType::SEQ) {
    return "Scalar: (" + std::to_string(this->Order) +
           (this->Offset == -1 ? ")"
                               : ", " + std::to_string(this->Offset) + ")");
  }
  auto B = (IsUnary) ? "" : "," + OpB.toString();
  auto Str = R.toString() + " = " + VN + "(" + OpA.toString() + B + ")";
  Str += " (" + std::to_string(this->Order);
  Str += this->Offset == -1 ? ")" : ", " + std::to_string(this->Offset) + ")";
  return Str;
}

// ---------------------------------------------
bool VectorIR::VectorOP::isBinOp() {
  return this->R.UOP[0]->getOutputInfo().IsBinaryOp;
}

// ---------------------------------------------
BinaryOperator::Opcode VectorIR::VectorOP::getBinOp() {
  return this->R.UOP[0]->getOutputInfo().MVOP.ClangOP;
}

// ---------------------------------------------
MVOp VectorIR::VectorOP::getMVOp() {
  return this->R.UOP[0]->getOutputInfo().MVOP;
}

// ---------------------------------------------
bool areInSameVector(int VL, Node::NodeListType &V, bool Store) {
  auto E = (V[0]->getOutputInfo().E) && (Store) ? V[0]->getOutputInfo().E
                                                : V[0]->getMVExpr();
  if (!E) {
    return false;
  }
  MVExprArray *A0;
  if (!(A0 = dyn_cast_or_null<MVExprArray>(E))) {
    return false;
  }
  for (auto i = 1; i < VL; ++i) {
    auto VPos = V[i];
    auto NewExpr = (VPos->getOutputInfo().E) && (Store)
                       ? VPos->getOutputInfo().E
                       : VPos->getMVExpr();
    if (!NewExpr) {
      return false;
    }
    if (auto Arr = dyn_cast_or_null<MVExprArray>(NewExpr)) {
      if (A0->getBaseName() != Arr->getBaseName()) {
        return false;
      }
    } else {
      return false;
    }
  }
  return true;
}

// ---------------------------------------------
std::vector<long> getMemIdx(int VL, Node::NodeListType &V, unsigned int Mask,
                            bool Store) {
  std::vector<long> Idx(VL);
  Idx[0] = 0;
  auto E = (V[0]->getOutputInfo().E != nullptr) && (Store)
               ? V[0]->getOutputInfo().E
               : V[0]->getMVExpr();
  if (!E) {
    return Idx;
  }
  MVExprArray::MVAffineIndex Idx0;
  if (auto Arr0 = dyn_cast_or_null<MVExprArray>(E)) {
    Idx0 = Arr0->getIndex().back();
  } else {
    return Idx;
  }
  for (int i = 1; i < VL; ++i) {
    auto E = (V[i]->getOutputInfo().E != nullptr) && (Store)
                 ? V[i]->getOutputInfo().E
                 : V[i]->getMVExpr();
    if (!E) {
      return Idx;
    }
    if (auto MV = dyn_cast_or_null<MVExprArray>(E)) {
      auto IdxI = MV->getIndex().back();
      Idx[i] = IdxI - Idx0;
    } else {
      Idx[0] = -1;
      return Idx;
    }
  }
  return Idx;
}

// ---------------------------------------------
unsigned int getMask(int VL, int VS, Node::NodeListType &V) {
  unsigned int Mask = 0x00;
  for (int n = 0; n < VS; ++n) {
    if ((V[n] != nullptr) && (n < VL)) {
      Mask |= (1 << n);
    }
  }
  return Mask;
}

// ---------------------------------------------
bool VectorIR::VOperand::checkIfAlreadyLoaded(Node *PrimaryNode) {
  for (auto T : MapLoads) {
    if (std::get<1>(T) == this->getName()) {
      if (std::get<0>(T)[0] == PrimaryNode->getScop()[0]) {
        return true;
      }
    }
  }
  return false;
}

// ---------------------------------------------
bool VectorIR::VOperand::checkIfAlreadyStored(Node::NodeListType &V) {
  for (int i = 0; i < (int)V.size(); ++i) {
    auto Reg = V[i]->getRegisterValue();
    if (MapStores.find(Reg) != MapStores.end()) {
      if (MapStores[Reg]) {
        return true;
      }
    }
  }
  return false;
}

// ---------------------------------------------
VectorIR::VOperand::VOperand(){/* empty constructor */};

// ---------------------------------------------
VectorIR::VOperand::VOperand(int VL, Node::NodeListType &V, bool Res) {
  // Init list of unit operands
  this->Order = V[0]->getTacID();
  this->Offset = V[0]->getUnrollFactor();
  // Check if array
  if ((V[0]->getMVExpr() != nullptr) &&
      (dyn_cast<MVExprArray>(V[0]->getMVExpr()))) {
    this->BaseArray = dyn_cast<MVExprArray>(V[0]->getMVExpr())->getBaseName();
  }
  // This is the number of elements not null in this Vector. This is different
  // from the regular size (this->Size), which is the actual vector size
  this->VSize = VL;
  for (int i = 0; i < VL; ++i) {
    this->UOP.push_back(V[i]);
  }
  auto PrimaryNode = V[0];

  // Get data type
  this->DType = MVDataType::CTypeToVDataType[PrimaryNode->getDataType()];
  if ((this->EqualVal) && (this->MemOp)) {
    if ((this->DType == MVDataType::VDataType::FLOAT) ||
        (this->DType == MVDataType::VDataType::DOUBLE)) {
      this->DType = MVDataType::VDataType(this->DType + 1);
    }
  }

  // Computing data width
  this->Width = getWidthFromVDataType(this->VSize, this->DType);
  this->Size = this->Width / MVDataType::VDataTypeWidthBits[this->DType];

  // Check whether the operand is a store or not
  if ((PrimaryNode->getNodeType() == Node::NODE_STORE) && (Res)) {
    this->Name = PrimaryNode->getRegisterValue();
    this->IsStore = true;
    for (int T = 0; T < VL; ++T) {
      this->StoreValues.push_back(V[T]->getRegisterValue());
      MapStores[V[T]->getRegisterValue()] = 1;
    }
  }

  // Check if there is a vector assigned for these operands
  auto VecAssigned = checkIfVectorAssigned(VL, V, this->getWidth());

  // Get name of this operand, otherwise create a custom name
  this->Name = VecAssigned
                   ? MapRegToVReg[std::make_tuple(
                         PrimaryNode->getRegisterValue(), this->getWidth())]
                   : genNewVOpName();

  auto AlreadyLoaded = checkIfAlreadyLoaded(PrimaryNode);
  auto AlreadyStored = checkIfAlreadyStored(this->UOP);

  // It is a temporal result if it has already been assigned. If within a loop,
  // it should have the same Scop
  this->IsTmpResult = VecAssigned && AlreadyLoaded && !this->IsStore;

  // So, if it has not been assigned yet, then we added to the list of loads
  // (or register that we are going to pack somehow). It can also be a store.
  if (!AlreadyLoaded) {
    MapLoads.push_back(
        std::make_tuple(PrimaryNode->getScop(), this->getName()));
  }

  // So if it has not been packed/loaded yet, then we consider it a load
  this->IsLoad = !AlreadyLoaded && !this->IsStore;

  // Checking if operands are all memory
  bool IsMemOp = true;

  // Tracking the operands
  for (int n = 0; n < VL; ++n) {
    IsMemOp = (IsMemOp) && (V[n]->needsMemLoad());
    if (!VecAssigned) {
      MapRegToVReg[std::make_tuple(V[n]->getRegisterValue(),
                                   this->getWidth())] = this->Name;
    }
    if (n > 0) {
      this->EqualVal = this->EqualVal && (V[n]->getRegisterValue() ==
                                          V[n - 1]->getRegisterValue());
    }
  }
  this->MemOp = (IsMemOp || AlreadyStored);

  // Get data mask
  this->Mask = getMask(this->VSize, this->Size, V);

  // Determine whether is partial or not
  this->IsPartial = !(this->Size == this->VSize);

  // In case we have to access to memory we are also interested in how we do it:
  // if we have to use an index for it, or if we have to shuffle data
  if ((this->MemOp) || (this->IsStore)) {
    this->SameVector = areInSameVector(VL, V, this->IsStore || AlreadyStored);
    if (this->SameVector) {
      // Get Memory index
      this->Idx = getMemIdx(VL, V, this->Mask, this->IsStore || AlreadyStored);
      if (this->Idx[0] != -1) {
        auto T = true;
        if (VL > 1) {
          for (int i = 1; i < VL; ++i) {
            T &= ((Idx[i] - 1) == Idx[i - 1]);
            Utils::printDebug(
                "VOperand", "Idx[i]-1 = " + std::to_string((Idx[i] - 1)) +
                                "; Idx[i-1] = " + std::to_string((Idx[i - 1])));
          }
        }
        Utils::printDebug("VOperand", "Contiguous = " + std::to_string(T));
        this->Contiguous = T;
      }
    }
  }
};

// ---------------------------------------------
VectorIR::VType getVectorOpType(int VL, Node::NodeListType &VOps,
                                Node::NodeListType &VLoadA,
                                Node::NodeListType &VLoadB) {
  // Premises of our algorithm
  // 1.- Check whether operations are sequential
  bool Seq = opsAreSequential(VL, VOps);

  // 2.- Check if operands have RAW dependencies with output of the operations
  bool RAW_A = rawDependencies(VL, VOps, VLoadA);
  bool RAW_B = rawDependencies(VL, VOps, VLoadB);
  // 3.- Check the atomicity of the operands: they can be computed/loaded before
  //     the operations
  bool Atomic_A = isAtomic(VL, VOps, VLoadA);
  bool Atomic_B = isAtomic(VL, VOps, VLoadB);

  // Type of VectorOP
  Utils::printDebug("VectorIR", "Seq = " + std::to_string(Seq) + "; " +
                                    "RAW_A = " + std::to_string(RAW_A) + "; " +
                                    "RAW_B = " + std::to_string(RAW_B) + "; " +
                                    "Atomic_A = " + std::to_string(Atomic_A) +
                                    "; " +
                                    "Atomic_B = " + std::to_string(Atomic_B));

  // Decide which type of VectorOp it is according to the features of its
  // VOperands
  if ((Seq) && ((RAW_A) || (RAW_B)) && Atomic_A && Atomic_B) {
    return VectorIR::VType::REDUCE;
  } else if ((!Seq) && (!RAW_A) && (!RAW_B) && Atomic_A && Atomic_B) {
    return VectorIR::VType::MAP;
  } else {
    return VectorIR::VType::SEQ;
  }
}

// ---------------------------------------------
VectorIR::VType getVectorOpType(int VL, Node::NodeListType &VOps,
                                Node::NodeListType &VLoadA) {
  // Premises of our algorithm
  // 1.- Check whether operations are sequential
  bool Seq = opsAreSequential(VL, VOps);
  if (Seq) {
    Utils::printDebug("CDAG", "Ops are sequential");
  }
  // 2.- Check if operands have RAW dependencies with output of the operations
  bool RAW_A = rawDependencies(VL, VOps, VLoadA);
  // 3.- Check the atomicity of the operands: they can be computed/loaded before
  //     the operations
  bool Atomic_A = isAtomic(VL, VOps, VLoadA);

  // Type of VectorOP
  Utils::printDebug("VectorIR", "Seq = " + std::to_string(Seq));
  Utils::printDebug("VectorIR", "RAW_A = " + std::to_string(RAW_A));
  Utils::printDebug("VectorIR", "Atomic_A = " + std::to_string(Atomic_A));

  // Decide which type of VectorOp it is according to the features of its
  // VOperands
  if ((Seq) && ((RAW_A) && Atomic_A)) {
    return VectorIR::VType::REDUCE;
  } else if ((!Seq) && (!RAW_A) && Atomic_A) {
    return VectorIR::VType::MAP;
  } else {
    return VectorIR::VType::SEQ;
  }
}

// ---------------------------------------------
VectorIR::VectorOP::VectorOP(int VL, Node::NodeListType &VOps,
                             Node::NodeListType &VLoadA,
                             Node::NodeListType &VLoadB) {
  // The assumption is that as all operations are the same, then all the
  // operations have the same TAC order
  this->Order = VOps[0]->getTacID();
  this->Offset = VOps[0]->getUnrollFactor();
  // By design, if the operand is an assignment, is because the operation is
  // unary, therefore: R = R OP B
  if (VOps[0]->getOutputInfo().MVOP.isAssignment()) {
    VLoadA = VLoadB;
    VLoadB[0] = nullptr;
  }
  if (VLoadB[0] != nullptr) {
    // Vector type will depend on the operations and operations, logically
    this->VT = getVectorOpType(VL, VOps, VLoadA, VLoadB);
  } else {
    // Vector type will depend on the operations and operations, logically
    this->VT = getVectorOpType(VL, VOps, VLoadA);
    this->IsUnary = true;
  }

  // If this operation is sequential, then we do not care about its vector
  // form, as it will be synthesized in its original form
  if (this->VT == VectorIR::VType::SEQ) {
    return;
  }

  // Creating vector operands
  this->OpA = VOperand(VL, VLoadA, false);
  // Vector operations could be unary, e.g. log(x)
  if (!this->IsUnary) {
    this->OpB = VOperand(VL, VLoadB, false);
  }

  // Result operand
  this->R = VOperand(VL, VOps, true);

  // Assuming that inputs and outputs have the same type
  this->OpA.DType = this->R.DType;
  this->OpB.DType = this->R.DType;

  // Name: operation (assuming all operations have the same value, which is a
  // valid assumption)
  this->VN = VOps[0]->getValue();

  // The width of the operation is the result width
  this->VW = this->R.getWidth();
  this->OpA.Width = this->VW;
  this->OpB.Width = this->VW;

  // Data type: vector operation will have the same data type as the original
  // result
  this->DT = this->R.DType;
}
