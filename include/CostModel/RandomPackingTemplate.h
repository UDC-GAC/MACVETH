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

#include "include/MVOptions.h"
#include "include/Utils.h"
#include <algorithm>
#include <cassert>
#include <list>
#include <map>
#include <sstream>
#include <string>

namespace macveth {

class TemplateOperand {
public:
  TemplateOperand(std::string &S) {
    int TokenInt = 0;
    size_t Pos = 0;
    int T = 0;
    while ((Pos = S.find(":")) != std::string::npos) {
      TokenInt = std::atoi(S.substr(0, Pos).c_str());
      S.erase(0, Pos + 1);
      if (T++ == 0) {
        ID = TokenInt;
      } else {
        MOD = TokenInt;
      }
    }
    assert((T == 2) && "Bad template!");
    Type = S;
  }

  int getOffset() const {
    assert(isMem());
    return MOD;
  }

  int getID() const { return ID; }
  bool isMem() const { return Type == "MEM"; }

private:
  mutable int ID = 0;
  mutable int MOD = 0;
  mutable std::string Type = "MEM";
};

using TemplateListT = std::vector<std::string>;

class RandomPackingTemplate {
public:
  RandomPackingTemplate(const std::string &Signature, int Cost)
      : Signature(Signature), Cost(Cost) {
    ID = RandomPackingTemplate::_UID++;
    std::string dir(__FILE__);
    dir = dir.substr(0, dir.find_last_of("\\/"));
    std::string TemplateFile = MVArchStr[MVOptions::Arch] + "_" +
                               MVISAStr[MVOptions::ISA] + "_" + Signature;
    TemplatePath = dir + "/RandomPackingTemplates/" + TemplateFile;
  }

  void readFile() const {
    std::ifstream F(TemplatePath);
    assert(!F.fail() && "File does not exist for Random Packing Template!");
    std::istream_iterator<std::string> Start(F), End;
    TemplateList = TemplateListT(Start, End);
  }

  TemplateListT getTemplates() const {
    readFile();
    return TemplateList;
  }

private:
  static inline unsigned long long _UID = 0;
  unsigned long long ID = 0;
  std::string Signature = "";
  unsigned long long Cost = 0;
  std::string TemplatePath = "";
  mutable TemplateListT TemplateList;
};
} // namespace macveth

#endif /* !MACVETH_COSTTABLE_H */
