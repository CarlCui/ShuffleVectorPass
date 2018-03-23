#include <chrono>
#include "./Patterns.h"
#include "llvm/Support/raw_ostream.h"
#include "immintrin.h"
#include "smmintrin.h"

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

    errs() << "]";
}

bool RotationPattern::matches(ShuffleVectorInst *inst) {
    errs() << "rotation pattern\n";

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();

    #ifdef SIMD_MATCHING

    // SSE experimental implementation
    auto mask = inst->getShuffleMask();

    auto maskSize = mask.size();

    uint8_t maskData[16] = {}; // 16 x i8, zero initialized
    uint8_t indeces[16] = {}; // an array of indeces ([0, 1, 2, ...])
    uint8_t lArr[16] = {};
    uint8_t lengthMaskArr[16] = {};

    for (unsigned i = 0; i < maskSize; i ++) {
        maskData[i] = (uint8_t)mask[i];
        indeces[i] = i;
        lArr[i] = maskSize;
        lengthMaskArr[i] = 0xFF;
    }

    __m128i maskVector;
    __m128i indexVector;
    __m128i constantLengthVector; // array of lengths
    __m128i lengthMaskVector; //

    maskVector = _mm_load_si128((__m128i*)&maskData);
    indexVector = _mm_load_si128((__m128i*)&indeces);
    constantLengthVector = _mm_load_si128((__m128i*)&lArr);
    lengthMaskVector = _mm_load_si128((__m128i*)&lengthMaskArr);

    __m128i diff = _mm_sub_epi8(indexVector, maskVector);
    __m128i modded = _mm_add_epi8(diff, constantLengthVector);
    __m128i cmpMask = _mm_cmpgt_epi8(diff, _mm_setzero_si128());
    __m128i result = _mm_blendv_epi8(modded, diff, cmpMask);

    int firstElement = _mm_extract_epi8(result, 0);

    __m128i constantFirst = _mm_set1_epi8((char)firstElement);
    __m128i constantFirstTillLength = _mm_blendv_epi8(_mm_setzero_si128(), constantFirst, lengthMaskVector);
    __m128i allEqual = _mm_sub_epi8(result, constantFirstTillLength);

    int allEqualResult = _mm_testz_si128(allEqual, _mm_setzero_si128());

    errs() << allEqualResult << "\n";

    errs() << "Time difference (micro) = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\n";
    errs() << "Time difference (nano) = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "\n";

    return allEqualResult == 0;
    #else

    auto mask = inst->getShuffleMask();

    auto maskSize = mask.size();

    for (unsigned i = 0; i < maskSize; i ++) {
        mask[i] = i - mask[i];

        // modulo
        if (mask[i] < 0) {
            mask[i] = mask[i] + maskSize;
        }
    }

    bool allEqual = true;

    for (unsigned i = 0; i < maskSize; i ++) {
        if (mask[i] != mask[0]) {
            allEqual = false;
        }
    }

    errs() << allEqual << "\n";


    errs() << "Time difference (micro) = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\n";
    errs() << "Time difference (nano) = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "\n";

    return allEqual;

    #endif
}

bool RotationPattern::optimize(ShuffleVectorInst *inst) {
    return false;
}
