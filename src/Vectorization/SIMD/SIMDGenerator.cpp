/**
 * File              : SIMDGenerator.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 22 Dec 2019 20:50:04 MST
 * Last Modified Date: Mér 15 Xan 2020 12:26:46 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Vectorization/SIMD/SIMDGenerator.h"
#include "include/MVOptions.h"
#include "include/PlcmntAlgo.h"
#include "include/Utils.h"
#include "include/Vectorization/SIMD/CostTable.h"
#include "clang/AST/OperationKinds.h"
#include "clang/AST/Type.h"
#include <regex>
#include <sstream>
#include <unistd.h>
#if __GNUC__ >= 8
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

using namespace macveth;

// ---------------------------------------------
void SIMDGenerator::populateTable(MVISA ISA) {
  std::string PathISA = "/CostsArch/" + MVISAStr[ISA];
  std::string dir(__FILE__);
  dir = dir.substr(0, dir.find_last_of("\\/"));
  PathISA = dir + PathISA;
  std::string Arch = MVArchStr[MVOptions::Arch];
  std::ifstream F(PathISA);
  std::string L, W;
  // Utils::printDebug("CostsTable", "PathISA = " + PathISA);
  if (F.is_open()) {
    while (getline(F, L)) {
      if ((L.rfind("#", 0) == 0) || (L == "")) {
        // Check if line is a comment
        continue;
      }

      std::stringstream TMP(L);
      std::vector<std::string> Args;
      while (getline(TMP, W, ',')) {
        Args.push_back(W);
      }
      if (((Args[0] == Arch) || (Args[0] == "UNDEF")) && (Args.size() == 4)) {
        CostTable::addRow(Arch, Args[1], std::stoi(Args[2]), Args[3]);
      } else if (((Args[0] == Arch) || (Args[0] == "UNDEF")) &&
                 (Args.size() == 6)) {
        CostTable::addRow(Arch, Args[1], std::stoi(Args[2]), std::stod(Args[3]),
                          std::stoi(Args[4]), Args[5]);
      } else if (((Args[0] == Arch) || (Args[0] == "UNDEF")) &&
                 (Args.size() == 7)) {
        CostTable::addRow(Arch, Args[1], std::stoi(Args[2]), std::stod(Args[3]),
                          std::stoi(Args[4]), std::stoi(Args[5]), Args[6]);
      }
    }
    F.close();
  } else {
    Utils::printDebug(
        "SIMDGenerator",
        "This should never happen: PATH for costs file not found");
    llvm::llvm_unreachable_internal();
  }
}

// ---------------------------------------------
std::string SIMDGenerator::getOpName(VectorIR::VOperand V, bool Ptr,
                                     bool RegVal) {
  if ((Ptr) && (V.IsStore || V.IsLoad)) {
    // Pointer to Rvalue
    return "&" + V.getRegName();
  }
  if (RegVal) {
    // Rvalue
    return V.getRegName();
  }
  // Other
  return V.Name;
}

// ---------------------------------------------
SIMDGenerator::SIMDInst
SIMDGenerator::addNonSIMDInst(VectorIR::VectorOP OP,
                              SIMDGenerator::SIMDType SType,
                              SIMDGenerator::SIMDInstListType *IL, int Order,
                              MVSourceLocation::Position P) {
  // Generate SIMD inst
  std::string Rhs;
  std::string Lhs = OP.R.getName();
  auto O = Order;
  if (O == -1) {
    O = OP.Order;
  }
  SIMDGenerator::SIMDInst I(Lhs, Rhs, O);
  I.MVSL.setOrder(O);
  I.MVSL.setPosition(P);
  // Retrieving cost of function
  I.SType = SType;

  // Adding instruction to the list
  IL->push_back(I);
  return I;
}

//---------------------------------------------
std::list<VectorIR::VectorOP> greedyOpsConsumer(Node::NodeListType NL,
                                                SIMDGenerator *SG) {
  std::list<VectorIR::VectorOP> VList;
  Node *VLoadA[64];
  Node *VLoadB[64];
  Node *VOps[64];
  int Cursor = 0;
repeat:
  bool IsUnary = false;
  // Consume nodes
  int VL = 4;
  // This is where magic should happen
  while (!NL.empty()) {
    if ((Cursor > 0) &&
        (NL.front()->getScop()[0] != VOps[Cursor - 1]->getScop()[0])) {
      break;
    }
    // Consume the first one
    VOps[Cursor] = NL.front();
    // Get vector length
    VL = SG->getMaxVectorSize(VOps[Cursor]->getDataType());
    // NOTE: how do you solve this? I mean, for reductions, for instance,
    // you will have different Plcmnts, something like: 1,2,3,4; but this
    // algorithm should be able to select them. So maybe when selecting
    // operations you should only look at the type of operations, but not at
    // the Plcmnt or schedule.
    // 06/03/2020: I think this is solved by tackling reductions as a non
    // standard case and, then, detecting them before going onto the general
    // case.
    // 06/25/2020: not sure if reductions should be handled that way...
    if ((Cursor > 0) &&
        (VOps[Cursor]->getValue().compare(VOps[Cursor - 1]->getValue()))) {
      Utils::printDebug("CDAG", "Full OPS of same type and placement = " +
                                    VOps[Cursor - 1]->getValue() + "; (" +
                                    VOps[Cursor]->getValue() + ")");
      break;
    }
    NL.erase(NL.begin());
    if (++Cursor == VL) {
      break;
    }
  }

  // Compute memory operands for the operations fetched above
  int i = 0;
  while ((i < Cursor) && (VOps[i] != nullptr)) {
    auto Aux = VOps[i]->getInputs();
    VLoadA[i] = Aux[0];
    if (Aux[1] != nullptr) {
      VLoadB[i] = Aux[1];
      Aux.erase(Aux.begin());
      IsUnary = false;
    } else {
      IsUnary = true;
    }
    i++;
  }

  // Debugging
  for (int n = 0; n < Cursor; ++n) {
    std::string B = IsUnary ? "" : VLoadB[n]->getRegisterValue() + "; ";
    Utils::printDebug("CDAG", VOps[n]->getRegisterValue() + " = " +
                                  VLoadA[n]->getRegisterValue() + " " +
                                  VOps[n]->getValue() + " " + B +
                                  VOps[n]->getSchedInfoStr());
  }

  if (Cursor != 0) {
    // Compute the vector cost
    auto NewVectInst =
        VectorIR::VectorOP(Cursor, VOps, VLoadA, (IsUnary) ? nullptr : VLoadB);
    VList.push_back(NewVectInst);
  }

  // Repeat process if list not empty
  if (!NL.empty()) {
    Cursor = 0;
    for (int i = 0; i < VL; ++i) {
      VOps[i] = VLoadA[i] = VLoadB[i] = nullptr;
    }
    goto repeat;
  }
  return VList;
}

//---------------------------------------------
std::list<VectorIR::VectorOP> getVectorOpFromCDAG(Node::NodeListType NList,
                                                  SIMDGenerator *SG) {
  // Returning list
  std::list<VectorIR::VectorOP> VList;

  // Working with a copy
  Node::NodeListType NL(NList);

  //
  Utils::printDebug("CDAG", "Detecting reductions");
  Node::NodeListType NRedux = PlcmntAlgo::detectReductions(&NL);
  if (NRedux.size() == 0) {
    Utils::printDebug("CDAG", "No reductions detected");
  }

  Utils::printDebug("CDAG", "General case");
  VList.splice(VList.end(), greedyOpsConsumer(NL, SG));
  Utils::printDebug("CDAG", "Reductions case");
  VList.splice(VList.end(), greedyOpsConsumer(NRedux, SG));

  return VList;
}

// ---------------------------------------------
SIMDGenerator::SIMDInfo SIMDGenerator::computeCostModel(CDAG *G,
                                                        SIMDGenerator *SG) {
  // Be clean
  // VectorIR::clearMappigs();

  // Set placement according to the desired algorithm
  Node::NodeListType NL = PlcmntAlgo::sortGraph(G->getNodeListOps());

  // Debugging
  for (auto N : NL) {
    Utils::printDebug("CDAG", N->toString());
  }

  // Setting CDAG
  SG->setCDAG(G);

  // Execute greedy algorithm
  std::list<VectorIR::VectorOP> VList = getVectorOpFromCDAG(NL, SG);

  // Order Vector Operations by TAC ID
  VList.sort([](VectorIR::VectorOP V1, VectorIR::VectorOP V2) {
    return V1.Order < V2.Order;
  });

  // If debug enabled, it will print the VectorOP obtained
  for (auto V : VList) {
    Utils::printDebug("VectorIR", V.toString());
  }

  // Generate the SIMD list
  SIMDGenerator::SIMDInstListType S = SG->getSIMDfromVectorOP(VList);

  return SG->computeSIMDCost(S);
}

// ---------------------------------------------
SIMDGenerator::SIMDInst
SIMDGenerator::addNonSIMDInst(std::string Lhs, std::string Rhs,
                              SIMDGenerator::SIMDType SType,
                              SIMDGenerator::SIMDInstListType *IL, int Order,
                              MVSourceLocation::Position P) {
  auto O = Order;
  if (O == -1) {
    O = IL->back().TacID;
  }
  SIMDGenerator::SIMDInst I(Lhs, Rhs, O);
  I.MVSL.setOrder(O);
  I.MVSL.setPosition(P);

  // Retrieving cost of function
  I.SType = SType;

  // Adding instruction to the list
  IL->push_back(I);
  return I;
}

// ---------------------------------------------
std::string SIMDGenerator::SIMDInst::render() {
  std::string FN = (MVOptions::MacroCode) ? MVFuncName : FuncName;
  std::string FullFunc = ((Result == "") || (SType == SIMDType::VSTORE))
                             ? FN
                             : Result + " = " + FN;
  if ((SType == SIMDType::VSEQ) || (Args.size() == 0))
    return FullFunc;
  int i = 0;
  FullFunc += "(";
  for (auto Op = Args.begin(); Op != Args.end(); ++Op) {
    FullFunc += (i++ == (Args.size() - 1)) ? (*Op + ")") : (*Op + ", ");
  }
  return FullFunc;
}

// ---------------------------------------------
SIMDGenerator::SIMDInfo SIMDGenerator::computeSIMDCost(SIMDInstListType S) {
  std::map<std::string, SIMDCostInfo> CostOp;
  std::map<std::string, long> NumOp;
  std::list<std::string> L;
  long TotCost = 0;
  for (SIMDInst I : S) {
    CostOp[I.FuncName] = I.Cost;
    NumOp[I.FuncName]++;
    TotCost += I.Cost.Latency;
  }
  SIMDInfo SI(S, CostOp, NumOp, TotCost);
  return SI;
}

// ---------------------------------------------
std::list<std::string> SIMDGenerator::renderSIMDasString(SIMDInstListType S) {
  std::list<std::string> L;
  // Render instructions
  for (auto I : S) {
    auto Inst =
        I.render() + ";\t// latency = " + std::to_string(I.Cost.Latency);
    L.push_back(Inst);
  }
  return L;
}

// ---------------------------------------------
bool equalValues(int VL, Node **N) {
  for (int n = 1; n < VL; ++n) {
    if (N[0]->getValue() != N[n]->getValue()) {
      return false;
    }
  }
  return true;
}

// ---------------------------------------------
bool SIMDGenerator::getSIMDVOperand(VectorIR::VOperand V,
                                    SIMDInstListType *IL) {
  SIMDInstListType TIL;
  if (V.IsLoad) {
    // Need to determine which type of memory/register load it is
    // We will say that it is a load if all the operands are contiguous and
    // need to be retrieved from memory
    //
    // We will say that it is a bcast if we need to replicate the values from
    // memory or a registers
    //
    // We will say it is a gather if we have to use an index to retrieve data
    // from memory (not contiguous)
    //
    // We will say it is a set if we have to explicitly set the values of the
    // vector operand
    bool EqualVal = equalValues(V.VSize, V.UOP);
    bool ContMem = V.MemOp && (V.Contiguous);
    bool ScatterMem = V.MemOp && !ContMem;
    bool ExpVal = !V.MemOp;

    bool NullIndex = false;
    if (V.Idx.size() > 0) {
      auto I0 = V.Idx[0];
      for (int i = 1; i < V.VSize; ++i) {
        if (V.Idx[i] == I0) {
          NullIndex = true;
          break;
        }
      }
    }

    Utils::printDebug("SIMDGenerator",
                      "V = " + V.Name +
                          "; EqualVal = " + std::to_string(EqualVal) +
                          "; ContMem = " + std::to_string(ContMem) +
                          "; ScatterMem = " + std::to_string(ScatterMem) +
                          "; SameVec = " + std::to_string(V.SameVector) +
                          "; ExpVal = " + std::to_string(ExpVal) +
                          "; NullIndex = " + std::to_string(NullIndex));

    // Load contiguous from memory
    if ((!EqualVal) && (ContMem) && (!ScatterMem)) {
      TIL = vpack(V);
      // Load from memory but not contiguous
    } else if ((!EqualVal) && (ScatterMem) && (V.SameVector) && (!NullIndex)) {
      TIL = vgather(V);
      // Replicate value
    } else if ((EqualVal) && (!ScatterMem) && (!ExpVal)) {
      TIL = vbcast(V);
    } else {
      // Set values directly
      TIL = vset(V);
    }

  } else {
    return false;
  }
  // Update the list
  IL->splice(IL->end(), TIL);

  return true;
}

// ---------------------------------------------
void SIMDGenerator::mapOperation(VectorIR::VectorOP V, SIMDInstListType *TI) {
  SIMDInstListType TIL;
  // Arranging the operands: maybe they need load, set, bcast...
  getSIMDVOperand(V.OpA, TI);
  getSIMDVOperand(V.OpB, TI);

  if (V.isBinOp()) {
    /// We can filter it by
    switch (V.getBinOp()) {
    case clang::BO_Add:
      TIL = vadd(V);
      break;
    case clang::BO_Sub:
      TIL = vsub(V);
      break;
    case clang::BO_Mul:
      TIL = vmul(V);
      break;
    case clang::BO_Div:
      TIL = vdiv(V);
      break;
    case clang::BO_Rem:
      TIL = vmod(V);
      break;
    default:
      TIL = vseq(V);
    }
  } else {
    // TODO decide what todo when we have the custom operations
    Utils::printDebug("SIMDGenerator", "it is not a binary operation");
    TIL = vfunc(V);
  }

  // If there is a store
  if (V.R.IsStore) {
    if (V.R.Contiguous) {
      TIL.splice(TIL.end(), vstore(V));
    } else {
      TIL.splice(TIL.end(), vscatter(V));
    }
  }
  TI->splice(TI->end(), TIL);
}

// ---------------------------------------------
void SIMDGenerator::reduceOperation(VectorIR::VectorOP V,
                                    SIMDInstListType *TI) {
  SIMDInstListType TIL;

  // Retrieve operands
  if (V.R.Name == V.OpA.Name)
    getSIMDVOperand(V.OpB, TI);
  if (V.R.Name == V.OpB.Name)
    getSIMDVOperand(V.OpA, TI);

  // Let the magic happens
  TIL = vreduce(V);
  TI->splice(TI->end(), TIL);
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
SIMDGenerator::getSIMDfromVectorOP(VectorIR::VectorOP V) {
  SIMDInstListType IL;

  // Arranging the operation
  switch (V.VT) {
  case VectorIR::VType::MAP:
    mapOperation(V, &IL);
    break;
  case VectorIR::VType::REDUCE:
    reduceOperation(V, &IL);
    break;
  case VectorIR::VType::SEQ:
    return vseq(V);
  };

  auto RegType = getRegisterType(V.DT, V.VW);
  // Registers used
  addRegToDeclare(RegType, V.R.getName());
  addRegToDeclare(RegType, V.OpA.getName());
  addRegToDeclare(RegType, V.OpB.getName());

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
SIMDGenerator::getSIMDfromVectorOP(std::list<VectorIR::VectorOP> VList) {
  SIMDInstListType IL;

  // Get list of SIMD instructions
  for (VectorIR::VectorOP V : VList) {
    IL.splice(IL.end(), getSIMDfromVectorOP(V));
  }

  // Then optimizations can be done, for instance, combine operatios such as
  // addition + multiplication. It will depend on the architecture/ISA.
  IL = peepholeOptimizations(IL);

  return IL;
}

// ---------------------------------------------
void SIMDGenerator::addRegToDeclare(std::string Type, std::string Name,
                                    int InitVal) {
  for (auto V : RegDeclared[Type]) {
    if (std::get<0>(V) == Name) {
      return;
    }
  }
  std::vector<std::string> L;
  L.push_back(std::to_string(InitVal));
  RegDeclared[Type].push_back(std::make_tuple(Name, L));
}

// ---------------------------------------------
void SIMDGenerator::addRegToDeclareInitVal(std::string Type, std::string Name,
                                           std::vector<std::string> InitVal) {
  for (auto V : RegDeclared[Type]) {
    if (std::get<0>(V) == Name) {
      return;
    }
  }
  RegDeclared[Type].push_back(std::make_tuple(Name, InitVal));
}

// ---------------------------------------------
std::string replacePattern(std::string P, std::regex R, std::string Rep) {
  return std::regex_replace(P, R, Rep);
}

// ---------------------------------------------
std::string SIMDGenerator::genGenericFunc(std::string F,
                                          std::vector<std::string> L) {
  auto R = F + "(";
  if (L.size() == 0) {
    return R + ")";
  }
  R += L[0];
  for (int i = 1; i < L.size(); ++i) {
    R += "," + L[i];
  }
  return R + ")";
}

// ---------------------------------------------
std::string SIMDGenerator::replacePatterns(std::string Pattern, std::string W,
                                           std::string D, std::string P,
                                           std::string S) {
  std::regex WidthRegex("(#W)");
  std::regex DataRegex("(#D)");
  std::regex PreRegex("(#P)");
  std::regex SuffRegex("(#S)");
  std::string NewFunc = Pattern;

  NewFunc = replacePattern(NewFunc, WidthRegex, W);
  NewFunc = replacePattern(NewFunc, DataRegex, D);
  NewFunc = replacePattern(NewFunc, PreRegex, P);
  NewFunc = replacePattern(NewFunc, SuffRegex, S);

  return NewFunc;
}

// ---------------------------------------------
void SIMDGenerator::clearMappings() {
  for (auto &X : RegDeclared) {
    X.second.clear();
  }
  RegDeclared.clear();
  AccmToReg.clear();
  AuxArrayReg = 0;
  AccmReg = 0;
  AuxRegId = 0;
  AuxReg.clear();
  InitReg.clear();
}
