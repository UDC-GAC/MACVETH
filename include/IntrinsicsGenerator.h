/**
 * File              : IntrinsicsGenerator.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 19 Nov 2019 09:49:18 MST
 * Last Modified Date: Xov 21 Nov 2019 15:34:15 MST
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

#ifndef INTRINSICS_H
#define INTRINSICS_H
#include <cstdarg>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "ThreeAddressCode.h"
#include "Utils.h"

namespace macveth {

// to avoid long declarations
typedef std::list<std::string> InstListType;

class IntrinsicsInsGen {
   public:
    // Main function:
    // It basically performs three passes:
    // 1.- Looks for memory loads
    // 2.- Looks for arithmetic operations
    // 3.- Generates memory stores
    static std::list<InstListType> translateTAC(
        std::list<macveth::TAC> TacList) {
        macveth::TAC* PrevTAC = NULL;
        std::list<InstListType> InstList;
        InstListType LoadList;
        // InstListType FuncList;
        InstListType StoreList;
        // generate loads
        for (macveth::TAC Tac : TacList) {
            TempExpr* Op1 = Tac.getB();
            TempExpr* Op2 = Tac.getC();
            if (needLoad(Op1)) {
                LoadList.push_back(genLoad(Op1));
            }
            if (needLoad(Op2)) {
                LoadList.push_back(genLoad(Op2));
            }
        }
        InstList.push_back(LoadList);

        // generate operations
        InstList.push_back(genIntrinsicsIns(TacList));

        // generate stores
        for (TAC Tac : TacList) {
            TempExpr* Res = Tac.getA();
            TempExpr* Op1 = Tac.getB();
            TempExpr* Op2 = Tac.getC();
            if (!Res->isNotClang()) {
                // Store in memory address given
                StoreList.push_back(genStore(Op1, Op2->getExprStr()));
            }
        }
        InstList.push_back(StoreList);
        printInstList(InstList);
        return InstList;
    }

   private:
    inline static std::list<std::string> RegDeclared;
    inline static std::list<std::string> TempRegDeclared;
    inline static std::map<std::string, std::string> RegMap;

    inline static std::map<std::string, int> TypeToWidth;
    inline static std::map<std::string, std::string> TypeToDataType;
    inline static std::map<BinaryOperator::Opcode, std::string> BOtoIntrinsic;

    // FIXME
    static std::string getFuncFromOpCode(BinaryOperator::Opcode Op) {
        return BOtoIntrinsic[Op];
    }

    // get bit width from data type
    static int getBitWidthFromType(clang::QualType ExprType) {
        return IntrinsicsInsGen::TypeToWidth[ExprType.getAsString()];
    }

    // get intrinsics instruction suffix from data type
    static std::string getDataTypeFromType(clang::QualType ExprType) {
        return IntrinsicsInsGen::TypeToDataType[ExprType.getAsString()];
    }

    // FIXME: global variable

    // Generates the body of the AVX2 instruction, e.g. __mm256_add_pd,
    // without any arguments. It is an auxiliary function
    static std::string genAVX2Ins(int BitWidth, std::string Name,
                                  std::string DataType) {
        std::string IntrinsicsIns =
            "_mm" + std::to_string(BitWidth) + "_" + Name + "_" + DataType;
        return IntrinsicsIns;
    }

    // generate AVX declarations if needed
    static std::string genRegDecl(std::string Name, int Width) {
        std::string RegDecl = "__m" + std::to_string(Width) + " " + Name;
        return RegDecl;
    }

    static std::string loadRegToStr(int RegNo) {
        return "loadReg" + std::to_string(RegNo);
    }

    // debugging purposes
    static void printInstList(std::list<InstListType> List) {
        for (InstListType L : List) {
            printList(L);
        }
    }

    // debugging purposes
    static void printList(InstListType List) {
        for (std::string S : List) {
            std::cout << S << std::endl;
        }
    }

    static std::string getAvailableReg(TempExpr* Op) {
        int RegNo = RegDeclared.size();
        int BitWidth = getBitWidthFromType(Op->getClangExpr()->getType());
        std::string RegName = loadRegToStr(RegNo);
        std::string RegDecl = genRegDecl(RegName, BitWidth);
        RegDeclared.push_back(Op->getExprStr());
        RegMap[Op->getExprStr()] = RegName;
        return RegDecl;
    }

    static std::string getRegister(TempExpr* Op) {
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

    static std::string getGenericFunction(std::string FuncName, TempExpr* Res,
                                          TempExpr* Op1, TempExpr* Op2) {
        // int BitWidth = getBitWidthFromType(St->getClangExpr()->getType());
        // FIXME
        int BitWidth = 256;
        std::string DataType = "pd";
        std::string LhsStr = getRegister(Res);
        std::string RhsStr = genVarArgsFunc(
            genAVX2Ins(BitWidth, FuncName, DataType),
            {RegMap[Op1->getExprStr()], RegMap[Op2->getExprStr()]});
        std::string FullInstruction = genAssignment(LhsStr, RhsStr);
        return FullInstruction;
    }

    static std::string genVarArgsFunc(std::string NameFunc,
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

    // Generate load instruction
    static std::string genLoad(TempExpr* Op) {
        std::string RegRes = getAvailableReg(Op);
        int BitWidth = getBitWidthFromType(Op->getClangExpr()->getType());
        std::string Name = "load";
        std::string DataType =
            getDataTypeFromType(Op->getClangExpr()->getType());
        // std::cout << Op->getClangExpr()->getType().getAsString() <<
        // std::endl;
        std::string RhsStr = genVarArgsFunc(
            genAVX2Ins(BitWidth, Name, DataType), {Op->getExprStr()});
        std::string AssignmentStr = genAssignment(RegRes, RhsStr);
        // std::cout << AssignmentStr << std::endl;
        return AssignmentStr;
    }

    // Generates the store instruction
    static std::string genStore(TempExpr* St, std::string Rhs) {
        int BitWidth = getBitWidthFromType(St->getClangExpr()->getType());
        std::string Name = "store";
        std::string DataType =
            getDataTypeFromType(St->getClangExpr()->getType());
        // std::cout << St->getClangExpr()->getType().getAsString() <<
        // std::endl;
        std::string AssignmentStr = genVarArgsFunc(
            genAVX2Ins(BitWidth, Name, DataType), {St->getExprStr(), Rhs});
        // std::cout << AssignmentStr << std::endl;
        return AssignmentStr;
    }

    // Generates the assignment code given LHS and RHS
    static std::string genAssignment(std::string Lhs, std::string Rhs) {
        std::string AssignmentStr = Lhs + " = " + Rhs;
        return AssignmentStr;
    }

    // FIXME
    // This is a fucking awful hack
    // TempExpr are used as wrappers of Expr and new Nodes. This class has
    // been created for the ease of TAC handling. So, if a TAC does not
    // belong to Expr class.
    static bool needLoad(macveth::TempExpr* Op) {
        if (IntrinsicsInsGen::RegMap.find(Op->getExprStr()) !=
            IntrinsicsInsGen::RegMap.end()) {
            return false;
        }
        // std::cout << "checking if it is not Clang" << std::endl;
        return !Op->isNotClang();
    }

    // Main function when generating instructions
    static InstListType genIntrinsicsIns(std::list<macveth::TAC> TacList) {
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
            TempExpr* Res = Tac.getA();
            TempExpr* Op1 = Tac.getB();
            TempExpr* Op2 = Tac.getC();
            // check whether you can fuse a mul and add/sub
            if (potentialFMA(&PrevTAC, &Tac)) {
                InsList.pop_back();
                std::string NewInst = generateFMA(&PrevTAC, &Tac);
                InsList.push_back(NewInst);
                PrevTAC = Tac;
                continue;
            }
            InsList.push_back(getGenericFunction(FuncName, Res, Op1, Op2));
            PrevTAC = Tac;
        }
        return InsList;
    }

    // generates a FMA from two consecutives TACs
    static std::string generateFMA(macveth::TAC* PrevTAC,
                                   macveth::TAC* ActualTAC) {
        std::string FuncName = getFuncFromOpCode(ActualTAC->getOP());
        macveth::TAC* MulTAC = (!FuncName.compare("mul")) ? ActualTAC : PrevTAC;
        macveth::TAC* OtherTAC =
            (FuncName.compare("mul")) ? ActualTAC : PrevTAC;

        FuncName = "fm" + getFuncFromOpCode(OtherTAC->getOP());
        TempExpr* OpA = MulTAC->getB();
        TempExpr* OpB = MulTAC->getC();
        TempExpr* OpC = !OtherTAC->getB()->isNotClang() ? OtherTAC->getB()
                                                        : OtherTAC->getC();

        int BitWidth = 256;
        std::string DataType = "pd";
        std::string LhsStr = getRegister(OtherTAC->getA());
        std::string RhsStr = genVarArgsFunc(
            genAVX2Ins(BitWidth, FuncName, DataType),
            {RegMap[OpA->getExprStr()], RegMap[OpB->getExprStr()],
             RegMap[OpC->getExprStr()]});
        std::string FullInstruction = genAssignment(LhsStr, RhsStr);

        return FullInstruction;
    }

    // checks whether a FMA can be generated
    static bool potentialFMA(macveth::TAC* PrevTAC, macveth::TAC* Tac) {
        if (PrevTAC == NULL) return false;
        std::string PrevTACOp = getFuncFromOpCode(PrevTAC->getOP());
        std::string TACOp = getFuncFromOpCode(Tac->getOP());
        if ((!PrevTACOp.compare("mul")) &&
            ((!TACOp.compare("add")) || (!TACOp.compare("sub")))) {
            return true;
        }
        if (((!PrevTACOp.compare("add")) || (!PrevTACOp.compare("sub"))) &&
            (!TACOp.compare("mul"))) {
            return true;
        }
        return false;
    }

   private:
    static IntrinsicsInsGen* Singleton;
    // to avoid creating instances of this class
    IntrinsicsInsGen() {
        TypeToWidth = {{"double", 256}, {"float", 256}};
        TypeToDataType = {{"double", "pd"}, {"float", "sp"}};
        BOtoIntrinsic = {{BO_Mul, "mul"},
                         {BO_Div, "div"},
                         {BO_Add, "add"},
                         {BO_Sub, "sub"},
                         {BO_Assign, "store"}};
    };

   public:
    static IntrinsicsInsGen* getInstance();
};  // class IntrinsicsInsGen

// Singleton pattern
IntrinsicsInsGen* IntrinsicsInsGen::Singleton = new IntrinsicsInsGen();
IntrinsicsInsGen* IntrinsicsInsGen::getInstance() {
    if (IntrinsicsInsGen::Singleton == 0) {
        IntrinsicsInsGen::Singleton = new IntrinsicsInsGen();
    }
    return IntrinsicsInsGen::Singleton;
}

}  // namespace macveth
#endif
