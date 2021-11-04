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

#ifndef MACVETH_RANDOMPACKINGTEMPLATE_H
#define MACVETH_RANDOMPACKINGTEMPLATE_H

#include "include/Utils.h"
#include <list>
#include <map>
#include <sstream>
#include <string.h>

namespace macveth {

class RandomPackingTemplate {
public:
  void generateCode();

  RandomPackingTemplate(std::string Signature, int Cost)
      : Signature(Signature), Cost(Cost) {
    this->ID = RandomPackingTable::_UID++;
  }

private:
  static inline unsigned long long _UID = 0;
  unsigned long long ID = 0;
  std::string Signature = "";
  unsigned long long Cost = 0;

  struct Template {
    std::vector 
  };
};
} // namespace macveth

#endif /* !MACVETH_COSTTABLE_H */
