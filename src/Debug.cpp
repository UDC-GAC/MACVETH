// MACVETH - Debug.cpp
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

#include "include/Debug.h"

// ---------------------------------------------
void printDebug(std::string M, std::string Msg) {
  if (MVOptions::Debug) {
    if (MVOptions::OutDebugFile != "") {
      std::ofstream O;
      O.open(Utils::getExePath() + MVOptions::OutDebugFile,
             std::ios_base::app); // append instead of overwrite
      O << "[" << M << "] " << Msg << std::endl;
      O.close();
    } else {
      std::cout << "[" << M << "] " << Msg << std::endl;
    }
  }
}

// ---------------------------------------------
void printDebug(std::string M, std::string Msg, DebugLevel DB) {
  if (!MVOptions::DLevel) {
    printDebug(M, Msg);
    return;
  }
  if (DB <= MVOptions::DLevel) {
    printDebug(M, Msg);
  }
}