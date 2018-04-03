#include "./Patterns.h"
#include "llvm/Support/raw_ostream.h"
#include "immintrin.h"
#include "smmintrin.h"

using namespace llvm;
using namespace ShuffleVectorOptimization;

uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

void countRdtscUsedCycles() {
    uint64_t a1 = rdtsc();
    uint64_t a2 = rdtsc();
    uint64_t a3 = rdtsc();
    uint64_t a4 = rdtsc();
    uint64_t a5 = rdtsc();
    uint64_t a6 = rdtsc();

    errs() << (a2 - a1) << "\n";
    errs() << (a3 - a2) << "\n";
    errs() << (a4 - a3) << "\n";
    errs() << (a5 - a4) << "\n";
    errs() << (a6 - a5) << "\n";
}

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

PatternMetadata *RotationPattern::matches(ShuffleVectorInst *inst) {
    //errs() << "rotation pattern\n";

    auto mask = inst->getShuffleMask();

    auto maskSize = mask.size();

    countRdtscUsedCycles();

    auto begin = rdtsc();

    for (unsigned i = 0; i < maskSize; i ++) {
        mask[i] = i - mask[i];

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

    auto end = rdtsc();
    errs() << "cycles = " << (end - begin) << "\n";

    if (allEqual) {
        return (new PatternMetadataRotate(mask[0]));
    }

    return NULL;
}

PatternMetadata *RotationPatternIdisa::matches(BitBlock maskVector, BitBlock indexVector, BitBlock lengthVector, BitBlock lengthMaskVector, BitBlock zeroVector) {
    // errs() << "rotation pattern idisa\n";

    countRdtscUsedCycles();

    auto begin = rdtsc();

    BitBlock diff = simd<fw>::sub(indexVector, maskVector);
    BitBlock modded = simd<fw>::add(diff, lengthVector);
    BitBlock cmpMask = simd<fw>::gt(diff, zeroVector);
    BitBlock result = simd<fw>::ifh(cmpMask, diff, modded);

    int firstElement = mvmd<fw>::extract<0>(result);

    BitBlock constantFirst = mvmd<fw>::fill(firstElement);
    BitBlock constantFirstTillLength = simd<fw>::ifh(lengthMaskVector, constantFirst, zeroVector);
    BitBlock allEqual = simd<fw>::sub(result, constantFirstTillLength);

    bool allEqualResult  = bitblock::any(allEqual);

    auto end = rdtsc();
    errs() << "cycles = " << (end - begin) << "\n";

    if (!allEqualResult) {
        return (new PatternMetadataRotate(firstElement));
    }

    return NULL;
}

PatternMetadata *RotationPatternIntrinsics::matches(ShuffleVectorInst *inst) {
    errs() << "rotation pattern intrinsics\n";

    auto mask = inst->getShuffleMask();

    auto maskSize = mask.size();

    countRdtscUsedCycles();

    auto begin = rdtsc();

    uint8_t maskData[16] = {}; // 16 x i8, zero initialized
    uint8_t indeces[16] = {}; // an array of indeces ([0, 1, 2, ...])
    uint8_t lengthMaskArr[16] = {};

    for (unsigned i = 0; i < maskSize; i ++) {
        maskData[i] = (uint8_t)mask[i];
        indeces[i] = i;
        lengthMaskArr[i] = 0xFF;
    }

    __m128i maskVector;
    __m128i indexVector;
    __m128i lengthMaskVector; //

    maskVector = _mm_load_si128((__m128i*)&maskData);
    indexVector = _mm_load_si128((__m128i*)&indeces);
    lengthMaskVector = _mm_load_si128((__m128i*)&lengthMaskArr);

    __m128i result = _mm_xor_si128(maskVector, indexVector);

    int firstElement = _mm_extract_epi8(result, 0);

    __m128i constantFirst = _mm_set1_epi8((char)firstElement);
    __m128i constantFirstTillLength = _mm_blendv_epi8(_mm_setzero_si128(), constantFirst, lengthMaskVector);
    __m128i allEqual = _mm_sub_epi8(result, constantFirstTillLength);

    int allEqualResult = _mm_testz_si128(allEqual, _mm_setzero_si128());

    auto end = rdtsc();
    errs() << "cycles = " << (end - begin) << "\n";

    //errs() << allEqualResult << "\n";

    if (allEqualResult == 0) {
        return (new PatternMetadataRotate(firstElement));;
    }

    return NULL;
}
