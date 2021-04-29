// MACVETH - Debug.h
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

#ifndef MACVETH_DEBUG_H
#define MACVETH_DEBUG_H

#include "include/Utils.h"

/// For printing debug information to file if specified
void printDebug(std::string M, std::string Msg);

/// For printing debug information to file if specified according to a debugging
/// level, i.e., if debugging level (specified by user) is higher or equal,
/// then print. This is useful if we want to discriminate messages
void printDebug(std::string M, std::string Msg, DebugLevel DB);

#define MACVETH_DEBUG(X, MSG) printDebug(X, MSG)

#endif /* !MACVETH_DEBUG_H */