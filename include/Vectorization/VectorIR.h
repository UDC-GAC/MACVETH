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
    /// In any other case, we just say that the vector operation must be
    /// sequential
    SEQ
  };

  /// Vector data types: most of them are self explanatory
  enum VDataType {
    /// This is the direct translation to "pd"
    DOUBLE,
    /// This is the direct translation onto "sd"
    SDOUBLE,
    /// This is the direct translation to "ps"
    FLOAT,
    /// This is the direct translation to "ss"
    SFLOAT,
    /// Unsigned 8-bit integer
    UINT8,
    /// Unsigned 16-bit integer
    UINT16,
    /// Unsigned 32-bit integer
    UINT32,
    /// Unsigned 64-bit integer
    UINT64,
    /// Signed 8-bit integer
    INT8,
    /// Signed 16-bit integer
    INT16,
    /// Signed 32-bit integer
    INT32,
    /// Signed 64-bit integer
    INT64,
    /// 128 bits, undefined type
    UNDEF128,
    /// 256 bits, undefined type
    UNDEF256,
    /// Input vector is INT128, output can be different
    IN_INT128,
    /// Input vector is FLOAT128, output can be different
    IN_FLOAT128,
    /// Input vector is DOUBLE128, output can be different
    IN_DOUBLE128,
    /// Input vector is INT256, output can be different
    IN_INT256,
    /// Input vector is FLOAT256, output can be different
    IN_FLOAT256,
    /// Input vector is DOUBLE256, output can be different
    IN_DOUBLE256
  };

  /// Table of equivalences between C/C++ basic numeric types and VectorIR's
  static inline std::map<std::string, VDataType> CTypeToVDataType = {
      {"double", DOUBLE},   {"float", FLOAT},     {"uint8_t", UINT8},
      {"uint16_t", UINT16}, {"uint32_t", UINT32}, {"uint64_t", UINT64},
      {"int8_t", INT8},     {"int16_t", INT16},   {"int32_t", INT32},
      {"int64_t", INT64},   {"int", INT32},       {"long", INT64},
  };

  /// Table of equivalences between the VDataTypes and the number of bits of
  /// type
  static inline std::map<VDataType, int> VDataTypeWidthBits = {
      {DOUBLE, 64}, {FLOAT, 32},  {SDOUBLE, 64}, {SFLOAT, 32},
      {UINT8, 8},   {UINT16, 16}, {UINT32, 32},  {UINT64, 64},
      {INT8, 8},    {INT16, 16},  {INT32, 32},   {INT64, 64},
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

  /// Compute the vector width needed from the number of operands and the type
  /// them
  static VWidth getWidthFromVDataType(int NOps, VDataType VData) {
    int Bits = VDataTypeWidthBits[VData] * NOps;
    // Utils::printDebug("VectorIR", "bits = " + std::to_string(Bits));
    if (Bits > 256) {
      return VWidth::W512;
    } else if (Bits > 128) {
      return VWidth::W256;
    } else if (Bits > 64) {
      return VWidth::W128;
    } else if (Bits > 32) {
      return VWidth::W64;
    } else if (Bits > 16) {
      return VWidth::W32;
    } else if (Bits > 8) {
      return VWidth::W16;
    }
    return VWidth::W8;
  }

  /// Unique identifier for the operand
  static inline unsigned int VID = 0;
  /// Keeping track of the correspondence between the registers name and the
  /// new naming
  static inline std::map<std::tuple<std::string, VectorIR::VWidth>, std::string>
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
    VDataType DType = VDataType::DOUBLE;
    /// Width of this operand
    VWidth Width = VWidth::W256;
    /// Mask for shuffling if necessary
    std::vector<long> Idx;
    /// Mask for shuffling if necessary
    unsigned int Shuffle = 0x0;
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
    bool checkIfVectorAssigned(int VL, Node *V[], VectorIR::VWidth);
    /// Get width
    VWidth getWidth() { return Width; }
    /// Get data type of the operand: assumption that all elements are the same
    /// type
    VDataType getDataType() {
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
    VOperand(int VL, Node *V[], bool Res);

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
    VWidth VW = VWidth::W256;
    /// Vector data type used
    VDataType DT = VDataType::DOUBLE;
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
    VectorOP(int VL, Node *VOps[], Node *VLoadA[], Node *VLoadB[]);
    /// Check if operation is sequential or not
    bool isSequential() { return this->VT == VType::SEQ; }
    /// Render vector operation as string
    std::string toString();
  };
};

} // namespace macveth
#endif /* !MACVETH_VECTORIR_H */
