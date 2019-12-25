/**
 * File              : VectorIR.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 09:59:02 MST
 * Last Modified Date: Mar 24 Dec 2019 16:39:34 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#ifndef MACVETH_VECTORAPI_H
#define MACVETH_VECTORAPI_H

#include <string.h>

#include "include/Node.h"
#include "include/Utils.h"

using namespace macveth;

namespace macveth {
/// VectorIR is a facade for the CDAG to compute the cost of the vectorization,
/// generate intrinsics and perform the optimizations.
class VectorIR {
public:
  static const int VL = 4;
  enum VOP { NOP, CONST, VREC, BINOP, BUILTIN, MEM };

  /// Vector width possible types
  enum VWidth { W64, W128, W256, W512 };

  /// Vector operand basically is a wrap of VL operands in the original Node
  struct VectorOperand {
    /// Unique identifier for the operand
    static inline unsigned int VID = 0;
    /// Keeping track of the correspondence between the registers name and the
    /// new naming
    static inline std::map<std::string, std::string> MapRegToVReg;
    /// Name identifying the vector operand
    std::string Name;
    /// Number of Nodes
    unsigned int Size;
    /// Array of variable size (Size elements actually) initialized when
    /// creating the object
    Node **UOP = nullptr;
    /// Width of this operand
    // VWidth Width;
    /// Mask for shuffling if necessary
    unsigned int Shuffle = 0x0;
    /// Mask to avoid elements if necessary
    unsigned int Mask = 0x0;
    bool IsResult = false;
    bool IsTemp = false;
    bool IsLoad = false;
    bool IsStore = false;
    /// Basic Cosntructor
    VectorOperand(int VL, Node *V[]) {
      this->Name = "VOP" + std::to_string(VectorOperand::VID++);
      this->UOP = new Node *[VL];
      for (int n = 0; n < VL; ++n) {
        this->UOP[n] = V[n];
        VectorOperand::MapRegToVReg[V[n]->getValue()] = this->Name;
      }
    };

    void printAsString() {
      std::cout << "-------------------------------------" << std::endl;
      std::cout << "VectorOperand: " << std::endl;
      for (int i = 0; i < this->Size; ++i)
        std::cout << "\t" << this->UOP[i]->getValue() << std::endl;
      std::cout << "-------------------------------------" << std::endl;
    }
  };

  /// Main component of the VectorIR which wraps the selected DAGs based on the
  /// placement and/or free scheduling. Basically represents the following:
  /// VectorOp -> <vector_result> = VOP(<list>OPS);
  /// Where each OPS is a VectorOperand, which basically wraps a list of
  /// UnitOperands
  struct VectorOP {
    VOP V;
    VectorOperand Result;
    std::list<VectorOperand> OPS;
  };

  /// Given a set of operations from the CDAG it computes the vector cost
  static int computeCostVectorOp(int VL, Node *VOps[], Node *VLoadA[],
                                 Node *VLoadB[]);

  static bool isVectorizable(int VL, Node *VOps[], int VOpsSched[],
                             Node *VLoadA[], int VLoadSchedA[], Node *VLoadB[],
                             int VLoadSchedB[]);

private:
};

} // namespace macveth
#endif
