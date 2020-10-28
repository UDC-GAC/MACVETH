/*
 * File	: include/Vectorization/SIMD/SIMDBackEndFactory.h
 * Author				 : Marcos Horro
 * Date					 : Fri 09 Oct 2020 04:53 +02:00
 *
 * Last Modified : Tue 20 Oct 2020 12:41 +02:00
 * Modified By	 : Marcos Horro (marcos.horro@udc.gal>)
 *
 * MIT License
 *
 * Copyright (c) 2020 Colorado State University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
  static SIMDBackEnd *getBackend(MVISA ISA) {
    SIMDBackEnd *G = nullptr;
    /// TODO: only AVX2 implemented yet...
    switch (ISA) {
    case SSE:
    case AVX:
    case AVX512:
      MVErr("This ISA has not been implemented yet!");
    case NATIVE:
    // FIXME: remove this at some point: native case should be determined at
    // some point, not to be contemplated exactly here
    case AVX2:
      G = AVX2BackEnd::getSingleton();
      if (G != nullptr) {
        return G;
      }
    }
    MVErr("No backend chosen!");
    return nullptr;
  }
};

} // namespace macveth
#endif /* !MACVETH_SIMDGENERATORFACTORY_H */
