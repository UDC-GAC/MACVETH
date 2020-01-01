/**
 * File              : VectorIR.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Ven 20 Dec 2019 09:59:02 MST
 * Last Modified Date: Mar 31 Dec 2019 18:44:06 MST
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
  enum VDataType {
    DOUBLE,
    FLOAT,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    INT8,
    INT16,
    INT32,
    INT64,
    /// 128 bits, undefined type
    UNDEF128,
    /// 256 bits, undefined type
    UNDEF256,
    /// Input vector is INT128, output is different
    IN_INT128,
    /// Input vector is FLOAT128, output is different
    IN_FLOAT128,
    /// Input vector is DOUBLE128, output is different
    IN_DOUBLE128,
    /// Input vector is INT256, output is different
    IN_INT256,
    /// Input vector is FLOAT256, output is different
    IN_FLOAT256,
    /// Input vector is DOUBLE256, output is different
    IN_DOUBLE256
  };

  /// Table of equivalences between C/C++ basic numeric types and VectorIR's
  static inline std::map<std::string, VDataType> CTypeToVDataType = {
      {"double", DOUBLE},   {"float", FLOAT},     {"uint8_t", UINT8},
      {"uint16_t", UINT16}, {"uint32_t", UINT32}, {"uint64_t", UINT64},
      {"int8_t", INT8},     {"int16_t", INT16},   {"int32_t", INT32},
      {"int64_t", INT64},
  };

  /// Vector width possible types
  enum VWidth {
    W8 = 8,
    W16 = 16,
    W32 = 32,
    W64 = 64,
    W128 = 128,
    W256 = 256,
    W512 = 512
  };

  /// Vector operand basically is a wrap of VL (vector lenght) operands in the
  /// original Node
  struct VOperand {
    /// Unique identifier for the operand
    static inline unsigned int VID = 0;
    /// Keeping track of the correspondence between the registers name and the
    /// new naming
    static inline std::map<std::string, std::string> MapRegToVReg;
    /// Keeping track of the correspondence between the registers name and the
    /// new naming
    static inline std::list<std::string> MapLoads;
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
    /// The vector operand is a temporal result if it has already been assigned
    /// before
    bool IsTmpResult = false;
    /// Vector operand is a lod if it is not a temporal result
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

    VWidth getWidth() { return VWidth::W256; }

    VDataType getDataType() {
      return CTypeToVDataType[this->UOP[0]->getDataType()];
    }

    /// Basic constructor
    VOperand(int VL, Node *V[]) {
      // Init list of unit operands
      this->UOP = (Node **)malloc(sizeof(Node *) * VL);
      // Check if there is a vector assigned for these operands
      auto VecAssigned = checkIfVectorAssigned(VL, V);
      this->Name = VecAssigned ? MapRegToVReg[V[0]->getRegisterValue()]
                               : "vop" + std::to_string(VID++);

      std::cout << this->Name << ", " << V[0]->getValue();
      std::cout << ", " << V[0]->getRegisterValue() << std::endl;

      this->IsTmpResult = VecAssigned;
      auto AlreadyLoaded = Utils::contains(MapLoads, this->getName());
      if (!AlreadyLoaded)
        MapLoads.push_back(this->getName());
      this->IsLoad = !AlreadyLoaded;
      for (int n = 0; n < VL; ++n) {
        this->UOP[n] = V[n];
        if (!VecAssigned) {
          MapRegToVReg[V[n]->getRegisterValue()] = this->Name;
        }
      }
    };

    /// Return name of VOperand
    std::string getName() { return this->Name; }

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
  /// a list of UnitOperands or Nodes
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
