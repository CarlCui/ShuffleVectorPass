#ifndef PATTERNS_H
#define PATTERNS_H

#define SIMD_MATCHING

#include "llvm/IR/Instructions.h"
#include "llvm/Support/Casting.h"

using namespace llvm;

namespace ShuffleVectorOptimization {

class Pattern {
public:
    enum PatternKind {
        PK_Rotate
    };

    Pattern(PatternKind K) : Kind(K) {}

    PatternKind getKind() const { return Kind; }

    virtual bool matches(ShuffleVectorInst *inst) = 0;
    virtual bool optimize(ShuffleVectorInst *inst) = 0;
private:
    const PatternKind Kind;
};

class RotationPattern : public Pattern {
private:
    int numberOfRotations; // sign indicates direction, and number indicates number of rotations
public:
    RotationPattern() : Pattern(PK_Rotate) {}

    bool matches(ShuffleVectorInst *inst) override;
    bool optimize(ShuffleVectorInst *inst) override;

    static bool classof(const Pattern *P) {
        return P->getKind() == PK_Rotate;
    }
};



}

#endif