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

PatternMetadata *RotationPatternIdisa::matches(ShuffleVectorInst *inst, CommonVectors commonVectors) {
    errs() << "rotation pattern idisa\n";

    auto indexVector = commonVectors.indexVector;
    auto maskVector = commonVectors.maskVector;
    auto lengthVector = commonVectors.lengthVector;
    auto lengthMaskVector = commonVectors.lengthMaskVector;
    auto zeroVector = commonVectors.zeroVector;

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

PatternMetadata *BroadcastPatternIdisa::matches(ShuffleVectorInst *inst, CommonVectors commonVectors) {
    errs() << "broadcast pattern idisa\n";

    auto indexVector = commonVectors.indexVector;
    auto maskVector = commonVectors.maskVector;
    auto lengthVector = commonVectors.lengthVector;
    auto lengthMaskVector = commonVectors.lengthMaskVector;
    auto zeroVector = commonVectors.zeroVector;

    auto begin = rdtsc();

    int firstElement = mvmd<fw>::extract<0>(maskVector);

    BitBlock constantFirst = mvmd<fw>::fill(firstElement);
    BitBlock constantFirstTillLength = simd<fw>::ifh(lengthMaskVector, constantFirst, zeroVector);
    BitBlock allEqual = simd<fw>::sub(maskVector, constantFirstTillLength);

    bool allEqualResult  = bitblock::any(allEqual);

    auto end = rdtsc();
    errs() << "cycles = " << (end - begin) << "\n";

    if (!allEqualResult) {
        return (new PatternMetadataBroadcast(firstElement));
    }

    return NULL;
}

PatternMetadata *OriginalPatternIdisa::matches(ShuffleVectorInst *inst, CommonVectors commonVectors) {
    errs() << "original pattern idisa\n";

    auto indexVector = commonVectors.indexVector;
    auto maskVector = commonVectors.maskVector;
    auto lengthVector = commonVectors.lengthVector;
    auto lengthMaskVector = commonVectors.lengthMaskVector;
    auto zeroVector = commonVectors.zeroVector;

    auto begin = rdtsc();

    BitBlock diff = simd<fw>::sub(maskVector, indexVector);

    bool someNoneZero = bitblock::any(diff);

    auto end = rdtsc();
    errs() << "cycles = " << (end - begin) << "\n";

    if (!someNoneZero) {
        return (new PatternMetadataOriginal());
    }

    return NULL;
}

PatternMetadata *MergePatternIdisa::matches(ShuffleVectorInst *inst, CommonVectors commonVectors) {
    errs() << "merge pattern idisa\n";

    auto maskLength = inst->getShuffleMask().size();
    auto vectorLength0 = inst->getOperand(0)->getType()->getVectorNumElements();
    auto vectorLength1 = inst->getOperand(1)->getType()->getVectorNumElements();

    if ((maskLength != vectorLength0 * 2) || (maskLength != vectorLength1 * 2)) {
        return NULL;
    }

    auto indexVector = commonVectors.indexVector;
    auto maskVector = commonVectors.maskVector;
    auto lengthVector = commonVectors.lengthVector;
    auto lengthMaskVector = commonVectors.lengthMaskVector;
    auto zeroVector = commonVectors.zeroVector;

    uint8_t mergeMask[16] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7};
    BitBlock mergeVector = bitblock::load_aligned((BitBlock*)&mergeMask);

    auto begin = rdtsc();

    BitBlock diff = simd<fw>::sub(maskVector, mergeVector);
    BitBlock modded = simd<fw>::sub(diff, mvmd<fw>::fill(vectorLength0));
    BitBlock gtMask = simd<fw>::gt(diff, zeroVector);
    BitBlock result = simd<fw>::ifh(gtMask, modded, diff);

    auto end = rdtsc();

    for (int i = 0; i < maskLength; i++) {
        if (mvmd<fw>::extract<2>(result) != 0) return NULL;
    }
    
    return (new PatternMetadataMerge());
}

PatternMetadata *BlendPatternIdisa::matches(ShuffleVectorInst *inst, CommonVectors commonVectors) {
    errs() << "blend pattern idisa\n";

    auto maskLength = inst->getShuffleMask().size();
    auto vectorLength0 = inst->getOperand(0)->getType()->getVectorNumElements();
    auto vectorLength1 = inst->getOperand(1)->getType()->getVectorNumElements();

    if ((maskLength != vectorLength0) || (maskLength != vectorLength1)) {
        return NULL;
    }

    auto indexVector = commonVectors.indexVector;
    auto maskVector = commonVectors.maskVector;
    auto lengthVector = commonVectors.lengthVector;
    auto lengthMaskVector = commonVectors.lengthMaskVector;
    auto zeroVector = commonVectors.zeroVector;

    auto begin = rdtsc();

    BitBlock diff = simd<fw>::sub(maskVector, indexVector);
    BitBlock modded = simd<fw>::sub(diff, mvmd<fw>::fill(maskLength));
    BitBlock gtMask = simd<fw>::gt(diff, zeroVector);
    BitBlock result = simd<fw>::ifh(gtMask, modded, diff);

    bool someNoneZero = bitblock::any(result);

    auto end = rdtsc();
    errs() << "cycles = " << (end - begin) << "\n";

    if (!someNoneZero) {
        return (new PatternMetadataBlend());
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

    // __m128i constantFirst = _mm_set1_epi8((char)firstElement);
    // __m128i constantFirstTillLength = _mm_blendv_epi8(_mm_setzero_si128(), constantFirst, lengthMaskVector);
    // __m128i allEqual = _mm_sub_epi8(result, constantFirstTillLength);

    // int allEqualResult = _mm_testz_si128(allEqual, _mm_setzero_si128());

    auto end = rdtsc();
    errs() << "cycles = " << (end - begin) << "\n";

    //errs() << allEqualResult << "\n";

    int allEqualResult = 0;

    if (allEqualResult == 0) {
        return (new PatternMetadataRotate(firstElement));;
    }

    return NULL;
}
