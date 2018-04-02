#ifndef PATTERNS_H
#define PATTERNS_H

#include "llvm/IR/Instructions.h"
#include "llvm/Support/Casting.h"
#include "./lib/bitblock.hpp"

using namespace llvm;

#define fw 8

namespace ShuffleVectorOptimization {

class Pattern {
public:
    enum PatternKind {
        PK_Sequential,
        PK_Seq_Rotate,
        PK_Seq_Rotate_Intrinsics, // experimental

        PK_Idida,
        PK_Idisa_Rotate
    };

    Pattern(PatternKind K) : Kind(K) {}

    PatternKind getKind() const { return Kind; }


private:
    const PatternKind Kind;
};

class PatternSequential : public Pattern {
public:
    PatternSequential(PatternKind K) : Pattern(K) {}

    virtual bool matches(ShuffleVectorInst *inst) = 0;

    static bool classof(const Pattern *P) {
        return P->getKind() >= PK_Sequential && P->getKind() <= PK_Seq_Rotate_Intrinsics;
    }
};

class RotationPattern : public PatternSequential {
public:
    RotationPattern() : PatternSequential(PK_Seq_Rotate) {}

    bool matches(ShuffleVectorInst *inst) override;

    static bool classof(const Pattern *P) {
        return P->getKind() == PK_Seq_Rotate;
    }
};

class RotationPatternIntrinsics : public PatternSequential {
public:
    RotationPatternIntrinsics() : PatternSequential(PK_Seq_Rotate_Intrinsics) {}

    bool matches(ShuffleVectorInst *inst) override;

    static bool classof(const Pattern *P) {
        return P->getKind() == PK_Seq_Rotate_Intrinsics;
    }
};

class PatternIdisa : public Pattern {
public:
    PatternIdisa(PatternKind K) : Pattern(K) {}

    virtual bool matches(BitBlock maskVector, BitBlock indexVector, BitBlock lengthVector, BitBlock zeroVector) = 0;

    static bool classof(const Pattern *P) {
        return P->getKind() >= PK_Idida && P->getKind() <= PK_Idisa_Rotate;
    }
};

class RotationPatternIdisa : public PatternIdisa {
public:
    RotationPatternIdisa() : PatternIdisa(PK_Idisa_Rotate) {}

    bool matches(BitBlock maskVector, BitBlock indexVector, BitBlock lengthVector, BitBlock zeroVector) override;

    static bool classof(const Pattern *P) {
        return P->getKind() == PK_Idisa_Rotate;
    }
};

}

#endif