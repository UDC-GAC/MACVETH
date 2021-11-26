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
    MACVETH_DEBUG("RandomPackingTable", RPModel);
    MACVETH_DEBUG("RandomPackingTable", __FILE__);

    assert(!F.fail() && "File does not exist for Random Packing Table!");
    auto Arch = Utils::toLowercase(MVArchStr[MVOptions::Arch]);
    auto ISA = Utils::toLowercase(MVISAStr[MVOptions::ISA]);
    if (F.is_open()) {
      std::string L, W;
      while (getline(F, L)) {
        if ((L.rfind("#", 0) == 0) || (L.empty())) {
          // Check if line is a comment
          continue;
        }
        // id, signature, cost, arch, isa, data type, version
        auto Tok = Split<','>::split(L);
        if ((Tok[IDX_arch] != Arch) || (Tok[IDX_isa] != ISA)) {
          continue;
        }
        auto Signature = Tok[IDX_sig];
        auto DataType = Tok[IDX_datatype];
        int Cost = std::atoi(Tok[IDX_cost].c_str());
        int Version = std::atoi(Tok[IDX_version].c_str());
        RandomPackingTable::Table.insert(std::make_pair(
            DataType + "_n" + Signature,
            RandomPackingTemplate(Signature, Cost, DataType, Version)));
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
  static constexpr int IDX_id = 0;
  static constexpr int IDX_sig = 1;
  static constexpr int IDX_cost = 2;
  static constexpr int IDX_arch = 3;
  static constexpr int IDX_isa = 4;
  static constexpr int IDX_datatype = 5;
  static constexpr int IDX_version = 6;
};

} // namespace macveth

#endif /* !MACVETH_COSTTABLE_H */
