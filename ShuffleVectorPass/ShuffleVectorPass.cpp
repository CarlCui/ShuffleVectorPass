#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

namespace {
  // Hello - The first implementation, without getAnalysisUsage.
  struct ShuffleVectorPass : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    ShuffleVectorPass() : FunctionPass(ID) {}

    bool runOnFunction(Function &function) override {

      for (auto &basicBlock : function) {
        for (auto &instruction : basicBlock) {

          if (auto *shuffleVectorInst = dyn_cast<ShuffleVectorInst>(&instruction)) {

            errs() << "encountered shufflevector instruction! \n";
          }
        }
      }
      return false;
    }
  };
}

char ShuffleVectorPass::ID = 0;
static RegisterPass<ShuffleVectorPass> X("sv", "Shuffle Vector Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
