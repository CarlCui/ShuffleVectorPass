#include "llvm/IR/Metadata.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "./PatternRecognition.h"
#include "./Patterns.h"
#include "./lib/bitblock.hpp"
#include "llvm/Support/raw_ostream.h"
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
    uint8_t lengthArr[16] = {};
    uint8_t lengthMaskArr[16] = {};

    for (unsigned i = 0; i < maskSize; i ++) {
        maskData[i] = (uint8_t)mask[i];
        indeces[i] = i;
        lengthArr[i] = maskSize;
        lengthMaskArr[i] = 0xFF;
    }

    BitBlock maskVector;
    BitBlock indexVector;
    BitBlock lengthVector;
    BitBlock lengthMaskVector;

    BitBlock zeroVector = mvmd<fw>::fill(0);

    maskVector = bitblock::load_aligned((BitBlock*)&maskData);
    indexVector = bitblock::load_aligned((BitBlock*)&indeces);
    lengthVector = bitblock::load_aligned((BitBlock*)lengthArr);
    lengthMaskVector = bitblock::load_aligned((BitBlock*)&lengthMaskArr);

    PatternMetadata *pm = NULL;

    for (Pattern *pattern : this->patterns) {

        PatternMetadata *temp = NULL;

        if (isa<PatternIdisa>(pattern)) {
            auto *p = cast<PatternIdisa>(pattern);

            temp = p->matches(maskVector, indexVector, lengthVector, lengthMaskVector, zeroVector);
        } else {
            auto *p = dyn_cast<PatternSequential>(pattern);
            temp = p->matches(inst);
        }

        if (temp != NULL) {
            pm = temp;
            break;
        }
    }

    if (pm != NULL) {
        inst->setMetadata("shuffleVector", pm->asMDNode(inst->getContext()));
    }

    return false;
}

PatternRecognition::PatternRecognition() {
    this->addAllIdisaPatterns();
    //this->addAllSequentialPatterns();
}

void PatternRecognition::addAllSequentialPatterns() {
    this->patterns.push_back(new RotationPattern());
}

void PatternRecognition::addAllIdisaPatterns() {
    this->patterns.push_back(new OriginalPatternIdisa());
    this->patterns.push_back(new BroadcastPatternIdisa());
    this->patterns.push_back(new RotationPatternIdisa());
}

PatternRecognition::~PatternRecognition() {

}



