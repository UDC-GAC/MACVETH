/**
 * File              : VectorIR.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 09:59:02 MST
 * Last Modified Date: Xov 26 Dec 2019 14:52:02 MST
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

  /// Types of vector operations we distinguish according their scheduling
  enum VType {
    /// No dependencies between operations, so it can be used a unique vector
    /// operation
    MAP,
    /// Reductions imply that the operations are sequential. Nonetheless we
    /// use this type in order to detect these special cases which can be
    /// optimized
    REDUCE,
    /// In any other case, we just say that the vector operation must be
    /// sequential
    SEQ
  };

  /// Vector data types
  enum VDataType { DOUBLE, FLOAT, INTEGER };

  /// Vector width possible types
  enum VWidth { W8, W16, W32, W64, W128, W256, W512 };

  /// Vector operand basically is a wrap of VL operands in the original Node
  struct VOperand {
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
    bool IsTmpResult = false;
    bool IsLoad = false;
    bool IsStore = false;

    /// Check if there is a vector already assigned wraping the same values
    bool checkIfVectorAssigned(int VL, Node *V[]) {
      for (int n = 0; n < VL; ++n) {
        if (MapRegToVReg.find(V[n]->getRegisterValue()) == MapRegToVReg.end()) {
          return false;
        }
      }
      return true;
    }

    /// Basic Cosntructor
    VOperand(int VL, Node *V[]) {
      // Init list of operands
      this->UOP = (Node **)malloc(sizeof(Node *) * VL);
      // Check if there is a vector assigned for these operands
      auto VecAssigned = checkIfVectorAssigned(VL, V);
      this->Name = VecAssigned ? MapRegToVReg[V[0]->getRegisterValue()]
                               : "VOP" + std::to_string(VID++);

      for (int n = 0; n < VL; ++n) {
        this->UOP[n] = V[n];
        if (!VecAssigned) {
          MapRegToVReg[V[n]->getRegisterValue()] = this->Name;
        }
      }
    };

    /// Printing the vector operand
    void printAsString() {
      std::cout << "-------------------------------------" << std::endl;
      std::cout << "VOperand: " << this->Name << std::endl;
      for (int i = 0; i < this->Size; ++i)
        std::cout << "\t" << this->UOP[i]->getValue() << std::endl;
      std::cout << "-------------------------------------" << std::endl;
    }
  };

  /// Main component of the VectorIR which wraps the selected DAGs based on
  /// the placement and/or free scheduling. Basically represents the
  /// following: VectorOP -> VOperand = VectorOP[VOperand,
  /// VOperand]; Where each OPS is a VOperand, which basically wraps
  /// a list of UnitOperands
  struct VectorOP {
    /// Type of operation
    VType VT = VType::MAP;
    /// Name of the vector operation
    std::string VN;
    /// Vector Width of data used in this operation
    VWidth VW;
    /// Vector data type used
    VDataType DT;
    /// Vector result
    VOperand R;
    /// Vector first operand
    VOperand OpA;
    /// Vector second operand
    VOperand OpB;
    /// Cost associated to this concrete VectorOP
    unsigned int Cost = 0;
    /// Constructor from the CDAG
    VectorOP(int VL, Node *VOps[], Node *VLoadA[], Node *VLoadB[]);
  };

  /// Given a set of operations from the CDAG it computes the vector cost
  static int computeCostVectorOp(int VL, Node *VOps[], Node *VLoadA[],
                                 Node *VLoadB[]);
};

} // namespace macveth
#endif
