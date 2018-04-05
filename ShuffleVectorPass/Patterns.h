#ifndef PATTERNS_H
#define PATTERNS_H

#include "llvm/IR/Instructions.h"
#include "llvm/Support/Casting.h"
#include "./lib/bitblock.hpp"
#include "./PatternMetadata.h"
#include "./CommonVectors.h"

using namespace llvm;

namespace ShuffleVectorOptimization {

class Pattern {
public:
    enum PatternKind {
        PK_Sequential,
        PK_Seq_Rotate,
        PK_Seq_Rotate_Intrinsics, // experimental

        PK_Idida,
        PK_Idida_Original,
        PK_Idisa_Broadcast,
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

    virtual PatternMetadata *matches(ShuffleVectorInst *inst) = 0;

    static bool classof(const Pattern *P) {
        return P->getKind() >= PK_Sequential && P->getKind() <= PK_Seq_Rotate_Intrinsics;
    }
};

class RotationPattern : public PatternSequential {
public:
    RotationPattern() : PatternSequential(PK_Seq_Rotate) {}

    PatternMetadata *matches(ShuffleVectorInst *inst) override;

    static bool classof(const Pattern *P) {
        return P->getKind() == PK_Seq_Rotate;
    }
};

class RotationPatternIntrinsics : public PatternSequential {
public:
    RotationPatternIntrinsics() : PatternSequential(PK_Seq_Rotate_Intrinsics) {}

    PatternMetadata *matches(ShuffleVectorInst *inst) override;

    static bool classof(const Pattern *P) {
        return P->getKind() == PK_Seq_Rotate_Intrinsics;
    }
};

class PatternIdisa : public Pattern {
public:
    PatternIdisa(PatternKind K) : Pattern(K) {}

    virtual PatternMetadata *matches(ShuffleVectorInst *inst, CommonVectors commonVectors) = 0;

    static bool classof(const Pattern *P) {
        return P->getKind() >= PK_Idida && P->getKind() <= PK_Idisa_Rotate;
    }
};

class BroadcastPatternIdisa : public PatternIdisa {
public:
    BroadcastPatternIdisa() : PatternIdisa(PK_Idisa_Broadcast) {}

    PatternMetadata *matches(ShuffleVectorInst *inst, CommonVectors commonVectors) override;

    static bool classof(const Pattern *P) {
        return P->getKind() == PK_Idisa_Broadcast;
    }
};

class OriginalPatternIdisa : public PatternIdisa {
public:
    OriginalPatternIdisa() : PatternIdisa(PK_Idida_Original) {}

    PatternMetadata *matches(ShuffleVectorInst *inst, CommonVectors commonVectors) override;

    static bool classof(const Pattern *P) {
        return P->getKind() == PK_Idida_Original;
    }
};

class RotationPatternIdisa : public PatternIdisa {
public:
    RotationPatternIdisa() : PatternIdisa(PK_Idisa_Rotate) {}

    PatternMetadata *matches(ShuffleVectorInst *inst, CommonVectors commonVectors) override;

    static bool classof(const Pattern *P) {
        return P->getKind() == PK_Idisa_Rotate;
    }
};

}

#endif