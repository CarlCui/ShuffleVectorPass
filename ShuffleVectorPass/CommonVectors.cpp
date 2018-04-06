#include "./CommonVectors.h"

using namespace llvm;
using namespace ShuffleVectorOptimization;

CommonVectors::CommonVectors(ShuffleVectorInst *inst) {
    auto mask = inst->getShuffleMask();

    auto maskSize = mask.size();

    uint8_t maskData[16] = {}; // 16 x i8, zero initialized
    uint8_t indeces[16] = {}; // an array of indeces ([0, 1, 2, ...])
    uint8_t lengthArr[16] = {};
    uint8_t lengthMaskArr[16] = {};

    for (unsigned i = 0; i < maskSize; i ++) {
        maskData[i] = (uint8_t)mask[i];
        indeces[i] = i;
        lengthArr[i] = maskSize;
        lengthMaskArr[i] = 0xFF;
    }

    this->maskVector = bitblock::load_aligned((BitBlock*)&maskData);
    this->indexVector = bitblock::load_aligned((BitBlock*)&indeces);
    this->lengthVector = bitblock::load_aligned((BitBlock*)lengthArr);
    this->lengthMaskVector = bitblock::load_aligned((BitBlock*)&lengthMaskArr);
    this->zeroVector = mvmd<fw>::fill(0);

}
