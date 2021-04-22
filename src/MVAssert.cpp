// MACVETH - MVAssert.cpp
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
//     Louis-Nöel Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Louis-Nöel Pouchet <pouchet@colostate.edu>

#include "include/MVAssert.h"

using namespace macveth;

// ---------------------------------------------
void macveth::MVAssertSkip(bool cond, std::string Msg, jmp_buf Goto, int Val) {
  if (!cond) {
    std::cerr << BOLDMAGENTA << "[MACVETH SKIPPING] " << RESET << Msg
              << std::endl;
    longjmp(Goto, Val);
  }
}

// ---------------------------------------------
void macveth::MVWarn(std::string Msg) {
  std::cerr << BOLDYELLOW << "[MACVETH WARNING] " << RESET << Msg << std::endl;
}

// ---------------------------------------------
void macveth::MVInfo(std::string Msg) {
  std::cerr << BOLDBLUE << "[MACVETH INFO] " << RESET << Msg << std::endl;
}

// ---------------------------------------------
void macveth::MVAssert(bool cond, const char *Msg) {
  if (!cond) {
    std::cerr << BOLDRED << "[MACVETH FATAL ERROR] " << RESET << Msg
              << std::endl;
    exit(1);
  }
}

// ---------------------------------------------
void macveth::MVErr(std::string Msg) {
  std::cerr << BOLDRED << "[MACVETH FATAL ERROR] " << RESET << Msg << std::endl;
  exit(1);
}