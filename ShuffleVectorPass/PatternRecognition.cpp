#include "llvm/IR/Instructions.h"
#include "./PatternRecognition.h"
#include "./Patterns.h"
#include "./lib/bitblock.hpp"
using namespace llvm;
using namespace ShuffleVectorOptimization;

bool canonicalForm(ShuffleVectorInst *inst) {
    return false;
}

bool PatternRecognition::optimizeShuffleVectorInst(ShuffleVectorInst *inst) {

    auto mask = inst->getShuffleMask();

    auto maskSize = mask.size();

    uint8_t maskData[16] = {}; // 16 x i8, zero initialized
    uint8_t indeces[16] = {}; // an array of indeces ([0, 1, 2, ...])
    uint8_t lengthMaskArr[16] = {};

    for (unsigned i = 0; i < maskSize; i ++) {
        maskData[i] = (uint8_t)mask[i];
        indeces[i] = i;
        lengthMaskArr[i] = 0xFF;
    }

    BitBlock maskVector;
    BitBlock indexVector;
    BitBlock lengthMaskVector; //

    BitBlock zeroVector = mvmd128<fw>::fill(0);

    maskVector = bitblock128::load_aligned((BitBlock*)&maskData);
    indexVector = bitblock128::load_aligned((BitBlock*)&indeces);
    lengthMaskVector = bitblock128::load_aligned((BitBlock*)&lengthMaskArr);

    for (Pattern *pattern : this->patterns) {
        // if (pattern->matches(inst)) {
        //     return pattern->optimize(inst);
        // }

        if (isa<PatternIdisa>(pattern)) {
            auto *p = cast<PatternIdisa>(pattern);

            p->matches(maskVector, indexVector, lengthMaskVector, zeroVector);
        } else {
            auto *p = dyn_cast<PatternSequential>(pattern);
            p->matches(inst);
        }
    }

    return false;
}

PatternRecognition::PatternRecognition() {
    //this->patterns.push_back(new RotationPattern());
    //this->patterns.push_back(new RotationPatternIntrinsics());
    this->patterns.push_back(new RotationPatternIdisa());
}

PatternRecognition::~PatternRecognition() {

}



