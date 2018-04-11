#include "llvm/IR/Metadata.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "./PatternRecognition.h"
#include "./Patterns.h"
#include "./lib/bitblock.hpp"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;
using namespace ShuffleVectorOptimization;

bool canonicalForm(ShuffleVectorInst *inst) {
    return false;
}

bool PatternRecognition::optimizeShuffleVectorInst(ShuffleVectorInst *inst) {

    bool result = false;

    CommonVectors commonVectors = CommonVectors(inst);

    PatternMetadata *pm = NULL;

    for (Pattern *pattern : this->patterns) {

        PatternMetadata *temp = NULL;

        if (isa<PatternIdisa>(pattern)) {
            auto *p = cast<PatternIdisa>(pattern);

            temp = p->matches(inst, commonVectors);
        } else {
            auto *p = dyn_cast<PatternSequential>(pattern);
            temp = p->matches(inst);
        }

        if (temp != NULL) {
            pm = temp;
            break;
        }
    }

    if (pm != NULL) {
        if (pm->getKind() == pm->PatternMDKind::PMDK_Merge) {
            // try to transform merge pattern to other IR instructions
            result = this->optimizeMerge(inst);
        } else {
            // if not optimized, set metadata for backend
            inst->setMetadata(PATTERN_METADATA_KIND_ID, pm->asMDNode(inst->getContext()));
        }
    }

    return result;
}

PatternRecognition::PatternRecognition() {
    this->addAllIdisaPatterns();
    //this->addAllSequentialPatterns();
}

void PatternRecognition::addAllSequentialPatterns() {
    this->patterns.push_back(new RotationPattern());
}

void PatternRecognition::addAllIdisaPatterns() {
    this->patterns.push_back(new OriginalPatternIdisa());
    this->patterns.push_back(new BroadcastPatternIdisa());
    this->patterns.push_back(new RotationPatternIdisa());
    this->patterns.push_back(new MergePatternIdisa());
    this->patterns.push_back(new BlendPatternIdisa());
}

bool PatternRecognition::optimizeMerge(ShuffleVectorInst *inst) {

    Value *op1 = inst->getOperand(0);
    Value *op2 = inst->getOperand(1);

    // only need type of op1 because op1 and op2 must be same type 
    // since it's a merge pattern    
    VectorType *opType = cast<VectorType>(op1->getType());

    unsigned numElements = opType->getNumElements();
    unsigned bitWidth = opType->getBitWidth() / numElements;

    IntegerType *originalIntType = IntegerType::get(inst->getContext(), bitWidth);
    IntegerType *extendIntType = IntegerType::get(inst->getContext(), 2 * bitWidth);
    VectorType *zextType = VectorType::get(extendIntType, numElements);
    VectorType *resultType = VectorType::get(originalIntType, numElements * 2);
    ConstantInt *constantInt = ConstantInt::getSigned(extendIntType, bitWidth);
    Constant *constantVector = ConstantVector::getSplat(numElements, constantInt);

    auto zextOp1 = new ZExtInst(op1, zextType, "", (Instruction*)inst);
    auto zextOp2 = new ZExtInst(op2, zextType, "", (Instruction*)inst);
    auto shl = BinaryOperator::Create(Instruction::Shl, (Value*)zextOp1, constantVector, "", inst); 
    auto logicOr = BinaryOperator::Create(Instruction::Or, (Value*)zextOp1, (Value*)zextOp2, "", inst); 
    auto bitCast = new BitCastInst((Value*)logicOr, resultType, "", inst);

    inst->replaceAllUsesWith((Value*) bitCast);

    return true;
}

PatternRecognition::~PatternRecognition() {

}
