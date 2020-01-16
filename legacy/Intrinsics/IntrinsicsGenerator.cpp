/**
 * File              : IntrinsicsGenerator.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Sáb 23 Nov 2019 11:34:15 MST
 * Last Modified Date: Mér 11 Dec 2019 18:03:37 MST
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

/// Assuming Op is already loaded, the main idea is to perform 2 permutations
/// and two additions
InstListType IntrinsicsInsGen::reduceVector(MVExpr *InOp, MVExpr *OutOp) {
  InstListType T;
  std::string OutputArr = "output";
  MVExpr *ymm0 = new MVExpr("ymm0", MVExpr::MVExprInfo::TMP_RES);
  MVExpr *ymm1 = new MVExpr("ymm1", MVExpr::MVExprInfo::TMP_RES);
  MVExpr *ymm2 = new MVExpr("ymm2", MVExpr::MVExprInfo::TMP_RES);
  MVExpr *ymm3 = new MVExpr("ymm3", MVExpr::MVExprInfo::TMP_RES);
  MVExpr *Out = new MVExpr(OutputArr, MVExpr::MVExprInfo::TMP_RES);

  /// Sequence of instructions for the vectorized reduction algorithm
  T.push_back(getGenericFunction("permute", {ymm0, InOp, new MVExpr("0x05")}));
  T.push_back(getGenericFunction("add", {ymm1, InOp, ymm0}));
  T.push_back(getGenericFunction("permute2f128",
                                 {ymm2, ymm1, ymm1, new MVExpr("0x01")}));
  T.push_back(getGenericFunction("add", {ymm3, ymm1, ymm2}));
  /// Generate store operation, this will load the result in a vector
  T.push_back(
      genAssignment("double " + OutputArr + "[4]", "{0.0,0.0,0.0,0.0}"));
  T.push_back(genStore(Out, ymm3));
  /// This generates an instruction for assignment
  T.push_back(
      genAssignment(OutOp->getExprStr(),
                    OutOp->getExprStr() + " + " + Out->getExprStr() + "[0]"));
  return T;
}

std::string IntrinsicsInsGen::getAvailableReg(MVExpr *Op) {
  int RegNo = RegDeclared.size();
  // int BitWidth = getBitWidthFromType(Op->getClangExpr()->getType());
  int BitWidth = 256;
  std::string RegName = loadRegToStr(RegNo);
  std::string RegDecl = genRegDecl(RegName, BitWidth);
  RegDeclared.push_back(Op->getExprStr());
  RegMap[Op->getExprStr()] = RegName;
  return RegDecl;
}

std::string IntrinsicsInsGen::getGenericFunction(std::string FuncName,
                                                 std::list<MVExpr> ExprList) {
  // FIXME
  int BitWidth = 256;
  std::string DataType = "pd";
  std::string LhsStr = getRegister(&ExprList.front());
  ExprList.pop_front();
  std::list<std::string> Operands;
  for (MVExpr T : ExprList) {
    if (!RegMap[T.getExprStr()].compare("")) {
      Operands.push_back(T.getExprStr());
    } else {
      Operands.push_back(RegMap[T.getExprStr()]);
    }
  }
  std::string RhsStr =
      genVarArgsFunc(genAVXIns(BitWidth, FuncName, DataType), Operands);
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

std::string IntrinsicsInsGen::genLoad(MVExpr *Op) {
  std::string RegRes = getAvailableReg(Op);
  // int BitWidth = getBitWidthFromType(Op->getClangExpr()->getType());
  int BitWidth = 256;
  std::string Name = "loadu";
  std::string DataType = "pd";
  // std::string DataType = getDataTypeFromType(Op->getClangExpr()->getType());
  /// FIXME
  std::string RhsStr = genVarArgsFunc(genAVXIns(BitWidth, Name, DataType),
                                      {"&" + Op->getExprStr()});
  std::string AssignmentStr = genAssignment(RegRes, RhsStr);
  // std::cout << AssignmentStr << std::endl;
  return AssignmentStr;
}

std::string IntrinsicsInsGen::genStore(MVExpr *St, MVExpr *Val) {
  // int BitWidth = getBitWidthFromType(St->getType());
  /// FIXME
  int BitWidth = 256;
  std::string Name = "storeu";
  std::string DataType = "pd";
  // std::string DataType = getDataTypeFromType(St->getClangExpr()->getType());
  // std::cout << St->getClangExpr()->getType().getAsString() <<
  // std::endl;
  std::string AssignmentStr =
      genVarArgsFunc(genAVXIns(BitWidth, Name, DataType),
                     {St->getExprStr(), Val->getExprStr()});
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
    /// check whether you can fuse a mul and add/sub
    if (potentialFMA(&PrevTAC, &Tac)) {
      TempRegDeclared.pop_back();
      InsList.pop_back();
      std::string NewInst = generateFMA(&PrevTAC, &Tac);
      InsList.push_back(NewInst);
      continue;
    }
    MVExpr *Res = Tac.getA();
    MVExpr *Op1 = Tac.getB();
    MVExpr *Op2 = Tac.getC();
    InsList.push_back(getGenericFunction(FuncName, {Res, Op1, Op2}));
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
  MVExpr *OpA = MulTAC->getB();
  MVExpr *OpB = MulTAC->getC();
  MVExpr *OpC =
      !OtherTAC->getB()->isNotClang() ? OtherTAC->getB() : OtherTAC->getC();
  /// FIXME
  int BitWidth = 256;
  std::string DataType = "pd";
  std::string LhsStr = getRegister(OtherTAC->getA());
  std::string RhsStr =
      genVarArgsFunc(genAVXIns(BitWidth, FuncName, DataType),
                     {RegMap[OpA->getExprStr()], RegMap[OpB->getExprStr()],
                      RegMap[OpC->getExprStr()]});
  std::string FullInstruction = genAssignment(LhsStr, RhsStr);

  return FullInstruction;
}

/// Clear mappings
void IntrinsicsInsGen::clearMappings() {
  TempRegDeclared.clear();
  RegMap.clear();
  RegDeclared.clear();
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
    MVExpr *Op1 = Tac.getB();
    MVExpr *Op2 = Tac.getC();
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
    MVExpr *Res = Tac.getA();
    MVExpr *Op1 = Tac.getB();
    MVExpr *Op2 = Tac.getC();
    if (!Res->isNotClang()) {
      /// Store in memory address given
      StoreList.push_back(genStore(Op1, Op2));
    }
  }
  InstList.push_back(StoreList);
  printInstList(InstList);
  /// Clearing mappings...
  clearMappings();
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

std::string IntrinsicsInsGen::getRegister(MVExpr *Op) {
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