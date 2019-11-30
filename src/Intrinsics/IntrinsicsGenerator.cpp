/**
 * File              : IntrinsicsGenerator.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Sáb 23 Nov 2019 11:34:15 MST
 * Last Modified Date: Ven 29 Nov 2019 10:05:27 MST
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

#include "include/Intrinsics/IntrinsicsGenerator.h"

using namespace macveth;

std::string IntrinsicsInsGen::getAvailableReg(TempExpr *Op) {
  int RegNo = RegDeclared.size();
  int BitWidth = getBitWidthFromType(Op->getClangExpr()->getType());
  std::string RegName = loadRegToStr(RegNo);
  std::string RegDecl = genRegDecl(RegName, BitWidth);
  RegDeclared.push_back(Op->getExprStr());
  RegMap[Op->getExprStr()] = RegName;
  return RegDecl;
}

std::string IntrinsicsInsGen::getGenericFunction(std::string FuncName,
                                                 TempExpr *Res, TempExpr *Op1,
                                                 TempExpr *Op2) {
  // int BitWidth = getBitWidthFromType(St->getClangExpr()->getType());
  // FIXME
  int BitWidth = 256;
  std::string DataType = "pd";
  std::string LhsStr = getRegister(Res);
  std::string RhsStr =
      genVarArgsFunc(genAVX2Ins(BitWidth, FuncName, DataType),
                     {RegMap[Op1->getExprStr()], RegMap[Op2->getExprStr()]});
  std::string FullInstruction = genAssignment(LhsStr, RhsStr);
  return FullInstruction;
}

std::string IntrinsicsInsGen::genVarArgsFunc(std::string NameFunc,
                                             std::list<std::string> Operands) {
  std::string FullFunc = NameFunc + "(";
  std::list<std::string>::iterator Op;
  int i = 0;
  for (Op = Operands.begin(); Op != Operands.end(); ++Op) {
    if (i++ == (Operands.size() - 1)) {
      FullFunc += *Op;
    } else {
      FullFunc += *Op + ", ";
    }
  }
  FullFunc += ")";
  return FullFunc;
}

std::string IntrinsicsInsGen::genLoad(TempExpr *Op) {
  std::string RegRes = getAvailableReg(Op);
  int BitWidth = getBitWidthFromType(Op->getClangExpr()->getType());
  std::string Name = "load";
  std::string DataType = getDataTypeFromType(Op->getClangExpr()->getType());
  // std::cout << Op->getClangExpr()->getType().getAsString() <<
  // std::endl;
  std::string RhsStr =
      genVarArgsFunc(genAVX2Ins(BitWidth, Name, DataType), {Op->getExprStr()});
  std::string AssignmentStr = genAssignment(RegRes, RhsStr);
  // std::cout << AssignmentStr << std::endl;
  return AssignmentStr;
}

std::string IntrinsicsInsGen::genStore(TempExpr *St, std::string Rhs) {
  int BitWidth = getBitWidthFromType(St->getClangExpr()->getType());
  std::string Name = "store";
  std::string DataType = getDataTypeFromType(St->getClangExpr()->getType());
  // std::cout << St->getClangExpr()->getType().getAsString() <<
  // std::endl;
  std::string AssignmentStr = genVarArgsFunc(
      genAVX2Ins(BitWidth, Name, DataType), {St->getExprStr(), Rhs});
  // std::cout << AssignmentStr << std::endl;
  return AssignmentStr;
}

InstListType
IntrinsicsInsGen::genIntrinsicsIns(std::list<macveth::TAC> TacList) {
  // we can have basically these situations:
  // 1.- a/tmp = tmp op tmp
  // 2.- a/tmp = b   op tmp
  // 3.- a/tmp = tmp op c
  // 4.- a/tmp = b   op c
  InstListType InsList;
  TAC PrevTAC;
  for (macveth::TAC Tac : TacList) {
    std::string FuncName = getFuncFromOpCode(Tac.getOP());
    if (!FuncName.compare("store")) {
      continue;
    }
    TempExpr *Res = Tac.getA();
    TempExpr *Op1 = Tac.getB();
    TempExpr *Op2 = Tac.getC();
    // check whether you can fuse a mul and add/sub
    if (potentialFMA(&PrevTAC, &Tac)) {
      InsList.pop_back();
      std::string NewInst = generateFMA(&PrevTAC, &Tac);
      InsList.push_back(NewInst);
      continue;
    }
    InsList.push_back(getGenericFunction(FuncName, Res, Op1, Op2));
    PrevTAC = Tac;
  }
  return InsList;
}

/// Generates either FMAdd or FMSub
std::string IntrinsicsInsGen::generateFMA(macveth::TAC *PrevTAC,
                                          macveth::TAC *ActualTAC) {
  std::string FuncName = getFuncFromOpCode(ActualTAC->getOP());
  macveth::TAC *MulTAC = (!FuncName.compare("mul")) ? ActualTAC : PrevTAC;
  macveth::TAC *OtherTAC = (FuncName.compare("mul")) ? ActualTAC : PrevTAC;

  FuncName = "fm" + getFuncFromOpCode(OtherTAC->getOP());
  TempExpr *OpA = MulTAC->getB();
  TempExpr *OpB = MulTAC->getC();
  TempExpr *OpC =
      !OtherTAC->getB()->isNotClang() ? OtherTAC->getB() : OtherTAC->getC();

  int BitWidth = 256;
  std::string DataType = "pd";
  std::string LhsStr = getRegister(OtherTAC->getA());
  std::string RhsStr =
      genVarArgsFunc(genAVX2Ins(BitWidth, FuncName, DataType),
                     {RegMap[OpA->getExprStr()], RegMap[OpB->getExprStr()],
                      RegMap[OpC->getExprStr()]});
  std::string FullInstruction = genAssignment(LhsStr, RhsStr);

  return FullInstruction;
}

std::list<InstListType> IntrinsicsInsGen::translateTAC(std::list<TAC> TacList) {
  /// Need to know which was the previous TAC to decide whether to use or not
  /// a FMA instruction
  TAC *PrevTAC = NULL;
  std::list<InstListType> InstList;
  InstListType LoadList;
  InstListType StoreList;
  /// Generate load operations if needed
  for (macveth::TAC Tac : TacList) {
    TempExpr *Op1 = Tac.getB();
    TempExpr *Op2 = Tac.getC();
    if (needLoad(Op1)) {
      LoadList.push_back(genLoad(Op1));
    }
    if (needLoad(Op2)) {
      LoadList.push_back(genLoad(Op2));
    }
  }
  InstList.push_back(LoadList);

  /// Generate body operations
  InstList.push_back(genIntrinsicsIns(TacList));

  /// Generate store operations
  for (TAC Tac : TacList) {
    TempExpr *Res = Tac.getA();
    TempExpr *Op1 = Tac.getB();
    TempExpr *Op2 = Tac.getC();
    if (!Res->isNotClang()) {
      /// Store in memory address given
      StoreList.push_back(genStore(Op1, Op2->getExprStr()));
    }
  }
  InstList.push_back(StoreList);
  printInstList(InstList);
  return InstList;
}

bool IntrinsicsInsGen::potentialFMA(macveth::TAC *PrevTAC, macveth::TAC *Tac) {
  if (PrevTAC == NULL)
    return false;
  std::string PrevTACOp = getFuncFromOpCode(PrevTAC->getOP());
  std::string TACOp = getFuncFromOpCode(Tac->getOP());
  if ((!PrevTACOp.compare("mul")) &&
      ((!TACOp.compare("add")) || (!TACOp.compare("sub")))) {
    PrevTAC = NULL;
    return true;
  }
  if (((!PrevTACOp.compare("add")) || (!PrevTACOp.compare("sub"))) &&
      (!TACOp.compare("mul"))) {
    PrevTAC = NULL;
    return true;
  }
  return false;
}

std::string IntrinsicsInsGen::getRegister(TempExpr *Op) {
  std::string RegName = Op->getExprStr();
  if (!Utils::contains(TempRegDeclared, RegName)) {
    TempRegDeclared.push_back(RegName);
    // FIXME
    // int BitWidth =
    // getBitWidthFromType(Op->getClangExpr()->getType());
    int BitWidth = 256;
    RegMap[Op->getExprStr()] = RegName;
    RegName = genRegDecl(RegName, BitWidth);
  }
  return RegName;
}

/// Singleton pattern implementation
IntrinsicsInsGen *IntrinsicsInsGen::Singleton = new IntrinsicsInsGen();
IntrinsicsInsGen *IntrinsicsInsGen::getInstance() {
  if (IntrinsicsInsGen::Singleton == 0) {
    IntrinsicsInsGen::Singleton = new IntrinsicsInsGen();
  }
  return IntrinsicsInsGen::Singleton;
}
