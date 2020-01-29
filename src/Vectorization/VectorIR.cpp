/**
 * File              : VectorIR.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 24 Dec 2019 16:41:08 MST
 * Last Modified Date: Mér 15 Xan 2020 12:32:46 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Vectorization/VectorIR.h"
#include <algorithm>
#include <bits/stdint-uintn.h>
#include <sys/types.h>

#define VECTORIR_DEBUG 1

// ---------------------------------------------
void printDebug(std::string M, std::string S) {
  if (VECTORIR_DEBUG)
    std::cout << "[" << M << " DEBUG] " << S << std::endl;
}

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
void VectorIR::VOperand::printAsString() {
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "VOperand: " << this->Name << std::endl;
  for (int i = 0; i < this->Size; ++i)
    std::cout << "\t" << this->UOP[i]->getValue() << std::endl;
  std::cout << "-------------------------------------" << std::endl;
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
  // FIXME
  int64_t *Idx = (int64_t *)malloc(sizeof(int64_t) * VL);
  Idx[0] = 0;
  if (VL <= 1)
    return Idx;
  for (int n = 0; n < VL - 1; ++n) {
    Idx[n + 1] = *V[n + 1] - *V[n];
    std::cout << "index " << std::to_string(n) << " = "
              << std::to_string(Idx[n]) << ",";
  }
  std::cout << std::endl;
  return Idx;
}

// ---------------------------------------------
unsigned int getShuffle(int VL, int Width, Node *V[]) {
  // TODO
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

  this->Name = VecAssigned ? MapRegToVReg[V[0]->getRegisterValue()]
                           : "vop" + std::to_string(VID++);
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
      std::cout << "VECTORIR = " << V[n - 1]->getRegisterValue() << " and "
                << V[n]->getRegisterValue() << "; then "
                << (V[n]->getRegisterValue() == V[n - 1]->getRegisterValue())
                << " is " << this->EqualVal << std::endl;
    }
  }
  this->MemOp = IsMemOp;

  // Get data mask
  this->Mask = getMask(VL, V);

  // Determine whether is partial or not
  if (Mask != ((1 << this->Size) - 1)) {
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
};

// ---------------------------------------------
VectorIR::VType getVectorType(int VL, Node *VOps[], Node *VLoadA[],
                              Node *VLoadB[]) {
  // Premises of our algorithm
  // 1.- Check wheter operations are sequential
  bool Seq = opsAreSequential(VL, VOps);
  if (Seq) {
    printDebug("CDAG", "Ops are sequential");
  }
  // 2.- Check if operands have RAW dependencies with output of the operations
  bool RAW_A = rawDependencies(VL, VOps, VLoadA);
  bool RAW_B = rawDependencies(VL, VOps, VLoadB);
  // 3.- Check the atomicity of the operands: they can be computed/loaded before
  //     the operations
  bool Atomic_A = isAtomic(VL, VOps, VLoadA);
  bool Atomic_B = isAtomic(VL, VOps, VLoadB);

  // Type of VectorOP
  if ((Seq) && ((RAW_A) || (RAW_B)) && Atomic_A && Atomic_B) {
    printDebug("VectorIR", "reduction");
    return VectorIR::VType::REDUCE;
  } else if ((!Seq) && (!RAW_A) && (!RAW_B) && Atomic_A && Atomic_B) {
    printDebug("VectorIR", "map");
    return VectorIR::VType::MAP;
  } else {
    printDebug("VectorIR", "sequential");
    return VectorIR::VType::SEQ;
  }
}

// ---------------------------------------------
VectorIR::VectorOP::VectorOP(int VL, Node *VOps[], Node *VLoadA[],
                             Node *VLoadB[])
    : OpA(VL, VLoadA, false), OpB(VL, VLoadB, false), R(VL, VOps, true) {

  this->VT = getVectorType(VL, VOps, VLoadA, VLoadB);

  // Name: operation (assuming all operations have the same value, which is a
  // valid assumption)
  this->VN = VOps[0]->getValue();

  // The width of the operation is the result width
  this->VW = this->R.getWidth();

  // Data type
  this->DT = CTypeToVDataType[VLoadA[0]->getDataType()];
}
