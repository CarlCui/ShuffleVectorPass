#ifndef SHUFFLE_VECTOR_OPTIMIZATION_H
#define SHUFFLE_VECTOR_OPTIMIZATION_H

#include "llvm/IR/Instructions.h"
#include "./Patterns.h"
using namespace llvm;

namespace ShuffleVectorOptimization {

class PatternRecognition {
private:
    SmallVector<Pattern *, 8> patterns;
    Pattern *recognizePattern();
    void addAllSequentialPatterns();
    void addAllIdisaPatterns();
    bool optimizeMerge(ShuffleVectorInst *inst);
public:
    PatternRecognition();
    ~PatternRecognition();
    bool optimizeShuffleVectorInst(ShuffleVectorInst *inst);
};

}

#endif
