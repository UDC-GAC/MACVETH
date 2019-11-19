/**
 * File              : S2SUtils.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 19 Nov 2019 08:57:03 MST
 * Last Modified Date: Mar 19 Nov 2019 08:59:14 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

#include "S2SUtils.h"

using namespace clang;
using namespace s2stranslator;

clang::LangOptions* Utils::getLangOpts() { return LangOpts; }
clang::SourceManager* Utils::getSourceMgr() { return SourceMgr; }
void Utils::setOpts(SourceManager* SO, LangOptions* LO) {
    SourceMgr = SO;
    LangOpts = LO;
}
