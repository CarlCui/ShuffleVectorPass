#include "llvm/IR/Instructions.h"
#include "./PatternRecognition.h"
#include "./Patterns.h"
using namespace llvm;
using namespace ShuffleVectorOptimization;

bool canonicalForm(ShuffleVectorInst *inst) {
    return false;
}

bool PatternRecognition::optimizeShuffleVectorInst(ShuffleVectorInst *inst) {
    for (Pattern *pattern : this->patterns) {
        if (pattern->matches(inst)) {
            return pattern->optimize(inst);
        }
    }

    return false;
}

PatternRecognition::PatternRecognition() {
    this->patterns.push_back(new RotationPattern());
}

PatternRecognition::~PatternRecognition() {

}



