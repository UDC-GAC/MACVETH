/**
 * File              : LoopTracker.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Dom 01 Dec 2019 00:03:04 MST
 * Last Modified Date: Ven 06 Dec 2019 11:09:01 MST
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

#ifndef MACVETH_LOOP_TRACKER
#define MACVETH_LOOP_TRACKER

#include "include/StmtWrapper.h"
#include <list>
#include <string>

namespace macveth {

class Loop;
class LoopTracker;

typedef std::list<Loop *> LoopListType;
typedef std::list<LoopListType> LoopHierType;

class Loop {
public:
  Loop(std::string V, std::string M, std::string I)
      : VarName(V), MaxVal(M), Incr(I){};

  /// Getters and setters
  std::string getVarName() const { return this->VarName; }
  std::string getMaxVal() const { return this->MaxVal; }
  std::string getIncr() const { return this->Incr; }

  /// Just adding statement to the list
  void addStmt(StmtWrapper S) { this->StmtList.push_back(S); }

  /// Checking whether the loop has or not a given statement
  bool hasStmt(StmtWrapper S);

private:
  /// Variable name
  std::string VarName;
  /// As the MaxVal can be a number or a predefined variable, for commodity we
  /// will just type it as string
  std::string MaxVal;
  /// Same reasoning as with MaxVal: can be numeric or a value
  std::string Incr;
  /// List of statements within loop
  std::list<StmtWrapper> StmtList;
};

/// Loop comparator
bool operator==(const Loop &a, const Loop &b) {
  return (!a.getVarName().compare(b.getVarName())) &
         (!a.getMaxVal().compare(b.getMaxVal())) &
         (!a.getIncr().compare(b.getIncr()));
}

class LoopTracker {

public:
  /// Add a Loop object to the map according to its parent. If it has no Loop
  ///  parent, we will just add it to the List of Lists
  static void addLoop(std::string FuncName, Loop *Parent, std::string VarName,
                      std::string MaxVal, std::string Incr);

  /// Return the loop for a given statement and function
  Loop *findLoopByStmt(std::string FuncName, StmtWrapper S) {
    LoopHierType LoopHier = MapFuncLoops[FuncName];
    for (LoopListType LList : LoopHier) {
      for (Loop *L : LList) {
        if (L->hasStmt(S)) {
          return L;
        }
      }
    }
    return nullptr;
  }

  /// Return the loop for a given the and function
  Loop *findLoopByVar(std::string FuncName, std::string Name) {
    LoopHierType LoopHier = MapFuncLoops[FuncName];
    for (LoopListType LList : LoopHier) {
      for (Loop *L : LList) {
        if (Name.compare(L->getVarName())) {
          return L;
        }
      }
    }
    return nullptr;
  }

private:
  /// This map is meant to keep track of the loops of each function
  static std::map<std::string, LoopHierType> MapFuncLoops;

private:
  static LoopTracker *Singleton;
  LoopTracker(){};

public:
  /// Singleton pattern implemented
  static LoopTracker *getInstance();
};

} // namespace macveth

#endif
