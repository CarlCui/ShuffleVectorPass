#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "./PatternRecognition.h"
using namespace llvm;
using namespace ShuffleVectorOptimization;

namespace {
  struct ShuffleVectorPass : public FunctionPass {
    static char ID;
    ShuffleVectorPass() : FunctionPass(ID) {}

    bool runOnFunction(Function &function) override {

      for (auto &basicBlock : function) {
        for (auto &instruction : basicBlock) {

          if (auto *inst = dyn_cast<ShuffleVectorInst>(&instruction)) {

            PatternRecognition patternRecognition;

            bool result = patternRecognition.optimizeShuffleVectorInst(inst);

            errs() << "encountered shufflevector instruction! \n";

            return result;
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
