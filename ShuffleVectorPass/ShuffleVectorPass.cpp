#include <vector>

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "./PatternRecognition.h"

using namespace std;
using namespace llvm;
using namespace ShuffleVectorOptimization;

namespace {
  struct ShuffleVectorPass : public FunctionPass {
    static char ID;
    ShuffleVectorPass() : FunctionPass(ID) {}

    bool runOnFunction(Function &function) override {

      bool modified = false;

      vector<Instruction*> shufflevectors;

      for (auto &basicBlock : function) {
        for (auto &instruction : basicBlock) {

          if (auto *inst = dyn_cast<ShuffleVectorInst>(&instruction)) {

            errs() << "encountered shufflevector instruction! \n";

            if (inst->getMetadata(PATTERN_METADATA_KIND_ID) != NULL) {
              continue;
            }

            PatternRecognition patternRecognition;

            bool result = patternRecognition.optimizeShuffleVectorInst(inst);

            if (result) {
              modified = true;
              shufflevectors.push_back(inst);
            }
          }
        }
      }

      for (int i = 0; i < shufflevectors.size(); i++) {
        shufflevectors[i]->eraseFromParent();
      }
      return modified;
    }
  };
}

char ShuffleVectorPass::ID = 0;
static RegisterPass<ShuffleVectorPass> X("sv", "Shuffle Vector Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
