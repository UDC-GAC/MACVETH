/**
 * File              : IntrinsicsGenerator.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Sáb 23 Nov 2019 11:34:15 MST
 * Last Modified Date: Sáb 23 Nov 2019 11:43:34 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "include/IntrinsicsGenerator.h"

#include "include/Utils.h"

using namespace macveth;

std::list<std::string> TempRegDeclared;

// Singleton pattern
IntrinsicsInsGen* IntrinsicsInsGen::Singleton = new IntrinsicsInsGen();
IntrinsicsInsGen* IntrinsicsInsGen::getInstance() {
    if (IntrinsicsInsGen::Singleton == 0) {
        IntrinsicsInsGen::Singleton = new IntrinsicsInsGen();
    }
    return IntrinsicsInsGen::Singleton;
}

std::string IntrinsicsInsGen::getRegister(TempExpr* Op) {
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
