// MACVETH - SIMDBackEndFactory.h
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

#ifndef MACVETH_SIMDGENERATORFACTORY_H
#define MACVETH_SIMDGENERATORFACTORY_H

#include "include/MVOptions.h"
#include "include/Vectorization/SIMD/AVX2/AVX2BackEnd.h"
#include "include/Vectorization/SIMD/SIMDBackEnd.h"
#include <string>

namespace macveth {

/// Wrapper for avoiding low level details when creating the different
/// backends
class SIMDBackEndFactory {
public:
  /// Return a specific backend given as input
  static SIMDBackEnd &getBackend(MVCPUInfo::MVISA ISA) {
    /// TODO: only AVX2 implemented yet...
    switch (ISA) {
    case MVCPUInfo::MVISA::SSE:
    case MVCPUInfo::MVISA::AVX:
    case MVCPUInfo::MVISA::AVX512:
    case MVCPUInfo::MVISA::AUTODETECT:
      MVErr("This ISA has not been implemented yet!");
      break;
    case MVCPUInfo::MVISA::AVX2:
      return AVX2BackEnd::getInstance();
    default:
      MVErr("No backend chosen!");
    }
    throw 0;
  }
};

} // namespace macveth
#endif /* !MACVETH_SIMDGENERATORFACTORY_H */
