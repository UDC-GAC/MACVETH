/**
 * LeftoverShufflePass.cpp
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
 *
 * Authors: Marcos Horro
 */
/**
 * File              : LeftoverShufflePass.cpp
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Last Modified Date: Mér 10 Xul 2019 14:52:25 CEST
 */
#define DEBUG_TYPE "topologyAware"

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/rae_ostream.h"
#include <map>

using namespace llvm;
namespace
{
  struct LeftoverShufflePass: public FunctionPass {
      std::map<std::string, int> varMap;
      static char ID;
      LeftoverShufflePass() : FunctionPass(ID) {}
      virtual bool runOnFunction(Function &F) {
          errs() << "Function " << F.getName() << "\n";
          for (Function::iterator bb = F.begin(), ebb = F.end(); bb != ebb; ++bb) {
              errs() << "\tBasic block " << bb.getName() << "\n";
              for (BasicBlock::iterator ins = bb->begin(), eins = bb->end(); ins != eins; ++ins) {
                  errs() << "\t\tInstruction " << ins.getName() << ":\n";
                for (User::op_iterator op = ins->begin(), eop = ins->op_end(); op != eop; ++op) {
                    errs() << "\t\t\tOperands " << op.getName() << "\n";
                }
              }
          }
          return false;
      }
  };
}
static RegisterPass<LeftoverShufflePass> X("LeftoverShufflePass", "Shuffling leftover statements to take advantage of SIMD instructions (aka vectorization)");
