// MACVETH - VectorIR.h
//
// Copyright (c) Colorado State University. 2019-2021
// Copyright (c) Universidade da Coruña. 2020-2021
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Authors:
//     Marcos Horro <marcos.horro@udc.es>
//     Louis-Noël Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Marcos Horro <marcos.horro@udc.es>

#ifndef MACVETH_VECTORIR_H
#define MACVETH_VECTORIR_H

#include "include/MVExpr/MVDataType.h"
#include "include/MVExpr/MVExprArray.h"
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
  static inline std::map<std::string, int> MapStores;

  /// Clearing all the mappings
  static void clear() {
    VectorIR::MapRegToVReg.clear();
    VectorIR::MapLoads.clear();
    VectorIR::MapStores.clear();
    VectorIR::VID = 0;
  }

  /// Prefix for operands
  static inline const std::string VOP_PREFIX = "__vop";

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
    // Node **UOP = nullptr;
    std::vector<Node *> UOP;
    /// Data type
    MVDataType::VDataType DType = MVDataType::VDataType::DOUBLE;
    /// Width of this operand
    MVDataType::VWidth Width = MVDataType::VWidth::W256;
    /// Base array
    std::string BaseArray = "";
    /// All store values
    std::vector<std::string> StoreValues;
    /// Mask for shuffling if necessary
    std::vector<long> Idx;
    /// Mask to avoid elements if necessary
    unsigned int Mask = 0x0;
    /// High bits are loaded
    bool HighBits = false;
    /// Low bits are loaded
    bool LowBits = false;
    /// UOPS are all the same
    bool EqualVal = true;
    /// Memory addres is unaligned
    bool Unaligned = false;
    /// Memory is contiguous
    bool Contiguous = true;
    /// For instance: [x,x+1,y,y+1] where x+2 != y
    bool ContiguousHalves = false;
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

    bool isDouble() { return DType == MVDataType::VDataType::DOUBLE; }

    bool isFloat() { return DType == MVDataType::VDataType::FLOAT; }

    /// Return name of VOperand
    std::string getName() { return Name; }

    /// Return name of the VOperand (const)
    std::string getName() const {return Name;}

    /// Returns if operands have been already loaded from memory
    bool checkIfAlreadyLoaded(Node *PrimaryNode);

    /// Return if operands already stored in memory
    bool checkIfAlreadyStored(Node::NodeListType &V);

    /// Generate a name according to the VID
    std::string genNewVOpName() { return VOP_PREFIX + std::to_string(VID++); }

    /// Return register name
    std::string getRegName(int Position, int Offset) const {
      if (Offset != 0) {
        auto Operand = this->UOP[Position]->getMVExpr();
        if (Operand != nullptr) {
          auto MVE = dyn_cast<MVExprArray>(Operand);
          return MVE->toStringWithOffset(Offset);
        } else {
          MVErr("Something went wrong...");
        }
      }
      return this->UOP[Position]->getRegisterValue();
    }

    /// Get loop where the result is computed
    std::string getOperandLoop() {
      return (UOP[0] != nullptr) ? UOP[0]->getLoopName() : "";
    }

    /// Copy constructor
    VOperand(const VOperand &V) {
      this->Name = V.Name;
      this->BaseArray = V.BaseArray;
      this->VSize = V.VSize;
      this->Size = V.Size;
      this->UOP = V.UOP;
      this->StoreValues = V.StoreValues;
      this->DType = V.DType;
      this->Width = V.Width;
      this->Idx = V.Idx;
      this->Mask = V.Mask;
      this->EqualVal = V.EqualVal;
      this->Unaligned = V.Unaligned;
      this->Contiguous = V.Contiguous;
      this->IsPartial = V.IsPartial;
      this->HighBits = V.HighBits;
      this->LowBits = V.LowBits;
      this->SameVector = V.SameVector;
      this->OutOfCacheLine = V.OutOfCacheLine;
      this->IsTmpResult = V.IsTmpResult;
      this->IsLoad = V.IsLoad;
      this->MemOp = V.MemOp;
      this->IsStore = V.IsStore;
      this->Order = V.Order;
      this->Offset = V.Offset;
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
    /// Get the result operand
    VOperand getResult() { return R; }
    VOperand getResult() const { return R; }
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
