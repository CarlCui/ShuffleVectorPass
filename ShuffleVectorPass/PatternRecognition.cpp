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

    CommonVectors commonVectors = CommonVectors(inst);

    PatternMetadata *pm = NULL;

    for (Pattern *pattern : this->patterns) {

        PatternMetadata *temp = NULL;

        if (isa<PatternIdisa>(pattern)) {
            auto *p = cast<PatternIdisa>(pattern);

            temp = p->matches(inst, commonVectors);
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
        inst->setMetadata(PATTERN_METADATA_KIND_ID, pm->asMDNode(inst->getContext()));
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
    //this->patterns.push_back(new MergePatternIdisa());
    this->patterns.push_back(new BlendPatternIdisa());
}

PatternRecognition::~PatternRecognition() {

}
