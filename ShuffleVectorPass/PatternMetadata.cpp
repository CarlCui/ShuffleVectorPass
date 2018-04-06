#include "./PatternMetadata.h"
#include "llvm/IR/Constants.h"

using namespace ShuffleVectorOptimization;

MDNode *PatternMetadataRotate::asMDNode(LLVMContext &context) {
    SmallVector<Metadata *, 2> mds;

    auto typeMD =  ConstantAsMetadata::get(ConstantInt::get(context, llvm::APInt(32, PMDK_Rotate, false)));
    auto numberOfRotationMD = ConstantAsMetadata::get(ConstantInt::get(context, llvm::APInt(32, this->numberOfRotation, false)));

    mds.push_back(typeMD);
    mds.push_back(numberOfRotationMD);

    MDNode* N = MDNode::get(context, mds);

    return N;
}

MDNode *PatternMetadataBroadcast::asMDNode(LLVMContext &context) {
    SmallVector<Metadata *, 2> mds;

    auto typeMD =  ConstantAsMetadata::get(ConstantInt::get(context, llvm::APInt(32, PMDK_Broadcast, false)));
    auto indexMD = ConstantAsMetadata::get(ConstantInt::get(context, llvm::APInt(32, this->index, false)));

    mds.push_back(typeMD);
    mds.push_back(indexMD);

    MDNode* N = MDNode::get(context, mds);

    return N;
}

MDNode *PatternMetadataOriginal::asMDNode (LLVMContext &context) {
    SmallVector<Metadata *, 1> mds;

    auto typeMD =  ConstantAsMetadata::get(ConstantInt::get(context, llvm::APInt(32, PMDK_Original, false)));

    mds.push_back(typeMD);

    MDNode* N = MDNode::get(context, mds);

    return N;
}

MDNode *PatternMetadataMerge::asMDNode (LLVMContext &context) {
    SmallVector<Metadata *, 1> mds;

    auto typeMD =  ConstantAsMetadata::get(ConstantInt::get(context, llvm::APInt(32, PMDK_Merge, false)));

    mds.push_back(typeMD);

    MDNode* N = MDNode::get(context, mds);

    return N;
}

MDNode *PatternMetadataBlend::asMDNode (LLVMContext &context) {
    SmallVector<Metadata *, 1> mds;

    auto typeMD =  ConstantAsMetadata::get(ConstantInt::get(context, llvm::APInt(32, PMDK_Blend, false)));

    mds.push_back(typeMD);

    MDNode* N = MDNode::get(context, mds);

    return N;
}
