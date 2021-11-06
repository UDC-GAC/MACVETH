// MACVETH - RandomPackingTable.h
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

#ifndef MACVETH_RANDOMPACKINGTABLE_H
#define MACVETH_RANDOMPACKINGTABLE_H

#include "include/CostModel/RandomPackingTemplate.h"
#include "include/MVOptions.h"
#include "include/Utils.h"
#include <list>
#include <map>
#include <optional>
#include <sstream>
#include <string>

namespace macveth {

using RPTableMap = std::map<std::string, RandomPackingTemplate>;

class RandomPackingTable {
public:
  /// Return a RandomPackingTemplate according to the signature key given.
  const std::optional<RandomPackingTemplate>
  getTemplate(const std::string &Signature) const {
    auto Search = RandomPackingTable::Table.find(Signature);
    if ((Search) != Table.end()) {
      return Search->second;
    }
    return {};
  }

  RandomPackingTable() {
    std::string dir(__FILE__);
    dir = dir.substr(0, dir.find_last_of("\\/"));
    std::string RPModel = dir + "/RandomPackingModel.csv";
    std::ifstream F(RPModel);
    assert(!F.fail() && "File does not exist for Random Packing Table!");
    std::string L, W;
    auto Arch = MVArchStr[MVOptions::Arch];
    auto ISA = MVISAStr[MVOptions::ISA];
    if (F.is_open()) {
      while (getline(F, L)) {
        if ((L.rfind("#", 0) == 0) || (L == "")) {
          // Check if line is a comment
          continue;
        }
        // Signature, Cost, Arch, ISA
        std::vector<std::string> Tok = split(L, ',');
        if ((Tok[2] != Arch) || (Tok[3] != ISA)) {
          continue;
        }
        RandomPackingTable::Table.insert(std::make_pair(
            Tok[0], RandomPackingTemplate(Tok[0], std::atoi(Tok[1].c_str()))));
      }
      F.close();
      return;
    }
    MACVETH_DEBUG("RandomPackingTable",
                  "This should never happen: PATH for costs file not found");
    llvm::llvm_unreachable_internal();
  };

private:
  /// Table holding the RandomPackingTemplates, containing the skeletons or
  /// "cooking recipes".
  static inline RPTableMap Table;
};

} // namespace macveth

#endif /* !MACVETH_COSTTABLE_H */
