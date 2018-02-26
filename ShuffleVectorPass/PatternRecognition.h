#ifndef SHUFFLE_VECTOR_OPTIMIZATION_H
#define SHUFFLE_VECTOR_OPTIMIZATION_H

#include "llvm/IR/Instructions.h"
using namespace llvm;

namespace ShuffleVectorOptimization {

class Pattern {
public:
    virtual bool matches(ShuffleVectorInst *inst);
    virtual bool optimize(ShuffleVectorInst *inst);
};

class PatternRecognition {
private:
    SmallVector<Pattern *, 8> patterns;
    Pattern *recognizePattern();
public:
    PatternRecognition();
    ~PatternRecognition();
    bool optimizeShuffleVectorInst(ShuffleVectorInst *inst);
};

}

#endif