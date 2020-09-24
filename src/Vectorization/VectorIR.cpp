/**
 * File					 : src/VectorIR.cpp
 * Author				 : Marcos Horro
 * Date					 : Wed 03 Jun 2020 04:28 +02:00
 *
 * Last Modified : Wed 10 Jun 2020 09:57 +02:00
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

#include "include/Vectorization/VectorIR.h"
#include "include/MVExpr/MVExprArray.h"
#include "include/Utils.h"
#include <algorithm>

// ---------------------------------------------
bool opsAreSequential(int VL, Node *VOps[]) {
  for (int i = 1; i < VL; ++i) {
    if ((VOps[i - 1]->getSchedInfo().FreeSched <
         VOps[i]->getSchedInfo().FreeSched)) {
      return true;
    }
  }
  // If there is only one operation, then it is also a sequential operation
  return (VL == 1);
}

// ---------------------------------------------
bool rawDependencies(int VL, Node *VOps[], Node *VLoad[]) {
  for (int i = 0; i < VL - 1; ++i) {
    if (VOps[i]->getOutputInfoName() == VLoad[i + 1]->getRegisterValue()) {
      return true;
    }
  }

  return false;
}

// ---------------------------------------------
bool isAtomic(int VL, Node *VOps[], Node *VLoad[]) {
  for (int i = 0; i < VL; ++i) {
    if (VOps[i]->getSchedInfo().FreeSched <=
        VLoad[i]->getSchedInfo().FreeSched) {
      return false;
    }
  }
  return true;
}

// ---------------------------------------------
bool VectorIR::VOperand::checkIfVectorAssigned(int VL, Node *V[]) {
  for (int n = 0; n < VL; ++n) {
    if (MapRegToVReg.find(V[n]->getRegisterValue()) == MapRegToVReg.end()) {
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
  for (int i = 1; i < this->VSize; ++i) {
    Str += ", " + (UOP[i]->getRegisterValue());
  }
  Str += "]";
  return Str;
}

// ---------------------------------------------
std::string VectorIR::VectorOP::toString() {
  if (this->VT == VectorIR::VType::SEQ) {
    return "Sequential";
  }
  auto B = (IsUnary) ? "" : "," + OpB.toString();
  auto Str = R.toString() + " = " + VN + "(" + OpA.toString() + B + ")";
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
bool areInSameVector(int VL, Node *V[]) {
  auto A0 = dyn_cast<MVExprArray>(V[0]->getMVExpr());
  if (!dyn_cast<MVExprArray>(V[0]->getMVExpr())) {
    return false;
  }
  for (auto i = 1; i < VL; ++i) {
    auto Arr = dyn_cast<MVExprArray>(V[i]->getMVExpr());
    if (!Arr) {
      return false;
    }
    if (A0->getBaseName() != Arr->getBaseName()) {
      return false;
    }
  }
  return true;
}

// ---------------------------------------------
int64_t *getMemIdx(int VL, Node *V[], unsigned int Mask) {
  int64_t *Idx = (int64_t *)calloc(sizeof(int64_t), VL);
  Idx[0] = 0;
  auto Arr0 = dyn_cast<MVExprArray>(V[0]->getMVExpr());
  auto Idx0 = Arr0->getIndex().back();
  for (int i = 1; i < VL; ++i) {
    auto MV = dyn_cast<MVExprArray>(V[i]->getMVExpr());
    if (!MV) {
      return nullptr;
    }
    auto IdxI = MV->getIndex().back();
    Idx[i] = IdxI - Idx0;
  }
  return Idx;
}

// ---------------------------------------------
unsigned int getShuffle(int VL, int Width, Node *V[]) {
  // TODO: don't know if this is worthable
  unsigned int Shuffle = 0x0;
  int Bits = Width / 4;
  for (int n = 0; n < VL; ++n) {
  }

  return Shuffle;
}

// ---------------------------------------------
unsigned int getMask(int VL, Node *V[]) {
  unsigned int Mask = 0x00;
  for (int n = 0; n < VL; ++n) {
    if (V[n] != nullptr) {
      Mask |= (1 << n);
    }
  }
  return Mask;
}

// ---------------------------------------------
VectorIR::VOperand::VOperand(){/* empty constructor */};

