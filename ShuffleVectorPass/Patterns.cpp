#include <chrono>
#include "./Patterns.h"
#include "llvm/Support/raw_ostream.h"
#include "./lib/bitblock.hpp"

using namespace llvm;
using namespace ShuffleVectorOptimization;

void print128_i8(__m128i var)
{
    uint8_t *val = (uint8_t*) &var;

    errs() << "[";
    for (unsigned i = 0; i < 16; i ++) {
        if (i != 0) {
            errs() << ", ";
        }
        errs() << (int)val[i];
    }

    errs() << "]\n";
}

bool RotationPattern::matches(ShuffleVectorInst *inst) {
    errs() << "rotation pattern\n";

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();

    // SSE experimental implementation
    auto mask = inst->getShuffleMask();

    auto maskSize = mask.size();

    const int fw = 128 / 16;

    uint8_t maskData[16] = {}; // 16 x i8, zero initialized
    uint8_t indeces[16] = {}; // an array of indeces ([0, 1, 2, ...])
    uint8_t lengthMaskArr[16] = {};

    for (unsigned i = 0; i < maskSize; i ++) {
        maskData[i] = (uint8_t)mask[i];
        indeces[i] = i;
        lengthMaskArr[i] = 0xFF;
    }

    BitBlock maskVector;
    BitBlock indexVector;
    BitBlock lengthMaskVector; //

    BitBlock zeroVector = mvmd128<fw>::fill(0);

    maskVector = bitblock128::load_aligned((BitBlock*)&maskData);
    indexVector = bitblock128::load_aligned((__m128i*)&indeces);
    lengthMaskVector = bitblock128::load_aligned((__m128i*)&lengthMaskArr);

    BitBlock result = simd_xor(maskVector, indexVector);

    int firstElement = mvmd128<fw>::extract<0>(result);

    BitBlock constantFirst = mvmd128<fw>::fill(firstElement);
    BitBlock constantFirstTillLength = simd128<fw>::ifh(lengthMaskVector, constantFirst, zeroVector);
    BitBlock allEqual = simd128<fw>::sub(result, constantFirstTillLength);

    bool allEqualResult  = bitblock::any(allEqual);

    errs() << allEqualResult << "\n";

    errs() << "Time difference (micro) = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\n";
    errs() << "Time difference (nano) = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "\n";

    return !allEqualResult;
}

bool RotationPattern::optimize(ShuffleVectorInst *inst) {
    return false;
}