/**
 * File              : LoopTracker.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 01 Dec 2019 10:15:58 MST
 * Last Modified Date: Dom 01 Dec 2019 20:46:32 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 *
 * Copyright (c) 2019 Marcos Horro <marcos.horro@udc.gal>
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

#include "include/LoopTracker.h"

using namespace macveth;
std::map<std::string, LoopHierType> LoopTracker::MapFuncLoops;

void LoopTracker::addLoop(std::string FuncName, Loop *Parent,
                          std::string VarName, std::string MaxVal,
                          std::string Incr) {
  LoopHierType LoopHier = LoopTracker::MapFuncLoops[FuncName];
  if (Parent == nullptr) {
    LoopHier.back().push_back(*(new Loop(VarName, MaxVal, Incr)));
    return;
  }
  for (LoopListType LList : LoopHier) {
    for (Loop L : LList) {
      if (L == *(Parent)) {
        LList.push_back(*(new Loop(VarName, MaxVal, Incr)));
      }
    }
  }
}
