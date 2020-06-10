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
  return false;
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
  std::string Str;
  Str = "[VectorIR] " + R.toString() + " = " + VN + "(" + OpA.toString() + "," +
        OpB.toString() + ")";
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
int64_t *getMemIdx(int VL, Node *V[], unsigned int Mask) {
  // FIXME: is this even possible?
  int64_t *Idx = (int64_t *)malloc(sizeof(int64_t) * VL);
  Idx[0] = 0;
  if (VL <= 1)
    return Idx;
  for (int n = 0; n < VL - 1; ++n) {
    Idx[n + 1] = *V[n + 1] - *V[n];
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

  if ((V[0]->getNodeType() == Node::NODE_STORE) && (Res)) {
    this->Name = V[0]->getRegisterValue();
    this->IsStore = true;
  }

  // Check if there is a vector assigned for these operands
  auto VecAssigned = checkIfVectorAssigned(VL, V);

  // Get name of this operand, otherwise create a custom name
  this->Name =
      VecAssigned ? MapRegToVReg[V[0]->getRegisterValue()] : genNewVOpName();
  // It is a temporal result if it has already been assigned
  this->IsTmpResult = VecAssigned;
  // auto AlreadyLoaded = Utils::contains(MapLoads, this->getName());
  auto AlreadyLoaded = std::find(MapLoads.begin(), MapLoads.end(),
                                 this->getName()) != MapLoads.end();
  // So, if it has not been assigned yet, then we added to the list of loads (or
  // register that we are going to pack somehow)
  if (!AlreadyLoaded)
    MapLoads.push_back(this->getName());
  // So if it has not been packed/loaded yet, then we consider it a load
  this->IsLoad = !AlreadyLoaded;

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

  // FIXME: Determine whether is partial or not
  if (true && (Mask != ((1 << VL) - 1))) {
    this->IsPartial = true;
  }

  // In case we have to access to memory we are also interested in how we do it:
  // if we have to use an index for it, or if we have to shuffle data
  if (this->MemOp) {
    // Get Memory index
    this->Idx = getMemIdx(VL, V, this->Mask);

    // Get shuffle index
    this->Shuffle = getShuffle(VL, this->getWidth(), V);
  }

  // Get data type
  this->DType = CTypeToVDataType[this->UOP[0]->getDataType()];
  if ((this->EqualVal) && (this->MemOp)) {
    this->DType = VectorIR::VDataType(this->DType + 1);
  }

  // Computin data width
  this->Width = getWidthFromVDataType(this->VSize, this->DType);
};

// ---------------------------------------------
VectorIR::VType getVectorOpType(int VL, Node *VOps[], Node *VLoadA[],
                                Node *VLoadB[]) {
  // Premises of our algorithm
  // 1.- Check wheter operations are sequential
  bool Seq = opsAreSequential(VL, VOps);
  if (Seq) {
    Utils::printDebug("CDAG", "Ops are sequential");
  }
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
VectorIR::VectorOP::VectorOP(int VL, Node *VOps[], Node *VLoadA[],
                             Node *VLoadB[])
    : OpA(VL, VLoadA, false), OpB(VL, VLoadB, false), R(VL, VOps, true) {

  // The assumption is that as all operations are the same, then all the
  // operations have the same TAC order
  this->Order = VOps[0]->getTacID();

  // Vector type will depend on the operations and operations, logically
  this->VT = getVectorOpType(VL, VOps, VLoadA, VLoadB);

  // Name: operation (assuming all operations have the same value, which is a
  // valid assumption)
  this->VN = VOps[0]->getValue();

  // The width of the operation is the result width
  this->VW = this->R.getWidth();

  // Data type
  this->DT = CTypeToVDataType[VLoadA[0]->getDataType()];
  this->R.DType = this->DT;
  // Utils::printDebug("VectorIR", VLoadA[0]->getDataType());

  // Ordering
  this->OpA.Order = this->Order;
  this->OpB.Order = this->Order;
  this->R.Order = this->Order;
}
