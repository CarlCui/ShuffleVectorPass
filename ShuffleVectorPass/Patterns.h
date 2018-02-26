#ifndef PATTERNS_H
#define PATTERNS_H

#include "./PatternRecognition.h"

using namespace llvm;

namespace ShuffleVectorOptimization {

class RotationPattern : public Pattern {
private:
    int numberOfRotations; // sign indicates direction, and number indicates number of rotations
public:
    bool matches(ShuffleVectorInst *inst) override;
    bool optimize(ShuffleVectorInst *inst) override;
};

}

#endif