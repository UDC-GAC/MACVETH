/**
 * File              : IntrinsicsGenerator.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 19 Nov 2019 09:49:18 MST
 * Last Modified Date: Mér 20 Nov 2019 17:19:13 MST
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

namespace s2stranslator {

// to avoid long declarations
typedef std::list<std::string> InstListType;

/*
 * Generic function to find if an element of any type exists in list
 */
template <typename T>
bool contains(std::list<T>& listOfElements, const T& element) {
    // Find the iterator if element in list
    auto it = std::find(listOfElements.begin(), listOfElements.end(), element);
    // return if iterator points to end or not. It points to end then it means
    // element
    // does not exists in list
    return it != listOfElements.end();
}

class IntrinsicsInsGen {
   public:
    // public function
    static std::string translateTAC(std::list<s2stranslator::TAC> TacList) {
        s2stranslator::TAC* PrevTAC = NULL;
        std::list<InstListType> InstList;
        // generate instructions for each TAC
        for (s2stranslator::TAC Tac : TacList) {
            // check whether you can fuse a multiplication and addition
            if (potentialFMA(PrevTAC, &Tac)) {
                InstList.pop_back();
                InstListType NewInstList = generateFMA(PrevTAC, &Tac);
                InstList.push_back(NewInstList);
                continue;
            }
            // otherwise
            InstListType NewInstList = genIntrinsicsIns(Tac);
            InstList.push_back(NewInstList);
            PrevTAC = &Tac;
        }
        printInstList(InstList);
        return "translateTAC";
    }

   private:
    inline static std::list<std::string> RegDeclared;
    inline static std::map<std::string, std::string> LoadReg;
    inline static std::list<std::string> TempRegDeclared;
    inline static std::map<std::string, std::string> TempReg;
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
        LoadReg[Op->getExprStr()] = RegName;
        return RegDecl;
    }

    static std::string getRegister(TempExpr* Op) {
        std::string RegName = Op->getExprStr();
        if (!contains(TempRegDeclared, RegName)) {
            TempRegDeclared.push_back(RegName);
            // FIXME
            // int BitWidth =
            // getBitWidthFromType(Op->getClangExpr()->getType());
            int BitWidth = 256;
            RegName = genRegDecl(RegName, BitWidth);
            TempReg[Op->getExprStr()] = RegName;
        }
        return RegName;
    }

    static std::string getGenericFunction(std::string FuncName, TempExpr* Res,
                                          TempExpr* Op1, TempExpr* Op2) {
        // int BitWidth = getBitWidthFromType(St->getClangExpr()->getType());
        // FIXME
        int BitWidth = 256;
        std::string DataType = "pd";
        std::string RhsStr = genVarArgsFunc(
            genAVX2Ins(BitWidth, FuncName, DataType),
            {LoadReg[Op1->getExprStr()], LoadReg[Op2->getExprStr()]});
        std::string LhsStr = getRegister(Res);
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
    static bool needLoad(s2stranslator::TempExpr* Op) {
        if (IntrinsicsInsGen::LoadReg.find(Op->getExprStr()) !=
            IntrinsicsInsGen::LoadReg.end()) {
            return false;
        }
        // std::cout << "checking if it is not Clang" << std::endl;
        return !Op->isNotClang();
    }

    // Main function when generating instructions
    static InstListType genIntrinsicsIns(s2stranslator::TAC Tac) {
        // we can have basically these situations:
        // 1.- a/tmp = tmp op tmp
        // 2.- a/tmp = b   op tmp
        // 3.- a/tmp = tmp op c
        // 4.- a/tmp = b   op c
        InstListType IntrinsicInsList;
        std::string FuncName = getFuncFromOpCode(Tac.getOP());
        TempExpr* Res = Tac.getA();
        TempExpr* Op1 = Tac.getB();
        TempExpr* Op2 = Tac.getC();

        // std::cout << Res->getExprStr() << " " << Op1->getExprStr() << " "
        //          << Op2->getExprStr() << std::endl;
        if (!FuncName.compare("store")) {
            goto store_inst;
        }

        if (needLoad(Op1)) {
            IntrinsicInsList.push_back(genLoad(Op1));
        }
        if (needLoad(Op2)) {
            IntrinsicInsList.push_back(genLoad(Op2));
        }

        IntrinsicInsList.push_back(getGenericFunction(FuncName, Res, Op1, Op2));

        // FIXME
        // if the A operand is a Clang expresion, then we must store the result
        // obtained. Need to fix this somehow, I think this could be confusing
        // for someone.
    store_inst:
        if (!Res->isNotClang()) {
            // Store in memory address given
            IntrinsicInsList.push_back(genStore(Op1, Op2->getExprStr()));
        }
        return IntrinsicInsList;
    }

    // generates a FMA from two consecutives TACs
    static InstListType generateFMA(s2stranslator::TAC* PrevTAC,
                                    s2stranslator::TAC* ActualTAC) {
        InstListType InstList;
        return InstList;
    }

    // checks whether a FMA can be generated
    static bool potentialFMA(s2stranslator::TAC* PrevTAC,
                             s2stranslator::TAC* Tac) {
        return false;
        if (!getFuncFromOpCode(PrevTAC->getOP()).compare("mul") &&
            (!getFuncFromOpCode(Tac->getOP()).compare("add"))) {
            return true;
        }
        if (!getFuncFromOpCode(PrevTAC->getOP()).compare("add") &&
            (!getFuncFromOpCode(Tac->getOP()).compare("mul"))) {
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

}  // namespace s2stranslator
#endif
