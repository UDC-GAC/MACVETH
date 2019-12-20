/**
 * File              : VectorAPI.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 13:18:13 MST
 * Last Modified Date: Ven 20 Dec 2019 15:07:34 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Vectorization/VectorAPI.h"

#define MOD_DEB "VectorAPI"

// ---------------------------------------------
void printDebug(std::string M, std::string S) {
  std::cout << "[" << M << " DEBUG] " << S << std::endl;
}

// ---------------------------------------------
bool opsAreSequential(int VL, Node *VOps[], int VOpsSched[]) {
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
int VectorAPI::computeCostVectorOp(int VL, Node *VOps[], int VOpsSched[],
                                   Node *VLoadA[], int VLoadSchedA[],
                                   Node *VLoadB[], int VLoadSchedB[]) {
  // Premises of our algorithm
  // 1.- Check wheter operations are sequential
  if (bool Seq = opsAreSequential(VL, VOps, VOpsSched)) {
    printDebug(MOD_DEB, "Ops are sequential");
  }
  // 2.- Check if operands have RAW dependencies with output of the operations
  bool RAW_A = rawDependencies(VL, VOps, VLoadA);
  bool RAW_B = rawDependencies(VL, VOps, VLoadB);
  // 3.- Check the atomicity of the operands: they can be computed/loaded before
  //     the operations
  bool Atomic_A = isAtomic(VL, VOps, VLoadA);
  bool Atomic_B = isAtomic(VL, VOps, VLoadB);
  return 0;
}
