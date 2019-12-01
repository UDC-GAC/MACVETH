/**
 * File              : LoopTracker.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 01 Dec 2019 00:03:04 MST
 * Last Modified Date: Dom 01 Dec 2019 00:26:33 MST
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

#include "include/StmtWrapper.h"
#include <list>
#include <string>

namespace macveth {

class Loop {
public:
  Loop(std::string V, std::string M, std::string I)
      : VarName(V), MaxVal(M), Incr(I){};

  /// Getters and setters
  std::string getVarName() { return this->VarName; }

  void addStmt(StmtWrapper S) { this->StmtList.push_back(S); }
  bool hasStmt(StmtWrapper S) {
    for (StmtWrapper SW : this->StmtList) {
      if (SW.getStmt() == S.getStmt()) {
        return true;
      }
    }
    return false;
  }

private:
  std::string VarName;
  std::string MaxVal;
  std::string Incr;
  std::list<StmtWrapper> StmtList;
};

class LoopTracker {
  typedef std::list<Loop> LoopListType;

public:
  void addLoop(std::string FuncName, std::string VarName, std::string MaxVal,
               std::string Incr) {
    LoopListType LoopList = MapFuncLoops[FuncName];
    LoopList.push_back(*(new Loop(VarName, MaxVal, Incr)));
  }

  Loop *findLoopByStmt(std::string FuncName, StmtWrapper S) {
    LoopListType LoopList = MapFuncLoops[FuncName];
    for (Loop L : LoopList) {
      if (L.hasStmt(S)) {
        return &L;
      }
    }
    return nullptr;
  }

  Loop *findLoopByVar(std::string FuncName, std::string Name) {
    LoopListType LoopList = MapFuncLoops[FuncName];
    for (Loop L : LoopList) {
      if (Name.compare(L.getVarName())) {
        return &L;
      }
    }
    return nullptr;
  }

private:
  static std::map<std::string, LoopListType> MapFuncLoops;

private:
  static LoopTracker *Singleton;
  LoopTracker(){};

public:
  static LoopTracker *getInstance();
};

} // namespace macveth
