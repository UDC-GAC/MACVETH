/**
 * File              : IntrinsicsGenerator.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 19 Nov 2019 09:49:18 MST
 * Last Modified Date: Mar 19 Nov 2019 15:49:18 MST
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
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "ThreeAddressCode.h"

namespace s2stranslator {
namespace intrinsics {

// FIXME
std::map<std::string, int> TypeToWidth = {{"double", 256}, {"float", 128}};

// FIXME
std::map<std::string, std::string> TypeToDataType = {{"double", "pd"},
                                                     {"float", "sp"}};

// FIXME
std::map<BinaryOperator::Opcode, std::string> BOtoIntrinsic = {
    {BO_Mul, "mul"},
    {BO_Div, "div"},
    {BO_Add, "add"},
    {BO_Sub, "sub"},
    {BO_Assign, "store"}};

// FIXME
std::string getFuncFromOpCode(BinaryOperator::Opcode Op) {
    return BOtoIntrinsic[Op];
}

// FIXME: global variable
std::map<int, bool> RegResultReady;
std::map<int, bool> RegUsing;
std::map<int, bool> RegDeclared;

int getBitWidthFromType(clang::QualType ExprType) {
    return TypeToWidth[ExprType.getAsString()];
}
std::string getDataTypeFromType(clang::QualType ExprType) {
    return TypeToDataType[ExprType.getAsString()];
}

// Generates the body of the AVX2 instruction, e.g. __mm256_add_pd, without
// any arguments. It is an auxiliary function
std::string genAVX2Ins(int BitWidth, std::string Name, std::string DataType) {
    std::string IntrinsicsIns =
        "_mm" + std::to_string(BitWidth) + "_" + Name + "_" + DataType;
    return IntrinsicsIns;
}

// generate AVX declarations if needed
std::string genRegDecl(int Number, int Width) {
    std::string RegDecl =
        "__m" + std::to_string(Width) + " t" + std::to_string(Number);
    RegDeclared[Number] = true;
    return RegDecl;
}

// Generates the store instruction
std::string genStore(TempExpr* St, std::string Rhs) {
    int BitWidth = getBitWidthFromType(St->getClangExpr()->getType());
    std::string Name = "store";
    std::string DataType = getDataTypeFromType(St->getClangExpr()->getType());
    std::string AssignmentStr = genAVX2Ins(BitWidth, Name, DataType);
    return AssignmentStr;
}

// Simply generates the assignment code given LHS and RHS
std::string genAssignment(std::string Lhs, std::string Rhs) {
    std::string AssignmentStr = Lhs + " = " + Rhs;
    return AssignmentStr;
}

// main function when generating instructions
std::string genIntrinsicsIns(s2stranslator::TAC Tac) {
    // we can have basically these situations:
    // 1.- a/tmp = tmp op tmp
    // 2.- a/tmp = b   op tmp
    // 3.- a/tmp = tmp op c
    // 4.- a/tmp = b   op c
    std::string IntrinsicIns = "";
    std::string FuncName = getFuncFromOpCode(Tac.getOP());
    TempExpr* Res = Tac.getA();
    TempExpr* Op1 = Tac.getB();
    TempExpr* Op2 = Tac.getC();
    if (Res->isNotClang()) {
    } else {
    }
    return "testing yet bitch";
}

// generates a FMA from two consecutives TACs
std::string generateFMA(s2stranslator::TAC* PrevTAC,
                        s2stranslator::TAC* ActualTAC) {
    return "";
}

// checks whether a FMA can be generated
bool potentialFMA(s2stranslator::TAC* PrevTAC, s2stranslator::TAC* Tac) {
    return false;
}

// public function
std::string translateTAC(std::list<s2stranslator::TAC> TacList) {
    s2stranslator::TAC* PrevTAC = NULL;
    std::list<std::string> InstList;
    // generate instructions for each TAC
    for (s2stranslator::TAC Tac : TacList) {
        // check whether you can fuse a multiplication and addition
        if (potentialFMA(PrevTAC, &Tac)) {
            InstList.pop_back();
            std::string NewInst = generateFMA(PrevTAC, &Tac);
            InstList.push_back(NewInst);
            continue;
        }
        // otherwise
        std::string NewInst = genIntrinsicsIns(Tac);
        InstList.push_back(NewInst);
        PrevTAC = &Tac;
    }
    return "translateTAC";
}

}  // namespace intrinsics
}  // namespace s2stranslator
#endif
