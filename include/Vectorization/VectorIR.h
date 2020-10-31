/*
 * File					 : include/Vectorization/VectorIR.h
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:40 +02:00
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

#ifndef MACVETH_VECTORIR_H
#define MACVETH_VECTORIR_H

#include "include/MVExpr/MVDataType.h"
#include "include/Node.h"

using namespace macveth;

namespace macveth {

/// VectorIR is a facade for the CDAG to compute the cost of the vectorization,
/// generate intrinsics and perform the optimizations.
class VectorIR {
public:
  /// Types of vector operations we distinguish according their scheduling
  enum VType {
    /// No dependencies between operations, so it can be used a unique vector
    /// operation
    MAP,
    /// Reductions imply that the operations are sequential. Nonetheless we
    /// use this type in order to detect these special cases which can be
    /// optimized
    REDUCE,
    /// Inductions, e.g. A[i] = x
    INDUCTION,
    /// In any other case, we just say that the vector operation must be
    /// sequential
    SEQ
  };

  /// Compute the vector width needed from the number of operands and the type
  /// them
  static MVDataType::VWidth getWidthFromVDataType(int NOps,
                                                  MVDataType::VDataType VData) {
    int Bits = MVDataType::VDataTypeWidthBits[VData] * NOps;
    // Utils::printDebug("VectorIR", "bits = " + std::to_string(Bits));
    if (Bits > 256) {
      return MVDataType::VWidth::W512;
    } else if (Bits > 128) {
      return MVDataType::VWidth::W256;
    } else if (Bits > 64) {
      return MVDataType::VWidth::W128;
    } else if (Bits > 32) {
      return MVDataType::VWidth::W64;
    } else if (Bits > 16) {
      return MVDataType::VWidth::W32;
    } else if (Bits > 8) {
      return MVDataType::VWidth::W16;
    }
    return MVDataType::VWidth::W8;
  }

  /// Unique identifier for the operand
  static inline unsigned int VID = 0;
  /// Keeping track of the correspondence between the registers name and the
  /// new naming
  static inline std::map<std::tuple<std::string, MVDataType::VWidth>,
                         std::string>
      MapRegToVReg;
  /// Keeping track of the loads in the program
  static inline std::list<std::tuple<std::vector<int>, std::string>> MapLoads;
  /// Keeping track of the stores in the program
  static inline std::list<std::string> MapStores;

  /// Clearing all the mappings
  static void clear() {
    VectorIR::MapRegToVReg.clear();
    VectorIR::MapLoads.clear();
    VectorIR::MapStores.clear();
    VectorIR::VID = 0;
  }

  /// Prefix for operands
  static inline const std::string VOP_PREFIX = "__mv_vop";

  /// Vector operand basically is a wrap of VL (vector length) operands in the
  /// original Node
  struct VOperand {
    /// Name identifying the vector operand
    std::string Name;
    /// Total size of vector
    unsigned int VSize;
    /// Number non null Nodes
    unsigned int Size = 4;
    /// Array of variable size (Size elements actually) initialized when
    /// creating the object
    Node **UOP = nullptr;
    /// Data type
    MVDataType::VDataType DType = MVDataType::VDataType::DOUBLE;
    /// Width of this operand
    MVDataType::VWidth Width = MVDataType::VWidth::W256;
    /// Mask for shuffling if necessary
    std::vector<long> Idx;
    /// Mask to avoid elements if necessary
    unsigned int Mask = 0x0;
    /// UOPS are all the same
    bool EqualVal = true;
    /// Memory addres is unaligned
    bool Unaligned = false;
    /// Memory is contiguous
    bool Contiguous = true;
    /// Is partial (mask is not all 1)
    bool IsPartial = false;
    /// Values are in the same vector
    bool SameVector = true;
    /// If the memory addresses cross cache line sizes
    bool OutOfCacheLine = false;
    /// The vector operand is a temporal result if it has already been assigned
    /// before
    bool IsTmpResult = false;
    /// Vector operand could be a load if it is not a temporal result
    bool IsLoad = false;
    /// Vector UOPs are memory addresses
    bool MemOp = false;
    /// Vector operand may be a store in memory operation
    bool IsStore = false;
    /// Order of the vector operation
    int Order = -1;
    /// Offset of the vector operation
    int Offset = -1;
    /// Check if there is a vector already assigned wraping the same values
    bool checkIfVectorAssigned(int VL, Node::NodeListType &V,
                               MVDataType::VWidth W);
    /// Get width
    MVDataType::VWidth getWidth() { return Width; }
    /// Get data type of the operand: assumption that all elements are the same
    /// type
    MVDataType::VDataType getDataType() {
      // Be careful with this
      return DType;
    }
    /// Return name of VOperand
    std::string getName() { return this->Name; }

    /// Generate a name according to the VID
    std::string genNewVOpName() { return VOP_PREFIX + std::to_string(VID++); }

    /// Return register name
    std::string getRegName() { return this->UOP[0]->getRegisterValue(); }

    /// Get loop where the result is computed
    std::string getOperandLoop() {
      if (UOP != nullptr) {
        if (UOP[0] != nullptr) {
          return UOP[0]->getLoopName();
        }
      }
      return "";
    }

    /// Printing the vector operand
    std::string toString();

    /// Basic constructor
    VOperand(int VL, Node::NodeListType &V, bool Res);

    /// Empty constructor
    VOperand();
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
    std::string VN = "";
    /// Vector Width of data used in this operation
    MVDataType::VWidth VW = MVDataType::VWidth::W256;
    /// Vector data type used
    MVDataType::VDataType DT = MVDataType::VDataType::DOUBLE;
    /// Vector first operand
    VOperand OpA;
    /// Vector second operand
    VOperand OpB;
    /// If there is only one operand, then it is unary
    bool IsUnary = false;
    /// Vector result
    VOperand R;
    /// Order of the vector operation
    int Order = -1;
    /// Offset regarding the original order
    int Offset = -1;
    /// Cost associated to this concrete VectorOP
    unsigned int Cost = 0;
    /// Check if it is a BinaryOperation
    bool isBinOp();
    /// Get the BinaryOperator::Opcode
    BinaryOperator::Opcode getBinOp();
    /// Get the MVOp
    MVOp getMVOp();
    /// Constructor from the CDAG
    VectorOP(int VL, Node::NodeListType &VOps, Node::NodeListType &VLoadA,
             Node::NodeListType &VLoadB);
    /// Check if operation is sequential or not
    bool isSequential() { return this->VT == VType::SEQ; }
    /// Render vector operation as string
    std::string toString();
  };
};

} // namespace macveth
#endif /* !MACVETH_VECTORIR_H */
