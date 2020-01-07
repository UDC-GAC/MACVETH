/**
 * File              : VectorIR.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 24 Dec 2019 16:41:08 MST
 * Last Modified Date: Dom 05 Xan 2020 17:49:22 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#include "include/Vectorization/VectorIR.h"
#include <bits/stdint-uintn.h>
#include <sys/types.h>

// ---------------------------------------------
void printDebug(std::string M, std::string S) {
  std::cout << "[" << M << " DEBUG] " << S << std::endl;
}

// ---------------------------------------------
bool opsAreSequential(int VL, Node *VOps[]) {
  for (int i = 1; i < VL; ++i) {
    if ((VOps[i - 1]->getSchedInfo().FreeSched <
         VOps[i]->getSchedInfo().FreeSched)) {
      // std::cout << "YES; THEY ARE SEQUENTIAL" << std::endl;
      return true;
    }
  }
  // std::cout << "NOPE; THEY ARE NOT SEQUENTIAL" << std::endl;
  return false;
}

// ---------------------------------------------
bool rawDependencies(int VL, Node *VOps[], Node *VLoad[]) {
  for (int i = 0; i < VL - 1; ++i) {
    if (VOps[i]->getOutputInfoName() == VLoad[i + 1]->getRegisterValue()) {
      // std::cout << "YES; THERE ARE RAWS" << std::endl;
      return true;
    }
  }
  // std::cout << "NOPE; THERE ARE NOT RAWS" << std::endl;

  return false;
}

// ---------------------------------------------
bool isAtomic(int VL, Node *VOps[], Node *VLoad[]) {
  for (int i = 0; i < VL; ++i) {
    if (VOps[i]->getSchedInfo().FreeSched <=
        VLoad[i]->getSchedInfo().FreeSched) {
      // std::cout << "NOPE; NOT ATOMICS BRO" << std::endl;
      return false;
    }
  }
  // std::cout << "YES; ATOMICS BRO" << std::endl;
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
  return this->R.UOP[0]->getOutputInfo().BOP;
}

// ---------------------------------------------
int64_t *getMemIdx(int VL, Node *V[]) {
  // FIXME
  int64_t *Idx = (int64_t *)malloc(sizeof(int64_t) * VL);
  Idx[0] = 0;
  if (VL <= 1)
    return Idx;
  for (int n = 0; n < VL; ++n) {
    Idx[n + 1] = V[n + 1] - V[n];
  }
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
  // TODO
  unsigned int Mask = 0x0;
  return Mask;
}

// ---------------------------------------------
VectorIR::VOperand::VOperand(int VL, Node *V[]) {
  // Init list of unit operands
  this->UOP = (Node **)malloc(sizeof(Node *) * VL);
  // Check if there is a vector assigned for these operands
  auto VecAssigned = checkIfVectorAssigned(VL, V);
  this->Name = VecAssigned ? MapRegToVReg[V[0]->getRegisterValue()]
                           : "VOp" + std::to_string(VID++);

  ///////////////////////////////////////////////////////////
  // std::cout << this->Name << ", " << V[0]->getValue();
  // std::cout << ", " << V[0]->getRegisterValue() << std::endl;
  ///////////////////////////////////////////////////////////

  // It is a temporal result if it has already been assigned
  this->IsTmpResult = VecAssigned;
  auto AlreadyLoaded = Utils::contains(MapLoads, this->getName());
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
    IsMemOp = (IsMemOp) && (V[n]->isMem());
    this->UOP[n] = V[n];
    if (!VecAssigned) {
      MapRegToVReg[V[n]->getRegisterValue()] = this->Name;
    }
  }
  this->MemOp = IsMemOp;

  // Get data mask
  this->Mask = getMask(VL, V);

  // In case we have to access to memory we are also interested in how we do it:
  // if we have to use an index for it, or if we have to shuffle data
  if (this->MemOp) {
    // Get Memory index
    this->Idx = getMemIdx(VL, V);

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
    : R(VL, VOps), OpA(VL, VLoadA), OpB(VL, VLoadB) {

  this->VT = getVectorType(VL, VOps, VLoadA, VLoadB);

  // Name: operation (assuming all operations have the same value, which is a
  // valid assumption)
  this->VN = VOps[0]->getValue();

  // The width of the operation is the result width
  this->VW = this->R.getWidth();

  // Data type
  this->DT = CTypeToVDataType[VLoadA[0]->getDataType()];
}
