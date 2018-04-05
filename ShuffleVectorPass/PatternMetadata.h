#ifndef PATTERNMETADATA_H
#define PATTERNMETADATA_H

#include "llvm/IR/Metadata.h"
#include "llvm/IR/Instructions.h"
using namespace llvm;

namespace ShuffleVectorOptimization {

#define PATTERN_METADATA_KIND_ID "shuffleVector.pattern"

class PatternMetadata {
public:
    enum PatternMDKind {
        PMDK_Rotate,
        PMDK_Broadcast,
        PMDK_Original,
        PMDK_Merge
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

class PatternMetadataBroadcast : public PatternMetadata {
private:
    int index;
public:
    PatternMetadataBroadcast(int index) : PatternMetadata(PMDK_Broadcast), index(index) {}

    static bool classof(const PatternMetadata *P) {
        return P->getKind() == PMDK_Broadcast;
    }

    MDNode *asMDNode(LLVMContext &context) override;
};

class PatternMetadataOriginal : public PatternMetadata {
public:
    PatternMetadataOriginal() : PatternMetadata(PMDK_Original) {}

    static bool classof(const PatternMetadata *P) {
        return P->getKind() == PMDK_Original;
    }

    MDNode *asMDNode(LLVMContext &context) override;
};

class PatternMetadataMerge : public PatternMetadata {
public:
    PatternMetadataMerge() : PatternMetadata(PMDK_Merge) {}

    static bool classof(const PatternMetadata *P) {
        return P->getKind() == PMDK_Merge;
    }

    MDNode *asMDNode(LLVMContext &context) override;
};

}

#endif
