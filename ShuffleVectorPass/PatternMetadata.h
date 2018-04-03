#ifndef PATTERNMETADATA_H
#define PATTERNMETADATA_H

#include "llvm/IR/Metadata.h"
#include "llvm/IR/Instructions.h"
using namespace llvm;

namespace ShuffleVectorOptimization {

class PatternMetadata {
public:
    enum PatternMDKind {
        PMDK_Rotate
    };

    PatternMetadata(PatternMDKind K) : Kind(K) {}

    PatternMDKind getKind() const { return Kind; }

    virtual MDNode *asMDNode(LLVMContext &context) = 0;

private:
    const PatternMDKind Kind;
};

class PatternMetadataRotate : public PatternMetadata {
private:
    int numberOfRotation;
public:
    PatternMetadataRotate(int numberOfRotation) : PatternMetadata(PMDK_Rotate), numberOfRotation(numberOfRotation) {}

    static bool classof(const PatternMetadata *P) {
        return P->getKind() == PMDK_Rotate;
    }

    MDNode *asMDNode(LLVMContext &context) override;
};

}

#endif
