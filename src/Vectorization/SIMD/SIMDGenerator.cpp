/**
 * File              : SIMDGenerator.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 22 Dec 2019 20:50:04 MST
 * Last Modified Date: Ven 27 Dec 2019 16:53:55 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/Vectorization/SIMD/SIMDGenerator.h"

using namespace macveth;

// ---------------------------------------------
std::list<std::string> SIMDGenerator::renderSIMDasString(SIMDInfo S) {
  std::list<std::string> L;
  // Render register declarations
  for (auto It = SIMDGenerator::RegDeclared.begin();
       It != SIMDGenerator::RegDeclared.end(); ++It) {
    std::string TypeRegDecl = It->first + " ";
    int i = 0;
    for (auto N = It->second.begin(); N != It->second.end(); ++N) {
      TypeRegDecl += (i++ == (It->second.size() - 1)) ? *N : (*N + ", ");
    }
    L.push_back(TypeRegDecl);
  }
  // For pretty printing: letting a blank line between declarations and the
  // actual instructions
  L.push_back("\n");
  // Render instructions
  for (SIMDInst I : S.SIMDList) {
    L.push_back(I.render());
  }
  return L;
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
