// Copyright 2021 Marcos Horro
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

#ifndef _MACVETH_INTRINSICS_EXTENSIONS_H
#define _MACVETH_INTRINSICS_EXTENSIONS_H

#define __mv_insertps(O, I, VAL, POS)                                          \
  __asm volatile("vinsertps\t%1, %2, %3, %0\n"                                 \
                 : "+x"(O)                                                     \
                 : "i"(POS), "m"(VAL), "x"(I));

#define __mv_blendps(O, I, VAL, POS)                                           \
  __asm volatile("vblendps\t%1, %2, %3, %0\n"                                  \
                 : "+x"(O)                                                     \
                 : "i"(POS), "m"(VAL), "x"(I));

#endif /* !_MACVETH_INTRINSICS_EXTENSIONS_H */