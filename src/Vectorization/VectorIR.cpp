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
//     Louis-Noël Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Marcos Horro <marcos.horro@udc.es>

#include "include/Vectorization/VectorIR.h"
#include "include/Debug.h"
#include "include/MVExpr/MVExprArray.h"
#include "include/Utils.h"
#include <algorithm>

// ---------------------------------------------
bool opsAreSequential(int VL, const NodeVectorT &VOps) {
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
bool rawDependencies(int VL, const NodeVectorT &VOps,
                     const NodeVectorT &VLoad) {
  for (int i = 0; i < VL - 1; ++i) {
    if (VOps[i]->getOutputInfoName() == VLoad[i + 1]->getRegisterValue()) {
      return true;
    }
  }

  return false;
}

// ---------------------------------------------
bool isAtomic(int VL, const NodeVectorT &VOps, const NodeVectorT &VLoad) {
  for (int i = 0; i < VL; ++i) {
    if (VOps[i]->getSchedInfo().FreeSched <=
        VLoad[i]->getSchedInfo().FreeSched) {
      return false;
    }
  }
  return true;
}

// ---------------------------------------------
bool VOperand::checkIfVectorAssigned(int VL, NodeVectorT &V,
                                     MVDataType::VWidth Width) {
  for (int n = 0; n < VL; ++n) {
    if (VectorIR::MapRegToVReg.find(V[n]->getRegisterValue()) ==
        VectorIR::MapRegToVReg.end()) {
      return false;
    }
  }
  return true;
}

// ---------------------------------------------
std::string getStrTypeOp(VectorIR::VType T) {
  switch (T) {
  case VectorIR::VType::SEQ:
    return "SEQ";
  case VectorIR::VType::INDUCTION:
    return "INDUCTION";
  case VectorIR::VType::MAP:
    return "MAP";
  case VectorIR::VType::REDUCE:
    return "REDUCE";
  case VectorIR::VType::SCATTER:
    return "SCATTER_OP";
  }
  return "undef";
}

// ---------------------------------------------
std::string VOperand::toString() {
  std::string Str = Name + "[" + (IsTmpResult ? "TempRes, " : "") +
                    (IsLoad ? "LoadOp, " : "") + (IsStore ? "StoreOp, " : "");
  if (this->Size < 1) {
    return Str + "]";
  }
  Str += (UOP[0]->getRegisterValue());
  for (size_t i = 1; i < this->VectorLength; ++i) {
    Str += ", " + (UOP[i]->getRegisterValue());
  }
  Str += "]";
  Str += " (ord " + std::to_string(this->Order) +
         (this->Offset == -1 ? ")"
                             : ", off " + std::to_string(this->Offset) + ")");
  Str += "; Part = " + std::to_string(IsPartial);
  Str += "; RegPack = " + std::to_string(RequiresRegisterPacking);
  Str += "; SamVec = " + std::to_string(SameVector);
  Str += "; EqVal = " + std::to_string(EqualVal);
  Str += "; LD = " + std::to_string(IsLoad);
  Str += "; ST = " + std::to_string(IsStore);
  Str += "; Mem = " + std::to_string(MemOp);
  Str +=
      "; V/Size = " + std::to_string(VectorLength) + "/" + std::to_string(Size);
  return Str;
}

// ---------------------------------------------
std::string VectorOP::toString() {
  std::string Str = "[" + getStrTypeOp(this->VT) + "] ";
  if (this->VT == VectorIR::VType::SEQ) {
    return Str + " (" + std::to_string(this->Order) +
           (this->Offset == -1 ? ")"
                               : ", " + std::to_string(this->Offset) + ")");
  }
  auto B = (IsUnary) ? "" : ",\n\t" + OpB.toString();
  Str += R.toString() + " = " + VN + "(\n\t" + OpA.toString() + B + ")";
  Str += " (ord " + std::to_string(this->Order);
  Str +=
      this->Offset == -1 ? ")" : ", off " + std::to_string(this->Offset) + ")";
  return Str;
}

// ---------------------------------------------
bool VectorOP::isBinOp() { return R.UOP[0]->getOutputInfo().IsBinaryOp; }

// ---------------------------------------------
BinaryOperator::Opcode VectorOP::getBinOp() {
  return R.UOP[0]->getOutputInfo().MVOP.ClangOP;
}

// ---------------------------------------------
MVOp VectorOP::getMVOp() { return R.UOP[0]->getOutputInfo().MVOP; }

// ---------------------------------------------
bool areInSameVector(int VL, NodeVectorT &V, bool Store) {
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
std::vector<long> getMemIdx(int VL, NodeVectorT &V, unsigned int Mask,
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
unsigned int getMask(int VL, int VS, NodeVectorT &V) {
  unsigned int Mask = 0x00;
  for (int n = 0; n < VS; ++n) {
    if ((V[n] != nullptr) && (n < VL)) {
      Mask |= (1 << n);
    }
  }
  return Mask;
}

// ---------------------------------------------
bool VOperand::checkIfAlreadyLoaded(Node *PrimaryNode) {
  for (auto T : VectorIR::MapLoads) {
    if (std::get<1>(T) == this->getName()) {
      if (std::get<0>(T)[0] == PrimaryNode->getScop()[0]) {
        return true;
      }
    }
  }
  return false;
}

// ---------------------------------------------
bool VOperand::checkIfAlreadyStored(NodeVectorT &V) {
  for (int i = 0; i < (int)V.size(); ++i) {
    auto Reg = V[i]->getRegisterValue();
    MACVETH_DEBUG("VOperand", "already stored reg = " + Reg);
    if (VectorIR::MapStores.find(Reg) != VectorIR::MapStores.end()) {
      MACVETH_DEBUG("VOperand", "already stored reg = " + Reg + " found!");
      if (VectorIR::MapStores[Reg] > 0) {
        MACVETH_DEBUG("VOperand", "already stored true");
        return true;
      }
    }
  }
  MACVETH_DEBUG("VOperand", "already stored = false");
  return false;
}

// ---------------------------------------------
VOperand::VOperand(){/* empty constructor */};

// ---------------------------------------------
VOperand::VOperand(int VectorLength, NodeVectorT &V, bool Res, int Order) {
  // Init list of unit operands
  this->Order = Order;
  for (int i = 0; i < VectorLength; ++i) {
    int NewOrder = V[i]->getTacID();
    int LoadOffset = Res ? 0 : 1;
    this->Offset = std::max(this->Offset, V[i]->getUnrollFactor());
    if (VectorIR::MapStores.find(V[i]->getRegisterValue()) !=
        VectorIR::MapStores.end()) {
      NewOrder = std::max(
          NewOrder, VectorIR::MapStores[V[i]->getRegisterValue()] + LoadOffset);
      this->Offset++;
      this->AfterWrite = true;
    }
    this->Order = std::max(this->Order, NewOrder);
  }
  auto PrimaryNode = V[0];
  // Check if array
  if ((PrimaryNode->getMVExpr() != nullptr) &&
      (dyn_cast<MVExprArray>(PrimaryNode->getMVExpr()))) {
    this->BaseArray =
        dyn_cast<MVExprArray>(PrimaryNode->getMVExpr())->getBaseName();
  }
  // This is the number of elements not null in this Vector. This is different
  // from the regular size (this->Size), which is the actual vector size
  this->VectorLength = VectorLength;

  // Get data type
  this->DType = MVDataType::CTypeToVDataType[PrimaryNode->getDataType()];

  // Computing data width
  this->Width =
      VectorIR::getWidthFromVDataType(this->VectorLength, this->DType);
  this->Size = this->Width / MVDataType::VDataTypeWidthBits[this->DType];
  for (int i = 0; i < VectorLength; ++i) {
    this->UOP.push_back(V[i]);
  }

  // Check whether the operand is a store or not
  if ((PrimaryNode->getNodeType() == Node::NODE_STORE) && (Res)) {
    this->Name = PrimaryNode->getRegisterValue();
    this->IsStore = true;
    for (int T = 0; T < VectorLength; ++T) {
      this->StoreValues.push_back(V[T]->getRegisterValue());
      VectorIR::MapStores[V[T]->getRegisterValue()] = this->Order;
      // When storing a value, it is not anymore in the "virtual registers"
      if (VectorIR::MapRegToVReg.find(V[T]->getRegisterValue()) ==
          VectorIR::MapRegToVReg.end()) {
        VectorIR::MapRegToVReg.erase(V[T]->getRegisterValue());
      }
    }
  }

  // Determine whether is partial or not
  this->IsPartial = !(this->Size == this->VectorLength);

  // Check if there is a vector assigned for these operands
  auto VecAssigned = checkIfVectorAssigned(VectorLength, V, this->getWidth());

  // Does it requires a register shuffling
  if (VecAssigned) {
    std::set<std::string> RegistersUsed;
    for (int n = 0; n < VectorLength; ++n) {
      auto NewRegIdx = VectorIR::MapRegToVReg[V[n]->getRegisterValue()];
      this->RegIdx.push_back(NewRegIdx);
      auto Reg = std::get<0>(NewRegIdx);
      RegistersUsed.insert(Reg);
      VectorIR::LiveOut[Reg] =
          std::max(this->Order, (int)VectorIR::LiveOut[Reg]);
      this->Order = std::max(this->Order, (int)VectorIR::LiveIn[Reg]);
      // Check if index of the register is the same
      this->RequiresRegisterPacking |= (n != std::get<1>(NewRegIdx));
      MACVETH_DEBUG("VOperand", "Reg = " + Reg + "; idx = " +
                                    std::to_string(std::get<1>(NewRegIdx)) +
                                    "; order = " + std::to_string(this->Order));
    }
    assert(RegistersUsed.size() >= 1 &&
           "We should have used at least one register");

    // Another condition to determine if register is partial or not: check
    // vector width of original register and this new vector:
    this->SameVector = (RegistersUsed.size() == 1);
    this->IsPartial = !this->SameVector;
    if (this->SameVector) {
      std::string RegName = *RegistersUsed.begin();
      if (!this->IsStore)
        this->IsPartial = (VectorIR::SlotsUsed[RegName] != VectorLength);
      this->IsPartial |=
          ((VectorLength > 2) && (VectorLength != (int)this->Size));
      if (this->RequiresRegisterPacking)
        this->IsPartial = (VectorIR::MapRegSize[RegName] != (int)this->Size);
    }
  }

  // Get name of this operand, otherwise create a custom name
  this->Name =
      (VecAssigned && !this->RequiresRegisterPacking &&
       (!this->IsPartial || this->SameVector))
          ? std::get<0>(VectorIR::MapRegToVReg[PrimaryNode->getRegisterValue()])
          : genNewVOpName();

  auto AlreadyLoaded = checkIfAlreadyLoaded(PrimaryNode);
  auto AlreadyStored = checkIfAlreadyStored(this->UOP);

  // It is a temporal result if it has already been assigned. If within a loop,
  // it should have the same Scop
  this->IsTmpResult = VecAssigned && AlreadyLoaded && !this->IsStore;

  // So, if it has not been assigned yet, then we added to the list of loads
  // (or register that we are going to pack somehow). It can also be a store.
  if ((!AlreadyLoaded) && (!this->IsStore))
    VectorIR::MapLoads.push_back(
        std::make_tuple(PrimaryNode->getScop(), this->getName()));

  // So if it has not been packed/loaded yet, then we consider it a load
  this->IsLoad = !AlreadyLoaded && !this->IsStore;

  // Checking if operands are all memory
  this->MemOp = AlreadyStored;

  // Tracking the operands
  bool IsMemOp = false;
  for (int n = 0; n < VectorLength; ++n) {
    MACVETH_DEBUG("VectorIR", V[n]->getRegisterValue() + "; needsMemLoad = " +
                                  std::to_string(V[n]->needsMemLoad()));
    // FIXME: all operands should be required to be from memory, not only one...
    IsMemOp |= (V[n]->needsMemLoad());
    if ((!VecAssigned) && (!this->IsStore) && Res)
      VectorIR::MapRegToVReg[V[n]->getRegisterValue()] =
          std::make_tuple(this->Name, n);

    VectorIR::SlotsUsed[this->Name] = VectorLength;
    if (!VecAssigned)
      VectorIR::LiveIn[this->Name] = this->Order;
    VectorIR::MapRegSize[this->Name] = VectorLength;
    if (n > 0) {
      this->EqualVal &=
          (V[n]->getRegisterValue() == V[n - 1]->getRegisterValue());
    }
  }
  this->MemOp |= (IsMemOp);
  MACVETH_DEBUG("VectorIR", "MemOp = " + std::to_string(this->MemOp));

  // Get data mask
  this->Mask = getMask(this->VectorLength, this->Size, V);

  // In case we have to access to memory we are also interested in how we do it:
  // if we have to use an index for it, or if we have to shuffle data
  if ((this->MemOp) || (this->IsStore)) {
    this->SameVector =
        areInSameVector(VectorLength, V, this->IsStore || AlreadyStored);
    if (this->SameVector) {
      // Get Memory index
      this->Idx = getMemIdx(VectorLength, V, this->Mask,
                            this->IsStore || AlreadyStored);
      if (this->Idx[0] != -1) {
        for (int i = 1; i < VectorLength; ++i) {
          this->Contiguous &= ((Idx[i] - 1) == Idx[i - 1]);
        }
      }
    }
  }
  this->LowBits = (this->VectorLength == 2) && (this->Size == 4);
  VectorIR::LiveOut[this->Name] = this->Order;
  MACVETH_DEBUG("VOperand", this->toString());
};

// ---------------------------------------------
bool areThereReductions(int VL, const NodeVectorT &VOps) {
  bool Redux = true;
  if (VL == 1) {
    return false;
  }
  int UB = VL;
  if (UB % 2 == 1) {
    UB--;
  }
  for (int i = 0; i < UB; i += 2) {
    auto ActualOpInfo = VOps[i]->getSchedInfo();
    auto NextOpInfo = VOps[i + 1]->getSchedInfo();
    auto CondSched = (ActualOpInfo.FreeSched < NextOpInfo.FreeSched);
    if ((!CondSched) && (ActualOpInfo.FreeSched > NextOpInfo.FreeSched)) {
      CondSched = (ActualOpInfo.TacID < NextOpInfo.TacID);
    }
    Redux &= CondSched;
  }
  return Redux;
}

// ---------------------------------------------
VectorIR::VType getVectorOpType(int VL, const NodeVectorT &VOps,
                                const NodeVectorT &VLoadA,
                                const NodeVectorT &VLoadB, bool Reduction) {
  // Premises of our algorithm
  // 1.- Check whether operations are sequential
  bool Seq = opsAreSequential(VL, VOps);

  bool ReduxPatterns = areThereReductions(VL, VOps);

  // 2.- Check if operands have RAW dependencies with output of the operations
  bool RAW_A = rawDependencies(VL, VOps, VLoadA);
  bool RAW_B = rawDependencies(VL, VOps, VLoadB);
  // 3.- Check the atomicity of the operands: they can be computed/loaded before
  //     the operations
  bool Atomic_A = isAtomic(VL, VOps, VLoadA);
  bool Atomic_B = isAtomic(VL, VOps, VLoadB);

  // Type of VectorOP
  MACVETH_DEBUG("VectorIR", "Seq = " + std::to_string(Seq) + "; Redux = " +
                                std::to_string(Reduction || ReduxPatterns) +
                                "; RAW_A = " + std::to_string(RAW_A) +
                                "; RAW_B = " + std::to_string(RAW_B) +
                                "; Atomic_A = " + std::to_string(Atomic_A) +
                                "; Atomic_B = " + std::to_string(Atomic_B));

  // Decide which type of VectorOp it is according to the features of its
  // VOperands
  if ((Seq || Reduction || ReduxPatterns) && ((RAW_A) || (RAW_B)) && Atomic_A &&
      Atomic_B) {
    return VectorIR::VType::REDUCE;
  } else if ((!Seq || ReduxPatterns) && (!RAW_A) && (!RAW_B) && Atomic_A &&
             Atomic_B) {
    return VectorIR::VType::MAP;
  }
  return VectorIR::VType::SEQ;
}

// ---------------------------------------------
VectorIR::VType getVectorOpType(int VL, const NodeVectorT &VOps,
                                const NodeVectorT &VLoadA) {
  // Premises of our algorithm
  // 1.- Check whether operations are sequential
  bool Seq = opsAreSequential(VL, VOps);
  if (Seq) {
    MACVETH_DEBUG("CDAG", "Ops are sequential");
  }
  // 2.- Check if operands have RAW dependencies with output of the operations
  bool RAW_A = rawDependencies(VL, VOps, VLoadA);
  // 3.- Check the atomicity of the operands: they can be computed/loaded before
  //     the operations
  bool Atomic_A = isAtomic(VL, VOps, VLoadA);

  // Type of VectorOP
  MACVETH_DEBUG("VectorIR", "Seq = " + std::to_string(Seq) +
                                "; RAW_A = " + std::to_string(RAW_A) +
                                "; Atomic_A = " + std::to_string(Atomic_A));

  // Decide which type of VectorOp it is according to the features of its
  // VOperands
  if ((Seq) && ((RAW_A) && Atomic_A)) {
    return VectorIR::VType::REDUCE;
  } else if ((!Seq) && (!RAW_A) && Atomic_A) {
    return VectorIR::VType::MAP;
  }
  return VectorIR::VType::SEQ;
}

// ---------------------------------------------
VectorOP::VectorOP(int VL, NodeVectorT &VOps, NodeVectorT &VLoadA,
                   NodeVectorT &VLoadB, bool Reduction) {
  // The assumption is that as all operations are the same, then all the
  // operations have the same TAC order
  auto PrimaryVOP = VOps[0];
  // Name: operation (assuming all operations have the same value, which is a
  // valid assumption)
  this->VN = PrimaryVOP->getValue();
  this->Order = PrimaryVOP->getTacID();
  this->Offset = PrimaryVOP->getUnrollFactor();
  for (int i = 1; i < VL; ++i) {
    this->Order = std::max(this->Order, VOps[i]->getTacID());
    this->Offset = std::max(this->Offset, VOps[i]->getUnrollFactor());
  }
  // By design, if the operand is an assignment, is because the operation is
  // unary, therefore: R = R OP B
  if (PrimaryVOP->getOutputInfo().MVOP.isAssignment()) {
    VLoadA = VLoadB;
    VLoadB[0] = nullptr;
  }
  if (VLoadB[0] != nullptr) {
    // Vector type will depend on the operations and operations, logically
    this->VT = getVectorOpType(VL, VOps, VLoadA, VLoadB, Reduction);
  } else {
    // Vector type will depend on the operations and operations, logically
    this->VT = getVectorOpType(VL, VOps, VLoadA);
    this->IsUnary = true;
  }

  // If this operation is sequential, then we do not care about its vector
  // form, as it will be synthesized in its original form
  if (this->isSequential()) {
    MACVETH_DEBUG("VectorOP", "This is a sequential operation");
    // for (int i = 0; i < VL; ++i)
    //   VectorIR::SequentialResults.push_back(VOps[i]->getRegisterValue());
    return;
  }

  if (this->isReduction()) {
    this->OpA = VOperand(VL, VLoadB, false);
    this->IsUnary = true;
  } else {
    // Creating vector operands
    this->OpA = VOperand(VL, VLoadA, false);
    // Vector operations could be unary, e.g. log(x)
    if (!this->IsUnary) {
      this->OpB = VOperand(VL, VLoadB, false);
    }
  }

  // Result operand
  this->Order = std::max(this->Order, this->OpA.Order);
  this->Order = std::max(this->Order, this->OpB.Order);
  this->R = VOperand(VL, VOps, true, this->Order);
  this->Order = std::max(this->Order, this->R.Order);

  // Update result order
  this->OpA.DType = this->R.DType;
  this->OpB.DType = this->R.DType;

  // The width of the operation is the result width
  this->VW = this->R.getWidth();
  this->OpA.Width = this->VW;
  this->OpB.Width = this->VW;

  // Data type: vector operation will have the same data type as the original
  // result
  this->DT = this->R.DType;

  MACVETH_DEBUG("VectorOP", "VectorOP => " + this->toString());
}
