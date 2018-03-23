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

      bool modified = false;

      for (auto &basicBlock : function) {
        for (auto &instruction : basicBlock) {

          if (auto *inst = dyn_cast<ShuffleVectorInst>(&instruction)) {

            errs() << "encountered shufflevector instruction! \n";

            PatternRecognition patternRecognition;

            bool result = patternRecognition.optimizeShuffleVectorInst(inst);

            if (result) {
              modified = true;
            }
          }
        }
      }
      return modified;
    }
  };
}

char ShuffleVectorPass::ID = 0;
static RegisterPass<ShuffleVectorPass> X("sv", "Shuffle Vector Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
