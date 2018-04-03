#include "./PatternMetadata.h"
#include "llvm/IR/Constants.h"

using namespace ShuffleVectorOptimization;

MDNode *PatternMetadataRotate::asMDNode(LLVMContext &context) {
    SmallVector<Metadata *, 4> mds;

    auto typeMD =  ConstantAsMetadata::get(ConstantInt::get(context, llvm::APInt(32, PMDK_Rotate, false)));
    auto numberOfRotationMD = ConstantAsMetadata::get(ConstantInt::get(context, llvm::APInt(32, this->numberOfRotation, false)));

    mds.push_back(typeMD);
    mds.push_back(numberOfRotationMD);

    MDNode* N = MDNode::get(context, mds);

    return N;
}
