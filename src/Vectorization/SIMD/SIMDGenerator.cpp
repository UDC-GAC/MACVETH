/**
 * File              : SIMDGenerator.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 22 Dec 2019 20:50:04 MST
 * Last Modified Date: Sáb 04 Xan 2020 10:43:58 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Vectorization/SIMD/SIMDGenerator.h"
#include "include/Vectorization/SIMD/CostTable.h"

using namespace macveth;

// ---------------------------------------------
std::string SIMDGenerator::SIMDInst::render() {
  std::string FullFunc =
      !Result.compare("") ? FuncName + "(" : Result + " = " + FuncName + "(";
  std::list<std::string>::iterator Op;
  int i = 0;
  for (Op = OPS.begin(); Op != OPS.end(); ++Op) {
    FullFunc += (i++ == (OPS.size() - 1)) ? (*Op + ")") : (*Op + ", ");
  }
  return FullFunc;
}

// ---------------------------------------------
std::list<std::string> SIMDGenerator::renderSIMDasString(SIMDInstListType S) {
  std::list<std::string> L;
  // Render register declarations
  for (auto It = SIMDGenerator::RegDeclared.begin();
       It != SIMDGenerator::RegDeclared.end(); ++It) {
    std::string TypeRegDecl = It->first + " ";
    int i = 0;
    for (auto N = It->second.begin(); N != It->second.end(); ++N) {
      TypeRegDecl += (i++ == (It->second.size() - 1)) ? *N : (*N + ", ");
    }
    TypeRegDecl += ";";
    L.push_back(TypeRegDecl);
  }
  // For pretty printing: letting a blank line between declarations and the
  // actual instructions
  L.push_back("\n");
  // Render instructions
  for (SIMDInst I : S) {
    L.push_back(I.render() + ";");
  }
  return L;
}

// ---------------------------------------------
bool SIMDGenerator::getSIMDVOperand(VectorIR::VOperand V,
                                    SIMDGenerator::SIMDInstListType *IL) {
  if (!V.IsLoad) {
    // TODO maybe it would be OK to perform another action in this case
    std::cout << "NOT LOAD " << V.Name << std::endl;
    return false;
  }

  // Need to determine which type of memory/register load it is
  // We will say that it is a load if all the operands are contiguous and need
  // to be retrieved from memory
  //
  // We will say that it is a bcast if we need to replicate the values from
  // memory or a registers
  //
  // We will say it is a gather if we have to use an index to retrieve data from
  // memory (not contiguous)
  //
  // We will say it is a set if we have to explicity set the values of the
  // vector operand

  SIMDGenerator::SIMDInstListType TIL;
  bool ContMem = V.MemOp && !(V.Shuffle & 0x0);
  bool ScatterMem = V.MemOp && !ContMem;
  bool ExpVal = !V.MemOp;

  if (ContMem) {
    TIL = vpack(V);
  } else if ((!ContMem) && (ScatterMem)) {
    TIL = vgather(V);
  } else if ((!ContMem) && (!ScatterMem) && (!ExpVal)) {
    TIL = vbcast(V);
  } else if (ExpVal) {
    TIL = vset(V);
  }

  // Update the list
  for (auto I : TIL) {
    IL->push_back(I);
  }

  return true;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
SIMDGenerator::getSIMDfromVectorOP(VectorIR::VectorOP V) {
  SIMDInstListType IL;

  VectorIR::VOperand VOpA = V.OpA;
  VectorIR::VOperand VOpB = V.OpB;

  // Arranging the operands: maybe they need load, set, bcast...
  getSIMDVOperand(VOpA, &IL);
  getSIMDVOperand(VOpB, &IL);

  // Get width as string
  std::string WidthS = getMapWidth()[V.VW];
  // Get data type as string
  std::string DataTypeS = getMapType()[V.DT];
  // FIXME Is it needed preffix?
  std::string PrefS = "";
  // FIXME Is it needed suffix?
  std::string SuffS = "";
  // Get name of the function
  std::string Pattern = CostTable::getPattern(getNArch(), V.VN);
  // Replace fills in pattern
  // std::string AVXFunc =
  //    replacePatterns(Pattern, WidthS, DataTypeS, PrefS, SuffS);

  // Generate function
  // SIMDInst Inst(V.R.getName(), AVXFunc, {VOpA.getName(), VOpB.getName()});
  // IL.push_back(Inst);

  // Registers used
  std::string RegType = getRegisterType(V.DT, V.VW);
  SIMDGenerator::addRegToDeclare(RegType, V.R.getName());
  SIMDGenerator::addRegToDeclare(RegType, VOpA.getName());
  SIMDGenerator::addRegToDeclare(RegType, VOpB.getName());

  return IL;
}

// ---------------------------------------------
SIMDGenerator::SIMDInstListType
SIMDGenerator::getSIMDfromVectorOP(std::list<VectorIR::VectorOP> VList) {
  SIMDInstListType I;
  for (VectorIR::VectorOP V : VList) {
    for (SIMDInst Inst : getSIMDfromVectorOP(V)) {
      I.push_back(Inst);
    }
  }
  // TODO Peephole optimizations:
  // Then optimizations can be done, for instance, combine operatios such as
  // addition + multiplication
  return I;
}

// ---------------------------------------------
void SIMDGenerator::addRegToDeclare(std::string Type, std::string Name) {
  if (!Utils::contains(SIMDGenerator::RegDeclared[Type], Name)) {
    SIMDGenerator::RegDeclared[Type].push_back(Name);
  }
}

// ---------------------------------------------
void SIMDGenerator::clearMappings() {
  for (auto &X : SIMDGenerator::RegDeclared) {
    X.second.clear();
  }
  SIMDGenerator::RegDeclared.clear();
}