// ---------------------------------------------
VectorIR::VOperand::VOperand(int VL, Node *V[], bool Res) {
  // Init list of unit operands
  this->UOP = (Node **)malloc(sizeof(Node *) * VL);

  // This is the number of elements in this Vector
  this->VSize = VL;

  auto PrimaryNode = V[0];

  // Check whether the operand is a store or not
  if ((PrimaryNode->getNodeType() == Node::NODE_STORE) && (Res)) {
    this->Name = PrimaryNode->getRegisterValue();
    this->IsStore = true;
  }

  // Check if there is a vector assigned for these operands
  auto VecAssigned = checkIfVectorAssigned(VL, V);

  // Get name of this operand, otherwise create a custom name
  this->Name = VecAssigned ? MapRegToVReg[PrimaryNode->getRegisterValue()]
                           : genNewVOpName();

  // Check if this has been already loaded
  auto AlreadyLoaded = false;
  for (auto T : MapLoads) {
    if (std::get<1>(T) == this->getName()) {
      if (std::get<0>(T)[0] == PrimaryNode->getScop()[0]) {
        AlreadyLoaded = true;
        break;
      }
    }
  }

  // It is a temporal result if it has already been assigned. If within a loop,
  // it should have the same Scop
  this->IsTmpResult = VecAssigned && AlreadyLoaded;

  // So, if it has not been assigned yet, then we added to the list of loads
  // (or register that we are going to pack somehow). It can also be a store.
  if (!AlreadyLoaded)
    MapLoads.push_back(
        std::make_tuple(PrimaryNode->getScop(), this->getName()));

  // So if it has not been packed/loaded yet, then we consider it a load
  this->IsLoad = !AlreadyLoaded && !this->IsStore;

  // Checking if operands are all memory
  bool IsMemOp = true;
  // Tracking the operands
  for (int n = 0; n < VL; ++n) {
    IsMemOp = (IsMemOp) && (V[n]->needsMemLoad());
    this->UOP[n] = V[n];
    if (!VecAssigned) {
      MapRegToVReg[V[n]->getRegisterValue()] = this->Name;
    }
    if (n > 0) {
      this->EqualVal = this->EqualVal && (V[n]->getRegisterValue() ==
                                          V[n - 1]->getRegisterValue());
    }
  }
  this->MemOp = IsMemOp;

  // Get data mask
  this->Mask = getMask(VL, V);

  // Determine whether is partial or not
  if (true && (Mask != ((1 << VL) - 1))) {
    this->IsPartial = true;
  }

  // In case we have to access to memory we are also interested in how we do it:
  // if we have to use an index for it, or if we have to shuffle data
  if (this->MemOp) {
    this->SameVector = areInSameVector(VL, V);
    if (this->SameVector) {
      // Get Memory index
      this->Idx = getMemIdx(VL, V, this->Mask);
      if (this->Idx != nullptr) {
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
    // TODO: Get shuffle index
    this->Shuffle = getShuffle(VL, this->getWidth(), V);
  }

  // Get data type
  this->DType = CTypeToVDataType[this->UOP[0]->getDataType()];
  if ((this->EqualVal) && (this->MemOp)) {
    if ((this->DType == VectorIR::VDataType::FLOAT) ||
        (this->DType == VectorIR::VDataType::DOUBLE)) {
      this->DType = VectorIR::VDataType(this->DType + 1);
    }
  }

  // Computing data width
  this->Width = getWidthFromVDataType(this->VSize, this->DType);
};

// ---------------------------------------------
VectorIR::VType getVectorOpType(int VL, Node *VOps[], Node *VLoadA[],
                                Node *VLoadB[]) {
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
  Utils::printDebug("VectorIR", "Seq = " + std::to_string(Seq));
  Utils::printDebug("VectorIR", "RAW_A = " + std::to_string(RAW_A));
  Utils::printDebug("VectorIR", "RAW_B = " + std::to_string(RAW_B));
  Utils::printDebug("VectorIR", "Atomic_A = " + std::to_string(Atomic_A));
  Utils::printDebug("VectorIR", "Atomic_B = " + std::to_string(Atomic_B));

  // Decide which type of VectorOp it is according to the features of its
  // VOperands
  if ((Seq) && ((RAW_A) || (RAW_B)) && Atomic_A && Atomic_B) {
    Utils::printDebug("VectorIR", "reduction");
    return VectorIR::VType::REDUCE;
  } else if ((!Seq) && (!RAW_A) && (!RAW_B) && Atomic_A && Atomic_B) {
    Utils::printDebug("VectorIR", "map");
    return VectorIR::VType::MAP;
  } else {
    Utils::printDebug("VectorIR", "sequential");
    return VectorIR::VType::SEQ;
  }
}

// ---------------------------------------------
VectorIR::VType getVectorOpType(int VL, Node *VOps[], Node *VLoadA[]) {
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
    Utils::printDebug("VectorIR", "reduction");
    return VectorIR::VType::REDUCE;
  } else if ((!Seq) && (!RAW_A) && Atomic_A) {
    Utils::printDebug("VectorIR", "map");
    return VectorIR::VType::MAP;
  } else {
    Utils::printDebug("VectorIR", "sequential");
    return VectorIR::VType::SEQ;
  }
}

// ---------------------------------------------
VectorIR::VectorOP::VectorOP(int VL, Node *VOps[], Node *VLoadA[],
                             Node *VLoadB[]) {
  // The assumption is that as all operations are the same, then all the
  // operations have the same TAC order
  this->Order = VOps[0]->getTacID();
  // By design, if the operand is an assignment, is because the operation is
  // unary, therefore: R = R OP B
  if (VOps[0]->getOutputInfo().MVOP.isAssignment()) {
    VLoadA = VLoadB;
    VLoadB = nullptr;
  }
  if (VLoadB != nullptr) {
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

  // Ordering
  this->OpA.Order = this->Order;
  this->OpB.Order = this->Order;
  this->R.Order = this->Order;
}
