/**
 * File              : S2SUtils.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Lun 18 Nov 2019 15:16:05 MST
 * Last Modified Date: Mar 19 Nov 2019 09:36:49 MST
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
#ifndef S2S_UTILS
#define S2S_UTILS

#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"

using namespace clang;

namespace s2stranslator {

class S2SUtils {
   public:
    // when declaring static members of class, when assigning them you need
    // to redeclare them or since C++17 you can just put inline
    inline static clang::SourceManager* SourceMgr;
    inline static clang::LangOptions* LangOpts;

    static clang::SourceManager* getSourceMgr();
    static clang::LangOptions* getLangOpts();
    static void setOpts(SourceManager* SO, LangOptions* LO);
};

// since they are only getters/setters
SourceManager* S2SUtils::getSourceMgr() { return S2SUtils::SourceMgr; }
LangOptions* S2SUtils::getLangOpts() { return S2SUtils::LangOpts; }
void S2SUtils::setOpts(SourceManager* SO, LangOptions* LO) {
    S2SUtils::SourceMgr = SO;
    S2SUtils::LangOpts = LO;
}

}  // namespace s2stranslator
#endif
