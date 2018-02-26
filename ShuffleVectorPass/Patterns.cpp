#include "./Patterns.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

bool RotationPattern::matches(ShuffleVectorInst *inst) {
    errs() << "rotation pattern\n";

    auto mask = inst->getShuffleMask();

    auto maskSize = mask.size();

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

    errs() << allEqual << "\n";

    return allEqual;
}

bool RotationPattern::optimize(ShuffleVectorInst *inst) {
    return false;
}