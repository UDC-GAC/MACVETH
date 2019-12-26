/**
 * File              : VectorIR.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 24 Dec 2019 16:41:08 MST
 * Last Modified Date: Xov 26 Dec 2019 12:26:15 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */
#include "include/Vectorization/VectorIR.h"

#define MOD_DEB "VectorAPI"

// ---------------------------------------------
void printDebug(std::string M, std::string S) {
  std::cout << "[" << M << " DEBUG] " << S << std::endl;
}

// ---------------------------------------------
bool opsAreSequential(int VL, Node *VOps[]) {
  for (int i = 1; i < VL; ++i) {
    if (VOps[i - 1]->getSchedInfo().FreeSched <
        VOps[i]->getSchedInfo().FreeSched)
      return true;
  }
  return false;
}

// ---------------------------------------------
bool rawDependencies(int VL, Node *VOps[], Node *VLoad[]) {
  for (int i = 0; i < VL - 1; ++i) {
    if (!VOps[i]->getOuputInfoValue().compare(
            VLoad[i + 1]->getRegisterValue())) {
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
VectorIR::VectorOP::VectorOP(int VL, Node *VOps[], Node *VLoadA[],
                             Node *VLoadB[])
    : R(VL, VOps), OpA(VL, VLoadA), OpB(VL, VLoadB) {
  // Premises of our algorithm
  // 1.- Check wheter operations are sequential
  bool Seq = opsAreSequential(VL, VOps);
  if (Seq) {
    printDebug(MOD_DEB, "Ops are sequential");
  }
  // 2.- Check if operands have RAW dependencies with output of the operations
  bool RAW_A = rawDependencies(VL, VOps, VLoadA);
  bool RAW_B = rawDependencies(VL, VOps, VLoadB);
  // 3.- Check the atomicity of the operands: they can be computed/loaded before
  //     the operations
  bool Atomic_A = isAtomic(VL, VOps, VLoadA);
  bool Atomic_B = isAtomic(VL, VOps, VLoadB);

  // Type of VectorOP
  if (bool Reduction =
          (Seq) && ((RAW_A) || (RAW_B)) && (!(Atomic_A && Atomic_B))) {
    this->VT = VType::REDUCE;
  } else if (bool Map =
                 (!Seq) && (!RAW_A) && (!RAW_B) && Atomic_A && Atomic_B) {
    this->VT = VType::MAP;
  } else {
    // bool Sequential = !Reduction && !Map;
    this->VT = VType::SEQ;
  }
}
